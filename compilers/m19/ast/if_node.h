// $Id: if_node.h,v 1.2 2019/05/12 20:19:46 ist186411 Exp $ -*- c++ -*-
#ifndef __M19_IFNODE_H__
#define __M19_IFNODE_H__

#include <cdk/ast/expression_node.h>

namespace m19 {

  class if_node: public cdk::basic_node {
    cdk::expression_node *_condition;
    cdk::basic_node *_block;

  public:
    if_node(int lineno, cdk::expression_node *condition, cdk::basic_node *block) :
        cdk::basic_node(lineno), _condition(condition), _block(block) {
    }

  public:
    cdk::expression_node *condition() {
      return _condition;
    }
    cdk::basic_node *block() {
      return _block;
    }

    void accept(basic_ast_visitor *sp, int level) {
      sp->do_if_node(this, level);
    }

  };

} // m19

#endif
