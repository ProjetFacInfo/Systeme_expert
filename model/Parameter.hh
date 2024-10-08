#pragma once
#include <string>

enum class TypeParameter{
    CONSTANT, VARIABLE
};

class Parameter{

private:

    TypeParameter _type;
    std::string _name;

public:

    Parameter(TypeParameter type, std::string name):_type(type),_name(name){}

};