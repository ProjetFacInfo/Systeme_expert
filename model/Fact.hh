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

    Fact(std::string const & name, std::vector<Parameter> const & parameters, bool value):_name(name),_parameters(parameters),_value(value){}

    bool operator==(Fact const & fact) const;

    std::string toString() const;

    std::string getName() const {
        return _name;
    }

    std::vector<Parameter> getParameters() const {
        return _parameters;
    }

    bool getValue() const {
        return _value;
    }

};

   
