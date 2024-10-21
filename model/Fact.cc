#include "Fact.hh"

bool Fact::operator==(Fact const &fact) const
{
    return _name == fact._name && _parameters == fact._parameters && _value == fact._value;
}

std::string Fact::toString() const
{
    std::string msg = "Fact: "+_name+" \n";
    for (auto const & p : _parameters){
        msg+="( " + p.toString() + " )\n";
    }
    msg += _value ? "true" : "false";
    return msg;
}