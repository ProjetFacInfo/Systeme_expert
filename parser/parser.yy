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

%type <std::shared_ptr<Fact>>                       fact
%type <std::shared_ptr<std::vector<Parameter>>>     parameters
%type <std::shared_ptr<std::vector<Parameter>>>     parameters_rule
%type <std::shared_ptr<Predicate>>                  predicate
%type <std::shared_ptr<std::vector<Predicate>>>     premise
%type <std::shared_ptr<std::vector<Predicate>>>     consequent
%type <std::shared_ptr<Rule>>                       rule

%%

programme:
    fact NL programme {
        std::cout << "fact" << std::endl;
        std::cout << $1->toString() << std::endl;
    }
    | rule NL programme {
        std::cout << "Regle" << std::endl;
    }
    | NL programme
    | END NL {
        std::cout << "fin" << std::endl;
        YYACCEPT;
    }

fact:
    ALPHANUMERIC_CHAIN '(' parameters ')' {
        $$ = std::make_shared<Fact>($1, *$3, true);
        std::cout << $$->toString() << std::endl;
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
        $$ = std::make_shared<Rule>($1, *$2, *$4);
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
    predicate {
        std::shared_ptr<std::vector<Predicate>> v = std::make_shared<std::vector<Predicate>>();
        v->push_back(*$1);
        $$ = v;
    }
    | predicate AND premise {
        $3->push_back(*$1);
        $$ = $3;
    }

predicate:
    ALPHANUMERIC_CHAIN '(' parameters_rule ')' {
        $$ = std::make_shared<Predicate>($1, *$3, true);
    }
    | NO ALPHANUMERIC_CHAIN '(' parameters_rule ')' {
        $$ = std::make_shared<Predicate>($2, *$4, false);
    }

consequent:
    premise {
        $$ = $1;
    }


%%

void yy::Parser::error( const location_type &l, const std::string & err_msg) {
    std::cerr << "Erreur : " << l << ", " << err_msg << std::endl;
}
