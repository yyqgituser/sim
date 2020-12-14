#include <ctime>
#include <iostream>
#include <fstream>

#include "TextReader.h"
#include "TextWriter.h"
#include "UTF8.h"

#include "SimScanner.h"
#include "SimHandler.h"
#include "SimHandlerAdapter.h"
#include "SimTokenType.h"

int main(int argc, char* argv[]) {
  if(argc < 2) {
    std::cout << "usage: " << argv[0] << " infile" << std::endl;
    return -1;
  }

  std::fstream fin(argv[1], std::ios::in | std::ios::binary);
  if(!fin.is_open()) {
    std::cout << "can't open file " << argv[1] << std::endl;
    return -1;    
  }

  TextReader reader(&fin, utf8_decoder);
  reader.skipUTF8BOM();  
 
  lex::SimHandlerAdapter handler;
  lex::SimScanner scanner(&reader, &handler);
  int count = 0;
  parser::SimTokenType type = scanner.scan();
  std::clock_t t1 = std::clock();
  while (type != parser::EOI) {
    if(false) {
      std::u32string *text = scanner.text();
      //writer.write(text->data(), 0, text->length());
      delete text;
    }
    count++;
    type = scanner.scan();
  }
  std::clock_t t2 = std::clock();
  
  fin.close();
  std::cout << "time: " << 1000 * (t2 - t1) / CLOCKS_PER_SEC << 
               "ms, symbol count: " << count << std::endl;
  
  return 0;
}

