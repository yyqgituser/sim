#ifndef _SIMBUFFEREDSCANNER_H
#define _SIMBUFFEREDSCANNER_H


#include "SimToken.h"

namespace parser {

class SimBufferedScanner {
public:
  virtual ~SimBufferedScanner() {};
  
  virtual SimToken *look(std::size_t i) = 0;

  virtual void consume(std::size_t num) = 0;
};

}

#endif
