%{
//-- don't change *any* of these: if you do, you'll break the compiler.
#include <algorithm>
#include <cdk/compiler.h>
#include "ast/all.h"
#define LINE               compiler->scanner()->lineno()
#define yylex()            compiler->scanner()->scan()
#define yyerror(s)         compiler->scanner()->error(s)
#define YYPARSE_PARAM_TYPE std::shared_ptr<cdk::compiler>
#define YYPARSE_PARAM      compiler
//-- don't change *any* of these --- END!
%}

%union {
  int                   i;	        /* INTEGER VALUE */
  double                d;          /* REAL VALUE */
  std::string          *s;	        /* SYMBOL NAME OR STRING LITERAL */
  cdk::basic_node      *node;	      /* NODE POINTER */
  cdk::sequence_node   *sequence;
  cdk::expression_node *expression; /* EXPRESSION NODES */
  cdk::lvalue_node     *lvalue;
  basic_type           *type;       /* EXPRESSION TYPES */
};
%token tPRIVATE tEXTERNAL tPUBLIC
%token tBREAK tCONTINUE tRETURN tPRINTLN tPLUS tAPPLY tTO tRANGE
%token tINSEC tFINSEC

%token<i> tINTEGER
%token<d> tREAL
%token<s> tSTRING tID

%type<node> declaration instruction body block cond_inst iter_inst
%type<sequence> args expressions mid_secs opt_expr
%type<sequence> file declarations innerdecls instructions opt_instructions
%type<expression> expression literal funcall
%type<lvalue> lvalue

%type<s> string
%type<type> data_type
%type<node> vardecl fundef fundecl init_sec mid_sec final_sec

%nonassoc REDUCE
%nonassoc ':'

%right '='
%right tPLUS
%left tOR
%left tAND
%nonassoc '~'
%left tEQ tNE
%left tGE tLE '>' '<'
%left '+' '-'
%left '*' '/' '%'
%nonassoc tUNARY
%nonassoc '?'
%nonassoc ']' '['
%nonassoc ')' '('

%{
//-- The rules below will be included in yyparse, the main parsing function.
%}
%%
file            : declarations { compiler->ast($$ = $1); }
                ;

declarations    :              declaration { $$ = new cdk::sequence_node(LINE, $1); }
                | declarations declaration { $$ = new cdk::sequence_node(LINE, $2, $1); }
                ;

declaration     : vardecl ';' { $$ = $1; }
                | fundecl     { $$ = $1; }
                | fundef      { $$ = $1; }
                ;

vardecl   : data_type tID                    { $$ = new m19::variable_declaration_node(LINE, $1, *$2, tPRIVATE, NULL); delete $2; }
          | data_type tID     '=' expression { $$ = new m19::variable_declaration_node(LINE, $1, *$2, tPRIVATE, $4); delete $2; }
          | data_type tID '?'                { $$ = new m19::variable_declaration_node(LINE, $1, *$2, tEXTERNAL, NULL); delete $2; }
          | data_type tID '!'                { $$ = new m19::variable_declaration_node(LINE, $1, *$2, tPUBLIC, NULL); delete $2; }
          | data_type tID '!' '=' expression { $$ = new m19::variable_declaration_node(LINE, $1, *$2, tPUBLIC, $5); delete $2; }
          ;

data_type : '#'               { $$ = new basic_type(4, basic_type::TYPE_INT); }
          | '%'               { $$ = new basic_type(8, basic_type::TYPE_DOUBLE); }
          | '$'               { $$ = new basic_type(4, basic_type::TYPE_STRING); }
          | '<' data_type '>' { $$ = new basic_type(4, basic_type::TYPE_POINTER); $$->_subtype = $2; }
          ;

fundecl   : '!'       tID     '('      ')' { $$ = new m19::function_declaration_node(LINE, *$2, tPRIVATE, NULL);  delete $2; }
          | '!'       tID     '(' args ')' { $$ = new m19::function_declaration_node(LINE, *$2, tPRIVATE, $4);    delete $2; }
          | '!'       tID '!' '('      ')' { $$ = new m19::function_declaration_node(LINE, *$2, tPUBLIC, NULL);   delete $2; }
          | '!'       tID '!' '(' args ')' { $$ = new m19::function_declaration_node(LINE, *$2, tPUBLIC, $5);     delete $2; }
          | '!'       tID '?' '('      ')' { $$ = new m19::function_declaration_node(LINE, *$2, tEXTERNAL, NULL); delete $2; }
          | '!'       tID '?' '(' args ')' { $$ = new m19::function_declaration_node(LINE, *$2, tEXTERNAL, $5);   delete $2; }

          | data_type tID     '('      ')' { $$ = new m19::function_declaration_node(LINE, $1, *$2, tPRIVATE, NULL);  delete $2; }
          | data_type tID     '(' args ')' { $$ = new m19::function_declaration_node(LINE, $1, *$2, tPRIVATE, $4);    delete $2; }
          | data_type tID '!' '('      ')' { $$ = new m19::function_declaration_node(LINE, $1, *$2, tPUBLIC, NULL);   delete $2; }
          | data_type tID '!' '(' args ')' { $$ = new m19::function_declaration_node(LINE, $1, *$2, tPUBLIC, $5);     delete $2; }
          | data_type tID '?' '('      ')' { $$ = new m19::function_declaration_node(LINE, $1, *$2, tEXTERNAL, NULL); delete $2; }
          | data_type tID '?' '(' args ')' { $$ = new m19::function_declaration_node(LINE, $1, *$2, tEXTERNAL, $5);   delete $2; }
          ;

fundef    : '!'       tID     '('      ')'             body { $$ = new m19::function_definition_node(LINE, *$2, tPRIVATE, NULL, NULL, $5); delete $2; } 
          | '!'       tID     '(' args ')'             body { $$ = new m19::function_definition_node(LINE, *$2, tPRIVATE, $4  , NULL, $6);   delete $2; } 
          | '!'       tID '!' '('      ')'             body { $$ = new m19::function_definition_node(LINE, *$2, tPUBLIC , NULL, NULL, $6);  delete $2; } 
          | '!'       tID '!' '(' args ')'             body { $$ = new m19::function_definition_node(LINE, *$2, tPUBLIC , $5  , NULL, $7);    delete $2; } 
          
          | data_type tID     '('      ')'             body { $$ = new m19::function_definition_node(LINE, $1, *$2, tPRIVATE, NULL, NULL, $5); delete $2; }
          | data_type tID     '(' args ')'             body { $$ = new m19::function_definition_node(LINE, $1, *$2, tPRIVATE, $4  , NULL, $6);   delete $2; }
          | data_type tID '!' '('      ')'             body { $$ = new m19::function_definition_node(LINE, $1, *$2, tPUBLIC , NULL, NULL, $6);  delete $2; }
          | data_type tID '!' '(' args ')'             body { $$ = new m19::function_definition_node(LINE, $1, *$2, tPUBLIC , $5  , NULL, $7);    delete $2; }

          | data_type tID     '('      ')' '=' literal body { $$ = new m19::function_definition_node(LINE, $1, *$2, tPRIVATE, NULL, $6, $7); delete $2; }
          | data_type tID     '(' args ')' '=' literal body { $$ = new m19::function_definition_node(LINE, $1, *$2, tPRIVATE, $4  , $7, $8);   delete $2; }
          | data_type tID '!' '('      ')' '=' literal body { $$ = new m19::function_definition_node(LINE, $1, *$2, tPUBLIC , NULL, $7, $8);  delete $2; }
          | data_type tID '!' '(' args ')' '=' literal body { $$ = new m19::function_definition_node(LINE, $1, *$2, tPUBLIC , $5  , $8, $9);    delete $2; }
          ;


args   :          vardecl    { $$ = new cdk::sequence_node(LINE, $1); }
       | args ',' vardecl    { $$ = new cdk::sequence_node(LINE, $3, $1); }
       ;

body   :          mid_secs           { $$ = new m19::body_node(LINE, NULL, $1  , NULL); }
       | init_sec                    { $$ = new m19::body_node(LINE, $1  , NULL, NULL); }
       |                   final_sec { $$ = new m19::body_node(LINE, NULL, NULL, $1); }
       | init_sec mid_secs           { $$ = new m19::body_node(LINE, $1  , $2  , NULL); }
       |          mid_secs final_sec { $$ = new m19::body_node(LINE, NULL, $1  , $2); }
       | init_sec mid_secs final_sec { $$ = new m19::body_node(LINE, $1  , $2  , $3); }
       ;

init_sec  : tINSEC block     { $$ = new m19::initial_section_node(LINE, $2); }
          ;

mid_secs  :          mid_sec { $$ = new cdk::sequence_node(LINE, $1); }
          | mid_secs mid_sec { $$ = new cdk::sequence_node(LINE, $2, $1); }
          ;

mid_sec   : '['            ']' block { $$ = new m19::section_node(LINE, NULL, $3, true); }
          | '[' expression ']' block { $$ = new m19::section_node(LINE, $2  , $4, true); }
          | '('            ')' block { $$ = new m19::section_node(LINE, NULL, $3, false); }
          | '(' expression ')' block { $$ = new m19::section_node(LINE, $2  , $4, false); }
          | block                    { $$ = $1; }
          ;

final_sec : tFINSEC block    { $$ = new m19::final_section_node(LINE, $2); }
          ;

block  : '{'           opt_instructions '}' { $$ = new m19::block_node(LINE, NULL, $2); }
       | '{'innerdecls opt_instructions '}' { $$ = new m19::block_node(LINE, $2, $3); }
       ;

innerdecls      :            vardecl ';'   { $$ = new cdk::sequence_node(LINE, $1); }
                | innerdecls vardecl ';'   { $$ = new cdk::sequence_node(LINE, $2, $1); }
                ;

opt_instructions: /* empty */              { $$ = new cdk::sequence_node(LINE); }
                | instructions             { $$ = $1; }
                ;

instructions    : instruction              { $$ = new cdk::sequence_node(LINE, $1); } 
                | instructions instruction { $$ = new cdk::sequence_node(LINE, $2, $1);}
                ;

instruction     : expression ';'           { $$ = new m19::evaluation_node(LINE, $1); }
                | expression '!'           { $$ = new m19::print_node(LINE, $1, false); }
                | expression tPRINTLN      { $$ = new m19::print_node(LINE, $1, true); }
                | tBREAK                   { $$ = new m19::break_node(LINE); }
                | tCONTINUE                { $$ = new m19::continue_node(LINE); }
                | tRETURN                  { $$ = new m19::return_node(LINE); }
                | tAPPLY tID tTO lvalue ':' expression tRANGE expression ';'
                                           {
                                             $$ = new m19::apply_node(LINE, *$2, new cdk::rvalue_node(LINE,$4), $6, $8);
                                             delete $2;
                                           }
                | cond_inst                { $$ = $1; }
                | iter_inst                { $$ = $1; }
                | block                    { $$ = $1; }
                ;

cond_inst       : '[' expression ']' '#' instruction                 { $$ = new m19::if_node(LINE, $2, $5); }
                | '[' expression ']' '?' instruction %prec REDUCE    { $$ = new m19::if_else_node(LINE, $2, $5, NULL); }
                | '[' expression ']' '?' instruction ':' instruction { $$ = new m19::if_else_node(LINE, $2, $5, $7); }
                ;

iter_inst       : '[' args     ';' opt_expr ';' opt_expr ']' instruction { $$ = new m19::for_node(LINE, $2, $4, $6, $8); }
                | '[' opt_expr ';' opt_expr ';' opt_expr ']' instruction { $$ = new m19::for_node(LINE, $2, $4, $6, $8); }
                ;


lvalue       : tID                         { $$ = new cdk::variable_node(LINE, *$1); delete $1; }
             | '@'    '[' expression ']'   { $$ = new m19::index_node(LINE, new cdk::rvalue_node(LINE, new cdk::variable_node(LINE, "@")), $3); }
             | lvalue '[' expression ']'   { $$ = new m19::index_node(LINE, new cdk::rvalue_node(LINE, $1), $3); }
             | '(' expression ')' '[' expression ']'   { $$ = new m19::index_node(LINE, $2, $5); }
             | funcall '[' expression ']' { $$ = new m19::index_node(LINE, $1, $3); }
             ;

expression   : literal                     { $$ = $1; }
             /* LEFT VALUES */
             | lvalue                      { $$ = new cdk::rvalue_node(LINE, $1); }
             /* PRIMARY EXPRESSION */
             | '(' expression ')'          { $$ = $2; }
             | '[' expression ']'          { $$ = new m19::stack_alloc_node(LINE, $2); }
             /* UNARY EXPRESSIONS */
             | '-' expression %prec tUNARY { $$ = new cdk::neg_node(LINE, $2); }
             | '+' expression %prec tUNARY { $$ = new m19::identity_node(LINE, $2); }
             | lvalue '?'                  { $$ = new m19::address_of_node(LINE, $1); }
             /* ADITIVE EXPRESSIONS */
             | expression '+' expression   { $$ = new cdk::add_node(LINE, $1, $3); }
             | expression '-' expression   { $$ = new cdk::sub_node(LINE, $1, $3); }
             /* MULTIPLICATIVE EXPRESSIONS */
             | expression '*' expression   { $$ = new cdk::mul_node(LINE, $1, $3); }
             | expression '/' expression   { $$ = new cdk::div_node(LINE, $1, $3); }
             | expression '%' expression   { $$ = new cdk::mod_node(LINE, $1, $3); }
             /* COMPARATIVE EXPRESSIONS */
             | expression '<' expression   { $$ = new cdk::lt_node(LINE, $1, $3); }
             | expression '>' expression   { $$ = new cdk::gt_node(LINE, $1, $3); }
             | expression tLE expression   { $$ = new cdk::le_node(LINE, $1, $3); }
             | expression tGE expression   { $$ = new cdk::ge_node(LINE, $1, $3); }
             /* EQUAL EXPRESSIONS */
             | expression tEQ expression   { $$ = new cdk::eq_node(LINE, $1, $3); }
             | expression tNE expression   { $$ = new cdk::ne_node(LINE, $1, $3); }
             /* LOGICAL EXPRESSIONS */
             | expression tAND expression  { $$ = new cdk::and_node(LINE, $1, $3); }
             | expression tOR expression   { $$ = new cdk::or_node(LINE, $1, $3); }
             | '~' expression              { $$ = new cdk::not_node(LINE, $2); }
             /* OTHER EXPESSION */
             | '@'                         { $$ = new m19::read_node(LINE); }
             /* FUNCTION CALLS */
             | funcall                     { $$ = $1; }
             /* ASSIGNMENT */
             | lvalue '=' expression       { $$ = new cdk::assignment_node(LINE, $1, $3); }
             | '@'    '=' expression       { $$ = new cdk::assignment_node(LINE, new cdk::variable_node(LINE, "@"), $3); }
             | lvalue tPLUS expression     { $$ = new m19::plus_equal_node(LINE, $1, $3); }
             ;

opt_expr     : /*VOID*/                    { $$ = new cdk::sequence_node(LINE); }
             | expressions                 { $$ = new cdk::sequence_node(LINE, $1); }
             ;

funcall      : tID '('             ')'     { $$ = new m19::function_call_node(LINE, *$1); delete $1; }
             | tID '(' expressions ')'     { $$ = new m19::function_call_node(LINE, *$1, $3); delete $1; }
             | '@' '('             ')'     { $$ = new m19::function_call_node(LINE, "@"); }
             | '@' '(' expressions ')'     { $$ = new m19::function_call_node(LINE, "@", $3); }
             ;

expressions  : expression                  { $$ = new cdk::sequence_node(LINE, $1); }
             | expressions ',' expression  { $$ = new cdk::sequence_node(LINE, $3, $1); }
             ;

literal      : tINTEGER                    { $$ = new cdk::integer_node(LINE, $1); }
             | tREAL                       { $$ = new cdk::double_node(LINE, $1); }
             | string                      { $$ = new cdk::string_node(LINE, $1); }
             ;

string       :        tSTRING              { $$ = $1; }
             | string tSTRING              { $$ = new std::string(*$1 + *$2); delete $1, delete $2; }
             ;
%%
