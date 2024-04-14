#ifndef __M19_SECTION_H__
#define __M19_SECTION_H__

#include <cdk/ast/basic_node.h>
#include <cdk/ast/expression_node.h>

namespace m19 {

  class section_node : public cdk::basic_node {
    cdk::expression_node *_expression;
    cdk::basic_node *_block;
    bool _type;

    public:
    section_node(int lineno,  cdk::expression_node *expression,
        cdk::basic_node *block, bool type) : cdk::basic_node(lineno), _expression(expression),
    _block(block), _type(type) { }

    public:
    cdk::expression_node *expression() {
      return _expression;
    }

    cdk::basic_node *block() {
      return _block;
    }

    bool type() {
      return _type;
    }

    void accept(basic_ast_visitor *sp, int level) {
      sp->do_section_node(this, level);
    }
  }; //m19
}
#endif
