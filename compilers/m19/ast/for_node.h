// $Id: for_node.h,v 1.3 2019/05/12 20:19:46 ist186411 Exp $ -*- c++ -*-
#ifndef __M19_FORNODE_H__
#define __M19_FORNODE_H__

#include <cdk/ast/sequence_node.h>

namespace m19 {

  class for_node: public cdk::basic_node {
    cdk::sequence_node *_init;
    cdk::sequence_node *_condition;
    cdk::sequence_node *_incr;
    cdk::basic_node *_block;

  public:
    for_node(int lineno, cdk::sequence_node *init, cdk::sequence_node *condition, 
        cdk::sequence_node *incr, cdk::basic_node *block) : 
      basic_node(lineno), _init(init), _condition(condition), _incr(incr), _block(block) { }

  public:
    cdk::sequence_node *init() {
      return _init;
    }

    cdk::sequence_node *condition() {
      return _condition;
    }

    cdk::sequence_node *incr() {
      return _incr;
    }
  
    cdk::basic_node *block() {
      return _block;
    }

    void accept(basic_ast_visitor *sp, int level) {
      sp->do_for_node(this, level);
    }

  };

} // m19

#endif
