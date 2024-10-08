#pragma once

#include <vector>

#include "Predicate.hh"

class Rule {

	private:
		std::string _nom;
		std::vector<Predicate> _premise;
		std::vector<Predicate> _consequence;

	public:
		Rule(std::string nom, std::vector<Predicate> premise, std::vector<Predicate> consequence)
			: _nom(nom), _premise(premise), _consequence(consequence) {}


};
