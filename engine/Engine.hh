#pragma once

#include <vector>

#include "Fact.hh"
#include "Rule.hh"

class Engine{
private:
    std::vector<Fact> _facts;
    std::vector<Rule> _rules;
public:
    Engine():_facts(),_rules(){}
    void addFact(Fact const & fact){_facts.push_back(fact);}
    void addRule(Rule const & rule){_rules.push_back(rule);}
};