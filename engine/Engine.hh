#pragma once

#include <vector>

#include "Fact.hh"
#include "Rule.hh"

class Engine{
private:
    std::vector<Fact> _facts;
    std::vector<Rule> _rules;
public:
    Engine(std::vector<Fact> facts, std::vector<Rule> rules):_facts(facts),_rules(rules){}
};