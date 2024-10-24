#include "Engine.hh"

Strategy Engine::STRATEGY = Strategy::FORWARD;
std::unique_ptr<Predicate> Engine::GOAL = nullptr;

std::vector<std::string> split(std::string s, const std::string& delimiter) {
    std::vector<std::string> tokens;
    size_t pos = 0;
    std::string token;
    while ((pos = s.find(delimiter)) != std::string::npos) {
        token = s.substr(0, pos);
        tokens.push_back(token);
        s.erase(0, pos + delimiter.length());
    }
    tokens.push_back(s);

    return tokens;
}

void Engine::setGoal(std::string goal)
{

    goal.erase(std::remove(goal.begin(), goal.end(), ' '), goal.end());

    std::string delimiter = "(";
    std::string name = goal.substr(0, goal.find(delimiter)); 

    std::vector<Parameter> parameters;

    std::vector<std::string> parameters_names = split(goal.substr(goal.find(delimiter)+1, goal.length()-2-goal.find(delimiter)) ,",");

    auto it = parameters_names.end();
    do {

        it--;
        if ((*it)[0]>=65 && (*it)[0]<=90)
            parameters.push_back(Parameter(TypeParameter::VARIABLE, *it));
        else
            parameters.push_back(Parameter(TypeParameter::CONSTANT, *it));

    } while(it != parameters_names.begin());

    Engine::GOAL = std::make_unique<Predicate>(Predicate(name, parameters, true));

}

void Engine::run()
{
    switch (Engine::STRATEGY)
    {
    case Strategy::FORWARD:
        forwardChaining();
        break;
    case Strategy::BACKWARD:
        backwardChaining();
        break;
    default:
        break;
    }
}

void Engine::forwardChaining() {
    bool addedNewFact = true;

    while (addedNewFact) {
        addedNewFact = false;
        for (auto const &rule : _rules) {
            std::map<std::string, std::string> varToConst;

            auto newFacts = rule.checkPremise(_facts, varToConst);

            if (newFacts) {
                for (auto const & newFact : *newFacts) {

                    if(std::find_if(_facts.begin(), _facts.end(), 
                    [&](Fact const & f) {return newFact == f;}) == _facts.end()) {

                        addedNewFact = true;

                        addFact(newFact);

                        std::cout << "New fact inferred: " << newFact.toString() << std::endl;

                        for (auto const & vtc : varToConst) {
                            std::cout << vtc.first << " : " << vtc.second << "\t";
                        }
                        std::cout << std::endl;
                    }
                }
            }
        }
    }

}

void insert(std::map<std::string,std::string>* m1, std::map<std::string,std::string> const & m2){
    for (auto const & m: m2){
        (*m1)[m.first] = m.second;
    }
}

void updateValues(std::map<std::string,std::string>* m1, std::map<std::string,std::string> const & m2){
    for (auto & m: *m1){
        auto el = m2.find(m.second);
        if (el != m2.end())
            m.second = el->second;
    }
}

void updateKeys(std::map<std::string,std::string>* m1, std::map<std::string,std::string> const & m2){
    for (auto & m: *m1){
        auto el = m2.find(m.first);
        if (el != m2.end()){
            auto node = m1->extract(m.first);
            node.key() = el->first;
            m1->insert(std::move(node));
        }
    }
}

void update(std::vector<std::map<std::string, std::string>> * blacklist, std::map<std::string,std::string> const & m){
    for (auto & b : *blacklist) updateKeys(&b, m);
}

std::map<std::string, std::string> submap(std::map<std::string, std::string> const & m, std::list<std::string> const & keys){
    std::map<std::string, std::string> subMap;
    for (auto const & key: keys){
        auto el = m.find(key);
        if (el != m.end()) subMap[key] = el->second;
    }
    return subMap;
}

class RuleBlackListHandle {
private:
    std::vector<std::map<std::string, std::string>> _blacklist;
    std::stack<std::vector<std::map<std::string, std::string>>> _currentBlackList;
    std::list<std::list<std::string>> _variablesParameters;
    std::list<std::list<std::string>>::iterator _current_it;
public:
    RuleBlackListHandle(Rule const & rule, std::vector<std::map<std::string, std::string>> const & blacklist, std::map<std::string, std::string> const & m)
    :_blacklist(blacklist),_currentBlackList(){
        std::list<std::string> variables;
        for (auto const & premise : rule.getPremises()){
            auto vars = premise.getVariables();
            vars.erase(std::remove_if(vars.begin(),vars.end(),[&variables](std::string const & variable){
                return (std::find(variables.begin(), variables.end(), variable) != variables.end());
            }),vars.end());
            for (auto const & var: vars) variables.push_back(var);
            _variablesParameters.push_back(vars);
        }
        _current_it = _variablesParameters.begin();
        update(&_blacklist,m);
        _currentBlackList.push(std::vector<std::map<std::string, std::string>>());
    }
    bool check(std::map<std::string, std::string> const & m) const{
        if (_blacklist.empty()) return true;
        for (auto const & bl : _blacklist){
            bool good = false;
            for (auto const & el : bl){
                auto it = m.find(el.first);
                if (it == m.end() || it->second != el.second){
                    good = true;
                    break;
                }
            }
            if (!good) return false;
        }
        return true;
    }
    std::vector<std::map<std::string,std::string>> getCurrentBlackList(){
        return _currentBlackList.top();
    }
    void dec(std::map<std::string, std::string> * m){
        for (auto const & variable: *_current_it) m->erase(variable); 
        _current_it--;
        _currentBlackList.pop();
        for (auto const & variable: *_current_it) m->erase(variable); 
    }
    void inc(std::map<std::string, std::string> const & m){
        std::map<std::string, std::string> subMap = submap(m,*_current_it);
        _currentBlackList.top().push_back(subMap);
        _current_it++;
        _currentBlackList.push(std::vector<std::map<std::string, std::string>>());
    }
};

bool check(std::vector<std::map<std::string, std::string>> const & blacklist, std::map<std::string, std::string> const & m){
    if (blacklist.empty()) return true;
    for (auto const & bl : blacklist){
        bool good = false;
        for (auto const & el : bl){
            auto it = m.find(el.first);
            if (it == m.end() || it->second != el.second){
                good = true;
                break;
            }
        }
        if (!good) return false;
    }
    return true;
}


bool Engine::backwardChaining_(std::vector<std::string>* logs, std::map<std::string, std::string>* m, std::vector<std::map<std::string, std::string>> const & blacklist, Predicate const & goal) const
{
    for (auto const & fact: _facts){
        std::map<std::string, std::string> m_;
        if(goal.calc(fact, &m_) && check(blacklist, m_)){
            logs->push_back(fact.toString());
            insert(m,m_);
            return true;
        }
    }
    for (auto const & rule : _rules){
        std::map<std::string, std::string> m2;
        auto premises = rule.checkConsequent(goal, &m2);
        if (premises){

            std::vector<std::string> log_;
            std::map<std::string, std::string> m_;

            RuleBlackListHandle ruleBlackListHandle(rule,blacklist,m2);

            bool good = true;
            auto it = premises->begin();
            while (it != premises -> end()){
                auto p = it->toNewPredicate(m_);
                if (!backwardChaining_(&log_,&m_,ruleBlackListHandle.getCurrentBlackList(),p) || !ruleBlackListHandle.check(m_)) {
                    if (it == premises->begin()){
                        good = false;
                        break;
                    }
                    it--;
                    ruleBlackListHandle.dec(&m_);
                }
                else {
                    it++;
                    ruleBlackListHandle.inc(m_);
                }
            }
            if (good){
                for (auto const & l : log_)
                    logs->push_back(l);
                std::string log;
                for (auto const & predicate: *premises){
                    auto p = predicate.toNewPredicate(m_);
                    log+=(p.toString()+" ");
                }
                log+="-> ";
                log+=rule.getConsequent().toNewPredicate(m_).toString();
                logs->push_back(log);
                updateValues(&m2,m_);
                insert(m,m2);
                return true;
            }
        }
    }
    return false;
}

void Engine::backwardChaining() const
{
    std::vector<std::string> logs;
    std::map<std::string, std::string> m;
    std::vector<std::map<std::string, std::string>> blacklist;
    if(backwardChaining_(&logs, &m, blacklist, *GOAL)){
        for (auto const & log: logs){
            std::cout << log << std::endl;
        }
    }
    for (auto const & m_ : m){
        std::cout << m_.first << " " << m_.second << std::endl;
    }
}
