#include "Predicate.hh"


std::string Predicate::toString() const {
	std::string s;

	s += _value ? "" : "NON";
		
	s += _name + "("; 

	auto it = _parameters.end()-1;

	while(it != _parameters.begin()){
		s+= (it->toString()+",");
		it--;
	}

	s+= (it->toString()+")");

	return s;
}

bool Predicate::calc(Predicate const & predicate, std::map<std::string, std::list<std::string>> * p1top2, std::map<std::string, std::string> * p2top1) const
{
	if (this->getName() != predicate.getName() || this->getValue() != predicate.getValue() || _parameters.size() != predicate._parameters.size()) return false;
	auto it1 = _parameters.begin();
	auto it2 = predicate._parameters.begin();
	while(it1 != _parameters.end()){
		if (it1->getType() == TypeParameter::VARIABLE && it2->getType() == TypeParameter::CONSTANT)
			(*p2top1)[it1->getValue()] = it2->getValue();
		else if (it1->getType() == TypeParameter::CONSTANT && it2->getType() == TypeParameter::CONSTANT && it1->getValue() != it2->getValue()) 
			return false;
		else if (it2->getType() == TypeParameter::VARIABLE && it1->getType() == TypeParameter::CONSTANT){
			auto it = p1top2->find(it2->getValue());
			if (it == p1top2->end()){
				(*p1top2)[it2->getValue()] = std::list<std::string>();
				(*p1top2)[it2->getValue()].push_back(it1->getValue());
			}
			else{
				if ((*p1top2)[it2->getValue()].front()[0] <= 90 && (*p1top2)[it2->getValue()].front()[0] >=65){
					(*p1top2)[it2->getValue()].push_front(it1->getValue());
				}
				else if ((*p1top2)[it2->getValue()].front() != it1->getValue()) return false;
			}
		}
		else{
			auto it = p1top2->find(it2->getValue());
			if (it == p1top2->end()){
				(*p1top2)[it2->getValue()] = std::list<std::string>();
			}
			(*p1top2)[it2->getValue()].push_back(it1->getValue());
		}
		it1++;
		it2++;
	}
	for (auto const & el: *p1top2){
		auto val = el.second.front();
		if (val[0] > 90 ||  val[0] < 65){
			auto it = el.second.begin();
			it++;
			while (it != el.second.end()){
				auto it2 = p1top2->find(*it);
				if (it2 != p1top2->end() && (it2->second.front()[0] > 90 || it2->second.front()[0] < 65) && it2->second.front() != val) return false;
				(*p2top1)[*it] = val;
			}
		}
	}

    return true;
}

bool Predicate::calc(Fact const &fact, std::map<std::string, std::string> *varToConst) const
{
	if (this->getName() != fact.getName() || this->getValue() != fact.getValue() || _parameters.size() != fact.getParameters().size()) return false;

	auto it1 = _parameters.begin();
	auto parameters_fact = fact.getParameters();
	auto it2 = parameters_fact.begin();
	while(it1 != _parameters.end()){
		if (it1->getType() == TypeParameter::VARIABLE){
			if (varToConst->find(it1->getValue()) != varToConst->end()){
				if ((*varToConst)[it1->getValue()] != it2->getValue()) return false;
			}
			else (*varToConst)[it1->getValue()] = it2->getValue();
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
		else{
			auto it2 = varToConst.find(it->second);
			while (it2 != varToConst.end()){
				it = it2;
				it2 = varToConst.find(it->second);
			}
			parameters.push_back(Parameter((it->second[0] >= 65 && it->second[0] <= 90)? TypeParameter::VARIABLE: TypeParameter::CONSTANT,it->second));
		}
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

std::list<std::string> Predicate::getVariables() const
{
	std::list<std::string> variables;
	for (auto const & parameter : _parameters){
		if (parameter.getType() == TypeParameter::VARIABLE) variables.push_back(parameter.getValue());
	}
    return variables;
}
