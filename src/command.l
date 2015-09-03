%top{
#include <string>
#include <sstream>
#include <cstdio>
#include "CommandSem.h"
#include "command_yacc.h"

using namespace std;

}

IDENTIFIER          ([_A-Za-z][_0-9A-Za-z+@#$%&/\-']*)
NUMBER              ([0-9]+)

%option noyywrap
%option bison-bridge

%%

"("                 { return '('; }
")"                 { return ')'; }

{IDENTIFIER}        {
                        *yylval_param = new CommandSem;
                        (*yylval_param)->s = string(yytext, yyleng);
                        return IDENTIFIER;
                    }

{NUMBER}            {
                        *yylval_param = new CommandSem;
                        stringstream(string(yytext, yyleng)) >> (*yylval_param)->i;
                        return NUMBER;
                    }

.                   { }
<<EOF>>             { yyterminate(); }

%%

static YY_BUFFER_STATE state;

void commandScanString(const char *str) {
    state = yy_scan_string(str);
}

void commandDoneString() {
    yy_delete_buffer(state);
}
