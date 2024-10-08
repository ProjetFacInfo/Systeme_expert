#pragma once

#include <vector>
#include <memory>

#include "Predicate.hh"

class Rule {

	private:
		std::string 					_nom;
		std::vector<std::shared_ptr<Predicate>> 	_premise;
		std::vector<std::shared_ptr<Predicate>> 	_consequence;

	public:
		Rule
		(std::string nom, 
		 std::vector<std::shared_ptr<Predicate>> premise, 
		 std::vector<std::shared_ptr<Predicate>> consequence)
			: _nom(nom), _premise(premise), _consequence(consequence) {}


};
