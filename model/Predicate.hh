#pragma once

#include <string>
#include <vector>
#include <map>

#include "Parameter.hh"
#include "Fact.hh"

class Predicate {

	private:
		std::string 		_name;
		std::vector<Parameter> 	_parameter;
		bool 			_value;
	
	public:
		Predicate(std::string const & name, std::vector<Parameter> const & parameter, bool value) 
			: _name(name), _parameter(parameter), _value(value) {}

		std::string toString() const;

		bool calc(Fact const & fact, std::map<std::string, std::string> * varToConst) const;
		Predicate toNewPredicate(std::map<std::string, std::string> * varToConst);
		Fact toFact(std::map<std::string, std::string> * varToConst) const;

};
