#pragma once

#include <string>
#include <vector>

#include "Parameter.hh"

class Predicate {

	private:
		std::string 		_name;
		std::vector<Parameter> 	_parameter;
		bool 			_value;
	
	public:
		Predicate(std::string const & name, std::vector<Parameter> const & parameter, bool value) 
			: _name(name), _parameter(parameter), _value(value) {}

		std::string toString() const;

};
