#include "Engine.hh"

Strategy Engine::STRATEGY = Strategy::FORWARD;
bool Engine::TRACE = false;
RuleChoice Engine::RULE_CHOICE = RuleChoice::NB_PREMISES_DESC;
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

std::vector<std::map<std::string,std::string>> updateBlacklist(std::vector<std::map<std::string,std::string>> const & blacklist, std::map<std::string, std::string> const & m){
    std::vector<std::map<std::string,std::string>> blacklist_;
    for (auto const & b : blacklist){
        std::map<std::string, std::string> b_(b);
        auto good = true;
        for (auto const & el : b){
            auto it = m.find(el.first);
            if (it != m.end()) {
                if (it->second == el.second) b_.erase(el.first);
                else {
                    good = false;
                    break;
                }
            }
        }
        if (good) blacklist_.push_back(b_);
    }
    return blacklist_;
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
    if (RULE_CHOICE == RuleChoice::NB_PREMISES_DESC) sortRulesByNbPremisesDesc();
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

            auto newFacts = rule.checkPremise(_facts);

            if (newFacts) {

                bool logRule = false;
                for (auto const & newFact : *newFacts) {

                    if(std::find_if(_facts.begin(), _facts.end(), 
                    [&](Fact const & f) {return newFact == f;}) == _facts.end()) {

                        addedNewFact = true;

                        addFact(newFact);

                        if (Engine::TRACE && !logRule){
                            std::cout << "Rule used: " << rule.toString() << std::endl;
                            logRule = true;
                        }

                        std::cout << "New fact inferred: " << newFact.toString() << std::endl;
                        std::cout << std::endl;

                    }
                }
            }
        }
    }

}


bool Engine::backwardChaining_(std::vector<std::string>* logs, std::map<std::string, std::string>* m, std::vector<std::map<std::string, std::string>> const & blacklist, Predicate const & goal) const
{
    for (auto const & fact: _facts){
        std::map<std::string, std::string> m_;
        if(goal.calc(fact, &m_) && check(blacklist, m_)){
            logs->push_back(fact.toString());
            insert(*m,m_);
            return true;
        }
    }
    for (auto const & rule : _rules){
        std::map<std::string, std::list<std::string>> m2;
        std::map<std::string, std::string> mLog;
        std::map<std::string, std::string> m_log;

        auto premises = rule.checkConsequent(goal, &m2, blacklist, mLog, m_log);

        if (premises){

            std::vector<std::string> log_;
            std::map<std::string, std::string> m_;

            std::map<std::string, std::string> m3;

            auto blacklistUpdate = updateBlacklist(blacklist, mLog);

            RuleBlackListHandle ruleBlackListHandle(rule.getPremises(),blacklistUpdate,m2,m_,m3);

            bool good = true;
            auto it = premises->begin();
            while (it != premises -> end()){
                auto p = it->toNewPredicate(m_);

                if (!backwardChaining_(&log_,&m_,ruleBlackListHandle.getCurrentBlackList(),p)) {
                    if (it == premises->begin()){
                        good = false;
                        break;
                    }
                    it--;
                    ruleBlackListHandle.dec(&m_);
                }
                else if (!ruleBlackListHandle.check(m_)){
                    ruleBlackListHandle.inc(m_);
                    ruleBlackListHandle.dec(&m_);
                    log_.pop_back();
                }
                else {
                    it++;
                    ruleBlackListHandle.inc(m_);
                }
            }
            if (good){
                for (auto const & l : log_)
                    logs->push_back(l);
                insert(m_log, m_);
                logs->push_back(rule.toString(m_log));
                insert(mLog, m_);
                updateValues(&m3,m_);
                insert(*m,m3);
                insert(mLog,*m);
                *m = mLog;
                *m = updateMap(*m);
                return true;
            }
        }
    }
    logs->clear();
    return false;
}

void Engine::backwardChaining() const
{

    std::vector<std::map<std::string, std::string>> blacklist;
    bool run = true;

    while(run){
        std::vector<std::string> logs;
        std::map<std::string, std::string> m;
        
        if(backwardChaining_(&logs, &m, blacklist, *GOAL)){

            std::cout << "True\n" << std::endl;

            if (Engine::TRACE){
                for (auto const & log: logs) std::cout << log << std::endl;
                std::cout << std::endl;
            }            

            if (!m.empty()){
                for (auto const & m_ : m){
                    std::cout << m_.first << " " << m_.second << std::endl;
                }

                std::cout << std::endl;

                std::cout << "Continue(y/n)? ";
                std::string rsp;
                std::cin >> rsp;
                if (rsp != "y") return;
                std::cout << std::endl;
                blacklist.push_back(m);
            }
            else run = false;

        }
        else{

            std::cout << "False" << std::endl;
            run = false;

        }
    }
    
}

void Engine::sortRulesByNbPremisesDesc()
{
    std::sort (_rules.begin(), _rules.end(), [](Rule r1, Rule r2){
        return r1.getPremises().size() > r2.getPremises().size();
    });
}
