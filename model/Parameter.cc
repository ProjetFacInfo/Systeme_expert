#include "Parameter.hh"

std::string Parameter::toString() const
{
    std::string msg;
    switch (_type)
    {
    case TypeParameter::CONSTANT:
        msg+="CONSTANT ";
        break;
    case TypeParameter::VARIABLE:
        msg+="VARIABLE ";
        break;
    default:
        break;
    }
    msg += _name;
    return msg;
}
