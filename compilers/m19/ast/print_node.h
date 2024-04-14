// $Id: print_node.h,v 1.3 2019/05/12 20:19:46 ist186411 Exp $ -*- c++ -*-
#ifndef __M19_PRINTNODE_H__
#define __M19_PRINTNODE_H__

#include <cdk/ast/expression_node.h>

namespace m19 {

  class print_node: public cdk::basic_node {
    cdk::expression_node *_argument;
    bool _newline = false;

  public:
    print_node(int lineno, cdk::expression_node *argument, bool newline = false) :
        cdk::basic_node(lineno), _argument(argument), _newline(newline) {
    }

  public:
    cdk::expression_node *argument() {
      return _argument;
    }

    bool newline() {
      return _newline;
    }

    void accept(basic_ast_visitor *sp, int level) {
      sp->do_print_node(this, level);
    }

  };

} // m19

#endif
