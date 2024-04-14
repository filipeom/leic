// $Id: if_else_node.h,v 1.2 2019/05/12 20:19:46 ist186411 Exp $ -*- c++ -*-
#ifndef __M19_IFELSENODE_H__
#define __M19_IFELSENODE_H__

#include <cdk/ast/expression_node.h>

namespace m19 {

  class if_else_node: public cdk::basic_node {
    cdk::expression_node *_condition;
    cdk::basic_node *_thenblock, *_elseblock;

  public:
    if_else_node(int lineno, cdk::expression_node *condition, cdk::basic_node *thenblock, cdk::basic_node *elseblock) :
        cdk::basic_node(lineno), _condition(condition), _thenblock(thenblock), _elseblock(elseblock) {
    }

  public:
    cdk::expression_node *condition() {
      return _condition;
    }
    cdk::basic_node *thenblock() {
      return _thenblock;
    }
    cdk::basic_node *elseblock() {
      return _elseblock;
    }

    void accept(basic_ast_visitor *sp, int level) {
      sp->do_if_else_node(this, level);
    }

  };

} // m19

#endif
