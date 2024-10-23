#pragma once

#include <vector>
#include <iostream>
#include <memory>
#include <algorithm>

#include "Fact.hh"
#include "Rule.hh"

enum class Strategy{
    FORWARD, BACKWARD
};

class Engine{
public:
    static Strategy STRATEGY;
    static std::unique_ptr<Predicate> GOAL;
    static void setGoal(std::string goal);
private:
    std::vector<Fact> _facts;
    std::vector<Rule> _rules;
public:
    Engine():_facts(),_rules(){}
    void addFact(Fact const & fact){_facts.push_back(fact);}
    void addRule(Rule const & rule){_rules.push_back(rule);}

    void run();

    void forwardChaining();
    bool backwardChaining_(std::vector<std::string> *logs, std::map<std::string, std::string> *m, std::map<std::string, std::vector<std::string>> const & blacklist, Predicate const &goal) const ;
    void backwardChaining() const;
};