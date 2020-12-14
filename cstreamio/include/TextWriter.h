#ifndef _TEXT_WRITER_H
#define _TEXT_WRITER_H

#include <iostream>
#include <cstdio>
#include <cstdlib>

#include "ByteBuffer.h"

using std::ostream;

// UCS4 characters to binary bytes
// all char in src_buf will be encode into dest byte buffer if the free
// space was enough.
typedef void (*charset_encoder) (
  const char32_t *src_buf, // source characters
  unsigned int offset, // from which to start encoding
  unsigned int count, // count of chars
  ByteBuffer *dest, // destination byte buffer, start from getPosition() until to getCapacity()
  unsigned int *nencoded); // output: how many chars were encoded

class TextWriter {
private:
  const static int DEFAULT_BUFFER = 4 * 1024;

  // lower layer output stream
  ostream *out;

  // bytes buffer
  ByteBuffer bb;

  // encoder
  charset_encoder encoder;

public:
  TextWriter(ostream *outputStream, charset_encoder pencoder) :
    out(outputStream), bb(DEFAULT_BUFFER), encoder(pencoder) {
    bb.setLimit(bb.getCapacity());
  }

  // this bufferSize indicates the size of buffer that be  used to write the binary
  // data to file, other than the character buffer
  TextWriter(ostream *outputStream, charset_encoder pencoder, int bufferSize) :
    out(outputStream), bb(bufferSize), encoder(pencoder) {
    bb.setLimit(bb.getCapacity());
  }

  ~TextWriter() {
    flush();
  }

public:  
  // writes count charaters from the specified offset to the output stream.
  // currently, this function can't support surrogate pairs.
  void write(const char32_t* char_buf, unsigned int offset, unsigned int count) {
    unsigned int nencoded = 0;
    encoder(char_buf, offset, count, &bb, &nencoded);
    if(nencoded == 0) {
      // the number of encoded char is 0, may free buffer was not enough.
      out->write(bb.getBuffer(), bb.getPosition());
      bb.setPosition(0);
      encoder(char_buf, offset, count, &bb, &nencoded);
      if(nencoded == 0) {
        fputs ("buffer too small exception\n", stderr);
        abort();
      }
    }
    offset += nencoded;
    count -= nencoded;
    while(count > 0) {
      // encoder was called, but number of remainder chars is not 0, it meens 
      // the byte buffer have been full.
      out->write(bb.getBuffer(), bb.getPosition());
      bb.setPosition(0);
      encoder(char_buf, offset, count, &bb, &nencoded);
      if(nencoded == 0) {
        fputs ("buffer too small exception\n", stderr);
        abort();
      }
      offset += nencoded;
      count -= nencoded;
    }
  }

  // flush to output stream
  void flush() {
    if(bb.getPosition() > 0) {
      out->write(bb.getBuffer(), bb.getPosition());
      out->flush();
      bb.setPosition(0);
    }
  }
};

#endif
