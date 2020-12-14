#include <stdexcept>

#include "SimScanner.h"

namespace lex {

extern unsigned char SIM_SANNNER_DATA[];

extern unsigned int SIM_SANNNER_DATA_LEN;

int SimScanner::STATE_NUM;

int SimScanner::INPUT_NUM;

int SimScanner::DEAD_STATE;

bool* SimScanner::ACTION_MASK = NULL;

int* SimScanner::ACTIONS = NULL;

bool* SimScanner::ACCEPTS = NULL;

int* SimScanner::FA_TABLE = NULL;

int* SimScanner::BASES;

int SimScanner::CAPACITY;

int* SimScanner::CHECK;

int* SimScanner::NEXTS;

int SimScanner::OPT_TOP = 0;

int* SimScanner::OPT_CMAP = NULL;

int* SimScanner::LEX_STATES = NULL;

int SimScanner::DEFAULT_LEX_STATE;

bool SimScanner::INITED = false; 

SimScanner::SimScanner(TextReader *reader, SimHandler *handler, int initBufferSize) {
  initIntance(initBufferSize, reader, handler);
}

SimScanner::SimScanner(TextReader *reader, SimHandler *handler) {
  initIntance(DEFUALT_BUFFER_SIZE, reader, handler);
}

int SimScanner::readVarint(MemoryInputStream *ins) {
  // (0x7f<<14) | 0x7f, clear 8th bit
  const static unsigned UNMASK_2_0 = 0x001fc07f;
  // ((0x7f<<14) | (0x7f))), clear 8th and 15th bit
  const static unsigned UNMASK_4_2_0 = 0xf01fc07f;
  unsigned int a, b;
  unsigned int v;

  a = readOneByte(ins);
  if ( (a & 0x80) == 0) {
    return (int)a;
  }

  /* 2 bytes */
  b = readOneByte(ins);
  // b: p1*
  if ( (b & 0x80) == 0) {
    a &= UNMASK_2_0;
    a <<= 7;
    v = a | b;
    return (int)v;
  }

  /* 3 bytes */
  a <<= 14;
  a |= readOneByte(ins); // a: p0* << 14 | p2*
  if ( (a & 0x80) == 0) {
    a &= UNMASK_4_2_0;
    b &= UNMASK_2_0;
    b <<= 7;
    v = a | b;
    return (int)v;
  }

  // 4 bytes，process 1, 3 and 2, 4 respectively
  a &= UNMASK_4_2_0; // a: p0 << 14 | p2
  b <<= 14;
  b |= readOneByte(ins); // b: p1* << 14 | p3*
  if ( (b & 0x80) == 0) {
    b &= UNMASK_4_2_0; // b: p1 << 14 | p3
    a <<= 7; /* a: p0 << 21 | p2 << 14 */
    v = a | b;
    return (int)v;
  }

  b &= UNMASK_4_2_0; // b: p1 << 14 | p3
  v = (a << 11) | (b << 4) | (readOneByte(ins) & 0xf);
  return (int)v;
}


void SimScanner::staticInit() {
  if(INITED) {
    return;
  }

  MemoryInputStream ins((const char*)SIM_SANNNER_DATA, SIM_SANNNER_DATA_LEN);
    
  STATE_NUM = readVarint(&ins);
  if (STATE_NUM < 0) {
    throw std::runtime_error("invalid stateNum");
  }
  INPUT_NUM = readVarint(&ins);
  if (INPUT_NUM < 0) {
    throw std::runtime_error("invalid inputNum");
  }
  DEAD_STATE = readVarint(&ins);

  const int ADDR_LINE_PER_UNIT = 6;
  const int BIT_INDEX_MASK = (1 << ADDR_LINE_PER_UNIT) - 1;

  int unitLength = ((STATE_NUM - 1) >> ADDR_LINE_PER_UNIT) + 1;
  long long *bits = new long long[unitLength];
  std::memset(bits, 0, sizeof(long long) * unitLength);
  int readLength = readVarint(&ins);
  if(readLength > unitLength) {
    throw std::runtime_error("invalid length");
  }
  for (int i = 0; i < readLength; i++) {
    bits[i] = readLong(&ins);
  }
  ACTION_MASK = new bool[STATE_NUM];
  for (int s = 0; s < STATE_NUM; s++) {
    ACTION_MASK[s] = ( (bits[s >> ADDR_LINE_PER_UNIT] & (1LL << (s & BIT_INDEX_MASK))) != 0);
  }

  ACTIONS = new int[STATE_NUM];
  for (int i = 0; i < STATE_NUM; i++) {
    ACTIONS[i] = -1;
  }
  for (int s = 0; s < STATE_NUM; s++) {
    if (ACTION_MASK[s]) {
      ACTIONS[s] = readVarint(&ins);
    }
  }

  std::memset(bits, 0, sizeof(long long) * unitLength);
  readLength = readVarint(&ins);
  if(readLength > unitLength) {
    delete[] bits;
    throw std::runtime_error("invalid length");
  }
  for (int i = 0; i < readLength; i++) {
    bits[i] = readLong(&ins);
  }
  ACCEPTS = new bool[STATE_NUM];
  for (int s = 0; s < STATE_NUM; s++) {
    ACCEPTS[s] = ( (bits[s >> ADDR_LINE_PER_UNIT] & (1LL << (s & BIT_INDEX_MASK))) != 0);
  }
  delete[] bits;
  FA_TABLE = new int[STATE_NUM];
  for (int i = 0; i < STATE_NUM; i++) {
    FA_TABLE[i] = -1;
  }
  for (int s = 0; s < STATE_NUM; s++) {
    if (ACCEPTS[s]) {
      FA_TABLE[s] = readVarint(&ins);
    }
  }

  BASES = new int[STATE_NUM];
  int ttm = (int)readOneByte(&ins);
  if(ttm != 2) {
    throw std::runtime_error("invalid ttm");
  }

  for (int s = 0; s < STATE_NUM; s++) {
    BASES[s] = readVarint(&ins);
  }
  CAPACITY = readVarint(&ins);
  CHECK = new int[CAPACITY];
  NEXTS = new int[CAPACITY];
  for (int i = 0; i < CAPACITY; i++) {
    int check = readVarint(&ins);
    if(check >= STATE_NUM) {
      throw std::runtime_error("invalid check");
    }
    CHECK[i] = check;
    int next = readVarint(&ins);
    if(next >= STATE_NUM) {
      throw std::runtime_error("invalid next");
    }
    NEXTS[i] = next;
  }
  
  OPT_TOP = readVarint(&ins);
  OPT_CMAP = new int[OPT_TOP];
  int rangeCnt = readVarint(&ins);
  for(int i = 0; i< rangeCnt; i++) {
    int from = readVarint(&ins);
    if(from >= OPT_TOP) {
      throw std::runtime_error("invalid from");
    }
    int to = readVarint(&ins);
    if (to >= (OPT_TOP + 1)) {
      throw std::runtime_error("invalid to");
    }
    int cc = readVarint(&ins);
    if(cc >= INPUT_NUM) {
      throw std::runtime_error("invalid cc");
    }
    for(int c = from; c < to; c++) {
      OPT_CMAP[c] = cc;
    }
  }
  int lexStateNum = readVarint(&ins);
  LEX_STATES = new int[lexStateNum];
  for (int i = 0; i < lexStateNum; i++) {
    int s = readVarint(&ins);
    if(s < 0 || s >= STATE_NUM) {
      throw std::runtime_error("invalid lex state");
    }      
    LEX_STATES[i] = s;
  }
  DEFAULT_LEX_STATE = readVarint(&ins);
  if(DEFAULT_LEX_STATE < 0 || DEFAULT_LEX_STATE >= lexStateNum) {
    throw std::runtime_error("invalid default lex state");
  }

  INITED = true;
}

parser::SimTokenType SimScanner::scan() {
  while (1) { // loop for Rule.TYPE_SKIP。
    int state = initial;
    int fid = -1;
    globalStart = globalPosition;
    tline = globalLine;
    tcolumn = globalColumn;
    int lineMark = -1, columnMark = -1, linestateMark = -1, next = -1;
    if(ACCEPTS[state]) {
      fid = FA_TABLE[state];
    }

    loop1:while (true) {
      int ch = readUC();
      if (ch == -1) {
        break;
      }
      int cc = 0;
      if(ch < OPT_TOP) {
        cc = OPT_CMAP[ch];
      }
      else {
        cc = (((ch >= 128) && (ch < 1114112)) ? 45 : -1);
      }
      int base = BASES[state];
      int pos = base + cc;
      if(pos >= 1 && pos < CAPACITY && CHECK[pos] == state) {
        next = NEXTS[pos];
      }
      else {
        next = DEAD_STATE;
      }
      if(next != DEAD_STATE) {
        if(ACCEPTS[next]) {
          fid = FA_TABLE[next];
            globalMark = globalPosition;
            lineMark = globalLine;
            columnMark = globalColumn;
            linestateMark = linestate;
        }
        state = next;
        goto loop1;
      }
      // dead。
      break;
    }

    if (globalStart == globalPosition) {
      return parser::EOI;
    }
    else if (fid != -1) {
      globalEnd = globalPosition = globalMark;
      globalLine = lineMark;
      globalColumn = columnMark;
      linestate = linestateMark;
      switch(fid) {
        case 0 :
          return parser::INT_LITERAL;

        case 1 :
          return parser::DOUBLE_LITERAL;

        case 2 :
          return parser::RETURN;

        case 3 :
          return parser::CONTINUE;

        case 4 :
          return parser::TRUE;

        case 5 :
          return parser::FALSE;

        case 6 :
          return parser::BREAK;

        case 7 :
          return parser::IF;

        case 8 :
          return parser::ELSE;

        case 9 :
          return parser::WHILE;

        case 10 :
          return parser::FOR;

        case 11 :
          return parser::INT;

        case 12 :
          return parser::VOID;

        case 13 :
          return parser::DOUBLE;

        case 14 :
          return parser::BOOLEAN;

        case 15 :
          return parser::STRING;

        case 16 :
          return parser::PLUS;

        case 17 :
          return parser::MINUS;

        case 18 :
          return parser::MULT;

        case 19 :
          return parser::DIV;

        case 20 :
          return parser::MOD;

        case 21 :
          return parser::NOT;

        case 22 :
          return parser::COMP;

        case 23 :
          return parser::LPAREN;

        case 24 :
          return parser::RPAREN;

        case 25 :
          return parser::SEMI;

        case 26 :
          return parser::LBRACK;

        case 27 :
          return parser::RBRACK;

        case 28 :
          return parser::LBRACE;

        case 29 :
          return parser::RBRACE;

        case 30 :
          return parser::COMMA;

        case 31 :
          return parser::EQ;

        case 32 :
          return parser::GT;

        case 33 :
          return parser::LT;

        case 34 :
          return parser::OR;

        case 35 :
          return parser::XOR;

        case 36 :
          return parser::AND;

        case 37 :
          return parser::EQEQ;

        case 38 :
          return parser::NOTEQ;

        case 39 :
          return parser::LTEQ;

        case 40 :
          return parser::GTEQ;

        case 41 :
          return parser::LSHIFT;

        case 42 :
          return parser::RSHIFT;

        case 43 :
          return parser::ANDAND;

        case 44 :
          return parser::OROR;

        case 45 :
          return parser::PLUSPLUS;

        case 46 :
          return parser::MINUSMINUS;

        case 47 :
          return parser::ELLIPSIS;

        case 48 :
          return parser::STRING_LITERAL;

        case 49 :
          return parser::IDENTIFIER;

        case 50 :
          continue;

        case 51 :
          return parser::EOI;

        default :
          throw std::runtime_error("invalid action");
      }
    }
    else {
      globalEnd = globalPosition = globalMark;
      globalLine = lineMark;
      globalColumn = columnMark;
      linestate = linestateMark;
      throw SimNoMatchException(tline, tcolumn);
    }
  }
}

void SimScanner::staticUninit() {
  if(!INITED) {
    return;
  }
  delete[] ACTION_MASK;
  ACTION_MASK = NULL;
  delete[] ACTIONS;
  ACTIONS = NULL;
  delete[] ACCEPTS;
  ACCEPTS = NULL;
  delete[] FA_TABLE;
  FA_TABLE = NULL;
  delete[] BASES;
  BASES = NULL;
  delete[] CHECK;
  CHECK = NULL;
  delete[] NEXTS;
  NEXTS = NULL;
  delete[] OPT_CMAP;
  OPT_CMAP = NULL;
  delete[] LEX_STATES;
  LEX_STATES = NULL;
  INITED = false;
}


void SimScanner::initIntance(int initBufferSize, TextReader *reader, SimHandler *handler) {
  if(!INITED) {
    staticInit();
  }
  limit = 0;
  globalStart = 0;
  globalEnd = 0;
  globalPosition = 0;
  globalMark = 0;
  linestate = 0;
  globalLine = 1, globalColumn = 1;
  cp = -1;
  marksSize = 10;
  marks = new int[marksSize];
  markCnt = 0;
  this->reader = reader;
  this->handler = handler;
  buffer = new char32_t[initBufferSize];
  bufferSize = initBufferSize;
  setLexState(DEFAULT_LEX_STATE);
}

void SimScanner::setLexState(int lexId) {
  initial = LEX_STATES[lexId];
}

}
