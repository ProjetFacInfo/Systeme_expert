#pragma once

#include <vector>

#include "Predicate.hh"

class Rule {

	private:
		std::string 					_name;
		std::vector<Predicate> 	_premise;
		std::vector<Predicate> 	_consequence;

	public:
		Rule
		(std::string name, 
		 std::vector<Predicate> premise, 
		 std::vector<Predicate> consequence)
			: _name(name), _premise(premise), _consequence(consequence) {}


};
