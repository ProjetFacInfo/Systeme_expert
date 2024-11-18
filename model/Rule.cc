#include "Rule.hh"

std::string Rule::toString() const {
	std::string s(_name+" ");

	auto it = _premises.end()-1;

	while (it != _premises.begin()){
		s += it->toString()+" ET ";
		it--;
	}

	s += it->toString()+" -> ";
	s += _consequent.toString();

	return s;
}

std::string Rule::toString(std::map<std::string, std::string> const &m) const
{
	std::string s(_name+" ");

	auto it = _premises.end()-1;
	while (it != _premises.begin()){
		s += it->toNewPredicate(m).toString()+" ET ";
		it--;
	}

	s += it->toNewPredicate(m).toString()+" -> ";
	s += _consequent.toNewPredicate(m).toString();

	return s;
}

std::shared_ptr<std::vector<Fact>> Rule::checkPremise(std::vector<Fact> const & facts) const
{

	std::vector<Fact> resFacts;

	std::vector<std::map<std::string, std::string>> blacklist;

	bool addNewFacts;

	std::map<std::string, std::string> m_m2;

	do {
		addNewFacts = false;

		std::map<std::string, std::string> m;
		RuleBlackListHandle ruleBlackListHandle(_premises,blacklist,std::map<std::string, std::list<std::string>>(), m , m_m2);

		bool good = true;
		auto it = _premises.begin();
		while (it != _premises.end()){

			auto p = it->toNewPredicate(m);

			bool ok = false;

			for (auto const & fact: facts){
				std::map<std::string, std::string> m_;
				if(p.calc(fact, &m_)){
					std::map<std::string, std::string> tempMap(m);
					insert(tempMap, m_);
					if (check(ruleBlackListHandle.getCurrentBlackList(), tempMap)){
						m = tempMap;
						ok = true;
						break;
					}
				}
			}
			if (!ok) {
				if (it == _premises.begin()){
					good = false;
					break;
				}
				it--;
				ruleBlackListHandle.dec(&m);
			}
			else if (!ruleBlackListHandle.check(m)){
                ruleBlackListHandle.inc(m);
                ruleBlackListHandle.dec(&m);
            }
			else {
				it++;
				ruleBlackListHandle.inc(m);
			}
		}
		if (good){
			
			Predicate p = _consequent.toNewPredicate(m);
			if (!p.isFact()) return nullptr;
			resFacts.push_back(p.toFact());
			addNewFacts = true;
			blacklist.push_back(m);
			
		}
	} while(addNewFacts);

    return std::make_shared<std::vector<Fact>>(resFacts);
}

std::shared_ptr<std::vector<Predicate>> Rule::checkConsequent(Predicate const &predicate, std::map<std::string, std::list<std::string>> * m, std::vector<std::map<std::string,std::string>> const & blacklist, std::map<std::string,std::string> &mLog, std::map<std::string, std::string> &m2) const
{
	if (!_consequent.calc(predicate, m, &m2)) return nullptr;

	/*
	for (auto const & el:*m){
		std::cout << el.first << " [";
		for (auto const & s:el.second) std::cout << s << " ";
		std::cout << "]" << std::endl;
	} 
	*/

	auto newConsequent = _consequent.toNewPredicate(m2);

	auto par1 = predicate.getParameters();
	auto par2 = newConsequent.getParameters();

	for (auto const & b: blacklist){

		auto it1 = par1.begin();
		auto it2 = par2.begin();
		bool good = false;

		while (it1 != par1.end()){

			if (it1->getType()==TypeParameter::VARIABLE){
				if (b.at(it1->getValue()) != it2->getValue()){
					good = true;
					break;
				}
			}
			it1++;
			it2++;
			
		}

		if (!good) return nullptr;
	}

	auto it1 = par1.begin();
	auto it2 = par2.begin();
	while (it1 != par1.end()){
		if (it1->getType()==TypeParameter::VARIABLE && it2->getType()==TypeParameter::CONSTANT){
			auto key = it1->getValue();
			auto value = it2->getValue();
			mLog[key] = value;
		}
		it1++;
		it2++;
	}

	std::vector<Predicate> predicates;
	for (auto const & premise : _premises){
		predicates.push_back(premise.toNewPredicate(m2));
	}

    return std::make_shared<std::vector<Predicate>>(predicates);
}
