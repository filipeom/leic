// $Id: type_checker.h,v 1.4 2019/05/22 17:48:32 ist186411 Exp $ -*- c++ -*-
#ifndef __M19_SEMANTICS_TYPE_CHECKER_H__
#define __M19_SEMANTICS_TYPE_CHECKER_H__

#include <string>
#include <iostream>
#include <cdk/symbol_table.h>
#include <cdk/ast/basic_node.h>
#include "targets/symbol.h"
#include "targets/basic_ast_visitor.h"

namespace m19 {

  class type_checker: public basic_ast_visitor {
    cdk::symbol_table<m19::symbol> &_symtab;
    std::shared_ptr<m19::symbol> _function;
    basic_ast_visitor *_parent;

  public:
    type_checker(std::shared_ptr<cdk::compiler> compiler, cdk::symbol_table<m19::symbol> &symtab,
        std::shared_ptr<m19::symbol> func, basic_ast_visitor *parent) :
        basic_ast_visitor(compiler), _symtab(symtab), _function(func), _parent(parent) {
    }

  public:
    ~type_checker() {
      os().flush();
    }

  protected:
    basic_type *copy_type(basic_type *type);
    void processUnaryExpression(cdk::unary_expression_node * const node, int lvl);
    void processGeneralLogicalExpression(cdk::binary_expression_node* const node, int lvl);
    void processIntOnlyExpression(cdk::binary_expression_node * const node, int lvl);
    void processIntDoubleExpression(cdk::binary_expression_node * const node, int lvl);
    void processIntDoublePointerExpression(cdk::binary_expression_node * const node, int lvl);
    template<typename T>
    void process_literal(cdk::literal_node<T> * const node, int lvl) {
    }

  public:
  // DO NOT EDIT THSES LINES
#define __IN_VISITOR_HEADER__
#include "ast/visitor_decls.h"       // AUTOMATICALLY GENERATED
#undef __IN_VISITOR_HEADER__
  // DO NOT EDIT THESE LINES: END

  };
//===========================================================================
//                     HELPER MACRO FOR TYPE CHECKING
//===========================================================================

#define CHECK_TYPES(compiler, symtab, function, node) { \
  try { \
    m19::type_checker checker(compiler, symtab, function, this); \
    (node)->accept(&checker, 0); \
  } \
  catch (const std::string &problem) { \
    std::cerr << (node)->lineno() << ": " << problem << std::endl; \
    return; \
  } \
}

#define ASSERT_SAFE_EXPRESSIONS CHECK_TYPES(_compiler, _symtab, _function, node)

} // m19

#endif
