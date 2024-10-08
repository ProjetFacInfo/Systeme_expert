#include <string>

#include "Parameter.hh"

class Predicate {

	private:
		std::string 	_nom;
		Parameter 	_parameter;
		bool 		_valeur;
	
	public:
		Predicate(std::string nom, Parameter parameter, bool valeur) 
			: _nom(nom), _parameter(parameter), _valeur(valeur) {}
};
