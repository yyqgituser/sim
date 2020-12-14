#ifndef _SIMSCANNER_H
#define _SIMSCANNER_H

#include <stdexcept>
#include "TextReader.h"
#include "SimNoMatchException.h"
#include "SimTokenType.h"
#include "SimHandler.h"
#include "MemoryInputStream.h"

namespace lex {

class SimScanner {
private:
  const static unsigned short MIN_HIGH = 0xD800;
  const static unsigned short MAX_HIGH = 0xDBFF;
  const static unsigned short MIN_LOW = 0xDC00;
  const static unsigned short MAX_LOW = 0xDFFF;

  static int STATE_NUM;
  
  static int INPUT_NUM;
  
  static int DEAD_STATE;

  static int DEFAULT_LEX_STATE;

  static bool* ACTION_MASK;

  static int* ACTIONS;

  static bool* ACCEPTS;

  static int* FA_TABLE;

  static int* BASES;

  static int CAPACITY;

  static int* CHECK;

  static int* NEXTS;

  static int OPT_TOP;

  static int* OPT_CMAP;

  static int* LEX_STATES;

  static bool INITED;

  const static int DEFUALT_BUFFER_SIZE = 1024 * 4;

  TextReader *reader;

  char32_t *buffer;

  int bufferSize;

  int limit;

  int globalStart;

  int globalEnd;

  int globalPosition;

  int globalMark;

  int linestate;

  int globalLine, globalColumn;

  int tline;

  int tcolumn;

  char32_t cp;

  int *marks;

  int marksSize;

  int markCnt;

  SimHandler *handler;

  int initial;

public:
  SimScanner(TextReader *reader, SimHandler *handler, int initBufferSize);

  SimScanner(TextReader *reader, SimHandler *handler);

  ~SimScanner() {
    delete[] buffer;
    delete[] marks;
  }

  static inline bool isHigh(char32_t c) {
    return (MIN_HIGH <= c) && (c <= MAX_HIGH);
  }

  static inline bool isLow(char32_t c) {
    return (MIN_LOW <= c) && (c <= MAX_LOW);
  }

  static inline char32_t toUCS4(char32_t h, char32_t l) {
    return (((h & 0x3ff) << 10) | (l & 0x3ff)) + 0x10000;
  }

  inline char32_t readUC() {
    if (globalPosition >= limit) {
      int eof = fill(); // fill() will modify some globalXXX variables
      if (eof) {
        this->cp = -1;
        return -1;
      }
    }
    char32_t ch1 = buffer[globalPosition++];
    if (linestate == 0) {
      if (ch1 == U'\r') {
        linestate = 1;
        globalLine++;
        globalColumn = 1;
      }
      else if (ch1 == U'\n') {
        globalLine++;
        globalColumn = 1;
      }
      else if(ch1 == '\t') {
        globalColumn += 4;
      } else {
        globalColumn++;
      }
    }
    else { // linestate == 1
      if (ch1 == U'\r') {
        globalLine++;
        globalColumn = 1;
      }
      else {
        linestate = 0;
      }
    }
    this->cp = ch1;
    return ch1;
  }

private:
  inline std::u32string getRangeText(int start, int end) {
    return std::u32string(&buffer[start], &buffer[end]);
  }

public:
  inline std::u32string text() {
    return getRangeText(globalStart, globalEnd);
  }

  inline char32_t ch() {
    return this->cp;
  }

  inline std::u32string rext() {
    return getRangeText(globalStart, globalPosition);
  }

  int newMark() {
    int minCapacity = markCnt + 1;
    int oldCapacity = marksSize;
    if (minCapacity > oldCapacity) {
      int *oldData = marks;
      int newCapacity = (oldCapacity * 3) / 2 + 1;
      if (newCapacity < minCapacity) {
        newCapacity = minCapacity;
      }
      marks = new int[newCapacity];
      std::memcpy(marks, oldData, markCnt * sizeof(int));
      marksSize = newCapacity;
      delete[] oldData;
    }
    int ret = markCnt;
    marks[ret] = globalPosition;
    markCnt++;
    return ret;
  }

  inline void cancelMark() {
    if (markCnt > 0) {
      markCnt--;
    }
  }

  inline void mark(int id) {
    marks[id] = globalPosition;
  }

  inline std::u32string getMarkedText(int mark1, int mark2) {
    return getRangeText(marks[mark1], marks[mark2]);
  }

  inline std::u32string getMarkedText(int mark) {
    return getRangeText(mark, globalPosition);
  }

public:
  inline int line() {
    return tline;
  }

public:
  inline int column() {
    return tcolumn;
  }

private:
  int fill() {
    if (globalStart > 0) {
      int i, j = 0;
      for(i = globalStart; i < limit; i++) {
        buffer[j++] = buffer[i];
      }
      limit -= globalStart;
      globalPosition -= globalStart;
      globalMark -= globalStart;
      globalEnd -= globalStart;
      globalStart = 0;
    }
    if (globalPosition >= bufferSize) {
      int newSize = globalPosition * 3 / 2 + 1;
      char32_t* newBuffer = new char32_t[newSize];
      std::memcpy(newBuffer, buffer, bufferSize * sizeof(char32_t));
      delete[] buffer;
      buffer = newBuffer;
      bufferSize = newSize;
    }
    int numRead = reader->read(buffer, limit,
                          bufferSize - limit);
    if (numRead == -1) {
      return 1;
    }
    else {
      limit += numRead;
      return 0;
    }
  }

private:
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
    return ch;
  }

  static int readVarint(MemoryInputStream *ins);

public:
  void setLexState(int lexId);

public:
  parser::SimTokenType scan();

public:
  static void staticInit();

public:
  static void staticUninit();

private:
  void initIntance(int initBufferSize, TextReader *reader, SimHandler *handler);

};

}

#endif
