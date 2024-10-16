#include "Predicate.hh"


std::string Predicate::toString() const {
	std::string s =  "Predicate name : " + _name + "\n";
		
	s += "parameter : \n "; 

	for ( auto const & v : _parameter ) {
		s += v.toString();
	}
	s += "\n";

	s += "value : "; 

	s += _value ? "true\n" : "false\n";

	return s;
}