#ifndef _SIMPARSER_H
#define _SIMPARSER_H

#include <vector>
#include "MemoryInputStream.h"

#include "SimParseAction.h"
#include "SimSemanticor.h"
#include "SimBufferedScanner.h"
#include "SimParserData.h"

namespace parser {

class SimParser {
private:
   std::vector<int> states;

   SimBufferedScanner *bscanner;

   SimSemanticor *semanticor;

public:
  SimParser(SimBufferedScanner* bscanner, SimSemanticor* semanticor) : 
                    bscanner(bscanner), semanticor(semanticor) {
    SimParserData::staticInit();
  }
  
  void parse();
};

}

#endif
