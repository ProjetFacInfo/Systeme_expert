#include <string>

#include "Parameter.hh"

class Predicate {

	private:
		std::string 	_name;
		Parameter 	_parameter;
		bool 		_value;
	
	public:
		Predicate(std::string name, Parameter parameter, bool value) 
			: _name(name), _parameter(parameter), _value(value) {}
};
