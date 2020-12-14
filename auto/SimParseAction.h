#ifndef _SIMPARSEACTION_H
#define _SIMPARSEACTION_H

#include <stdexcept>

namespace parser {

class SimParseAction {
public:
  static const unsigned char ACTION_SHIFT = 0;

  static const unsigned char ACTION_REDUCE = 1;

  static const unsigned char ACTION_ACTION = 2;

  static const unsigned char ACTION_LOOK = 3;

private:
  unsigned char type;

public:
  unsigned int value;

  SimParseAction(unsigned char _type, unsigned int _value) :
    type(_type), value(_value) {
    if (type > ACTION_LOOK) {
      throw std::runtime_error("Invalid action type");
    }
  }

  bool isShift() {
    return type == ACTION_SHIFT;
  }

  bool isReduce() {
    return type == ACTION_REDUCE;
  }

  bool isAction() {
    return type == ACTION_ACTION;
  }

  bool isLook() {
    return type == ACTION_LOOK;
  }

  signed char getType() {
    return type;
  }
};

}

#endif
