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

    Parameter(TypeParameter const & type, std::string const & name):_type(type),_name(name){}

    std::string toString() const;


    TypeParameter getType() const {
        return _type;
    }

    std::string getName() const {
        return _name;
    }

};