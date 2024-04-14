#ifndef __M19_VARIABLE_DECLARATION_H__
#define __M19_VARIABLE_DECLARATION_H__

#include <cdk/ast/basic_node.h>
#include <cdk/ast/expression_node.h>
#include <cdk/basic_type.h>

namespace m19 {

  class variable_declaration_node: public cdk::basic_node {
    basic_type *_varType;
    std::string _identifier;
    int _qualifier;
    cdk::expression_node *_initializer;

  public:
    variable_declaration_node(int lineno, basic_type *varType, const std::string &identifier, int qualifier,
                              cdk::expression_node *initializer) :
        cdk::basic_node(lineno), _varType(varType), _identifier(identifier), _qualifier(qualifier),  _initializer(initializer) {
    }

  public:
    bool constant() {
      return _varType == _varType->subtype();
    }
    basic_type *varType() {
      return _varType;
    }
    const std::string &identifier() const {
      return _identifier;
    }
    int qualifier() {
      return _qualifier;
    }

    cdk::expression_node *initializer() {
      return _initializer;
    }

    void accept(basic_ast_visitor *sp, int level) {
      sp->do_variable_declaration_node(this, level);
    }

  };

} // m19

#endif
