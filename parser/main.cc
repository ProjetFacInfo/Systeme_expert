#include "parser.hpp"
#include "scanner.hh"
#include "driver.hh"

#include <iostream>
#include <fstream>

#include <cstring>

#include <stdio.h>  
#include <unistd.h>  

#include "Engine.hh"

int main( int  argc, char* argv[]) {

    std::string str;

    int opt;

    while((opt = getopt(argc, argv, "tf:g:s:")) != -1){
        switch (opt)
        {
            case 'f':
            {
                str = optarg;
                break;
            }
            case 'g':
            {
                Engine::STRATEGY = Strategy::BACKWARD;
                Engine::setGoal(optarg);
                break;
            }
            case 't':
            {
                Engine::TRACE = true;
                break;
            }
            case 's':
            {
                if (std::string(optarg) == "1"){
                    Engine::RULE_CHOICE = RuleChoice::NB_PREMISES_DESC;
                }
                else if (std::string(optarg) == "2"){
                    Engine::RULE_CHOICE = RuleChoice::RECENT_PREMISE;
                }
                else {
                    Engine::RULE_CHOICE = RuleChoice::DEFAULT;
                }
                break;
            }
            default:
                break;
        }
    }

    Driver * driver = new Driver;
    if (str != ""){
        std::ifstream file(str);
        Scanner * scanner = new Scanner(file, std::cout);
        yy::Parser * parser = new yy::Parser(*scanner, *driver);
        parser->parse();    
    }
    else {
        Scanner * scanner = new Scanner(std::cin, std::cout);
        yy::Parser * parser = new yy::Parser(*scanner, *driver);
        parser->parse();    
    }
    
    
    return 0;
}
