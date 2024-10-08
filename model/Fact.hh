#pragma once

#include <string>
#include <vector>

#include "Parameter.hh"

class Fact {

private:

    std::string _name;
    std::vector<Parameter> _parameters;

public:

    Fact(std::string name, std::vector<Parameter> parameters):_name(name),_parameters(parameters){}

};

   