%top{
#include <string>
#include <sstream>
#include <cstdio>
#include "CubeSem.h"
#include "cube_yacc.h"

using namespace std;
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
{WHITESPACE}        { }
{NEWLINE}           { }
{LINE_COMMENT}      { }
{STRING_LITERAL}    {
                        *yylval_param = new CubeSem(CubeSem::semString);
                        (*yylval_param)->string1 = string(yytext + 1, yyleng - 2);
                        return STRING;
                    }

","                 { return ','; }
";"                 { return ';'; }
"{"                 { return '{'; }
"}"                 { return '}'; }
"("                 { return '('; }
")"                 { return ')'; }
"="                 { return '='; }
"alias"             { return ALIAS; }
"at"                { return AT; }
"auto"              { return AUTO; }
"axisangle"         { return AXISANGLE; }
"bandage"           { return BANDAGE; }
"binding"           { return BINDING; }
"block"             { return BLOCK; }
"click"             { return CLICK; }
"cycle"             { return CYCLE; }
"equivalence"       { return EQUIVALENCE; }
"forbid"            { return FORBID; }
"geometry"          { return GEOMETRY; }
"group"             { return GROUP; }
"include"           { return INCLUDE; }
"inverse"           { return INVERSE; }
"invoke"            { return INVOKE; }
"keyboard"          { return KEYBOARD; }
"metadata"          { return METADATA; }
"model"             { return MODEL; }
"operation"         { return OPERATION; }
"position"          { return POSITION; }
"remove"            { return REMOVE; }
"require"           { return REQUIRE; }
"rotate"            { return ROTATE; }
"shift"             { return SHIFT; }
"start"             { return START; }
"symmetry"          { return SYMMETRY; }
"tag"               { return TAG; }
"translate"         { return TRANSLATE; }

{IDENTIFIER}        {
                        *yylval_param = new CubeSem(CubeSem::semIdentifier);
                        (*yylval_param)->string1 = string(yytext, yyleng);
                        return IDENTIFIER;
                    }

{NUMBER}            {
                        *yylval_param = new CubeSem(CubeSem::semNumber);
                        stringstream(string(yytext, yyleng)) >> (*yylval_param)->number;
                        return NUMBER;
                    }

.                   { }
<<EOF>>             { yyterminate(); }
