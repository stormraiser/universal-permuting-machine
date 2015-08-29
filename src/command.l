%top{
#include <string>
#include <sstream>
#include <cstdio>
#include "CommandSem.h"
#include "command_yacc.h"

#define YYSTYPE CommandSem*

using namespace std;

}

%{
static string buffer;

extern CommandSem *commandlval;
%}

IDENTIFIER          ([_A-Za-z][_0-9A-Za-z+@#$%&/\-']*)
NUMBER              ([0-9]+)

%option noyywrap

%%

"("                 { return '('; }
")"                 { return ')'; }

{IDENTIFIER}        {
                        commandlval = new CommandSem;
                        commandlval->s = string(yytext, yyleng);
                        return IDENTIFIER;
                    }

{NUMBER}            {
                        commandlval = new CommandSem;
                        buffer = string(yytext, yyleng);
                        stringstream(buffer) >> commandlval->i;
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
