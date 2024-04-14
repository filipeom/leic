#include <string>
#include <iostream>
#include "targets/xml_writer.h"
#include "targets/type_checker.h"
#include "targets/symbol.h"
#include "ast/all.h"  // AUTOMATICALLY GENERATED
#include "m19_parser.tab.h"

//===========================================================================
// HELPER METHODS 
//===========================================================================

static std::string type_name(basic_type *type) {
  if (basic_type::TYPE_INT == type->name()) {
    return "integer";
  }

  if (basic_type::TYPE_DOUBLE == type->name()) {
    return "double";
  }

  if (basic_type::TYPE_STRING == type->name()) {
    return "string";
  }

  if (basic_type::TYPE_VOID == type->name()) {
    return "void";
  }

  if (basic_type::TYPE_POINTER == type->name()) {
    std::string strptr = "pointer";
    basic_type *ptr = type->subtype();
    while(nullptr != ptr) {
      strptr += " to " + type_name(ptr);
      ptr = ptr->subtype();
    }
    return strptr;
  } else {
    return "unknown type";
  }
}
static std::string qualifier_name(int qualifier) {
  if (tPRIVATE == qualifier) {
    return "public";
  }

  if (tPRIVATE == qualifier) {
    return "private";
  }

  if (tEXTERNAL == qualifier) {
    return "extern";
  } else {
    return "unknown qualifier";
  }
}
void m19::xml_writer::do_unary_expression(cdk::unary_expression_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;

  openTag(node, lvl);
  node->argument()->accept(this, lvl + 2);
  closeTag(node, lvl);
}
void m19::xml_writer::do_binary_expression(cdk::binary_expression_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;

  openTag(node, lvl);
  node->left()->accept(this, lvl + 2);
  node->right()->accept(this, lvl + 2);
  closeTag(node, lvl);
}

//===========================================================================

void m19::xml_writer::do_nil_node(cdk::nil_node * const node, int lvl) {
  openTag(node, lvl);
  closeTag(node, lvl);
}
void m19::xml_writer::do_data_node(cdk::data_node * const node, int lvl) {
  // VOID
}

//===========================================================================
// SEQUENCE NODE
//===========================================================================

void m19::xml_writer::do_sequence_node(cdk::sequence_node * const node, int lvl) {
  os() << std::string(lvl, ' ') <<  "<" << node->label() << " size='" << node->size() << "'>" << std::endl;
  for (size_t i = 0; i < node->size(); i++) {
    cdk::basic_node *n = node->node(i);
    if (nullptr == n) {
      break;
    }
    n->accept(this, lvl + 2);
  }
  closeTag(node, lvl);
}

//===========================================================================
// LITERALS
//===========================================================================

void m19::xml_writer::do_integer_node(cdk::integer_node * const node, int lvl) {
  process_literal(node, lvl);
}
void m19::xml_writer::do_double_node(cdk::double_node * const node, int lvl) {
  process_literal(node, lvl);
}
void m19::xml_writer::do_string_node(cdk::string_node * const node, int lvl) {
  process_literal(node, lvl);
}

//===========================================================================
// OPERATORS 
//===========================================================================

void m19::xml_writer::do_neg_node(cdk::neg_node * const node, int lvl) {
  do_unary_expression(node, lvl);
}
void m19::xml_writer::do_identity_node(m19::identity_node * const node, int lvl) {
  do_unary_expression(node, lvl);
}
void m19::xml_writer::do_add_node(cdk::add_node * const node, int lvl) {
  do_binary_expression(node, lvl);
}
void m19::xml_writer::do_sub_node(cdk::sub_node * const node, int lvl) {
  do_binary_expression(node, lvl);
}
void m19::xml_writer::do_mul_node(cdk::mul_node * const node, int lvl) {
  do_binary_expression(node, lvl);
}
void m19::xml_writer::do_div_node(cdk::div_node * const node, int lvl) {
  do_binary_expression(node, lvl);
}
void m19::xml_writer::do_mod_node(cdk::mod_node * const node, int lvl) {
  do_binary_expression(node, lvl);
}
void m19::xml_writer::do_lt_node(cdk::lt_node * const node, int lvl) {
  do_binary_expression(node, lvl);
}
void m19::xml_writer::do_le_node(cdk::le_node * const node, int lvl) {
  do_binary_expression(node, lvl);
}
void m19::xml_writer::do_ge_node(cdk::ge_node * const node, int lvl) {
  do_binary_expression(node, lvl);
}
void m19::xml_writer::do_gt_node(cdk::gt_node * const node, int lvl) {
  do_binary_expression(node, lvl);
}
void m19::xml_writer::do_ne_node(cdk::ne_node * const node, int lvl) {
  do_binary_expression(node, lvl);
}
void m19::xml_writer::do_eq_node(cdk::eq_node * const node, int lvl) {
  do_binary_expression(node, lvl);
}
void m19::xml_writer::do_not_node(cdk::not_node * const node, int lvl) {
  do_unary_expression(node, lvl);
}
void m19::xml_writer::do_and_node(cdk::and_node * const node, int lvl) {
  do_binary_expression(node, lvl);
}
void m19::xml_writer::do_or_node(cdk::or_node * const node, int lvl) {
  do_binary_expression(node, lvl);
}
void m19::xml_writer::do_plus_equal_node(m19::plus_equal_node * const node, int lvl) {
}

//===========================================================================
// VARIABLES
//===========================================================================

void m19::xml_writer::do_variable_node(cdk::variable_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;

  os() << std::string(lvl, ' ') << "<" << node->label() << ">" << node->name() << "</" << node->label() << ">" << std::endl;
}
void m19::xml_writer::do_variable_declaration_node(m19::variable_declaration_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;

  reset_new_symbol();

  os() << std::string(lvl, ' ') << "<" << node->label() << " name='" << node->identifier() << "' qualifier='" 
    << qualifier_name(node->qualifier()) << "' type='" << type_name(node->varType()) << "'>"
    << std::endl;

  if (nullptr != node->initializer()) {
    openTag("initializer", lvl + 2);
    node->initializer()->accept(this, lvl + 4);
    closeTag("initializer", lvl + 2);
  }
  closeTag(node, lvl);
}

//===========================================================================
// LVALUES
//===========================================================================

void m19::xml_writer::do_rvalue_node(cdk::rvalue_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;

  openTag(node, lvl);
  node->lvalue()->accept(this, lvl + 4);
  closeTag(node, lvl);
}
void m19::xml_writer::do_index_node(m19::index_node * const node, int lvl) {
  openTag(node, lvl);
  openTag("base", lvl + 2);
  node->base()->accept(this, lvl + 2);
  closeTag("base", lvl + 2);
  openTag("index", lvl + 2);
  node->index()->accept(this, lvl +2);
  closeTag("index", lvl +2);
  closeTag(node, lvl);
}

//===========================================================================
// EXPRESSIONS
//===========================================================================

void m19::xml_writer::do_assignment_node(cdk::assignment_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;

  openTag(node, lvl);
  node->lvalue()->accept(this, lvl + 2);
  node->rvalue()->accept(this, lvl + 2);
  closeTag(node, lvl);
}
void m19::xml_writer::do_address_of_node(m19::address_of_node * const node, int lvl) {
  openTag(node, lvl);
  node->lvalue()->accept(this, lvl + 2);
  closeTag(node, lvl);
}
void m19::xml_writer::do_read_node(m19::read_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;

  openTag(node, lvl);
  closeTag(node, lvl);
}
void m19::xml_writer::do_stack_alloc_node(m19::stack_alloc_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;

  openTag(node, lvl);
  node->argument()->accept(this, lvl + 2);
  closeTag(node, lvl);
}

//===========================================================================
// FUNCTION BODY
//===========================================================================

void m19::xml_writer::do_block_node(m19::block_node * const node, int lvl) {
  openTag(node, lvl);
  openTag("declarations", lvl + 2);
  if (nullptr != node->declarations()) {
    node->declarations()->accept(this, lvl + 4);
  }
  closeTag("declarations", lvl + 2);

  openTag("instructions", lvl + 2);
  if (nullptr != node->instructions()) {
    node->instructions()->accept(this, lvl + 4);
  }
  closeTag("instructions", lvl + 2);
  closeTag(node, lvl);
}
void m19::xml_writer::do_initial_section_node(m19::initial_section_node * const node, int lvl) {
  openTag(node, lvl);
  node->block()->accept(this, lvl + 2);
  closeTag(node, lvl);
}
void m19::xml_writer::do_section_node(m19::section_node * const node, int lvl) {
  openTag(node, lvl);
  openTag("expression", lvl + 2);
  if (nullptr != node->expression()) {
    node->expression()->accept(this, lvl + 4);
  }
  closeTag("expression", lvl + 2);
  node->block()->accept(this, lvl + 2);
  closeTag(node, lvl);
}
void m19::xml_writer::do_final_section_node(m19::final_section_node * const node, int lvl) {
  openTag(node, lvl);
  node->block()->accept(this, lvl + 2);
  closeTag(node, lvl);
}
void m19::xml_writer::do_body_node(m19::body_node * const node, int lvl) {
  openTag(node, lvl);
  if (nullptr != node->initial_sec()) {
    node->initial_sec()->accept(this, lvl + 2);
  }
  openTag("middle-section", lvl + 2);
  if (nullptr != node->middle_sec()) {
    node->middle_sec()->accept(this, lvl + 4);
  }
  closeTag("middle-section", lvl + 2);
  if (nullptr != node->final_sec()) {
    node->final_sec()->accept(this, lvl + 2);
  }
  closeTag(node, lvl);
}

//===========================================================================
// GENERAL INSTRUCTIONS
//===========================================================================

void m19::xml_writer::do_print_node(m19::print_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;

  std::string argtype = "";
  if (basic_type::TYPE_INT == node->argument()->type()->name()) {
    argtype = "integer";
  } else if (basic_type::TYPE_DOUBLE == node->argument()->type()->name()) {
    argtype = "real";
  } else if (basic_type::TYPE_STRING == node->argument()->type()->name()) {
    argtype = "string";
  } else if (basic_type::TYPE_POINTER == node->argument()->type()->name()) {
    argtype = "pointer";
  } else {
    argtype = "unknown type";
  }

  os() << std::string(lvl, ' ') << "<" << node->label() << " type='" << argtype << "'>" << std::endl;
  node->argument()->accept(this, lvl + 2);
  closeTag(node, lvl);
}
void m19::xml_writer::do_break_node(m19::break_node * const node, int lvl) {
  openTag(node, lvl);
  closeTag(node, lvl);
}
void m19::xml_writer::do_continue_node(m19::continue_node * const node, int lvl) {
  openTag(node, lvl);
  closeTag(node, lvl);
}
void m19::xml_writer::do_return_node(m19::return_node * const node, int lvl) {
  openTag(node, lvl);
  openTag(node, lvl);
}
void m19::xml_writer::do_evaluation_node(m19::evaluation_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;

  openTag(node, lvl);
  node->argument()->accept(this, lvl + 2);
  closeTag(node, lvl);
}
void m19::xml_writer::do_apply_node(m19::apply_node * const node, int lvl) {
  // TODo
}

//===========================================================================
// CONDITIONAL INSTRUCTIONS
//===========================================================================

void m19::xml_writer::do_if_else_node(m19::if_else_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;

  openTag(node, lvl);
  openTag("condition", lvl + 2);
  node->condition()->accept(this, lvl + 4);
  closeTag("condition", lvl + 2);
  openTag("then", lvl + 2);
  node->thenblock()->accept(this, lvl + 4);
  closeTag("then", lvl + 2);
  if (nullptr != node->elseblock()) {
    openTag("else", lvl + 2);
    node->elseblock()->accept(this, lvl + 4);
    closeTag("else", lvl + 2);
  }
  closeTag(node, lvl);
}
void m19::xml_writer::do_if_node(m19::if_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;

  openTag(node, lvl);
  openTag("condition", lvl + 2);
  node->condition()->accept(this, lvl + 4);
  closeTag("condition", lvl + 2);
  openTag("then", lvl + 2);
  node->block()->accept(this, lvl + 4);
  closeTag("then", lvl + 2);
  closeTag(node, lvl);
}

//===========================================================================
// FOR INSTRUCTION
//===========================================================================

void m19::xml_writer::do_for_node(m19::for_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;

  openTag(node, lvl);
  openTag("init", lvl + 2);
  if (nullptr != node->init()) {
    node->init()->accept(this, lvl + 4);
  }
  closeTag("init", lvl + 2);
  openTag("condition", lvl + 2);
  if (nullptr != node->condition()) {
    node->condition()->accept(this, lvl + 4);
  }
  closeTag("condition", lvl + 2);
  openTag("increment", lvl + 2);
  if (nullptr != node->incr()) {
    node->incr()->accept(this, lvl + 4);
  }
  closeTag("icrement", lvl + 2);
  openTag("instruction", lvl + 2);
  node->block()->accept(this, lvl + 2);
  closeTag("instruction", lvl + 2);
  closeTag(node, lvl);
}

//===========================================================================
// FUNCTIONS
//===========================================================================

void m19::xml_writer::do_function_call_node(m19::function_call_node * const node, int lvl) {
  os() << std::string(lvl, ' ') << "<" << node->label() << " name='" << node->identifier() << "'>" << std::endl;
  openTag("arguments", lvl + 2);
  if (nullptr != node->arguments()) {
    node->arguments()->accept(this, lvl + 4);
  }
  closeTag("arguments", lvl + 2);
  closeTag(node, lvl);
}
void m19::xml_writer::do_function_declaration_node(m19::function_declaration_node * const node, int lvl) {
  reset_new_symbol();
  os() << std::string(lvl, ' ') << "<" << node->label() << " name='" << node->identifier() << "' qualifier='"
       << qualifier_name(node->qualifier()) << "' type='" << type_name(node->type()) << "'>" << std::endl;

  openTag("arguments", lvl + 2);
  if (nullptr != node->arguments()) {
    _symtab.push();
    node->arguments()->accept(this, lvl + 4);
    _symtab.pop();
  }
  closeTag("arguments", lvl + 2);
  closeTag(node, lvl);
}
void m19::xml_writer::do_function_definition_node(m19::function_definition_node * const node, int lvl) {
  _symtab.push();

  os() << std::string(lvl, ' ') << "<" << node->label() << " name='" << node->identifier() << "' qualifier='"
       << qualifier_name(node->qualifier()) << "' type='" << type_name(node->type()) << "'>" << std::endl; 

  openTag("arguments", lvl + 2);
  if (nullptr != node->arguments()) {
    node->arguments()->accept(this, lvl + 4);
  }
  closeTag("arguments", lvl + 2);
  node->body()->accept(this, lvl + 2);
  closeTag(node, lvl);

  _symtab.pop();
}
