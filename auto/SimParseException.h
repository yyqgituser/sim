#ifndef _SIMPARSEEXCEPTION_H
#define _SIMPARSEEXCEPTION_H

namespace parser {

class SimParseException {
private:
  const char* msg;
  
  int line;

  int column;

public:
  SimParseException()
    : line(-1), column(-1) {
  }

  SimParseException(int _line, int _column)
    : line(_line), column(_column) {
  }

  SimParseException(const char* _msg, int _line, int _column)
    : msg(_msg), line(_line), column(_column) {
  }

  const char* getMsg() {
    return msg;
  }

  int getLine() {
    return line;
  }

  int getColumn() {
    return column;
  }
};

}

#endif
