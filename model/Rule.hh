#pragma once

#include <vector>
#include <memory>
#include <iostream>

#include "Predicate.hh"

class Rule {

	private:
		std::string 		_name;
		std::vector<Predicate> 	_premises;
		Predicate 	_consequent;

	public:
		Rule
		(std::string const & name, 
		 std::vector<Predicate> const & premise, 
		 Predicate const & consequent)
			: _name(name), _premises(premise), _consequent(consequent) {}

		std::string toString() const;

		std::shared_ptr<std::vector<Fact>> checkPremise(std::vector<Fact> const & facts) const;
		std::shared_ptr<std::vector<Predicate>> checkConsequent(Predicate const & predicate, std::map<std::string, std::string> * m) const;

};
