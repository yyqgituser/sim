#ifndef _BYTE_BUFFER_H
#define _BYTE_BUFFER_H

#include <cstdio>
#include <cstdlib>

class ByteBuffer {
private:
  // current position
  unsigned int position;

  // end of data
  unsigned int limit;

  // buf size
  unsigned int capacity;

  // buffer
  char* buf;

public:
  ByteBuffer(unsigned int cap) :
    position(0), limit(0), capacity(cap) {    
    buf = new char[capacity];
  }

  ~ByteBuffer() {
    if(buf != (char *)0) {
      delete[] buf;
    }
  }

  inline char get() {
    return buf[position++];
  }
  
  inline unsigned int getPosition() {
    return position;
  }
  
  inline void setPosition(unsigned int newPosition) {
    if (newPosition > limit) {
      fputs ("index out of bound\n", stderr);
      abort();
    }

    position = newPosition;
  }
  
  inline unsigned int getLimit() {
    return limit;
  }
  
  inline void setLimit(unsigned int newLimit) {
    if (newLimit > capacity) {
      throw;
    }

    limit = newLimit;

    if (position > limit) {
      position = limit;
    }
  }
  
  inline unsigned int getCapacity() {
    return capacity;
  }
  
  inline char* getBuffer() {
    return buf;
  }

  inline bool hasRemaining() {
    return position < limit;
  }

  // return numbers of elements in the buffer.
  inline unsigned int remaining() {
    return limit - position;
  }

  inline unsigned int freeSpace() {
    return capacity - limit;
  }
  
  // compact this buffer
  // copy bytes from current position to beginning of the buffer
  inline void compact() {
    unsigned int i, j;

    for(i = position, j = 0; i < limit; i++) {
      buf[j++] = buf[i];
    }
    position = 0; limit = j;
  }

};


#endif
