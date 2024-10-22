#include "Predicate.hh"


std::string Predicate::toString() const {
	std::string s =  "Predicate name : " + _name + "\n";
		
	s += "parameter : \n "; 

	for ( auto const & v : _parameters ) {
		s += v.toString();
	}
	s += "\n";

	s += "value : "; 

	s += _value ? "true\n" : "false\n";

	return s;
}

bool Predicate::calc(Predicate const & predicate, std::map<std::string, std::string> * p1top2, std::map<std::string, std::string> * p2top1) const
{
	if (this->getName() != predicate.getName() || this->getValue() != predicate.getValue() || _parameters.size() != predicate.getParameters().size()) return false;
	auto it1 = _parameters.begin();
	auto it2 = predicate._parameters.begin();
	while(it1 != _parameters.end()){
		if (it1->getType() == TypeParameter::VARIABLE && it2->getType() == TypeParameter::CONSTANT){
			(*p2top1)[it1->getValue()] = it2->getValue();
		}
		else if (it2->getType() == TypeParameter::VARIABLE && it1->getType() == TypeParameter::CONSTANT){
			(*p1top2)[it2->getValue()] = it1->getValue();
		}
		else if (it1->getType() == TypeParameter::CONSTANT && it2->getType() == TypeParameter::CONSTANT && it1->getValue() != it2->getValue()) return false;
		it1++;
		it2++;
	}
    return true;
}

bool Predicate::calc(Fact const &fact, std::map<std::string, std::string> *varToConst) const
{
	if (this->getName() != fact.getName() || this->getValue() != fact.getValue() || _parameters.size() != fact.getParameters().size()) return false;
	auto it1 = _parameters.begin();
	auto it2 = fact.getParameters().begin();
	while(it1 < _parameters.end()){
		if (it1->getType() == TypeParameter::VARIABLE){
			(*varToConst)[it1->getValue()] = it2->getValue();
		}
		else if (it1->getType() == TypeParameter::CONSTANT && it1->getValue() != it2->getValue()) return false;
		it1++;
		it2++;
	}
    return true;
}

Predicate Predicate::toNewPredicate(std::map<std::string, std::string> const & varToConst) const
{
	std::vector<Parameter> parameters;
	for (auto const & parameter : _parameters){
		auto it = varToConst.find(parameter.getValue());
		if (it == varToConst.end()) parameters.push_back(Parameter(parameter.getType(),parameter.getValue()));
		else parameters.push_back(Parameter(TypeParameter::CONSTANT,it->second));
	}
    return Predicate(_name,parameters,_value);
}

bool Predicate::isFact() const
{
	for (auto const & parameter : _parameters){
		if (parameter.getType() != TypeParameter::CONSTANT) return false;
	}
    return true;
}

Fact Predicate::toFact() const
{
    return Fact(_name,_parameters,_value);
}
