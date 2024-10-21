#pragma once

#include <string>
#include <vector>
#include <map>

#include "Parameter.hh"
#include "Fact.hh"

class Predicate {

	private:
		std::string 		_name;
		std::vector<Parameter> _parameters;
		bool 			_value;
	
	public:
		Predicate(std::string const & name, std::vector<Parameter> const & parameters, bool value) 
			: _name(name), _parameters(parameters), _value(value) {}

		std::string toString() const;

		bool calc(Predicate const & predicate, std::map<std::string, std::string> * varToConst) const;

		Predicate toNewPredicate(std::map<std::string, std::string> * varToConst);
		bool isFact() const;
		Fact toFact() const;


		std::vector<Parameter> getParameters() const {return _parameters;}
};
