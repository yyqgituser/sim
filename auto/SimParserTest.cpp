#include <ctime>
#include <iostream>
#include <fstream>

#include "TextReader.h"
#include "TextWriter.h"
#include "UTF8.h"

#include "SimScanner.h"
#include "SimHandlerAdapter.h"
#include "SimDefaultBufferedScanner.h"
#include "SimSemanticorAdapter.h"
#include "SimNoMatchException.h"
#include "SimParseException.h"
#include "SimParser.h"

int main(int argc, char* argv[]) {
  if(argc < 2) {
    std::cout << "usage: << argv[0] << " infile" << std::endl;
    return -1;
  }
  std::fstream fin(argv[1], std::ios::in | std::ios::binary);
  if(!fin.is_open()) {
    std::cout << "can't open file " << argv[1] << std::endl;
    return -1;    
  }

  try {
    TextReader reader(&fin, utf8_decoder);
    reader.skipUTF8BOM();  
    lex::SimHandlerAdapter handler;
    lex::SimScanner scanner(&reader, &handler);

    parser::SimSemanticorAdapter semanticor;
    parser::SimDefaultBufferedScanner bscanner(&scanner);
    parser::SimParser parser(&bscanner, &semanticor);

    std::clock_t t1 = std::clock();
    parser.parse();
    std::clock_t t2 = std::clock();

    fin.close();

    std::cout << "parse successfully, time: " << 1000 * (t2 - t1) / CLOCKS_PER_SEC << "ms" << std::endl;
  }
  catch (lex::SimNoMatchException &e) {
    std::cout << e.getLine() << ", " << e.getColumn() << ", lexical error" << std::endl;
  }
  catch (parser::SimParseException &e) {
    std::cout << e.getLine() << ", " << e.getColumn() << ", parse error: " << 
	             e.getMsg() << std::endl;
  }

  return 0;
}

