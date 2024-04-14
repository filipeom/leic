#include <string>
#include <vector>
#include "targets/type_checker.h"
#include "ast/all.h"  // AUTOMATICALLY GENERATED
#include "m19_parser.tab.h"

#define ASSERT_UNSPEC \
{ if (node->type() != nullptr && \
    node->type()->name() != basic_type::TYPE_UNSPEC) return; }

//===========================================================================
// HELPER METHODS 
//===========================================================================

basic_type *m19::type_checker::copy_type(basic_type *type) {
  basic_type *copy = new basic_type(type->size(), type->name());
  for (basic_type *src = type->subtype(), *dest = copy; nullptr != src; src = src->subtype(), dest = dest->subtype()) {
    dest->_subtype = new basic_type(src->size(), src->name());
  }
  return copy;
}
void m19::type_checker::processUnaryExpression(cdk::unary_expression_node * const node, int lvl) {
  node->argument()->accept(this, lvl + 2);
  if (basic_type::TYPE_INT != node->argument()->type()->name() &&
      basic_type::TYPE_DOUBLE != node->argument()->type()->name()) {
    throw std::string("wrong type in argument of unary expression");
  }

  node->type(node->argument()->type());
}
void m19::type_checker::processIntOnlyExpression(cdk::binary_expression_node * const node, int lvl) {
  ASSERT_UNSPEC;
  node->left()->accept(this, lvl + 2);
  if (basic_type::TYPE_INT != node->left()->type()->name()) {
    throw std::string("wrong type in left argument of binary expression");
  }

  node->right()->accept(this, lvl + 2);
  if (basic_type::TYPE_INT != node->right()->type()->name()) {
    throw std::string("wrong type in right argument of binary expression");
  }

  node->type(new basic_type(4, basic_type::TYPE_INT));
}
void m19::type_checker::processIntDoubleExpression(cdk::binary_expression_node * const node, int lvl) {
  ASSERT_UNSPEC;
  node->left()->accept(this, lvl + 2);
  node->right()->accept(this, lvl + 2);

  if (basic_type::TYPE_DOUBLE == node->left()->type()->name() &&
      basic_type::TYPE_DOUBLE == node->right()->type()->name()) {
    node->type(new basic_type(8, basic_type::TYPE_DOUBLE));
  } else if (basic_type::TYPE_DOUBLE == node->left()->type()->name() &&
      basic_type::TYPE_INT == node->right()->type()->name()) {
    node->type(new basic_type(8, basic_type::TYPE_DOUBLE));
  } else if (basic_type::TYPE_INT == node->left()->type()->name() &&
      basic_type::TYPE_DOUBLE == node->right()->type()->name()) {
    node->type(new basic_type(8, basic_type::TYPE_DOUBLE));
  } else if (basic_type::TYPE_INT == node->left()->type()->name() &&
      basic_type::TYPE_INT == node->right()->type()->name()) {
    node->type(new basic_type(4, basic_type::TYPE_INT));
  } else if (basic_type::TYPE_UNSPEC == node->left()->type()->name() ||
      basic_type::TYPE_UNSPEC == node->right()->type()->name()) {
    node->type(new basic_type(4, basic_type::TYPE_INT));
    node->left()->type(new basic_type(4, basic_type::TYPE_INT));
    node->right()->type(new basic_type(4, basic_type::TYPE_INT));
  } else {
    throw std::string("wrong types in binary expression");
  }
}
void m19::type_checker::processIntDoublePointerExpression(cdk::binary_expression_node * const node, int lvl) {
  ASSERT_UNSPEC;
  node->left()->accept(this, lvl + 2);
  node->right()->accept(this, lvl + 2);

  if (basic_type::TYPE_DOUBLE == node->left()->type()->name() &&
      basic_type::TYPE_DOUBLE == node->right()->type()->name()) {
    node->type(new basic_type(8, basic_type::TYPE_DOUBLE));
  } else if (basic_type::TYPE_DOUBLE == node->left()->type()->name() &&
      basic_type::TYPE_INT == node->right()->type()->name()) {
    node->type(new basic_type(8, basic_type::TYPE_DOUBLE));
  } else if (basic_type::TYPE_INT == node->left()->type()->name() &&
      basic_type::TYPE_DOUBLE == node->right()->type()->name()) {
    node->type(new basic_type(8, basic_type::TYPE_DOUBLE));
  } else if (basic_type::TYPE_POINTER == node->left()->type()->name() &&
      basic_type::TYPE_INT == node->right()->type()->name()) {
    node->type(new basic_type(4, basic_type::TYPE_POINTER));
    node->type()->_subtype = new basic_type(node->left()->type()->subtype()->size(),
        node->left()->type()->subtype()->name());
  } else if (basic_type::TYPE_INT == node->left()->type()->name() &&
      basic_type::TYPE_POINTER == node->right()->type()->name()) {
    node->type(new basic_type(4, basic_type::TYPE_POINTER));
    node->type()->_subtype = new basic_type(node->right()->type()->subtype()->size(),
        node->right()->type()->subtype()->name());
  } else if (basic_type::TYPE_INT == node->left()->type()->name() &&
      basic_type::TYPE_INT == node->right()->type()->name()) {
    node->type(new basic_type(4, basic_type::TYPE_INT));
  } else if (basic_type::TYPE_UNSPEC == node->left()->type()->name() ||
      basic_type::TYPE_UNSPEC == node->right()->type()->name()) {
    node->type(new basic_type(4, basic_type::TYPE_INT));
    node->left()->type(new basic_type(4, basic_type::TYPE_INT));
    node->right()->type(new basic_type(4, basic_type::TYPE_INT));
  } else {
    throw std::string("wrong types in binary expression");
  }
}
void m19::type_checker::processGeneralLogicalExpression(cdk::binary_expression_node * const node, int lvl) {
  ASSERT_UNSPEC;
  node->left()->accept(this, lvl + 2);
  node->right()->accept(this, lvl + 2);
  if (node->left()->type()->name() != node->right()->type()->name()) {
    throw std::string("same type expected on both sides of equality operator");
  }
  node->type(new basic_type(4, basic_type::TYPE_INT));
}

//===========================================================================

void m19::type_checker::do_nil_node(cdk::nil_node * const node, int lvl) {
  // VOID
}
void m19::type_checker::do_data_node(cdk::data_node * const node, int lvl) {
  // VOID
}

//===========================================================================
// SEQUENCE NODE
//===========================================================================

void m19::type_checker::do_sequence_node(cdk::sequence_node * const node, int lvl) {
  for (size_t i = 0; i < node->size(); i++) {
    node->node(i)->accept(this, lvl);
  }
}

//===========================================================================
// LITERALS
//===========================================================================

void m19::type_checker::do_integer_node(cdk::integer_node * const node, int lvl) {
  ASSERT_UNSPEC;

  node->type(new basic_type(4, basic_type::TYPE_INT));
}
void m19::type_checker::do_double_node(cdk::double_node * const node, int lvl) {
  ASSERT_UNSPEC;

  node->type(new basic_type(8, basic_type::TYPE_DOUBLE));
}
void m19::type_checker::do_string_node(cdk::string_node * const node, int lvl) {
  ASSERT_UNSPEC;

  node->type(new basic_type(4, basic_type::TYPE_STRING));
}

//===========================================================================
// OPERATORS 
//===========================================================================

void m19::type_checker::do_neg_node(cdk::neg_node * const node, int lvl) {
  processUnaryExpression(node, lvl);
}
void m19::type_checker::do_identity_node(m19::identity_node * const node, int lvl) {
  processUnaryExpression(node, lvl);
}
void m19::type_checker::do_add_node(cdk::add_node * const node, int lvl) {
  processIntDoublePointerExpression(node, lvl);
}
void m19::type_checker::do_sub_node(cdk::sub_node * const node, int lvl) {
  processIntDoublePointerExpression(node, lvl);
}
void m19::type_checker::do_mul_node(cdk::mul_node * const node, int lvl) {
  processIntDoubleExpression(node, lvl);
}
void m19::type_checker::do_div_node(cdk::div_node * const node, int lvl) {
  processIntDoubleExpression(node, lvl);
}
void m19::type_checker::do_mod_node(cdk::mod_node * const node, int lvl) {
  processIntOnlyExpression(node, lvl);
}
void m19::type_checker::do_lt_node(cdk::lt_node * const node, int lvl) {
  processIntOnlyExpression(node, lvl);
}
void m19::type_checker::do_le_node(cdk::le_node * const node, int lvl) {
  processIntOnlyExpression(node, lvl);
}
void m19::type_checker::do_ge_node(cdk::ge_node * const node, int lvl) {
  processIntOnlyExpression(node, lvl);
}
void m19::type_checker::do_gt_node(cdk::gt_node * const node, int lvl) {
  processIntOnlyExpression(node, lvl);
}
void m19::type_checker::do_ne_node(cdk::ne_node * const node, int lvl) {
  processGeneralLogicalExpression(node, lvl);
}
void m19::type_checker::do_eq_node(cdk::eq_node * const node, int lvl) {
  processGeneralLogicalExpression(node, lvl);
}
void m19::type_checker::do_not_node(cdk::not_node * const node, int lvl) {
  ASSERT_UNSPEC;
  node->argument()->accept(this, lvl + 2);
  if (basic_type::TYPE_INT == node->argument()->type()->name()) {
    node->type(new basic_type(4, basic_type::TYPE_INT));
  } else if (basic_type::TYPE_UNSPEC == node->argument()->type()->name()) {
    node->type(new basic_type(4, basic_type::TYPE_INT));
    node->argument()->type(new basic_type(4, basic_type::TYPE_INT));
  } else {
    throw std::string("wrong type in unary logical expression");
  }
}
void m19::type_checker::do_and_node(cdk::and_node * const node, int lvl) {
  processIntOnlyExpression(node, lvl);
}
void m19::type_checker::do_or_node(cdk::or_node * const node, int lvl) {
  processIntOnlyExpression(node, lvl);
}
void m19::type_checker::do_plus_equal_node(m19::plus_equal_node * const node, int lvl) {
  ASSERT_UNSPEC;

  node->lvalue()->accept(this, lvl + 4);
  node->rvalue()->accept(this, lvl + 4);

  if (basic_type::TYPE_INT == node->lvalue()->type()->name()) {
    if (basic_type::TYPE_INT == node->rvalue()->type()->name()) {
      node->type(new basic_type(4, basic_type::TYPE_INT));
    } else if (basic_type::TYPE_UNSPEC == node->rvalue()->type()->name()) {
      node->type(new basic_type(4, basic_type::TYPE_INT));
      node->rvalue()->type(new basic_type(4, basic_type::TYPE_INT));
    } else
      throw std::string("wrong assignment to integer");
  } else if (basic_type::TYPE_POINTER == node->lvalue()->type()->name()) {
    if (basic_type::TYPE_POINTER == node->rvalue()->type()->name()) {
      // CHECK POINTER LEVEL AND TYPE
      basic_type *ltype = node->lvalue()->type()->subtype();
      basic_type *rtype = node->rvalue()->type();
      for (; nullptr != ltype; ltype = ltype->subtype(), rtype = rtype->subtype()) {
        if (nullptr == rtype->subtype()) {
          throw std::string("undefined right value data type");
        } else if (basic_type::TYPE_UNSPEC == rtype->subtype()->name() &&
            basic_type::TYPE_POINTER != ltype->name()) {
          // STACK ALLOC
          rtype->_subtype = new basic_type(ltype->size(), ltype->name());
        } else if (rtype->subtype()->name() != ltype->name()) {
          throw std::string("wrong assignment to pointer");
        }
      }
      node->type(copy_type(node->lvalue()->type()));
    } else if (basic_type::TYPE_INT == node->rvalue()->type()->name()) {
      // CHECK INT LITERAL FOR 0
      cdk::integer_node *literal = dynamic_cast<cdk::integer_node*>(node->rvalue());
      if (nullptr != literal) {
        if (0 == literal->value()) {
          node->type(new basic_type(4, basic_type::TYPE_POINTER));
        } else {
          throw std::string("invalid integer assignment to pointer");
        }
      } else {
        throw std::string("wrong assignment to pointer");
      }
    } else if (basic_type::TYPE_UNSPEC == node->rvalue()->type()->name()) {
      node->type(new basic_type(4, basic_type::TYPE_ERROR));
      node->rvalue()->type(new basic_type(4, basic_type::TYPE_ERROR));
    } else {
      throw std::string("wrong assignment to pointer");
    }
  } else if (basic_type::TYPE_DOUBLE == node->lvalue()->type()->name()) {
    if (basic_type::TYPE_DOUBLE == node->rvalue()->type()->name() ||
        basic_type::TYPE_INT == node->rvalue()->type()->name()) {
      node->type(new basic_type(8, basic_type::TYPE_DOUBLE));
    } else if (basic_type::TYPE_UNSPEC == node->rvalue()->type()->name()) {
      node->type(new basic_type(8, basic_type::TYPE_DOUBLE));
      node->rvalue()->type(new basic_type(8, basic_type::TYPE_DOUBLE));
    } else {
      throw std::string("wrong assignment to real");
    }
  } else if (basic_type::TYPE_STRING == node->lvalue()->type()->name()) {
    if (basic_type::TYPE_STRING == node->rvalue()->type()->name()) {
      node->type(new basic_type(4, basic_type::TYPE_STRING));
    } else if (basic_type::TYPE_UNSPEC == node->rvalue()->type()->name()) {
      node->type(new basic_type(4, basic_type::TYPE_STRING));
      node->rvalue()->type(new basic_type(4, basic_type::TYPE_STRING));
    } else {
      throw std::string("wrong assignment to string");
    }
  } else {
    throw std::string("wrong types in assignment");
  }

}

//===========================================================================
// VARIABLES
//===========================================================================

void m19::type_checker::do_variable_node(cdk::variable_node * const node, int lvl) {
  ASSERT_UNSPEC;

  const std::string &id = node->name();
  std::shared_ptr<m19::symbol> symbol = _symtab.find(id);
  if (nullptr != symbol) {
    node->type(symbol->type());
  } else {
    throw std::string("undeclared variable \"" + id + "\"");
  }
}
void m19::type_checker::do_variable_declaration_node(m19::variable_declaration_node * const node, int lvl) {
  if (nullptr != node->initializer()) {
    node->initializer()->accept(this, lvl + 2);

    if (basic_type::TYPE_INT == node->varType()->name()) {
      if (basic_type::TYPE_INT != node->initializer()->type()->name()) {
        throw std::string("wrong type for initializer (integer expected).");
      }
    } else if (basic_type::TYPE_DOUBLE == node->varType()->name()) {
      if (basic_type::TYPE_INT != node->initializer()->type()->name() &&
          basic_type::TYPE_DOUBLE != node->initializer()->type()->name()) { 
        throw std::string("wrong type for initializer (integer or double expected).");
      }
    } else if (basic_type::TYPE_STRING == node->varType()->name()) {
      if (basic_type::TYPE_STRING != node->initializer()->type()->name()) {
        throw std::string("wrong type for initializer (string expected).");
      }
    } else if (basic_type::TYPE_POINTER == node->varType()->name()) {
      if (basic_type::TYPE_POINTER != node->initializer()->type()->name() &&
          basic_type::TYPE_INT != node->initializer()->type()->name()) { 
        throw std::string("wrong type for initializer (pointer expected).");
      } else if (basic_type::TYPE_POINTER == node->initializer()->type()->name()) {
        // STACK ALLOC
        if (basic_type::TYPE_UNSPEC == node->initializer()->type()->subtype()->name()) {
          node->initializer()->type()->_subtype = new basic_type(node->varType()->subtype()->size(),
              node->varType()->subtype()->name());
        }
      }
    } else {
      throw std::string("unknown type for initializer.");
    }
  }

  const std::string &id = node->identifier();
  std::shared_ptr<m19::symbol> symbol = std::make_shared<m19::symbol>
    (node->varType(), id, node->qualifier(), (bool)node->initializer(), false);
  if (_symtab.insert(id, symbol)) {
    _parent->set_new_symbol(symbol);
  } else {
    throw std::string("variable \"" + id + "\" redeclared");
  }
}

//===========================================================================
// LVALUES
//===========================================================================

void m19::type_checker::do_rvalue_node(cdk::rvalue_node * const node, int lvl) {
  ASSERT_UNSPEC;

  node->lvalue()->accept(this, lvl);
  node->type(node->lvalue()->type());
}
void m19::type_checker::do_index_node(m19::index_node * const node, int lvl) {
  ASSERT_UNSPEC;

  if (node->base()) {
    node->base()->accept(this, lvl + 2);
    if (basic_type::TYPE_POINTER != node->base()->type()->name()) {
      throw std::string("pointer expression expected in index left-value");
    }
  } 

  node->index()->accept(this, lvl + 2);
  if (basic_type::TYPE_INT != node->index()->type()->name()) {
    throw std::string("integer expression expected in left-value index");
  }

  // JUST MAKE NODE TYPE THE SUBTYPE OF THE POINTER
  node->type(new basic_type(node->base()->type()->subtype()->size(), node->base()->type()->subtype()->name()));
}

//===========================================================================
// EXPRESSIONS
//===========================================================================

void m19::type_checker::do_assignment_node(cdk::assignment_node * const node, int lvl) {
  ASSERT_UNSPEC;

  node->lvalue()->accept(this, lvl + 4);
  node->rvalue()->accept(this, lvl + 4);

  if (basic_type::TYPE_INT == node->lvalue()->type()->name()) {
    if (basic_type::TYPE_INT == node->rvalue()->type()->name()) {
      node->type(new basic_type(4, basic_type::TYPE_INT));
    } else if (basic_type::TYPE_UNSPEC == node->rvalue()->type()->name()) {
      node->type(new basic_type(4, basic_type::TYPE_INT));
      node->rvalue()->type(new basic_type(4, basic_type::TYPE_INT));
    } else
      throw std::string("wrong assignment to integer");
  } else if (basic_type::TYPE_POINTER == node->lvalue()->type()->name()) {
    if (basic_type::TYPE_POINTER == node->rvalue()->type()->name()) {
      // CHECK POINTER LEVEL AND TYPE
      basic_type *ltype = node->lvalue()->type()->subtype();
      basic_type *rtype = node->rvalue()->type();
      for (; nullptr != ltype; ltype = ltype->subtype(), rtype = rtype->subtype()) {
        if (nullptr == rtype->subtype()) {
          throw std::string("undefined right value data type");
        } else if (basic_type::TYPE_UNSPEC == rtype->subtype()->name() &&
            basic_type::TYPE_POINTER != ltype->name()) {
          // STACK ALLOC
          rtype->_subtype = new basic_type(ltype->size(), ltype->name());
        } else if (rtype->subtype()->name() != ltype->name()) {
          throw std::string("wrong assignment to pointer");
        }
      }
      node->type(copy_type(node->lvalue()->type()));
    } else if (basic_type::TYPE_INT == node->rvalue()->type()->name()) {
      // CHECK INT LITERAL FOR 0
      cdk::integer_node *literal = dynamic_cast<cdk::integer_node*>(node->rvalue());
      if (nullptr != literal) {
        if (0 == literal->value()) {
          node->type(new basic_type(4, basic_type::TYPE_POINTER));
        } else {
          throw std::string("invalid integer assignment to pointer");
        }
      } else {
        throw std::string("wrong assignment to pointer");
      }
    } else if (basic_type::TYPE_UNSPEC == node->rvalue()->type()->name()) {
      node->type(new basic_type(4, basic_type::TYPE_ERROR));
      node->rvalue()->type(new basic_type(4, basic_type::TYPE_ERROR));
    } else {
      throw std::string("wrong assignment to pointer");
    }
  } else if (basic_type::TYPE_DOUBLE == node->lvalue()->type()->name()) {
    if (basic_type::TYPE_DOUBLE == node->rvalue()->type()->name() ||
        basic_type::TYPE_INT == node->rvalue()->type()->name()) {
      node->type(new basic_type(8, basic_type::TYPE_DOUBLE));
    } else if (basic_type::TYPE_UNSPEC == node->rvalue()->type()->name()) {
      node->type(new basic_type(8, basic_type::TYPE_DOUBLE));
      node->rvalue()->type(new basic_type(8, basic_type::TYPE_DOUBLE));
    } else {
      throw std::string("wrong assignment to real");
    }
  } else if (basic_type::TYPE_STRING == node->lvalue()->type()->name()) {
    if (basic_type::TYPE_STRING == node->rvalue()->type()->name()) {
      node->type(new basic_type(4, basic_type::TYPE_STRING));
    } else if (basic_type::TYPE_UNSPEC == node->rvalue()->type()->name()) {
      node->type(new basic_type(4, basic_type::TYPE_STRING));
      node->rvalue()->type(new basic_type(4, basic_type::TYPE_STRING));
    } else {
      throw std::string("wrong assignment to string");
    }
  } else {
    throw std::string("wrong types in assignment");
  }
}
void m19::type_checker::do_address_of_node(m19::address_of_node * const node, int lvl) {
  ASSERT_UNSPEC;

  node->lvalue()->accept(this, lvl + 2);
  if (basic_type::TYPE_DOUBLE == node->lvalue()->type()->name()) {
    node->type(new basic_type(4, basic_type::TYPE_POINTER));
    node->type()->_subtype = new basic_type(8, basic_type::TYPE_DOUBLE);
  } else if (basic_type::TYPE_INT == node->lvalue()->type()->name()) {
    node->type(new basic_type(4, basic_type::TYPE_POINTER));
    node->type()->_subtype = new basic_type(4, basic_type::TYPE_INT);
  } else if (basic_type::TYPE_STRING == node->lvalue()->type()->name()) {
    node->type(new basic_type(4, basic_type::TYPE_POINTER));
    node->type()->_subtype = new basic_type(4, basic_type::TYPE_STRING);
  } else {
    throw std::string("wrong type in unary logical expression");
  }
}
void m19::type_checker::do_read_node(m19::read_node * const node, int lvl) {
  node->type(new basic_type(0, basic_type::TYPE_UNSPEC));
}
void m19::type_checker::do_stack_alloc_node(m19::stack_alloc_node * const node, int lvl) {
  ASSERT_UNSPEC;

  node->argument()->accept(this, lvl + 2);
  if (basic_type::TYPE_INT != node->argument()->type()->name()) { 
    throw std::string("integer expression expected in allocation expression");
  }

  node->type(new basic_type(4, basic_type::TYPE_POINTER));
  node->type()->_subtype = new basic_type(0, basic_type::TYPE_UNSPEC);
}

//===========================================================================
// FUNCTION BODY
//===========================================================================

void m19::type_checker::do_block_node(m19::block_node * const node, int lvl) {
  // VOID 
}
void m19::type_checker::do_initial_section_node(m19::initial_section_node * const node, int lvl) {
  // VOID 
}
void m19::type_checker::do_section_node(m19::section_node * const node, int lvl) {
  node->expression()->accept(this, lvl + 4);
  if (basic_type::TYPE_INT != node->expression()->type()->name()) {
    throw std::string("expected integer condition");
  }
}
void m19::type_checker::do_final_section_node(m19::final_section_node * const node, int lvl) {
  // VOID 
}
void m19::type_checker::do_body_node(m19::body_node * const node, int lvl) {
  // VOID 
}

//===========================================================================
// GENERAL INSTRUCTIONS
//===========================================================================

void m19::type_checker::do_print_node(m19::print_node * const node, int lvl) {
  node->argument()->accept(this, lvl + 2);
}
void m19::type_checker::do_break_node(m19::break_node * const node, int lvl) {
  // VOID
}
void m19::type_checker::do_continue_node(m19::continue_node * const node, int lvl) {
  // VOID 
}
void m19::type_checker::do_return_node(m19::return_node * const node, int lvl) {
  // VOID 
}
void m19::type_checker::do_evaluation_node(m19::evaluation_node * const node, int lvl) {
  node->argument()->accept(this, lvl + 2);
}
void m19::type_checker::do_apply_node(m19::apply_node * const node, int lvl) {

  const std::string &id = (("@" == node->function()) ? _function->name() : node->function());
  std::shared_ptr<m19::symbol> symbol = _symtab.find(id);

  if (nullptr == symbol) {
    throw std::string("symbol \"" + id + "\" is undeclared.");
  }

  if (false == symbol->isFunction()) {
    throw std::string("symbol \"" + id + "\" is not a function.");
  }

  node->base()->accept(this, lvl + 2);
  if (basic_type::TYPE_POINTER != node->base()->type()->name()) {
    throw std::string("pointer expression expected in index left-value");
  }

  node->from()->accept(this, lvl + 2);
  if (basic_type::TYPE_INT != node->from()->type()->name()) {
    throw std::string("integer expected in left range value");
  }

  node->to()->accept(this, lvl + 2);
  if (basic_type::TYPE_INT != node->from()->type()->name()) {
    throw std::string("integer expected in right range value");
  }

}

//===========================================================================
// CONDITIONAL INSTRUCTIONS
//===========================================================================

void m19::type_checker::do_if_else_node(m19::if_else_node * const node, int lvl) {
  node->condition()->accept(this, lvl + 4);
  if (basic_type::TYPE_INT != node->condition()->type()->name()) {
    throw std::string("expected integer condition");
  }
}
void m19::type_checker::do_if_node(m19::if_node * const node, int lvl) {
  node->condition()->accept(this, lvl + 4);
  if (basic_type::TYPE_INT != node->condition()->type()->name()) {
    throw std::string("expected integer condition");
  }
}

//===========================================================================
// FOR INSTRUCTION
//===========================================================================

void m19::type_checker::do_for_node(m19::for_node * const node, int lvl) {
  // VOID
}

//===========================================================================
// FUNCTIONS
//===========================================================================

void m19::type_checker::do_function_call_node(m19::function_call_node * const node, int lvl) {
  ASSERT_UNSPEC;

  const std::string &id = (("@" == node->identifier()) ? _function->name() : node->identifier());
  std::shared_ptr<m19::symbol> symbol = _symtab.find(id);

  if (nullptr == symbol) {
    throw std::string("symbol \"" + id + "\" is undeclared.");
  }

  if (false == symbol->isFunction()) {
    throw std::string("symbol \"" + id + "\" is not a function.");
  }

  node->type(symbol->type());

  if (nullptr != node->arguments()) {
    if (symbol->arguments().size() == node->arguments()->size()) {
      for (size_t ix = 0; ix < node->arguments()->size(); ix++) {
        cdk::expression_node *arg = dynamic_cast<cdk::expression_node*>(node->arguments()->node(ix));
        arg->accept(this, lvl + 4);
        if (basic_type::TYPE_INT == symbol->arguments()[ix]) {
          if (basic_type::TYPE_INT != arg->type()->name()) {
            throw std::string("expecting integer in function call argument");
          }
        } else if (basic_type::TYPE_DOUBLE == symbol->arguments()[ix]) {
          if (basic_type::TYPE_DOUBLE != arg->type()->name() &&
              basic_type::TYPE_INT != arg->type()->name()) {
            throw std::string("expecting double in function call argument");
          }
        } else if (basic_type::TYPE_STRING == symbol->arguments()[ix]) {
          if (basic_type::TYPE_STRING != arg->type()->name()) {
            throw std::string("expecting string in function call argument");
          }
        } else if (basic_type::TYPE_POINTER == symbol->arguments()[ix]) {
          if (basic_type::TYPE_POINTER != arg->type()->name() &&
              basic_type::TYPE_INT != arg->type()->name()) {
            throw std::string("expecting pointer in function call argument");
          } 
          // TODO: VERIFY LITERAL 0 OF POINTER
        }
        // TODO: STACK ALLOC - TYPE UNSPEC IN SUBTYPE OF POINTER
        // TODO: READ - UNSPEC
      }
    } else {
      throw std::string("unexpected number of arguments in function call");
    }
  }
}
void m19::type_checker::do_function_declaration_node(m19::function_declaration_node * const node, int lvl) {
  std::string id;

  if ("m19" == node->identifier()) {
    id = "_main";
  } else if ("_main" == node->identifier()) {
    id = "._main";
  } else {
    id = node->identifier();
  }

  std::shared_ptr<m19::symbol> function = std::make_shared<m19::symbol>
    (node->type(), id, node->qualifier(), false, true, true);

  if (nullptr != node->arguments()) {
    std::vector<basic_type::type> args;
    for (size_t ix = 0; ix < node->arguments()->size(); ix++) {
      m19::variable_declaration_node *arg = dynamic_cast<m19::variable_declaration_node*>(node->arguments()->node(ix));
      if (nullptr != arg) {
        args.push_back(arg->varType()->name());
      } else {
        // SHOULD NEVER HAPPEN, PARSER DOESN'T ALLOW THIS
        throw std::string("invalid argument declaration");
      }
    }
    function->arguments(args);
  }

  std::shared_ptr<m19::symbol> previous = _symtab.find(function->name());
  if (nullptr != previous) {
    throw std::string("conflicting declaration for \"" + id + "\"");
  } else {
    _symtab.insert(function->name(), function);
    _parent->set_new_symbol(function);
  }
}
void m19::type_checker::do_function_definition_node(m19::function_definition_node * const node, int lvl) {
  std::string id;

  if ("m19" == node->identifier()) {
    id = "_main";
  } else if ("_main" == node->identifier()) {
    id = "._main";
  } else {
    id = node->identifier();
  }

  std::shared_ptr<m19::symbol> function = std::make_shared<m19::symbol>
    (node->type(), id, node->qualifier(), false, true);

  function->set_offset(-node->type()->size());
  if (nullptr != node->arguments()) {
    std::vector<basic_type::type> args;
    for (size_t ix = 0; ix < node->arguments()->size(); ix++) {
      m19::variable_declaration_node *arg = dynamic_cast<m19::variable_declaration_node*>(node->arguments()->node(ix));
      if (nullptr != arg) {
        args.push_back(arg->varType()->name());
      } else {
        // SHOULD NEVER HAPPEN, PARSER DOESN'T ALLOW THIS
        throw std::string("invalid argument declaration");
      }
    }
    function->arguments(args);
  }

  std::shared_ptr<m19::symbol> previous = _symtab.find(function->name());
  if (nullptr != previous) {
    if ((tPUBLIC == previous->qualifier() && tPUBLIC == node->qualifier()) ||
        (tPRIVATE == previous->qualifier() && tPRIVATE == node->qualifier())) {
      _symtab.replace(function->name(), function);
      _parent->set_new_symbol(function);
    } else {
      throw std::string("conflicting definition for \"" + function->name() + "\"");
    }
  } else {
    _symtab.insert(function->name(), function);
    _parent->set_new_symbol(function);
  }
}
