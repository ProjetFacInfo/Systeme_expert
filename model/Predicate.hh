#pragma once

#include <string>
#include <vector>
#include <map>
#include <iostream>
#include <list>
#include <algorithm>

#include "Parameter.hh"
#include "Fact.hh"

class Predicate {

	private:
		std::string _name;
		std::vector<Parameter> _parameters;
		bool _value;

	public:
		Predicate(std::string const & name, std::vector<Parameter> const & parameters, bool value) 
			: _name(name), _parameters(parameters), _value(value) {}

		std::string toString() const;

		bool calc(Predicate const & predicate, std::map<std::string, std::list<std::string>> * p1top2, std::map<std::string, std::string> * p2top1) const;
		bool calc(Fact const & fact, std::map<std::string, std::string> * varToConst) const;
		Predicate toNewPredicate(std::map<std::string, std::string> const & varToConst) const;
		bool isFact() const;
		Fact toFact() const;
		std::list<std::string> getVariables() const;


		std::vector<Parameter> getParameters() const {return _parameters;}

		std::string getName() const {
			return _name;
		}

		bool getValue() const {
			return _value;
		}
};
