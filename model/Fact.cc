#include "Fact.hh"

std::string Fact::toString() const
{
    std::string msg = "Fact: "+_name+" \n";
    for (auto const & p : _parameters){
        msg+="( " + p.toString() + " )\n";
    }
    msg += _value ? "true" : "false";
    return msg;
}