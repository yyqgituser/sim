#ifndef _SIMDEFAULTBUFFEREDSCANNER_H
#define _SIMDEFAULTBUFFEREDSCANNER_H

#include <string>
#include <deque>

#include "SimToken.h"
#include "SimScanner.h"
#include "SimBufferedScanner.h"

namespace parser {

class SimDefaultBufferedScanner : public SimBufferedScanner {
private:
  lex::SimScanner* scanner;
  
  std::deque<SimToken *> queue;

public:
  SimDefaultBufferedScanner(lex::SimScanner* _scanner) : scanner(_scanner) {
  }
  
  virtual ~SimDefaultBufferedScanner() {
    for (std::deque<SimToken *>::iterator itr = queue.begin(); itr != queue.end(); itr++) {
      delete *itr;
    }
  }

private:
  // scan to k(exclude)
  void scanTo(std::size_t k) {
    for (std::size_t i = queue.size(); i < k; i++) {
      int id = (int)scanner->scan();
      queue.push_back(new SimToken(id, scanner->text(), scanner->line(), scanner->column()));
    }
  }

public:
  SimToken *look(std::size_t i) {
    scanTo(i + 1);
    return queue[i];
  }

  void consume(std::size_t num) {
    std::size_t i;

    for(i = 0; i < num; i++) {
      queue.pop_front();
    }
    while(i < num) {
      scanner->scan();
      i++;
    }
  }
};

}

#endif
