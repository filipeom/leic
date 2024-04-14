%option c++ prefix="m19_scanner_" outfile="m19_scanner.cpp"
%option stack noyywrap yylineno 8bit
%{ 
// make relevant includes before including the parser's tab file
#include <string>
#include <sstream>
#include <utility>
#include <cmath>
#include <cerrno>
#include <cdk/ast/sequence_node.h>
#include <cdk/ast/expression_node.h>
#include <cdk/ast/lvalue_node.h>
#include "m19_parser.tab.h"

static std::ostringstream strlit;

// don't change this
#define yyerror LexerError

std::pair<char, double> getSciNotation(std::string str) {
  size_t i;
  for (i = 0; i < str.size(); i++)
    switch(str[i]) {
      case 'a':
        return std::make_pair('a', pow(10, -18));
      case 'f':
        return std::make_pair('f', pow(10, -15));
      case 'p':
        return std::make_pair('p', pow(10, -12));
      case 'n':
        return std::make_pair('n', pow(10, -9));
      case 'u':
        return std::make_pair('u', pow(10, -6));
      case 'm':
        return std::make_pair('m', pow(10, -3));
      case 'k':
        return std::make_pair('k', pow(10, 3));
      case 'M':
        return std::make_pair('M', pow(10, 6));
      case 'G':
        return std::make_pair('G', pow(10, 9));
      case 'T':
        return std::make_pair('T', pow(10, 12));
      case 'P':
        return std::make_pair('P', pow(10, 15));
      case 'E':
        return std::make_pair('E', pow(10, 18));
    }
  return std::make_pair('.', 1.0);
}

double scitod(const std::string sci) {
  size_t pos;
  char buffer[sci.size()+1];

  sci.copy(buffer, sci.size());
  std::string dstr(buffer);

  auto pair = getSciNotation(dstr);

  pos = dstr.find(pair.first);
  dstr.replace(pos, 1, ".");
  return strtod(dstr.c_str(), nullptr) * pair.second;
}
%}

SPACE   [ \t\n]
INT     [[:digit:]]+

IDENTIFIER [A-Za-z][A-Za-z0-9_]*

%x X_STRING X_BACKSLASH X_COMMENT
%%

  /* ====================================================================== */
  /*                             3.2 COMMENTS                               */
  /* ====================================================================== */
"//".*                 ; /* ignore comments */

"/*"                   yy_push_state(X_COMMENT);
<X_COMMENT>"/*"        yy_push_state(X_COMMENT);
<X_COMMENT>"*/"        yy_pop_state();
<X_COMMENT>.|\n        ; /* ignore content */

  /* ====================================================================== */
  /*                              3.4 TYPES                                 */
  /* ====================================================================== */
                      
">>@"                  return tBREAK;
"@>>"                  return tCONTINUE;
">@<"                  return tRETURN;

"<<"                   return tINSEC;
">>"                   return tFINSEC;
"!!"                   return tPRINTLN;

">="                   return tGE;
"<="                   return tLE;
"=="                   return tEQ;
"!="                   return tNE;
"||"                   return tOR;
"&&"                   return tAND;

"+="                   return tPLUS;

"apply"                return tAPPLY;
"->"                   return tTO;
".."                   return tRANGE;

  /* ====================================================================== */
  /*                              INTEGERS                                  */
  /* ====================================================================== */

0|[1-9][0-9]*          { yylval.i = strtol(yytext, nullptr, 10); if (ERANGE == errno) { yyerror("Integer overflow"); } return tINTEGER; }
0x([a-fA-F0-9]{1,2})   { yylval.i = strtol(yytext, nullptr, 16); if (ERANGE == errno) { yyerror("Integer overflow"); } return tINTEGER; }

  /* ====================================================================== */
  /*                               FLOATS                                   */
  /* ====================================================================== */

([0-9]*\.[0-9]+|[0-9]+\.[0-9])   { yylval.d = strtod(yytext, nullptr); if (ERANGE == errno) { yyerror("Real overflow"); } return tREAL; }
([0-9]+[afpnumkMGTPE][0-9]*)     { yylval.d = scitod(yytext); if (ERANGE == errno) { yyerror("Real overflow"); } return tREAL; }

  /* ====================================================================== */
  /*                            IDENTIFIERS                                 */
  /* ====================================================================== */

{IDENTIFIER}      yylval.s = new std::string(yytext); return tID;

  /* ====================================================================== */
  /*                              STRINGS                                   */
  /* ====================================================================== */

"\""                   yy_push_state(X_STRING);
<X_STRING>"\\"         yy_push_state(X_BACKSLASH);
<X_STRING>"\""         {
                         yylval.s = new std::string(strlit.str());
                         strlit.str("");
                         yy_pop_state();
                         return tSTRING;
                       }
<X_STRING>.            strlit << *yytext;

<X_BACKSLASH>n         strlit << '\n'; yy_pop_state();
<X_BACKSLASH>r         strlit << '\r'; yy_pop_state();
<X_BACKSLASH>t         strlit << '\t'; yy_pop_state();
<X_BACKSLASH>"\""      strlit << '"' ; yy_pop_state();
<X_BACKSLASH>"\\"      strlit << '\\'; yy_pop_state();

<X_BACKSLASH>([0-9a-fA-F]){1,2}  {
                                   strlit << (char)(unsigned char) strtoul(yytext, NULL, 16);
                                   yy_pop_state();
                                 }
<X_BACKSLASH>.         strlit << *yytext; yy_pop_state();

  /* ====================================================================== */
  /*                              OTHER CHARs                               */
  /* ====================================================================== */

[!#$%/()[\]{}=?*+-<>.;~@] return *yytext;

{SPACE}+               ; /* ignore whitespace */
.                      yyerror("Unknown character");

  /* ====================================================================== */
  /*                              THE END                                   */
  /* ====================================================================== */
%%
