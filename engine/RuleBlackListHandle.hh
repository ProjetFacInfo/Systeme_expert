#pragma once

#include <vector>
#include <stack>
#include <list>
#include <map>
#include <string>
#include <vector>
#include <algorithm>

#include "Predicate.hh"

class RuleBlackListHandle {
private:
    std::vector<std::map<std::string, std::string>> _blacklist;
    std::stack<std::vector<std::map<std::string, std::string>>> _currentBlackList;
    std::list<std::list<std::string>> _variablesParameters;
    std::list<std::list<std::string>>::iterator _current_it;
public:

    RuleBlackListHandle(std::vector<Predicate> const & premisses, std::vector<std::map<std::string, std::string>> const & blacklist, std::map<std::string, std::list<std::string>> const & m, std::map<std::string, std::string> & m_, std::map<std::string, std::string> & m3);

    bool check(std::map<std::string, std::string> const & m) const;

    std::vector<std::map<std::string,std::string>> getCurrentBlackList(){return _currentBlackList.top();}

    void dec(std::map<std::string, std::string> * m);

    void inc(std::map<std::string, std::string> const & m);

};

bool check(std::vector<std::map<std::string, std::string>> const & blacklist, std::map<std::string, std::string> const & m);

void insert(std::map<std::string,std::string>* m1, std::map<std::string,std::string> const & m2);

void updateValues(std::map<std::string,std::string>* m1, std::map<std::string,std::string> const & m2);