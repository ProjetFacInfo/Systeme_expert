#include "parser.hpp"
#include "scanner.hh"
#include "driver.hh"

#include <iostream>
#include <fstream>

#include <cstring>

int main( int  argc, char* argv[]) {

    if (argc > 1){
        Driver * driver = new Driver;
        std::ifstream file(argv[1]);
        Scanner * scanner = new Scanner(file, std::cout);
        yy::Parser * parser = new yy::Parser(*scanner, *driver);

        parser->parse();
    }
    else {
        Driver * driver = new Driver;
        Scanner * scanner = new Scanner(std::cin, std::cout);
        yy::Parser * parser = new yy::Parser(*scanner, *driver);

        parser->parse();
    }
    
    
    return 0;
}
