#ifndef _SIMTOKEN_H
#define _SIMTOKEN_H

#include <string>

namespace parser {

class SimToken {
public:
  int id;

  std::u32string text;

  int line;

  int column;

public:
  SimToken() : SimToken(-1, 0, -1, -1) {
  }

  SimToken(int id) : SimToken(id, 0, -1, -1) {
  }

  SimToken(int id, std::u32string text) : SimToken(id, text, -1, -1) {
  }

  SimToken(int id, std::u32string text, int line, int column) : id(id), text(text), line(line), column(column) {
  }

  SimToken(int id, int line, int column) : SimToken(id, 0, -1, -1) {
  }

  ~SimToken() {
  }
};

}

#endif
