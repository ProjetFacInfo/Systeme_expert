#pragma once

#include <vector>
#include <memory>

#include "Predicate.hh"

class Rule {

	private:
		std::string 					_name;
		std::vector<std::shared_ptr<Predicate>> 	_premise;
		std::vector<std::shared_ptr<Predicate>> 	_consequence;

	public:
		Rule
		(std::string name, 
		 std::vector<std::shared_ptr<Predicate>> premise, 
		 std::vector<std::shared_ptr<Predicate>> consequence)
			: _name(name), _premise(premise), _consequence(consequence) {}


};
