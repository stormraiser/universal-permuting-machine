%{
#include "CubeSem.h"
#include <iostream>
#include "Errors.h"
#include "Translator.h"

using namespace std;

int yylex(CubeSem **lvalp, YYLTYPE *llocp);
void yyerror (YYLTYPE *locp, CubeSem **valp, char const *msg) {
    Translator::globalErrorList.push_back(new GenericError(*locp, "Syntax error"));
};

%}

%define api.value.type {CubeSem*}
%define api.pure full
%parse-param {CubeSem **treeTop}

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
                            *treeTop = $$;
                        }
                    ;

identifier_list     :   identifier_list ',' IDENTIFIER
                        {
                            $$ = $1;
                            $$->location = @$;
                            $$->stringList.push_back($3->string1);
                        }
                    |   IDENTIFIER
                        {
                            $$ = new CubeSem(@$, CubeSem::semStringList);
                            $$->stringList.push_back($1->string1);
                        }
                    ;

identifier_tuple    :   '(' identifier_list ')'
                        {
                            $$ = $2;
                            $$->location = @$;
                            $$->type = CubeSem::semTuple;
                        }
                    |   IDENTIFIER
                        {
                            $$ = new CubeSem(@$, CubeSem::semTuple);
                            $$->stringList.push_back($1->string1);
                        }
                    ;

tuple_list          :   tuple_list ',' identifier_tuple
                        {
                            $$ = $1;
                            $$->location = @$;
                            $$->childList.push_back($3);
                        }
                    |   identifier_tuple
                        {
                            $$ = new CubeSem(@$, CubeSem::semTupleList);
                            $$->childList.push_back($1);
                        }
                    ;

number_list         :   number_list ',' NUMBER
                        {
                            $$ = $1;
                            $$->location = @$;
                            $$->numberList.push_back($3->number);
                        }
                    |   NUMBER
                        {
                            $$ = new CubeSem(@$, CubeSem::semNumberList);
                            $$->numberList.push_back($1->number);
                        }
                    ;

number_tuple        :   '(' number_list ')'
                        {
                            $$ = $2;
                            $$->location = @$;
                            $$->type = CubeSem::semNumberTuple;
                        }
                    ;

block_position_set  :   identifier_tuple ':' identifier_tuple
                        {
                            $$ = new CubeSem(@$, CubeSem::semBlockPositionSet);
                            $$->childList.push_back($1);
                            $$->childList.push_back($2);
                        }
                    ;

block_position_item :   IDENTIFIER ':' IDENTIFIER
                        {
                            $$ = new CubeSem(@$, CubeSem::semBlockPositionItem);
                            $$->string1 = $1->string1;
                            $$->string2 = $3->string1;
                        }
                    ;

position_item       :   IDENTIFIER
                    |   block_position

tuple_list_keyword  :   BLOCK ALIAS
                        {
                            $$ = new CubeSem(@$, CubeSem::semBlockAliasStmt);
                        }
                    |   BLOCK EQUIVALENCE
                        {
                            $$ = new CubeSem(@$, CubeSem::semBlockEquivalenceStmt);
                        }
                    |   CYCLE
                        {
                            $$ = new CubeSem(@$, CubeSem::semCycleStmt);
                        }
                    |   FORBID
                        {
                            $$ = new CubeSem(@$, CubeSem::semForbidStmt);
                        }
                    |   INVOKE
                        {
                            $$ = new CubeSem(@$, CubeSem::semInvokeStmt);
                        }
                    |   POSITION
                        {
                            $$ = new CubeSem(@$, CubeSem::semPositionStmt);
                        }
                    |   POSITION ALIAS
                        {
                            $$ = new CubeSem(@$, CubeSem::semPositionAliasStmt);
                        }
                    |   POSITION EQUIVALENCE
                        {
                            $$ = new CubeSem(@$, CubeSem::semPositionEquivalenceStmt);
                        }
                    |   REQUIRE
                        {
                            $$ = new CubeSem(@$, CubeSem::semRequireStmt);
                        }
                    |   SHIFT
                        {
                            $$ = new CubeSem(@$, CubeSem::semShiftStmt);
                        }
                    ;

tuple_list_stmt     :   tuple_list_keyword tuple_list ';'
                        {
                            $$ = $2;
                            $$->location = @$;
                            $$->type = $1->type;
                        }
                    ;

def_keyword         :   BINDING
                        {
                            $$ = new CubeSem(@$, CubeSem::semBindingStmt);
                        }
                    |   GEOMETRY
                        {
                            $$ = new CubeSem(@$, CubeSem::semGeometryStmt);
                        }
                    |   GROUP
                        {
                            $$ = new CubeSem(@$, CubeSem::semGroupStmt);
                        }
                    |   SYMMETRY
                        {
                            $$ = new CubeSem(@$, CubeSem::semSymmetryStmt);
                        }
                    ;

def_stmt            :   def_keyword IDENTIFIER stmt
                        {
                            $$ = $1;
                            $$->location = @$;
                            $$->string1 = $2->string1;
                            $$->child = $3;
                        }
                    ;

operation_stmt      :   OPERATION IDENTIFIER stmt
                        {
                            $$ = new CubeSem(@$, CubeSem::semOperationStmt);
                            $$->string1 = $2->string1;
                            $$->child = $3;
                        }

transform_keyword   :   TRANSLATE
                        {
                            $$ = new CubeSem(@$, CubeSem::semTranslateStmt);
                        }
                    |   ROTATE
                        {
                            $$ = new CubeSem(@$, CubeSem::semRotateStmt);
                        }
                    |   AXISANGLE
                        {
                            $$ = new CubeSem(@$, CubeSem::semAxisAngleStmt);
                        }
                    ;

transform_stmt      :   transform_keyword number_tuple stmt
                        {
                            $$ = $2;
                            $$->location = @$;
                            $$->type = $1->type;
                            $$->child = $3;
                        }
                    ;

include_stmt        :   INCLUDE STRING ';'
                        {
                            $$ = new CubeSem(@$, CubeSem::semIncludeStmt);
                            $$->string1 = $2->string1;
                        }
                    ;

model_stmt          :   MODEL STRING ';'
                        {
                            $$ = new CubeSem(@$, CubeSem::semModelStmt);
                            $$->string1 = $2->string1;
                        }
                    ;

auto_stmt           :   AUTO tuple_list stmt
                        {
                            $$ = $2;
                            $$->location = @$;
                            $$->type = CubeSem::semAutoStmt;
                            $$->child = $3;
                        }
                    ;

color_tag           :   IDENTIFIER '=' number_tuple
                        {
                            $$ = $3;
                            $$->location = @$;
                            $$->type = CubeSem::semColorTag;
                            $$->string1 = $1->string1;
                        }
                    ;

tag_list            :   tag_list ',' IDENTIFIER
                        {
                            $$ = $1;
                            $$->location = @$;
                            $$->childList.push_back($3);
                        }
                    |   tag_list ',' color_tag
                        {
                            $$ = $1;
                            $$->location = @$;
                            $$->childList.push_back($3);
                        }
                    |   IDENTIFIER
                        {
                            $$ = new CubeSem(@$, CubeSem::semTagStmt);
                            $$->childList.push_back($1);
                        }
                    |   color_tag
                        {
                            $$ = new CubeSem(@$, CubeSem::semTagStmt);
                            $$->childList.push_back($1);
                        }
                    ;

tag_stmt            :   TAG tag_list ';'
                        {
                            $$ = $2;
                            $$->location = @$;
                        }
                    ;

block_item          :   IDENTIFIER '=' IDENTIFIER identifier_tuple
                        {
                            $$ = new CubeSem(@$, CubeSem::semBlockItem);
                            $$->string1 = $1->string1;
                            $$->string2 = $3->string1;
                            $$->child = $4;
                        }
                    |   IDENTIFIER
                    ;

block_list          :   block_list ',' block_item
                        {
                            $$ = $1;
                            $$->location = @$;
                            $$->childList.push_back($3);
                        }
                    |   block_item
                        {
                            $$ = new CubeSem(@$, CubeSem::semBlockStmt);
                            $$->childList.push_back($1);
                        }
                    ;

block_stmt          :   BLOCK block_list ';'
                        {
                            $$ = $2;
                            $$->location = @$;
                        }
                    ;

remove_keyword      :   BINDING
                        {
                            $$ = new CubeSem(@$, CubeSem::semRemoveBindingStmt);
                        }
                    |   OPERATION
                        {
                            $$ = new CubeSem(@$, CubeSem::semRemoveOperationStmt);
                        }
                    |   BLOCK
                        {
                            $$ = new CubeSem(@$, CubeSem::semRemoveBlockStmt);
                        }
                    |   POSITION
                        {
                            $$ = new CubeSem(@$, CubeSem::semRemovePositionStmt);
                        }
                    ;

remove_stmt         :   REMOVE remove_keyword identifier_list ';'
                        {
                            $$ = $2;
                            $$->location = @$;
                            $$->stringList = $3->stringList;
                        }
                    ;

inverse_stmt        :   OPERATION IDENTIFIER INVERSE IDENTIFIER ';'
                        {
                            $$ = new CubeSem(@$, CubeSem::semInverseStmt);
                            $$->string1 = $2->string1;
                            $$->string2 = $4->string1;
                        }
                    ;

control_stmt        :   KEYBOARD STRING ';'
                        {
                            $$ = new CubeSem(@$, CubeSem::semKeyboardStmt);
                            $$->string1 = $2->string1;
                        }
                    |   CLICK STRING ';'
                        {
                            $$ = new CubeSem(@$, CubeSem::semClickStmt);
                            $$->string2 = $2->string1;
                        }
                    |   KEYBOARD STRING CLICK STRING ';'
                        {
                            $$ = new CubeSem(@$, CubeSem::semClickStmt);
                            $$->string1 = $2->string1;
                            $$->string2 = $4->string1;
                        }
                    ;

start_item          :   IDENTIFIER '=' IDENTIFIER
                        {
                            $$ = new CubeSem(@$, CubeSem::semStartItem);
                            $$->string1 = $1->string1;
                            $$->string2 = $3->string1;
                        }
                    ;

start_list          :   start_list ',' start_item
                        {
                            $$ = $1;
                            $$->location = @$;
                            $1->childList.push_back($3);
                        }
                    |   start_item
                        {
                            $$ = new CubeSem(@$, CubeSem::semStartStmt);
                            $$->childList.push_back($1);
                        }
                    ;

start_stmt          :   START start_list ';'
                        {
                            $$ = $2;
                            $$->location = @$;
                        }
                    ;

metadata_item       :   STRING '=' STRING ';'
                        {
                            $$ = new CubeSem(@$, CubeSem::semMetadataItem);
                            $$->string1 = $1->string1;
                            $$->string2 = $3->string1;
                        }
                    ;

metadata_list       :   metadata_list metadata_item
                        {
                            $$ = $1;
                            $$->location = @$;
                            $$->childList.push_back($2);
                        }
                    |   metadata_item
                        {
                            $$ = new CubeSem(@$, CubeSem::semMetadataStmt);
                            $$->childList.push_back($1);
                        }
                    ;

metadata_stmt       :   METADATA '{' metadata_list '}'
                        {
                            $$ = $3;
                            $$->location = @$;
                        }
                    ;

bandage_item        :   identifier_tuple '=' IDENTIFIER identifier_tuple
                        {
                            $$ = new CubeSem(@$, CubeSem::semBandageItem);
                            $$->childList.push_back($1);
                            $$->childList.push_back($4);
                            $$->string1 = $3->string1;
                        }
                    |   identifier_tuple
                    ;

bandage_list        :   bandage_list ',' bandage_item
                        {
                            $$ = $1;
                            $$->location = @$;
                            $$->childList.push_back($3);
                        }
                    |   bandage_item
                        {
                            $$ = new CubeSem(@$, CubeSem::semBandageStmt);
                            $$->childList.push_back($1);
                        }
                    ;

bandage_stmt        :   BANDAGE bandage_list ';'
                        {
                            $$ = $2;
                            $$->location = @$;
                        }

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
                    |   bandage_stmt
                    |   stmt_block
                    |   ';'
                        {
                            $$ = new CubeSem(@$, CubeSem::semEmptyStmt);
                        }
                    |   error
                        {
                            $$ = new CubeSem(@$, CubeSem::semEmptyStmt);
                        }
                    ;

stmt_list           :   stmt_list stmt
                        {
                            $$ = $1;
                            $$->location = @$;
                            $$->childList.push_back($2);
                        }
                    |   %empty
                        {
                            $$ = new CubeSem(@$, CubeSem::semStmtBlock);
                        }
                    ;

stmt_block          :   '{' stmt_list '}'
                        {
                            $$ = $2;
                            $$->location = @$;
                        }
                    ;
