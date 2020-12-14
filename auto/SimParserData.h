#ifndef _SIMPARSERDATA_H
#define _SIMPARSERDATA_H

#include <vector>
#include "MemoryInputStream.h"

#include "SimParseAction.h"
#include "SimSemanticor.h"
#include "SimBufferedScanner.h"

namespace parser {

class SimParserData {
public:
   static unsigned int TERMINAL_MIN;

   static unsigned int TERMINAL_NUM;

   static unsigned int ACTION_MIN;

   static unsigned int ACTION_NUM;

   static unsigned int NONTERMINAL_MIN;

   static unsigned int NONTERMINAL_NUM;

   static int START_ID;

   static int EOI_ID;

   static int ERROR_ID;

   static int MAX_LOOK;

   static unsigned int DFA_STATE_NUM;

   static unsigned int** PROD_INFOS;

   static unsigned int PROD_INFO_SIZE;

   static SimParseAction** ACTIONS;

   static int* DFA_STATE_ACTIONS;

   static unsigned int PARSE_STATE_NUM;

   static int* PARSE_STATE_ACTIONS;

   static int* PARSE_BASES;

   static int PARSE_CAPACITY;

   static int* PARSE_CHECK;

   static int* PARSE_NEXTS;

   static int GOTO_STATE_NUM;

   static int* GOTO_BASES;

   static int GOTO_CAPACITY;

   static int* GOTO_CHECK;

   static int* GOTO_NEXTS;

   static int INIT_STATE;

   static int ACCEPT_STATE;

  static SimParseAction* parseNext(int state, int input);

  static int gotoNext(int state, int input);

  static void staticInit();

  static bool doAction(SimSemanticor *semanticor, int aid);

private:
   static bool INITED;

  static unsigned int* readProductionInfo(MemoryInputStream* ins);
  
  static int readInt2(MemoryInputStream *ins);

  static inline int readInt(MemoryInputStream *ins) {
    int ch1 = ins->read();
    int ch2 = ins->read();
    int ch3 = ins->read();
    int ch4 = ins->read();
    if ((ch1 | ch2 | ch3 | ch4) < 0)
      throw std::runtime_error("invalid Int");
    return ((ch1 << 24) + (ch2 << 16) + (ch3 << 8) + (ch4 << 0));
  }

  static inline void readFully(MemoryInputStream *ins, char *b, int off, int len) {
    int n = 0;
    while (n < len) {
      int count = ins->read(b, off + n, len - n);
      if (count < 0) {
          throw std::runtime_error("invalid bytes");
      }
      n += count;
    }
  }

  static inline long long readLong(MemoryInputStream *ins) {
  	char readBuffer[8];
    readFully(ins, readBuffer, 0, 8);
    return (((long long)readBuffer[0] << 56) |
           ((long long)(readBuffer[1] & 255) << 48) |
           ((long long)(readBuffer[2] & 255) << 40) |
           ((long long)(readBuffer[3] & 255) << 32) |
           ((long long)(readBuffer[4] & 255) << 24) |
           ((readBuffer[5] & 255) << 16) |
           ((readBuffer[6] & 255) << 8) |
           ((readBuffer[7] & 255) << 0));
  }

  static inline unsigned int readOneByte(MemoryInputStream *ins) {
    int ch = ins->read();
    if (ch < 0)
      throw std::runtime_error("invalid eof");
    return (unsigned int)ch;
  }

  static int readVarint(MemoryInputStream *ins);
  
  static SimParseAction* readAction(MemoryInputStream* ins);

  static void loadActions(MemoryInputStream* ins);

  static void loadParseData(MemoryInputStream* ins);

  static void loadGotoData(MemoryInputStream* ins);
};

}

#endif
