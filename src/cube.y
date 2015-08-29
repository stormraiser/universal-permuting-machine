%{
#include "CubeSem.h"
#include <iostream>

using namespace std;

int yylex();
void yyerror(char const*) {};
extern CubeSem *treeTop;

%}

%define api.value.type {CubeSem*}

%token  ALIAS       AT          AUTO        AXISANGLE
%token  BANDAGE     BINDING     BLOCK       CLICK
%token  CYCLE       EQUIVALENCE FORBID      GEOMETRY
%token  GROUP       INCLUDE     INVERSE     INVOKE
%token  KEYBOARD    METADATA    MODEL       OPERATION
%token  POSITION    REMOVE      REQUIRE     ROTATE
%token  SHIFT       START       SYMMETRY    TAG
%token  TRANSLATE

%token  IDENTIFIER  NUMBER      STRING

%%

cube_spec           :   stmt_list
                        {
                            $$ = $1;
                            treeTop = $$;
                        }
                    ;

identifier_list     :   identifier_list ',' IDENTIFIER
                        {
                            $$ = $1;
                            $$->stringList.push_back($3->string1);
                        }
                    |   IDENTIFIER
                        {
                            $$ = new CubeSem($1->location, CubeSem::semStringList);
                            $$->stringList.push_back($1->string1);
                        }
                    ;

tuple               :   '(' identifier_list ')'
                        {
                            $$ = $2;
                            $$->location = $1->location;
                            $$->type = CubeSem::semTuple;
                        }
                    |   IDENTIFIER
                        {
                            $$ = new CubeSem($1->location, CubeSem::semTuple);
                            $$->stringList.push_back($1->string1);
                        }
                    ;

tuple_list          :   tuple_list ',' tuple
                        {
                            $$ = $1;
                            $$->childList.push_back($3);
                        }
                    |   tuple
                        {
                            $$ = new CubeSem($1->location, CubeSem::semTupleList);
                            $$->childList.push_back($1);
                        }
                    ;

number_list         :   number_list ',' NUMBER
                        {
                            $$ = $1;
                            $$->numberList.push_back($3->number);
                        }
                    |   NUMBER
                        {
                            $$ = new CubeSem($1->location, CubeSem::semNumberList);
                            $$->numberList.push_back($1->number);
                        }
                    ;

number_tuple        :   '(' number_list ')'
                        {
                            $$ = $2;
                            $$->location = $1->location;
                            $$->type = CubeSem::semNumberTuple;
                        }
                    ;

tuple_list_keyword  :   BANDAGE
                        {
                            $$ = new CubeSem($1->location, CubeSem::semBandageStmt);
                        }
                    |   BLOCK ALIAS
                        {
                            $$ = new CubeSem($1->location, CubeSem::semBlockAliasStmt);
                        }
                    |   BLOCK EQUIVALENCE
                        {
                            $$ = new CubeSem($1->location, CubeSem::semBlockEquivalenceStmt);
                        }
                    |   CYCLE
                        {
                            $$ = new CubeSem($1->location, CubeSem::semCycleStmt);
                        }
                    |   FORBID
                        {
                            $$ = new CubeSem($1->location, CubeSem::semForbidStmt);
                        }
                    |   INVOKE
                        {
                            $$ = new CubeSem($1->location, CubeSem::semInvokeStmt);
                        }
                    |   POSITION
                        {
                            $$ = new CubeSem($1->location, CubeSem::semPositionStmt);
                        }
                    |   POSITION ALIAS
                        {
                            $$ = new CubeSem($1->location, CubeSem::semPositionAliasStmt);
                        }
                    |   POSITION EQUIVALENCE
                        {
                            $$ = new CubeSem($1->location, CubeSem::semPositionEquivalenceStmt);
                        }
                    |   REQUIRE
                        {
                            $$ = new CubeSem($1->location, CubeSem::semRequireStmt);
                        }
                    |   SHIFT
                        {
                            $$ = new CubeSem($1->location, CubeSem::semShiftStmt);
                        }
                    ;

tuple_list_stmt     :   tuple_list_keyword tuple_list ';'
                        {
                            $$ = $2;
                            $$->location = $1->location;
                            $$->type = $1->type;
                        }
                    ;

def_keyword         :   BINDING
                        {
                            $$ = new CubeSem($1->location, CubeSem::semBindingStmt);
                        }
                    |   GEOMETRY
                        {
                            $$ = new CubeSem($1->location, CubeSem::semGeometryStmt);
                        }
                    |   GROUP
                        {
                            $$ = new CubeSem($1->location, CubeSem::semGroupStmt);
                        }
                    |   SYMMETRY
                        {
                            $$ = new CubeSem($1->location, CubeSem::semSymmetryStmt);
                        }
                    ;

def_stmt            :   def_keyword IDENTIFIER stmt
                        {
                            $$ = $1;
                            $$->string1 = $2->string1;
                            $$->child = $3;
                        }
                    ;

operation_stmt      :   OPERATION IDENTIFIER stmt
                        {
                            $$ = new CubeSem($1->location, CubeSem::semOperationStmt);
                            $$->string1 = $2->string1;
                            $$->child = $3;
                        }

transform_keyword   :   TRANSLATE
                        {
                            $$ = new CubeSem($1->location, CubeSem::semTranslateStmt);
                        }
                    |   ROTATE
                        {
                            $$ = new CubeSem($1->location, CubeSem::semRotateStmt);
                        }
                    |   AXISANGLE
                        {
                            $$ = new CubeSem($1->location, CubeSem::semAxisAngleStmt);
                        }
                    ;

transform_stmt      :   transform_keyword number_tuple stmt
                        {
                            $$ = $2;
                            $$->location = $1->location;
                            $$->type = $1->type;
                            $$->child = $3;
                        }
                    ;

include_stmt        :   INCLUDE STRING ';'
                        {
                            $$ = new CubeSem($1->location, CubeSem::semIncludeStmt);
                            $$->string1 = $2->string1;
                        }
                    ;

model_stmt          :   MODEL STRING ';'
                        {
                            $$ = new CubeSem($1->location, CubeSem::semModelStmt);
                            $$->string1 = $2->string1;
                        }
                    ;

auto_stmt           :   AUTO tuple_list stmt
                        {
                            $$ = $2;
                            $$->location = $1->location;
                            $$->type = CubeSem::semAutoStmt;
                            $$->child = $3;
                        }
                    ;

color_tag           :   IDENTIFIER '=' number_tuple
                        {
                            $$ = $3;
                            $$->location = $1->location;
                            $$->type = CubeSem::semColorTag;
                            $$->string1 = $1->string1;
                        }
                    ;

tag_list            :   tag_list ',' IDENTIFIER
                        {
                            $$ = $1;
                            $$->childList.push_back($3);
                        }
                    |   tag_list ',' color_tag
                        {
                            $$ = $1;
                            $$->childList.push_back($3);
                        }
                    |   IDENTIFIER
                        {
                            $$ = new CubeSem($1->location, CubeSem::semTagStmt);
                            $$->childList.push_back($1);
                        }
                    |   color_tag
                        {
                            $$ = new CubeSem($1->location, CubeSem::semTagStmt);
                            $$->childList.push_back($1);
                        }
                    ;

tag_stmt            :   TAG tag_list ';'
                        {
                            $$ = $2;
                            $$->location = $1->location;
                        }
                    ;

block_item          :   IDENTIFIER '=' IDENTIFIER tuple
                        {
                            $$ = new CubeSem($1->location, CubeSem::semBlockItem);
                            $$->string1 = $1->string1;
                            $$->string2 = $3->string1;
                            $$->child = $4;
                        }
                    |   IDENTIFIER
                        {
                            $$ = $1;
                        }

block_list          :   block_list ',' block_item
                        {
                            $$ = $1;
                            $$->childList.push_back($3);
                        }
                    |   block_item
                        {
                            $$ = new CubeSem($1->location, CubeSem::semBlockStmt);
                            $$->childList.push_back($1);
                        }
                    ;

block_stmt          :   BLOCK block_list ';'
                        {
                            $$ = $2;
                            $$->location = $1->location;
                        }
                    ;

remove_keyword      :   BINDING
                        {
                            $$ = new CubeSem($1->location, CubeSem::semRemoveBindingStmt);
                        }
                    |   OPERATION
                        {
                            $$ = new CubeSem($1->location, CubeSem::semRemoveOperationStmt);
                        }
                    |   BLOCK
                        {
                            $$ = new CubeSem($1->location, CubeSem::semRemoveBlockStmt);
                        }
                    |   POSITION
                        {
                            $$ = new CubeSem($1->location, CubeSem::semRemovePositionStmt);
                        }
                    ;

remove_stmt         :   REMOVE remove_keyword identifier_list ';'
                        {
                            $$ = $2;
                            $$->location = $1->location;
                            $$->stringList = $3->stringList;
                        }
                    ;

inverse_stmt        :   OPERATION IDENTIFIER INVERSE IDENTIFIER ';'
                        {
                            $$ = new CubeSem($1->location, CubeSem::semInverseStmt);
                            $$->string1 = $2->string1;
                            $$->string2 = $4->string1;
                        }
                    ;

control_stmt        :   KEYBOARD STRING ';'
                        {
                            $$ = new CubeSem($1->location, CubeSem::semKeyboardStmt);
                            $$->string1 = $2->string1;
                        }
                    |   CLICK STRING ';'
                        {
                            $$ = new CubeSem($1->location, CubeSem::semClickStmt);
                            $$->string2 = $2->string1;
                        }
                    |   KEYBOARD STRING CLICK STRING ';'
                        {
                            $$ = new CubeSem($1->location, CubeSem::semClickStmt);
                            $$->string1 = $2->string1;
                            $$->string2 = $4->string1;
                        }
                    ;

start_item          :   IDENTIFIER '=' IDENTIFIER
                        {
                            $$ = new CubeSem($1->location, CubeSem::semStartItem);
                            $$->string1 = $1->string1;
                            $$->string2 = $3->string1;
                        }
                    ;

start_list          :   start_list ',' start_item
                        {
                            $$ = $1;
                            $1->childList.push_back($3);
                        }
                    |   start_item
                        {
                            $$ = new CubeSem($1->location, CubeSem::semStartStmt);
                            $$->childList.push_back($1);
                        }
                    ;

start_stmt          :   START start_list ';'
                        {
                            $$ = $2;
                            $$->location = $1->location;
                        }
                    ;

metadata_item       :   STRING '=' STRING ';'
                        {
                            $$ = new CubeSem($1->location, CubeSem::semMetadataItem);
                            $$->string1 = $1->string1;
                            $$->string2 = $3->string1;
                        }
                    ;

metadata_list       :   metadata_list metadata_item
                        {
                            $$ = $1;
                            $$->childList.push_back($2);
                        }
                    |   metadata_item
                        {
                            $$ = new CubeSem($1->location, CubeSem::semMetadataStmt);
                            $$->childList.push_back($1);
                        }
                    ;

metadata_stmt       :   METADATA '{' metadata_list '}'
                        {
                            $$ = $3;
                            $$->location = $1->location;
                        }
                    ;

stmt                :   tuple_list_stmt
                    |   def_stmt
                    |   operation_stmt
                    |   transform_stmt
                    |   include_stmt
                    |   model_stmt
                    |   auto_stmt
                    |   tag_stmt
                    |   block_stmt
                    |   remove_stmt
                    |   inverse_stmt
                    |   control_stmt
                    |   start_stmt
                    |   metadata_stmt
                    |   stmt_block
                    |   ';'
                        {
                            $$ = new CubeSem($1->location, CubeSem::semEmptyStmt);
                        }
                    ;

stmt_list           :   stmt_list stmt
                        {
                            $$ = $1;
                            $$->childList.push_back($2);
                        }
                    |   stmt
                        {
                            $$ = new CubeSem($1->location, CubeSem::semStmtBlock);
                            $$->childList.push_back($1);
                        }
                    ;

stmt_block          :   '{' stmt_list '}'
                        {
                            $$ = $2;
                            $$->location = $1->location;
                        }
                    ;
