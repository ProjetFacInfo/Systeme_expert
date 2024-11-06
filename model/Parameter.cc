#include "Parameter.hh"

bool Parameter::operator==(Parameter const &parameter) const
{
    return _value == parameter._value && parameter._type == _type;
}

std::string Parameter::toString() const
{
    std::string msg;
    /*
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
    */
    msg += _value;
    return msg;
}
