#include "Engine.hh"

Strategy Engine::STRATEGY = Strategy::FORWARD;
std::unique_ptr<Predicate> Engine::GOAL = nullptr;

std::vector<std::string> split(std::string s, const std::string& delimiter) {
    std::vector<std::string> tokens;
    size_t pos = 0;
    std::string token;
    while ((pos = s.find(delimiter)) != std::string::npos) {
        token = s.substr(0, pos);
        tokens.push_back(token);
        s.erase(0, pos + delimiter.length());
    }
    tokens.push_back(s);

    return tokens;
}

void Engine::setGoal(std::string goal)
{

    goal.erase(std::remove(goal.begin(), goal.end(), ' '), goal.end());

    std::string delimiter = "(";
    std::string name = goal.substr(0, goal.find(delimiter)); 

    std::vector<Parameter> parameters;

    std::vector<std::string> parameters_names = split(goal.substr(goal.find(delimiter)+1, goal.length()-2-goal.find(delimiter)) ,",");

    for (auto parameter_name : parameters_names){
        parameters.push_back(Parameter(TypeParameter::CONSTANT, parameter_name));
    }

    Engine::GOAL = std::make_unique<Predicate>(Predicate(name, parameters, true));

    std::cout << Engine::GOAL->toString() << std::endl;

}

void Engine::run()
{
    switch (Engine::STRATEGY)
    {
    case Strategy::FORWARD:
        forwardChaining();
        break;
    case Strategy::BACKWARD:
        backwardChaining();
        break;
    default:
        break;
    }
}

void Engine::forwardChaining()
{
    // TODO
}

void Engine::backwardChaining()
{
    // TODO
}
