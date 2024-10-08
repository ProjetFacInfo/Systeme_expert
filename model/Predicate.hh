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
		Predicate(std::string name, std::vector<Parameter> parameter, bool value) 
			: _name(name), _parameter(parameter), _value(value) {}
};
