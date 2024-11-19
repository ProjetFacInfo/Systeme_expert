#pragma once

#include <vector>
#include <iostream>
#include <memory>
#include <algorithm>
#include <stack>
#include <list>

#include "Fact.hh"
#include "Rule.hh"
#include "RuleBlackListHandle.hh"

enum class Strategy{
    FORWARD, BACKWARD
};

enum class RuleChoice{
    DEFAULT, NB_PREMISES_DESC
};

class Engine{
public:
    static Strategy STRATEGY;
    static bool TRACE;
    static RuleChoice RULE_CHOICE;
    static std::unique_ptr<Predicate> GOAL;
    static void setGoal(std::string goal);
private:
    std::vector<Fact> _facts;
    std::vector<Rule> _rules;

    std::vector<std::string> _backwardBreakLoop; 
public:
    Engine():_facts(),_rules(),_backwardBreakLoop(){}
    void addFact(Fact const & fact){_facts.push_back(fact);}
    void addRule(Rule const & rule){_rules.push_back(rule);}

    void run();

    void forwardChaining();
    bool backwardChaining_(std::vector<std::string> *logs, std::map<std::string, std::string> *m, std::vector<std::map<std::string, std::string>> const & blacklist, Predicate const &goal) ;
    void backwardChaining();


    void sortRulesByNbPremisesDesc();
};