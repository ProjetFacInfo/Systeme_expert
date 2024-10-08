%skeleton "lalr1.cc"
%require "3.0"

%defines
%define api.parser.class { Parser }
%define api.value.type variant
%define parse.assert

%locations

%code requires{
    #include <memory>

    #include "contexte.hh"
    #include "Fact.hh"
    #include "Rule.hh"

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

%type <std::shared_ptr<Fact>>   fact
%type <std::shared_ptr<std::vector<Parameter>>>     parameters
%type <std::shared_ptr<std::vector<Parameter>>>     parameters_rule

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
    ALPHANUMERIC_CHAIN '(' parameters ')' {
        $$ = std::make_shared<Fact>($1, *$3, true);
    }
    | NO ALPHANUMERIC_CHAIN '(' parameters ')' {
        $$ = std::make_shared<Fact>($2, *$4, false);
    }

parameters:
    ALPHANUMERIC_CHAIN {
        std::shared_ptr<std::vector<Parameter>> v = std::make_shared<std::vector<Parameter>>();
        v->push_back(Parameter(TypeParameter::CONSTANT, $1));
        $$ = v;
    }
    | ALPHANUMERIC_CHAIN ',' parameters {
        $3->push_back(Parameter(TypeParameter::CONSTANT, $1));
        $$ = $3;
    }

rule:
    ALPHANUMERIC_CHAIN premise ARROW consequent {

    }

parameters_rule:
    ALPHANUMERIC_CHAIN {
        std::shared_ptr<std::vector<Parameter>> v = std::make_shared<std::vector<Parameter>>();
        v->push_back(Parameter(TypeParameter::CONSTANT, $1));
        $$ = v;
    }
    | VARIABLE {
        std::shared_ptr<std::vector<Parameter>> v = std::make_shared<std::vector<Parameter>>();
        v->push_back(Parameter(TypeParameter::VARIABLE, $1));
        $$ = v;
    }
    | ALPHANUMERIC_CHAIN ',' parameters_rule {
        $3->push_back(Parameter(TypeParameter::CONSTANT, $1));
        $$ = $3;
    }
    | VARIABLE ',' parameters_rule {
        $3->push_back(Parameter(TypeParameter::VARIABLE, $1));
        $$ = $3;
    }

premise:
    element {

    }
    | element AND premise {

    }

element:
    ALPHANUMERIC_CHAIN '(' parameters_rule ')' {

    }
    | NO ALPHANUMERIC_CHAIN '(' parameters_rule ')' {

    }

consequent:
    premise {
        //$$ = $1;
    }


%%

void yy::Parser::error( const location_type &l, const std::string & err_msg) {
    std::cerr << "Erreur : " << l << ", " << err_msg << std::endl;
}
