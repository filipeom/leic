#ifndef __M19_SEMANTICS_XML_TARGET_H__
#define __M19_SEMANTICS_XML_TARGET_H__

#include <cdk/basic_target.h>
#include <cdk/compiler.h>
#include "targets/xml_writer.h"

namespace m19 {

  class xml_target: public cdk::basic_target {
    static xml_target _self;

  private:
    xml_target() :
        cdk::basic_target("xml") {
    }

  public:
    bool evaluate(std::shared_ptr<cdk::compiler> compiler) {
      // THIS SYMBOL TABLE WILL BE USED TO CHECK IDENTIFIERS
      // AN EXCEPTION WILL BE THROWN IF IDENTIFIERS ARE USED BEFORE DECLARATION
      cdk::symbol_table<m19::symbol> symtab;

      xml_writer writer(compiler, symtab);
      compiler->ast()->accept(&writer, 0);
      return true;
    }

  };

} // m19

#endif
