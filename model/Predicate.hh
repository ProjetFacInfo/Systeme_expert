#include <string>

#include "Parameter.hh"

class Predicate {

	private:
		std::string _nom;
		Parameter _parameter;
	
	public:
		Predicate(std::string nom, Parameter parameter) 
			: _nom(nom), _parameter(parameter) {}
};
