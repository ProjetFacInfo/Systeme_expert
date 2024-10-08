#pragma once

#include <string>
#include <vector>

#include "Parameter.hh"

class Fact {

private:

    std::string _name;
    std::vector<Parameter> _parameters;
    bool _value;

public:

    Fact(std::string name, std::vector<Parameter> parameters, bool value):_name(name),_parameters(parameters),_value(value){}

    std::string toString() const;

};

   