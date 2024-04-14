#ifndef __M19_SEMANTICS_POSTFIX_WRITER_H__
#define __M19_SEMANTICS_POSTFIX_WRITER_H__

#include <set>
#include <stack>
#include <string>
#include <iostream>
#include <cdk/symbol_table.h>
#include <cdk/emitters/basic_postfix_emitter.h>
#include "targets/basic_ast_visitor.h"
#include "targets/symbol.h"

namespace m19 {

  class postfix_writer: public basic_ast_visitor {
    cdk::symbol_table<m19::symbol> &_symtab;
  
    std::set<std::string> _functionsToDeclare;

    bool _inFunctionArgs, _inFunctionBody, _inFinalSec;
    std::stack<int> _forIncr, _forEnd;
    std::shared_ptr<m19::symbol> _function;
    int _offset;

    int _currentBodyFinalSecLabel;
    int _currentFunctionExitLabel;

    cdk::basic_postfix_emitter &_pf;
    int _lbl;

  public:
    postfix_writer(std::shared_ptr<cdk::compiler> compiler, cdk::symbol_table<m19::symbol> &symtab,
                   cdk::basic_postfix_emitter &pf) :
        basic_ast_visitor(compiler), _symtab(symtab), _inFunctionArgs(false), 
        _inFunctionBody(false), _inFinalSec(false), _function(nullptr),
        _offset(0), _currentBodyFinalSecLabel(0), _currentFunctionExitLabel(0), 
        _pf(pf), _lbl(0) {
    }

  public:
    ~postfix_writer() {
      os().flush();
    }

  private:
    // METHOD USED TO GENERATE SEQUENTIAL LABELS.
    inline std::string mklbl(int lbl) {
      std::ostringstream oss;
      if (lbl < 0)
        oss << ".L" << -lbl;
      else
        oss << "_L" << lbl;
      return oss.str();
    }

    void error(int lineno, std::string msg) {
      std::cerr << "Error: " << lineno << ": " << msg << std::endl;
    }

  protected:
    basic_type *copy_type(basic_type *type);

  public:
  // DO NOT EDIT THESE LINES
#define __IN_VISITOR_HEADER__
#include "ast/visitor_decls.h"       // AUTOMATICALLY GENERATED
#undef __IN_VISITOR_HEADER__
  // DO NOT EDIT THESE LINES: END

  };

} // m19

#endif
