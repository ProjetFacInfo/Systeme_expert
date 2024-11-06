#include "Fact.hh"

bool Fact::operator==(Fact const &fact) const
{
    return _name == fact._name && _parameters == fact._parameters && _value == fact._value;
}

std::string Fact::toString() const
{
    std::string msg;

    msg += _value ? "" : "NON ";

    msg += _name+"(";

    auto it = _parameters.end()-1;

    while (it != _parameters.begin()){
        msg+=it->toString()+",";
        it--;
    }

    msg+=it->toString()+")";
    
    return msg;
}