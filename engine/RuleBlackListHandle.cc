#include "RuleBlackListHandle.hh"

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

RuleBlackListHandle::RuleBlackListHandle(std::vector<Predicate> const & premisses, std::vector<std::map<std::string, std::string>> const & blacklist, std::map<std::string, std::string> const & m)
    :_blacklist(blacklist),_currentBlackList(){
    std::list<std::string> variables;
        for (auto const & premise : premisses){
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

bool RuleBlackListHandle::check(std::map<std::string, std::string> const & m) const{
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

void RuleBlackListHandle::dec(std::map<std::string, std::string> * m){
    if (_current_it != _variablesParameters.end()) for (auto const & variable: *_current_it) m->erase(variable); 
    _current_it--;
    _currentBlackList.pop();
    for (auto const & variable: *_current_it) m->erase(variable); 
}

void RuleBlackListHandle::inc(std::map<std::string, std::string> const & m){
    std::map<std::string, std::string> subMap = submap(m,*_current_it);
    _currentBlackList.top().push_back(subMap);
    _current_it++;
    _currentBlackList.push(std::vector<std::map<std::string, std::string>>());
}