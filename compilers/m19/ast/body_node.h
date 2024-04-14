#ifndef __M19_BODY_H__
#define __M19_BODY_H__

#include <cdk/ast/basic_node.h>
#include <cdk/ast/sequence_node.h>
#include "targets/basic_ast_visitor.h"

namespace m19 {

  class body_node: public cdk::basic_node {
    cdk::basic_node *_initial_sec;
    cdk::sequence_node *_middle_sec;
    cdk::basic_node *_final_sec;

  public:
    body_node(int lineno, cdk::basic_node *init, cdk::sequence_node *mid,
        cdk::basic_node *fin) : cdk::basic_node(lineno), _initial_sec(init), _middle_sec(mid), _final_sec(fin) {

    }

  public:
    cdk::basic_node *initial_sec() {
      return _initial_sec;
    }

    cdk::sequence_node *middle_sec() {
      return _middle_sec;
    }

    cdk::basic_node *final_sec() {
      return _final_sec;
    }

    void accept(basic_ast_visitor *sp, int level) {
      sp->do_body_node(this, level);
    }

  };

} // m19

#endif
