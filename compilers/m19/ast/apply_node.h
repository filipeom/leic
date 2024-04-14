#ifndef __M19_APPLY_H__
#define __M19_APPLY_H__

#include <cdk/ast/basic_node.h>
#include <cdk/ast/expression_node.h>
#include "targets/basic_ast_visitor.h"

namespace m19 {

  class apply_node: public cdk::basic_node {
    std::string _function;
    cdk::expression_node *_base;
    cdk::expression_node *_from;
    cdk::expression_node *_to;

  public:
    apply_node(int lineno, std::string function, cdk::expression_node *base,
        cdk::expression_node *from, cdk::expression_node *to) : cdk::basic_node(lineno), 
    _function(function), _base(base), _from(from), _to(to) {
    }

  public:
    std::string function() {
      return _function;
    }
    cdk::expression_node *base() {
      return _base;
    }
    cdk::expression_node *from() {
      return _from;
    }
    cdk::expression_node *to() {
      return _to;
    }

    void accept(basic_ast_visitor *sp, int level) {
      sp->do_apply_node(this, level);
    }

  };

} // m19

#endif
