%top{
#include <string>
#include <sstream>
#include <cstdio>
#include "CubeSem.h"
#include "cube_yacc.h"

using namespace std;

int nullSem(int token);
}

%{
static string buffer;

static int line = 1, column = 1;
%}

NEWLINE             (\n|\r|\n\r)
WHITESPACE          ([ \t]+)
LINE_COMMENT        ("//"[^\n]*\n)
IDENTIFIER          ([_A-Za-z][_0-9A-Za-z+@#$%&/\-']*)
NUMBER              (-?[0-9]*(\.[0-9]*)?)
STRING_LITERAL      L?\"([^\"\\\n]|(\\['\"?\\abfnrtv])|(\\([0123456]{1,3}))|(\\x[[:xdigit:]]+)|(\\u([[:xdigit:]]{4}))|(\\U([[:xdigit:]]{8})))*\"

%x C
%option noyywrap

%%
{WHITESPACE}        { column += yyleng; }
{NEWLINE}           { line += 1; column = 1; }
{LINE_COMMENT}      { column += yyleng; }
{STRING_LITERAL}    {
                        cubelval = new CubeSem(line, column, CubeSem::semString);
                        column += yyleng;
                        cubelval->string1 = string(yytext + 1, yyleng - 2);
                        return STRING;
                    }

","                 { return nullSem(','); }
";"                 { return nullSem(';'); }
"{"                 { return nullSem('{'); }
"}"                 { return nullSem('}'); }
"("                 { return nullSem('('); }
")"                 { return nullSem(')'); }
"="                 { return nullSem('='); }
"alias"             { return nullSem(ALIAS); }
"at"                { return nullSem(AT); }
"auto"              { return nullSem(AUTO); }
"axisangle"         { return nullSem(AXISANGLE); }
"bandage"           { return nullSem(BANDAGE); }
"binding"           { return nullSem(BINDING); }
"block"             { return nullSem(BLOCK); }
"click"             { return nullSem(CLICK); }
"cycle"             { return nullSem(CYCLE); }
"equivalence"       { return nullSem(EQUIVALENCE); }
"forbid"            { return nullSem(FORBID); }
"geometry"          { return nullSem(GEOMETRY); }
"group"             { return nullSem(GROUP); }
"include"           { return nullSem(INCLUDE); }
"inverse"           { return nullSem(INVERSE); }
"invoke"            { return nullSem(INVOKE); }
"keyboard"          { return nullSem(KEYBOARD); }
"metadata"          { return nullSem(METADATA); }
"model"             { return nullSem(MODEL); }
"operation"         { return nullSem(OPERATION); }
"position"          { return nullSem(POSITION); }
"remove"            { return nullSem(REMOVE); }
"require"           { return nullSem(REQUIRE); }
"rotate"            { return nullSem(ROTATE); }
"shift"             { return nullSem(SHIFT); }
"start"             { return nullSem(START); }
"symmetry"          { return nullSem(SYMMETRY); }
"tag"               { return nullSem(TAG); }
"translate"         { return nullSem(TRANSLATE); }

<INITIAL>"/*"       { column += yyleng; BEGIN(C); }
<C>"*/"             { column += yyleng; BEGIN(INITIAL); }
<C>[^*\n]           { column += yyleng; }
<C>"*"[^/]          { column += yyleng; }
<C>\n               { line++; column = 1; }

{IDENTIFIER}        {
                        column += yyleng;
                        cubelval = new CubeSem(line, column, CubeSem::semIdentifier);
                        cubelval->string1 = string(yytext, yyleng);
                        return IDENTIFIER;
                    }

{NUMBER}            {
                        column += yyleng;
                        cubelval = new CubeSem(line, column, CubeSem::semNumber);
                        buffer = string(yytext, yyleng);
                        stringstream(buffer) >> cubelval->number;
                        return NUMBER;
                    }

.                   { column += yyleng; }
<<EOF>>             { yyterminate(); }

%%

int nullSem(int token) {
    cubelval = new CubeSem(line, column, CubeSem::semNull);
    //printf("lexer: %d %d %d\n", line, column, token);
    column += yyleng;
    return token;
}
