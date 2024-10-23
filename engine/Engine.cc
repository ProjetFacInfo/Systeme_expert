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

void Engine::forwardChaining()
{
    
}

void insert(std::map<std::string,std::string>* m1, std::map<std::string,std::string> const & m2){
    for (auto const & m: m2){
        (*m1)[m.first] = m.second;
    }
}

void update(std::map<std::string,std::string>* m1, std::map<std::string,std::string> const & m2){
    for (auto & m: *m1){
        auto el = m2.find(m.second);
        if (el != m2.end())
            m.second = el->second;
    }
}

void insert(std::map<std::string,std::vector<std::string>>* m1, std::map<std::string,std::vector<std::string>> const & m2){
    for (auto const & m: m2){
        (*m1)[m.first] = m.second;
    }
}

void update(std::map<std::string,std::vector<std::string>>* m1, std::map<std::string,std::string> const & m2){
    for (auto & m: *m1){
        auto el = m2.find(m.first);
        if (el != m2.end()){
            auto node = m1->extract(m.first);
            node.key() = el->first;
            m1->insert(std::move(node));
        }
    }
}

bool Engine::backwardChaining_(std::vector<std::string>* logs, std::map<std::string, std::string>* m, std::map<std::string, std::vector<std::string>> const & blacklist, Predicate const & goal) const
{
    for (auto const & fact: _facts){
        std::map<std::string, std::string> m_;
        if(goal.calc(fact, &m_)){
            logs->push_back(fact.toString());
            insert(m,m_);
            return true;
        }
    }
    for (auto const & rule : _rules){
        std::map<std::string, std::string> m2;
        auto premises = rule.checkConsequent(goal, &m2);
        if (premises){

            std::map<std::string, std::vector<std::string>> blacklist_;
            insert(&blacklist_,blacklist);
            update(&blacklist_,m2);

            std::vector<std::string> log_;
            std::map<std::string, std::string> m_;
            bool good = true;
            for (auto const & predicate : *premises){
                auto p = predicate.toNewPredicate(m_);
                if (!backwardChaining_(&log_,&m_,blacklist,p)) {
                    good = false;
                    break;
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
                update(&m2,m_);
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
    std::map<std::string, std::vector<std::string>> blacklist;
    if(backwardChaining_(&logs, &m, blacklist, *GOAL)){
        for (auto const & log: logs){
            std::cout << log << std::endl;
        }
    }
    for (auto const & m_ : m){
        std::cout << m_.first << " " << m_.second << std::endl;
    }
}
