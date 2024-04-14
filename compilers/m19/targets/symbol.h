#ifndef __M19_SEMANTICS_SYMBOL_H__
#define __M19_SEMANTICS_SYMBOL_H__

#include <string>
#include <vector>
#include <cdk/basic_type.h>

namespace m19 {

  class symbol {
    basic_type *_type;
    std::string _name;
    long _value;

    int _qualifier;
    bool _initialized;
    int _offset = 0;
    bool _function;
    bool _forward;

    std::vector<basic_type::type> _arguments;

  public:
    symbol(basic_type *type, const std::string &name, int qualifier, bool initializer, bool funciton, bool forward = false) :
        _type(type), _name(name), _value(0), _qualifier(qualifier), _initialized(initializer),
        _function(funciton), _forward(forward) {
    }

    virtual ~symbol() {
      delete _type;
    }

    basic_type *type() const {
      return _type;
    }
    const std::string &name() const {
      return _name;
    }
    long value() const {
      return _value;
    }
    long value(long v) {
      return _value = v;
    }
    int qualifier() const {
      return _qualifier;
    }
    bool initialized() const {
      return _initialized;
    }
    int offset() const {
      return _offset;
    }
    bool global() const {
      return _offset == 0;
    }
    void set_offset(int offset) {
      _offset = offset;
    }
    bool isFunction() const {
      return _function;
    }
    std::vector<basic_type::type> arguments() {
      return _arguments;
    }
    void arguments(std::vector<basic_type::type> args) {
      _arguments = args;
    } 
  };

} // m19

#endif
