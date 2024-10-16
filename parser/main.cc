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

    Scanner * scanner = new Scanner(std::cin, std::cout);

    int opt;

    while((opt = getopt(argc, argv, "f:g:")) != -1){
        switch (opt)
        {
            case 'f':
            {
                std::ifstream file(optarg);
                scanner = new Scanner(file, std::cout);
                break;
            }
            case 'g':
            {
                Engine::STRATEGY = Strategy::BACKWARD;
                Engine::setGoal(optarg);
                break;
            }
            default:
                break;
            }
    }

    Driver * driver = new Driver;
    yy::Parser * parser = new yy::Parser(*scanner, *driver);
    parser->parse();    
    
    return 0;
}
