%skeleton "lalr1.cc"
%require "3.0"

%defines
%define api.parser.class { Parser }
%define api.value.type variant
%define parse.assert

%locations

%code requires{
    #include "contexte.hh"

    class Scanner;
    class Driver;
}

%parse-param { Scanner &scanner }
%parse-param { Driver &driver }

%code{
    #include <iostream>
    #include <string>
    #include <memory>
    
    #include "scanner.hh"
    #include "driver.hh"

    #undef  yylex
    #define yylex scanner.yylex
}

%token                  NL
%token                  END
%token                  NO
%token                  AND
%token                  ARROW
%token <std::string>    VARIABLE
%token <std::string>    ALPHANUMERIC_CHAIN

%%

programme:
    fact NL programme {
        
    }
    | rule NL programme {
        
    }
    | NL programme
    | END NL {
        YYACCEPT;
    }

fact:
    constant {

    }
    | NO constant {

    }

constant:
    ALPHANUMERIC_CHAIN '(' parameters ')' {

    }

parameters:
    ALPHANUMERIC_CHAIN {

    }
    | ALPHANUMERIC_CHAIN ',' parameters {

    }

rule:
    ALPHANUMERIC_CHAIN premise ARROW consequent {

    }

parameters_rule:
    ALPHANUMERIC_CHAIN {

    }
    | VARIABLE {

    }
    | ALPHANUMERIC_CHAIN ',' parameters_rule {

    }
    | VARIABLE ',' parameters_rule {

    }

premise:
    element {

    }
    | element AND premise {

    }

element:
    ALPHANUMERIC_CHAIN '(' parameters_rule ')' {

    }

consequent:
    premise {
        //$$ = $1;
    }


%%

void yy::Parser::error( const location_type &l, const std::string & err_msg) {
    std::cerr << "Erreur : " << l << ", " << err_msg << std::endl;
}
