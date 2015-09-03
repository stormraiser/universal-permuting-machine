%{

#include "CommandSem.h"

#include <iostream>
#include <vector>
#include <string>

using namespace std;

int yylex(CommandSem **lvalp);
void yyerror (CommandSem **valp, char const *msg) {};

%}

%define api.value.type {CommandSem*}
%define api.pure full
%parse-param {CommandSem **command}

%token IDENTIFIER NUMBER

%%

command :   seq
        {
            $$ = $1;
            *command = $$;
        }
        ;

seq     :   seq elem
        {
            $$ = $1;
            $$->stringList.insert($$->stringList.end(), $2->stringList.begin(), $2->stringList.end());
        }
        |   elem
        {
            $$ = $1;
        }
        ;

elem    :   IDENTIFIER
        {
            $$ = $1;
            $$->stringList.push_back($$->s);
        }
        |   elem NUMBER
        {
            $$ = new CommandSem;
            for (int i = 0; i < $2->i; i++) {
                $$->stringList.insert($$->stringList.end(), $1->stringList.begin(), $1->stringList.end());
            }
        }
        |   '(' seq ')'
        {
            $$ = $2;
        }
        ;
