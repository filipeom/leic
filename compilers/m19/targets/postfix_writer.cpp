#include <string>
#include <sstream>
#include "targets/type_checker.h"
#include "targets/postfix_writer.h"
#include "targets/frame_size_calculator.h"
#include "ast/all.h"
#include "m19_parser.tab.h"

//===========================================================================
// HELPER METHODS 
//===========================================================================

basic_type *m19::postfix_writer::copy_type(basic_type *type) {
  basic_type *copy = new basic_type(type->size(), type->name());
  for (basic_type *src = type->subtype(), *dest = copy; nullptr != src; src = src->subtype(), dest = dest->subtype()) {
    dest->_subtype = new basic_type(src->size(), src->name());
  }
  return copy;
}

//===========================================================================

void m19::postfix_writer::do_nil_node(cdk::nil_node * const node, int lvl) {
  // VOID 
}
void m19::postfix_writer::do_data_node(cdk::data_node * const node, int lvl) {
  // VOID 
}

//===========================================================================
// SEQUENCE NODE
//===========================================================================

void m19::postfix_writer::do_sequence_node(cdk::sequence_node * const node, int lvl) {
  for (size_t i = 0; i < node->size(); i++) {
    node->node(i)->accept(this, lvl);
  }
}

//===========================================================================
// LITERALS
//===========================================================================

void m19::postfix_writer::do_integer_node(cdk::integer_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;

  if (nullptr != _function) {
    _pf.INT(node->value());
  } else {
    _pf.SINT(node->value());
  }
}
void m19::postfix_writer::do_double_node(cdk::double_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;

  if (nullptr != _function) {
    _pf.DOUBLE(node->value());
  } else {
    _pf.SDOUBLE(node->value());
  }
}
void m19::postfix_writer::do_string_node(cdk::string_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;

  int strlit = ++_lbl;

  _pf.RODATA();
  _pf.ALIGN();
  _pf.LABEL(mklbl(strlit));
  _pf.SSTRING(node->value());
  if (nullptr != _function) {
    _pf.TEXT();
    _pf.ADDR(mklbl(strlit));
  } else {
    _pf.DATA();
    _pf.SADDR(mklbl(strlit));
  }
}

//===========================================================================
// OPERATORS 
//===========================================================================

void m19::postfix_writer::do_neg_node(cdk::neg_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;

  node->argument()->accept(this, lvl);
  _pf.NEG();
}
void m19::postfix_writer::do_identity_node(m19::identity_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;

  node->argument()->accept(this, lvl);
}
void m19::postfix_writer::do_add_node(cdk::add_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;

  node->left()->accept(this, lvl);
  if (basic_type::TYPE_DOUBLE == node->type()->name() &&
      basic_type::TYPE_INT == node->left()->type()->name()) {
    _pf.I2D();
  } else if (basic_type::TYPE_POINTER == node->type()->name() &&
      basic_type::TYPE_INT == node->left()->type()->name()) {
    if (basic_type::TYPE_DOUBLE == node->type()->subtype()->name()) {
      _pf.INT(3);
    } else {
      _pf.INT(2);
    }
    _pf.SHTL();
  }

  node->right()->accept(this, lvl);
  if (basic_type::TYPE_DOUBLE == node->type()->name() &&
      basic_type::TYPE_INT == node->right()->type()->name()) {
    _pf.I2D();
  } else if (basic_type::TYPE_POINTER == node->type()->name() &&
      basic_type::TYPE_INT == node->right()->type()->name()) {
    if (basic_type::TYPE_DOUBLE == node->type()->subtype()->name()) {
      _pf.INT(3);
    } else {
      _pf.INT(2);
    }
    _pf.SHTL();
  }

  if (basic_type::TYPE_DOUBLE == node->type()->name()) {
    _pf.DADD();
  } else {
    _pf.ADD();
  } 
}
void m19::postfix_writer::do_sub_node(cdk::sub_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;

  node->left()->accept(this, lvl);
  if (basic_type::TYPE_DOUBLE == node->type()->name() &&
      basic_type::TYPE_INT == node->left()->type()->name()) {
    _pf.I2D();
  }

  node->right()->accept(this, lvl);
  if (basic_type::TYPE_DOUBLE == node->type()->name() &&
      basic_type::TYPE_INT == node->right()->type()->name()) {
    _pf.I2D();
  } else if (basic_type::TYPE_POINTER == node->type()->name() &&
      basic_type::TYPE_INT == node->right()->type()->name()) {
    if (basic_type::TYPE_DOUBLE == node->type()->subtype()->name()) {
      _pf.INT(3);
    } else {
      _pf.INT(2);
    }
    _pf.SHTL();
  }

  if (basic_type::TYPE_DOUBLE == node->type()->name()) {
    _pf.DSUB();
  } else { 
    _pf.SUB();
  }
}
void m19::postfix_writer::do_mul_node(cdk::mul_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;

  node->left()->accept(this, lvl);
  if (basic_type::TYPE_DOUBLE == node->type()->name() &&
      basic_type::TYPE_INT == node->left()->type()->name()) {
    _pf.I2D();
  }

  node->right()->accept(this, lvl);
  if (basic_type::TYPE_DOUBLE == node->type()->name() &&
      basic_type::TYPE_INT == node->right()->type()->name()) {
    _pf.I2D();
  }

  if (basic_type::TYPE_DOUBLE == node->type()->name()) {
    _pf.DMUL();
  } else { 
    _pf.MUL();
  }
}
void m19::postfix_writer::do_div_node(cdk::div_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;

  node->left()->accept(this, lvl);
  if (basic_type::TYPE_DOUBLE == node->type()->name() &&
      basic_type::TYPE_INT == node->left()->type()->name()) {
    _pf.I2D();
  }

  node->right()->accept(this, lvl);
  if (basic_type::TYPE_DOUBLE == node->type()->name() &&
      basic_type::TYPE_INT == node->right()->type()->name()) {
    _pf.I2D();
  }

  if (basic_type::TYPE_DOUBLE == node->type()->name()) {
    _pf.DDIV();
  } else { 
    _pf.DIV();
  }
}
void m19::postfix_writer::do_mod_node(cdk::mod_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;

  node->left()->accept(this, lvl);
  node->right()->accept(this, lvl);
  _pf.MOD();
}
void m19::postfix_writer::do_lt_node(cdk::lt_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;

  node->left()->accept(this, lvl);
  node->right()->accept(this, lvl);
  _pf.LT();
}
void m19::postfix_writer::do_le_node(cdk::le_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;

  node->left()->accept(this, lvl);
  node->right()->accept(this, lvl);
  _pf.LE();
}
void m19::postfix_writer::do_ge_node(cdk::ge_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;

  node->left()->accept(this, lvl);
  node->right()->accept(this, lvl);
  _pf.GE();
}
void m19::postfix_writer::do_gt_node(cdk::gt_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;

  node->left()->accept(this, lvl);
  node->right()->accept(this, lvl);
  _pf.GT();
}
void m19::postfix_writer::do_ne_node(cdk::ne_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;

  node->left()->accept(this, lvl + 2);
  if (basic_type::TYPE_INT == node->left()->type()->name() &&
      basic_type::TYPE_DOUBLE == node->right()->type()->name()) {
    _pf.I2D();
  }

  node->right()->accept(this, lvl + 2);
  if (basic_type::TYPE_INT == node->right()->type()->name() &&
      basic_type::TYPE_DOUBLE == node->left()->type()->name()) {
    _pf.I2D();
  }

  _pf.NE();
}
void m19::postfix_writer::do_eq_node(cdk::eq_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;

  node->left()->accept(this, lvl + 2);
  if (basic_type::TYPE_INT == node->left()->type()->name() &&
      basic_type::TYPE_DOUBLE == node->right()->type()->name()) {
    _pf.I2D();
  }

  node->right()->accept(this, lvl + 2);
  if (basic_type::TYPE_INT == node->right()->type()->name() &&
      basic_type::TYPE_DOUBLE == node->left()->type()->name()) {
    _pf.I2D();
  }

  _pf.EQ();
}
void m19::postfix_writer::do_not_node(cdk::not_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;

  _pf.NOT();
}
void m19::postfix_writer::do_and_node(cdk::and_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;

  int endlbl = ++_lbl;

  node->left()->accept(this, lvl + 2);
  _pf.DUP32();
  _pf.JZ(mklbl(endlbl));
  node->right()->accept(this, lvl + 2);
  _pf.AND();

  _pf.ALIGN();
  _pf.LABEL(mklbl(endlbl));
}
void m19::postfix_writer::do_or_node(cdk::or_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;

  int endlbl = ++_lbl;

  node->left()->accept(this, lvl + 2);
  _pf.DUP32();
  _pf.JNZ(mklbl(endlbl));
  node->right()->accept(this, lvl + 2);
  _pf.OR();

  _pf.ALIGN();
  _pf.LABEL(mklbl(endlbl));
}
void m19::postfix_writer::do_plus_equal_node(m19::plus_equal_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;

  node->rvalue()->accept(this, lvl + 2);

  node->lvalue()->accept(this, lvl + 2);
  if (basic_type::TYPE_DOUBLE == node->type()->name()) {
    _pf.LDDOUBLE();
  } else {
    _pf.LDINT();
  }

  _pf.ADD();
  if (basic_type::TYPE_DOUBLE == node->type()->name()) {
    _pf.DUP64();
  } else {
    _pf.DUP32();
  }

  node->lvalue()->accept(this, lvl + 2);
  if (basic_type::TYPE_DOUBLE == node->type()->name()) {
    _pf.STDOUBLE();
  } else {
    _pf.STINT();
  }

}

//===========================================================================
// VARIABLES
//===========================================================================

void m19::postfix_writer::do_variable_node(cdk::variable_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;

  std::shared_ptr<m19::symbol> symbol = _symtab.find(node->name());
  if (0 == symbol->offset()) {
    _pf.ADDR(symbol->name());
  } else {
    _pf.LOCAL(symbol->offset());
  }
}
void m19::postfix_writer::do_variable_declaration_node(m19::variable_declaration_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;

  int offset = 0;
  int typesize = node->varType()->size();
  const std::string &id = node->identifier();

  // CALCULATE OFFSET
  if (_inFunctionArgs) {
    offset = _offset;
    _offset += typesize;
  } else if (_inFunctionBody) {
    _offset -= typesize;
    offset = _offset;
  } else {
    offset = 0;
  }

  std::shared_ptr<m19::symbol> symbol = new_symbol();
  if (nullptr != symbol) {
    symbol->set_offset(offset);
    reset_new_symbol();
  }

  if (_inFunctionArgs) {
    // NOTHING MORE TO DO
    return;
  } else if (_inFunctionBody) {
    if (nullptr != node->initializer()) {
      node->initializer()->accept(this, lvl);
      if (basic_type::TYPE_INT == node->varType()->name() ||
          basic_type::TYPE_STRING == node->varType()->name() ||
          basic_type::TYPE_POINTER == node->varType()->name()) {
        _pf.LOCAL(symbol->offset());
        _pf.STINT();
      } else if (basic_type::TYPE_DOUBLE == node->varType()->name()) {
        if (basic_type::TYPE_INT == node->initializer()->type()->name()) {
          _pf.I2D();
        }
        _pf.LOCAL(symbol->offset());
        _pf.STDOUBLE();
      } else {
        error(node->lineno(), "cannot initialize");
      }
    }
  } else {
    if (nullptr != node->initializer()) {
      _pf.DATA();
      _pf.ALIGN();
      _pf.LABEL(id);
      if (basic_type::TYPE_INT == node->varType()->name() ||
          basic_type::TYPE_STRING == node->varType()->name() ||
          basic_type::TYPE_POINTER == node->varType()->name()) {
        node->initializer()->accept(this, lvl); 
      } else if (basic_type::TYPE_DOUBLE == node->varType()->name()) {
        if (basic_type::TYPE_DOUBLE == node->initializer()->type()->name()) {
          node->initializer()->accept(this, lvl);
        } else if (basic_type::TYPE_INT == node->initializer()->type()->name()) {
          // DOESN'T GO TO THE STACK, SO WE CAN'T USE I2D()
          cdk::integer_node *integer = dynamic_cast<cdk::integer_node *>(node->initializer());
          cdk::double_node i2d(integer->lineno(), integer->value());
          i2d.accept(this, lvl);
        } else {
          error(node->lineno(), "\"" + id + "\" has bad initializer for real value");
        }
      } else {
        error(node->lineno(), "\"" + id + "\" has unexpected initializer");
      }
    } else {
      _pf.BSS();
      _pf.ALIGN();
      _pf.LABEL(id);
      _pf.SALLOC(typesize);
    } 
  }
  return;
}

//===========================================================================
// LVALUES
//===========================================================================

void m19::postfix_writer::do_rvalue_node(cdk::rvalue_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;

  node->lvalue()->accept(this, lvl);
  if (basic_type::TYPE_DOUBLE == node->type()->name()) {
    _pf.LDDOUBLE();
  } else {
    _pf.LDINT();
  }
}
void m19::postfix_writer::do_index_node(m19::index_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;

  if (node->base()) {
    node->base()->accept(this, lvl + 2);
  } 

  node->index()->accept(this, lvl + 2);
  if (basic_type::TYPE_DOUBLE == node->type()->name()) {
    _pf.INT(3);
  } else {
    _pf.INT(2);
  }
  _pf.SHTL();
  _pf.ADD();
}

//===========================================================================
// EXPRESSIONS
//===========================================================================

void m19::postfix_writer::do_assignment_node(cdk::assignment_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;

  node->rvalue()->accept(this, lvl + 2);
  if (basic_type::TYPE_DOUBLE == node->type()->name()) {
    if (basic_type::TYPE_INT == node->rvalue()->type()->name()) {
      _pf.I2D();
    }
    _pf.DUP64();
  } else {
    _pf.DUP32();
  }

  node->lvalue()->accept(this, lvl + 2);
  if (basic_type::TYPE_DOUBLE == node->type()->name()) {
    _pf.STDOUBLE();
  } else {
    _pf.STINT();
  }
}
void m19::postfix_writer::do_address_of_node(m19::address_of_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;

  node->lvalue()->accept(this, lvl + 2);
}
void m19::postfix_writer::do_read_node(m19::read_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;

  _functionsToDeclare.insert("readi");
  _pf.CALL("readi");
  _pf.LDFVAL32();
}
void m19::postfix_writer::do_stack_alloc_node(m19::stack_alloc_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;

  node->argument()->accept(this, lvl + 2);
  if (basic_type::TYPE_DOUBLE == node->type()->subtype()->name()) {
    _pf.INT(3);
  } else {
    _pf.INT(2);
  }
  _pf.SHTL();
  _pf.ALLOC();
  _pf.SP();
}

//===========================================================================
// FUNCTION BODY
//===========================================================================

void m19::postfix_writer::do_block_node(m19::block_node * const node, int lvl) {
  if (nullptr != node->declarations()) {
    node->declarations()->accept(this, lvl + 2);
  }

  if (nullptr != node->instructions()) {
    node->instructions()->accept(this, lvl + 2);
  }
}
void m19::postfix_writer::do_initial_section_node(m19::initial_section_node * const node, int lvl) {
  node->block()->accept(this, lvl);
}
void m19::postfix_writer::do_section_node(m19::section_node * const node, int lvl) {
  int endlbl = ++_lbl;

  if (nullptr != node->expression()) {
    node->expression()->accept(this, lvl + 2);
    _pf.JZ(mklbl(endlbl));
  }

  _symtab.push();
  node->block()->accept(this, lvl);
  _symtab.pop();

  if (false != node->type()) {
    _pf.JMP(mklbl(_currentBodyFinalSecLabel));
  }

  _pf.ALIGN();
  _pf.LABEL(mklbl(endlbl));

}
void m19::postfix_writer::do_final_section_node(m19::final_section_node * const node, int lvl) {
  node->block()->accept(this, lvl);
}
void m19::postfix_writer::do_body_node(m19::body_node * const node, int lvl) {
  _currentBodyFinalSecLabel = ++_lbl;

  _symtab.push();
  if (nullptr != node->initial_sec()) {
    node->initial_sec()->accept(this, lvl + 2);
  }

  if (nullptr != node->middle_sec()) {
    node->middle_sec()->accept(this, lvl + 2);
  }

  _pf.ALIGN();
  _pf.LABEL(mklbl(_currentBodyFinalSecLabel));

  if (nullptr != node->final_sec()) {
    _inFinalSec = true;
    node->final_sec()->accept(this, lvl + 2);
    _inFinalSec = false;
  }
  _symtab.pop();
}

//===========================================================================
// GENERAL INSTRUCTIONS
//===========================================================================

void m19::postfix_writer::do_print_node(m19::print_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;

  node->argument()->accept(this, lvl);
  if (basic_type::TYPE_INT == node->argument()->type()->name()) {
    _functionsToDeclare.insert("printi");
    _pf.CALL("printi");
    _pf.TRASH(4);
  } else if (basic_type::TYPE_DOUBLE == node->argument()->type()->name()) {
    _functionsToDeclare.insert("printd");
    _pf.CALL("printd");
    _pf.TRASH(8);
  } else if (basic_type::TYPE_STRING == node->argument()->type()->name()) {
    _functionsToDeclare.insert("prints");
    _pf.CALL("prints");
    _pf.TRASH(4);
  } else {
    error(node->lineno(), "cannot print expression of unknown type");
    return;
  }

  if (node->newline()) {
    _functionsToDeclare.insert("println");
    _pf.CALL("println");
  }
}
void m19::postfix_writer::do_break_node(m19::break_node * const node, int lvl) {
  if (0 == _forEnd.size()) {
    error(node->lineno(), "\"break\" outside of for-loop");
    return;
  }

  _pf.JMP(mklbl(_forEnd.top()));
}
void m19::postfix_writer::do_continue_node(m19::continue_node * const node, int lvl) {
  if (0 == _forIncr.size()) {
    error(node->lineno(), "\"continue\" outside of for-loop");
    return;
  }

  _pf.JMP(mklbl(_forIncr.top()));
}
void m19::postfix_writer::do_return_node(m19::return_node * const node, int lvl) {
  if (_inFinalSec) {
    _pf.JMP(mklbl(_currentFunctionExitLabel));
  } else {
    // FIXME IF THERE'S TIME - THIS WILL WORK BECAUSE IF THERE'S NO FINAL SECTION
    // IT WILL JUMP TO WHERE THE FINAL SECTION SHOULD HAVE BEEN WHICH WILL BE THE
    // THE END OF THE FUNCTION
    _pf.JMP(mklbl(_currentBodyFinalSecLabel));
  }
}
void m19::postfix_writer::do_evaluation_node(m19::evaluation_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;

  node->argument()->accept(this, lvl);
  _pf.TRASH(node->argument()->type()->size());
}
void m19::postfix_writer::do_apply_node(m19::apply_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;

  int offset;
  int condlbl = ++_lbl;
  int endlbl = ++_lbl;

  // AUTOMATIC LOCAL VARIABLE THAT HOLDS CURRENT INDEX
  _offset -= 4;
  offset = _offset;

  node->from()->accept(this, lvl);
  _pf.LOCAL(offset);
  _pf.STINT();

  _pf.ALIGN();
  _pf.LABEL(mklbl(condlbl));
  // WHILE CONDITION
  _pf.LOCAL(offset);
  _pf.LDINT();
  node->to()->accept(this, lvl);
  _pf.LE();
  _pf.JZ(mklbl(endlbl));
  
  // ADD ARGUMENTS TO STACK 
  node->base()->accept(this, lvl);
  _pf.LOCAL(offset);
  _pf.LDINT();
  if (basic_type::TYPE_DOUBLE == node->base()->type()->subtype()->name()) {
    _pf.INT(3);
  } else {
    _pf.INT(2);
  }
  _pf.SHTL();
  // FUNCTION CALL
  _pf.ADD();
  _pf.CALL(node->function());
  _pf.TRASH(4);
  _pf.LDFVAL32();
  // INSTRUCTION ;
  _pf.TRASH(4);
  
  // INCREMENT LOCAL
  _pf.LOCAL(offset);
  _pf.LDINT();
  _pf.INT(1);
  _pf.ADD();
  _pf.DUP32();
  _pf.LOCAL(offset);
  _pf.STINT();
  // INSTRUCTION ;
  _pf.TRASH(4);

  // JUMP TO COND
  _pf.JMP(mklbl(condlbl));

  // WHILE END
  _pf.ALIGN();
  _pf.LABEL(mklbl(endlbl));
}

//===========================================================================
// CONDITIONAL INSTRUCTIONS
//===========================================================================

void m19::postfix_writer::do_if_else_node(m19::if_else_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;

  int endlbl; 
  int elselbl = endlbl = ++_lbl;

  node->condition()->accept(this, lvl);
  _pf.JZ(mklbl(elselbl));

  node->thenblock()->accept(this, lvl + 2);
  if (nullptr != node->elseblock()) {
    _pf.JMP(mklbl(endlbl = ++_lbl));
    _pf.ALIGN();
    _pf.LABEL(mklbl(elselbl));
    node->elseblock()->accept(this, lvl + 2);
  }

  _pf.ALIGN(); 
  _pf.LABEL(mklbl(endlbl));
}
void m19::postfix_writer::do_if_node(m19::if_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;

  int endlbl = ++_lbl;

  node->condition()->accept(this, lvl);
  _pf.JZ(mklbl(endlbl));

  node->block()->accept(this, lvl + 2);

  _pf.ALIGN();
  _pf.LABEL(mklbl(endlbl));
}

//===========================================================================
// FOR INSTRUCTION
//===========================================================================

void m19::postfix_writer::do_for_node(m19::for_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;

  int condlbl = ++_lbl;
  int incrlbl = ++_lbl;
  int endlbl = ++_lbl;

  _forIncr.push(incrlbl);
  _forEnd.push(endlbl);

  node->init()->accept(this, lvl + 2);

  _pf.ALIGN();
  _pf.LABEL(mklbl(condlbl));
  node->condition()->accept(this, lvl);
  _pf.JZ(mklbl(endlbl));

  node->block()->accept(this, lvl + 2);

  _pf.ALIGN();
  _pf.LABEL(mklbl(incrlbl));
  node->incr()->accept(this, lvl + 2);
  _pf.JMP(mklbl(condlbl));

  _pf.ALIGN();
  _pf.LABEL(mklbl(endlbl));

  _forEnd.pop();
  _forIncr.pop();
}

//===========================================================================
// FUNCTIONS
//===========================================================================

void m19::postfix_writer::do_function_call_node(m19::function_call_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;

  const std::string &id = ((node->identifier() == "@") ? _function->name() : node->identifier());
  std::shared_ptr<m19::symbol> symbol = _symtab.find(id);

  size_t argsSize = 0;
  if (nullptr != node->arguments()) {
    for (size_t ax = node->arguments()->size(); ax > 0; ax--) {
      cdk::expression_node *arg = dynamic_cast<cdk::expression_node*>(node->arguments()->node(ax-1));

      arg->accept(this, lvl + 2);
      if (basic_type::TYPE_DOUBLE == symbol->arguments()[ax-1] &&
          basic_type::TYPE_INT == arg->type()->name()) {
        _pf.I2D();
        argsSize += 8;
      } else {
        argsSize += arg->type()->size();
      }
    }
  }

  _pf.CALL(id);
  if (0 != argsSize) {
    _pf.TRASH(argsSize);
  }

  if (basic_type::TYPE_INT == symbol->type()->name() ||
      basic_type::TYPE_POINTER == symbol->type()->name() || 
      basic_type::TYPE_STRING == symbol->type()->name()) {
    _pf.LDFVAL32();
  } else if (basic_type::TYPE_DOUBLE == symbol->type()->name()) {
    _pf.LDFVAL64();
  }
}
void m19::postfix_writer::do_function_declaration_node(m19::function_declaration_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;

  std::shared_ptr<m19::symbol> function = new_symbol();
  if (nullptr != function) {
    _functionsToDeclare.insert(function->name());
    reset_new_symbol();
  }
}
void m19::postfix_writer::do_function_definition_node(m19::function_definition_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;

  _currentFunctionExitLabel = ++_lbl;

  _function = new_symbol();
  if (nullptr != _function) {
    _functionsToDeclare.erase(_function->name());
    reset_new_symbol();
  }

  _offset = 8;
  _symtab.push();
  if (nullptr != node->arguments()) {
    _inFunctionArgs = true;
    node->arguments()->accept(this, lvl + 2);
    _inFunctionArgs = false;
  }

  _pf.TEXT();
  _pf.ALIGN();
  if (tPUBLIC == node->qualifier()) {
    _pf.GLOBAL(_function->name(), _pf.FUNC());
  }
  _pf.LABEL(_function->name());

  // CALCULATE STACK FRAME SIZE
  frame_size_calculator lsc(_compiler, _symtab);
  node->accept(&lsc, lvl);
  _pf.ENTER(lsc.localsize()); 

  _offset = 0;
  _inFunctionBody = true;

  os() << "        ;; before body" << std::endl;
  basic_type *type = _function->type();
  if (basic_type::TYPE_VOID != type->name()) {
    // MAKES HANDLING @ AS A LOCAL VARIABLE
    m19::variable_declaration_node *retval = new m19::variable_declaration_node(node->lineno(),
        copy_type(type), "@", tPRIVATE, node->literal());
    retval->accept(this, lvl + 2);
  }
  node->body()->accept(this, lvl + 4);

  _pf.ALIGN();
  _pf.LABEL(mklbl(_currentFunctionExitLabel));
  if (basic_type::TYPE_VOID != type->name()) {
    if (basic_type::TYPE_INT == type->name() ||
        basic_type::TYPE_STRING == type->name() ||
        basic_type::TYPE_POINTER == type->name()) {
      _pf.LOCV(-_function->type()->size());
      _pf.STFVAL32();
    } else if (basic_type::TYPE_DOUBLE == type->name()) {
      _pf.LOCAL(-_function->type()->size());
      _pf.LDDOUBLE();
      _pf.STFVAL64();
    }
  }
  os() << "        ;; after body" << std::endl;

  _inFunctionBody = false;
  _symtab.pop();

  _pf.LEAVE();
  _pf.RET();

  if ("m19" == node->identifier()) {
    for (std::string s : _functionsToDeclare) {
      _pf.EXTERN(s);
    }
  }
}
