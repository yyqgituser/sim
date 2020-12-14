#ifndef _SIMNOMATCHEXCEPTION_H
#define _SIMNOMATCHEXCEPTION_H

namespace lex {

class SimNoMatchException {
private:
  int line;

  int column;

public:

  explicit SimNoMatchException(int pline, int pcolumn)
    : line(pline), column(pcolumn) {
  }

public:
  int getLine() {
    return line;
  }

public:
  int getColumn() {
    return column;
  }
};

}

#endif
