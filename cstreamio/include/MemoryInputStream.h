#ifndef _MEMORY_INPUT_STREAM_H
#define _MEMORY_INPUT_STREAM_H

#include <cstring>

class MemoryInputStream {
private:
  // buffer
  const char* data;

  // end of data
  unsigned int limit;

  // current position
  unsigned int position;

public:
  MemoryInputStream(const char *mem, unsigned int size) :
    data(mem), limit(size), position(0) {
  }

  ~MemoryInputStream() {
  }

  // reads one byte, return -1 if there is no data for the end of stream.
  inline int read() {
    return position < limit ? (data[position++] & 0xff) : -1;
  }

  inline int read(char* buffer, unsigned int offset, unsigned int count) {
    if(position == limit) {
      return -1;
    }

    unsigned int remaining = limit - position;
    unsigned int cnt = count > remaining ? remaining : count;
    std::memcpy(buffer + offset, data + position, cnt);
    position += cnt;
    return cnt;
  }
};

#endif
