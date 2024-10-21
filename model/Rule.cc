#include "Rule.hh"

std::string Rule::toString() const {
	std::string s = "Rule name : " + _name  + "\n";

	s += "premise : \n";
	for (auto const & p : _premise) {
		s += p.toString();
	}
	s += "\n";

	s += "consequent : \n";
	for (auto const & c : _consequent) {
		s += c.toString();
	}
	s += "\n";

	return s;
}

std::shared_ptr<std::vector<Fact>> Rule::checkPremise(std::vector<Fact> const &facts) const
{
	// TODO
    return nullptr;
}

std::shared_ptr<std::vector<Predicate>> Rule::checkConsequent(Predicate const &fact) const
{
	// TODO
    return nullptr;
}
