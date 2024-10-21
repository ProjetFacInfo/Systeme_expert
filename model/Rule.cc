#include "Rule.hh"

std::string Rule::toString() const {
	std::string s = "Rule name : " + _name  + "\n";

	s += "premise : \n";
	for (auto const & p : _premises) {
		s += p.toString();
	}
	s += "\n";

	s += "consequent : \n";
	s += _consequent.toString();
	s += "\n";

	return s;
}

std::shared_ptr<std::vector<Fact>> Rule::checkPremise(std::vector<Fact> const &facts) const
{
	std::map<std::string, std::string> m;
	bool good = true;

	for (auto const & premise : _premises){
		good = false;
		for (auto const & fact : facts){
			if (premise.calc(fact, &m)){
				good = true;
				break;
			}
		}
		if (!good) return nullptr;
	}

	std::vector<Fact> resFacts;

	Predicate p = _consequent.toNewPredicate(m);
	if (!p.isFact()) return nullptr;
	resFacts.push_back(p.toFact());

    return std::make_shared<std::vector<Fact>>(resFacts);
}

std::shared_ptr<std::vector<Predicate>> Rule::checkConsequent(Predicate const &predicate, std::map<std::string, std::string> * m) const
{
	std::map<std::string, std::string> m2;

	if (!_consequent.calc(predicate, m, &m2)) return nullptr;

	std::vector<Predicate> predicates;
	for (auto const & premise : _premises){
		predicates.push_back(premise.toNewPredicate(m2));
	}

    return std::make_shared<std::vector<Predicate>>(predicates);
}
