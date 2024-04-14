#ifndef __PLUS_EQUAL_NODE_H__
#define __PLUS_EQUAL_NODE_H__

#include <cdk/ast/expression_node.h>
#include <cdk/ast/lvalue_node.h>

namespace m19 {

  /**
   * Class for describing the addition ('+=') operator
   */
  class plus_equal_node: public cdk::expression_node {
    cdk::lvalue_node *_lvalue;
    cdk::expression_node *_rvalue;

    public:
      /**
       * @param lineno source code line number for this node
       * @param left first operand
       * @param right second operand
       */
      plus_equal_node(int lineno, cdk::lvalue_node *left, cdk::expression_node *right) :
        cdk::expression_node(lineno), _lvalue(left), _rvalue(right) {
        }

      cdk::lvalue_node *lvalue() {
        return _lvalue;
      }
      cdk::expression_node *rvalue() {
        return _rvalue;
      }
  
      /**
       * @param sp semantic processor visitor
       * @param level syntactic tree level
       */
      void accept(basic_ast_visitor *sp, int level) {
        sp->do_plus_equal_node(this, level);
      }

  };

} // m19 

#endif
