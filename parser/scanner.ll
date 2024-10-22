%{

#include "scanner.hh"
#include <cstdlib>

#define YY_NO_UNISTD_H

using token = yy::Parser::token;

#undef  YY_DECL
#define YY_DECL int Scanner::yylex( yy::Parser::semantic_type * const lval, yy::Parser::location_type *loc )

/* update location on matching */
#define YY_USER_ACTION loc->step(); loc->columns(yyleng);

%}

%option c++
%option yyclass="Scanner"
%option noyywrap

%%
%{
    yylval = lval;
%}


<<EOF>> return token::END;

"fin" return token::END;

"NON" {
    return token::NO;
}

"ET" {
    return token::AND;
}

"->" {
    return token::ARROW;
}

"(" {
    return '(';
}

")" {
    return ')';
}

"," {
    return ',';
}

[A-Z][a-zA-Z0-9_]* {
    yylval->build<std::string>(YYText());
    return token::VARIABLE;
}

[a-zA-Z0-9_]+ {
    yylval->build<std::string>(YYText());
    return token::ALPHANUMERIC_CHAIN;
}

"\n"          {
    loc->lines();
    return token::NL;
}

[ \t] {
    
}

%%
