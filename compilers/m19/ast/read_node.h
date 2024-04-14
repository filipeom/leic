// $Id: read_node.h,v 1.3 2019/05/12 20:19:46 ist186411 Exp $ -*- c++ -*-
#ifndef __M19_READNODE_H__
#define __M19_READNODE_H__

#include <cdk/ast/expression_node.h>

namespace m19 {

  class read_node: public cdk::expression_node{

  public:
    read_node(int lineno) : cdk::expression_node(lineno) {
    }

  public:

    void accept(basic_ast_visitor *sp, int level) {
      sp->do_read_node(this, level);
    }

  };

} // m19

#endif
