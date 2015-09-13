%top{
#include <string>
#include <sstream>
#include <cstdio>
#include "CubeSem.h"
#include "cube_yacc.h"

using namespace std;

#define UPDATE_LOCATION \
    yylloc_param->first_line = yylloc_param->last_line; \
    yylloc_param->first_column = yylloc_param->last_column; \
    for (int i = 0; i < yyleng; i++) { \
        if (yytext[i] == '\n') { \
            yylloc_param->last_line++; \
            yylloc_param->last_column = 1; \
        } \
        else { \
            yylloc->last_column++; \
        } \
    }

}

NEWLINE             (\n|\r)
WHITESPACE          ([ \t]+)
LINE_COMMENT        ("//"[^\n]*\n)
IDENTIFIER          ([_A-Za-z][_0-9A-Za-z+@#$%&/\-']*)
NUMBER              (-?[0-9]*(\.[0-9]*)?)
STRING_LITERAL      \"([^\"\\\n]|(\\['\"?\\abfnrtv])|(\\([0123456]{1,3}))|(\\x[[:xdigit:]]+)|(\\u([[:xdigit:]]{4}))|(\\U([[:xdigit:]]{8})))*\"

%x C
%option noyywrap
%option bison-bridge
%option bison-locations

%%
{WHITESPACE}        { UPDATE_LOCATION }
{NEWLINE}           { UPDATE_LOCATION }
{LINE_COMMENT}      { UPDATE_LOCATION }
{STRING_LITERAL}    {
                        UPDATE_LOCATION
                        *yylval_param = new CubeSem(CubeSem::semString);
                        (*yylval_param)->string1 = string(yytext + 1, yyleng - 2);
                        return STRING;
                    }

","                 { UPDATE_LOCATION return ','; }
":"                 { UPDATE_LOCATION return ':'; }
";"                 { UPDATE_LOCATION return ';'; }
"{"                 { UPDATE_LOCATION return '{'; }
"}"                 { UPDATE_LOCATION return '}'; }
"("                 { UPDATE_LOCATION return '('; }
")"                 { UPDATE_LOCATION return ')'; }
"="                 { UPDATE_LOCATION return '='; }
"alias"             { UPDATE_LOCATION return ALIAS; }
"at"                { UPDATE_LOCATION return AT; }
"auto"              { UPDATE_LOCATION return AUTO; }
"axisangle"         { UPDATE_LOCATION return AXISANGLE; }
"bandage"           { UPDATE_LOCATION return BANDAGE; }
"binding"           { UPDATE_LOCATION return BINDING; }
"block"             { UPDATE_LOCATION return BLOCK; }
"click"             { UPDATE_LOCATION return CLICK; }
"cycle"             { UPDATE_LOCATION return CYCLE; }
"equivalence"       { UPDATE_LOCATION return EQUIVALENCE; }
"forbid"            { UPDATE_LOCATION return FORBID; }
"geometry"          { UPDATE_LOCATION return GEOMETRY; }
"group"             { UPDATE_LOCATION return GROUP; }
"include"           { UPDATE_LOCATION return INCLUDE; }
"inverse"           { UPDATE_LOCATION return INVERSE; }
"invoke"            { UPDATE_LOCATION return INVOKE; }
"keyboard"          { UPDATE_LOCATION return KEYBOARD; }
"metadata"          { UPDATE_LOCATION return METADATA; }
"model"             { UPDATE_LOCATION return MODEL; }
"operation"         { UPDATE_LOCATION return OPERATION; }
"position"          { UPDATE_LOCATION return POSITION; }
"remove"            { UPDATE_LOCATION return REMOVE; }
"require"           { UPDATE_LOCATION return REQUIRE; }
"rotate"            { UPDATE_LOCATION return ROTATE; }
"shift"             { UPDATE_LOCATION return SHIFT; }
"start"             { UPDATE_LOCATION return START; }
"symmetry"          { UPDATE_LOCATION return SYMMETRY; }
"tag"               { UPDATE_LOCATION return TAG; }
"translate"         { UPDATE_LOCATION return TRANSLATE; }

{IDENTIFIER}        {
                        UPDATE_LOCATION
                        *yylval_param = new CubeSem(CubeSem::semIdentifier);
                        (*yylval_param)->string1 = string(yytext, yyleng);
                        return IDENTIFIER;
                    }

{NUMBER}            {
                        UPDATE_LOCATION
                        *yylval_param = new CubeSem(CubeSem::semNumber);
                        stringstream(string(yytext, yyleng)) >> (*yylval_param)->number;
                        return NUMBER;
                    }

.                   { UPDATE_LOCATION }
<<EOF>>             { yyterminate(); }
