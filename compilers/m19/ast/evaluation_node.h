// $Id: evaluation_node.h,v 1.3 2019/05/12 20:19:46 ist186411 Exp $
#ifndef __M19_EVALUATIONNODE_H__
#define __M19_EVALUATIONNODE_H__

#include <cdk/ast/basic_node.h>
#include <cdk/ast/expression_node.h>

namespace m19 {

  class evaluation_node: public cdk::basic_node {
    cdk::expression_node *_argument;

  public:
    evaluation_node(int lineno, cdk::expression_node *argument) :
        cdk::basic_node(lineno), _argument(argument) {
    }

  public:
    cdk::expression_node *argument() {
      return _argument;
    }

    void accept(basic_ast_visitor *sp, int level) {
      sp->do_evaluation_node(this, level);
    }

  };

} // m19

#endif
