#pragma once

#include <vector>

#include "Predicate.hh"

class Rule {

	private:
		std::string 		_name;
		std::vector<Predicate> 	_premise;
		std::vector<Predicate> 	_consequent;

	public:
		Rule
		(std::string const & name, 
		 std::vector<Predicate> const & premise, 
		 std::vector<Predicate> const & consequent)
			: _name(name), _premise(premise), _consequent(consequent) {}

		std::string toString() const;

};
