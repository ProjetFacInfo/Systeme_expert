#include "RuleBlackListHandle.hh"

std::map<std::string, std::string> updateMap(std::map<std::string, std::string> const & m){

    std::map<std::string, std::string> m2;

    for (auto const & el : m){
		auto it = m.find(el.first);
		auto it2 = m.find(it->second);
		while (it2 != m.end()){
			it = it2;
			it2 = m.find(it->second);
		}
		m2[el.first] = it->second;
	}

    return m2;
}

void insert(std::map<std::string,std::string>& m1, std::map<std::string,std::string> const & m2){
    for (auto const & m: m2){
        m1[m.first] = m.second;
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

bool updateKeys(std::map<std::string,std::string>& m1, std::map<std::string, std::string> const & m3){
    
    std::map<std::string, std::list<std::string>> rm3;

    for (auto const & el:m3){
        auto it = rm3.find(el.second);
        if (it == rm3.end()) rm3[el.second] = std::list<std::string>();
        rm3[el.second].push_back(el.first);
    }

    std::map<std::string, std::string> m2;

    for (auto const & el:rm3){
        std::string val = m1.operator[](el.second.front());
        for (auto const & el2:el.second){
            if (m1.operator[](el2) != val) return false;
        }
        if (el.first[0] >= 65 && el.first[0] <= 90) m2[el.first] = val;
    }

    m1 = m2;
    return true;

}

void update(std::vector<std::map<std::string, std::string>> & blacklist, std::map<std::string,std::list<std::string>> const & m2, std::map<std::string, std::string> & m__, std::map<std::string, std::string> & m3){
    
    std::map<std::string, std::string> m;
    std::map<std::string, std::vector<std::string>> rm;
    for (auto & m_: m2){

        if (m_.second.size()>1){
            auto pivot = m_.second.front();

            if (pivot[0] <= 90 && pivot[0] >= 65){

                if (rm.find(pivot) == rm.end()) rm[pivot] = std::vector<std::string>();
            
                auto it = m_.second.begin();
                it++;
                while (it != m_.second.end()){
                    m[*it] = pivot;
                    auto it2 = rm.find(*it);
                    if (it2 != rm.end()){
                        for (auto const & i:it2->second){
                            m[i] = pivot;
                        }
                        rm.erase(it2);
                    }
                    rm[pivot].push_back(*it);
                    it++;
                }

                m.erase(pivot);

            }

        }
    }
    m__ = m;

    for (auto & m_: m2){

        auto val = m_.second.front(); 

        auto it = m.find(val);
        while (it != m.end()){
            val = it->second;
            it = m.find(val);
        }

        if (m_.first[0] <= 90 && m_.first[0] >= 65) m3[m_.first] = val;

    }

    if (!m3.empty()){
        std::vector<std::map<std::string, std::string>> blacklist_;
    
        for (auto & b : blacklist){
            if (updateKeys(b, m3)) blacklist_.push_back(b);
        } 

        blacklist = blacklist_;
    }
    
}

std::map<std::string, std::string> submap(std::map<std::string, std::string> const & m, std::list<std::string> const & keys){
    std::map<std::string, std::string> subMap;
    for (auto const & key: keys){
        auto el = m.find(key);
        if (el != m.end()) subMap[key] = el->second;
    }
    return subMap;
}

std::vector<std::map<std::string,std::string>> FilterRaw(std::vector<std::map<std::string,std::string>> vec) {
    std::vector<std::map<std::string,std::string>> out;
    for (auto const & m : vec){
        std::map<std::string,std::string> m_;
        for (auto const & el:m){
            if (el.second != ""){
                m_[el.first] = el.second;
            }
        }
        if (!m_.empty()) out.push_back(m_);
    }
    return out;
}

RuleBlackListHandle::RuleBlackListHandle(std::vector<Predicate> const & premisses, std::vector<std::map<std::string, std::string>> const & blacklist, std::map<std::string, std::list<std::string>> const & m, std::map<std::string, std::string> & m_, std::map<std::string, std::string> & m3)
    :_blacklist(blacklist),_currentBlackList(){

    update(_blacklist,m,m_,m3);

    _blacklist = FilterRaw(_blacklist);

    std::list<std::string> variables;
        
    for (auto const & p : premisses){

        auto premise = p.toNewPredicate(m_);

        auto vars = premise.getVariables();
        
        vars.erase(std::remove_if(vars.begin(),vars.end(),[&variables](std::string const & variable){
        
            return (std::find(variables.begin(), variables.end(), variable) != variables.end());
        
        }),vars.end());
        
        for (auto const & var: vars) variables.push_back(var);
        
        _variablesParameters.push_back(vars);
    }
    _current_it = _variablesParameters.begin();
    
    _currentBlackList.push(std::vector<std::map<std::string, std::string>>());
}

bool RuleBlackListHandle::check(std::map<std::string, std::string> const & m) const{

    /*
    for (auto const & el:m) std::cout << el.first << " " << el.second << std::endl;
    std::cout << std::endl;
    */

    if (_blacklist.empty()) return true;
    for (auto const & bl : _blacklist){
        bool good = (bl.empty() ? true : false);
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

void RuleBlackListHandle::inc(std::map<std::string, std::string> m){
    m = updateMap(m);
    std::map<std::string, std::string> subMap = submap(m,*_current_it);
    _currentBlackList.top().push_back(subMap);
    _current_it++;
    _currentBlackList.push(std::vector<std::map<std::string, std::string>>());
}