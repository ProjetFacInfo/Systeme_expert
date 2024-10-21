#pragma once
#include <string>

enum class TypeParameter{
    CONSTANT, VARIABLE
};

class Parameter{

private:
    TypeParameter _type;
    std::string _value;

public:

    Parameter(TypeParameter const & type, std::string const & value):_type(type),_value(value){}

    std::string toString() const;


    TypeParameter getType() const {
        return _type;
    }

    std::string getValue() const {
        return _value;
    }

};