#ifndef _TEXT_READER_H
#define _TEXT_READER_H

#include <iostream>
#include <cstdio>
#include <cstdlib>

#include "ByteBuffer.h"

using std::istream;

// binary bytes to UTF-32 characters
typedef void (*charset_decoder) (
  ByteBuffer *src, // byte data: consider the data is from getPosition() to getLimit()
  char32_t *dest_buf, // destination char buffer
  unsigned int offset, // start to save chars
  unsigned int count,  // maximum chars to decode
  unsigned int *ndecoded // the number of characters decoded
  );

class TextReader {
private:
  const static int DEFAULT_BUFFER = 4 * 1024;
  
  // byte order mark
  const static int UTF8_BOM = 0xEFBBBF;

  // lower layer input stream
  istream *in;

  // bytes buffer
  ByteBuffer bb;

  // decoder
  charset_decoder decoder;

public:
  TextReader(istream *inputStream, charset_decoder pdecoder) :
    in(inputStream), bb(DEFAULT_BUFFER), decoder(pdecoder) {
  }

  // this bufferSize indicates the size of buffer that can be  used to read the binary
  // data from file, other than the character buffer
  TextReader(istream *inputStream, charset_decoder pdecoder, int bufferSize) :
    in(inputStream), bb(bufferSize), decoder(pdecoder) {
  }

  ~TextReader() {
  }

private:
  // read in data from input stream 
  inline int doRead() {
    unsigned int limit = bb.getLimit();
    unsigned int nfree = bb.getCapacity() - limit;
    if(nfree == 0) {
      fputs ("buffer too small exception\n", stderr);
      abort();
    }

    if(!in->good()) {
      return -1;
    }

    in->read(bb.getBuffer() + limit, nfree);
    int nread = in->gcount();
    if(nread == -1) {
      return -1;
    }
    bb.setLimit(limit + nread);
    return nread;
  }

public:  
  // reads up to count unicode characters from the stream and put them to the
  // destination buffer, this function will return immediately at least one
  // character was decoded.
  int read(char32_t* buffer, unsigned int offset, unsigned int count) {
    if(count == 0) {
      return 0;
    }

    unsigned int ndecoded = 0;
    if(bb.hasRemaining()) {
      decoder(&bb, buffer, offset, count, &ndecoded);

      // return immediately when some chars is available
      if(ndecoded > 0) {
        return ndecoded;
      }
    }

    bb.compact();
    while(ndecoded == 0) {
      int nread = doRead();
      if(nread == -1) {
        if(bb.hasRemaining()) {
          // Ignore coding error
          buffer[offset] = 0xFFFDU;
          bb.setPosition(bb.getLimit());
          return 1;
        }
        return -1;
      }
      
      decoder(&bb, buffer, offset, count, &ndecoded);
    }
    return ndecoded;
  }

  // skip the unicode Byte Order Mark, must be called before any actual read operation,
  // and should be called when the decoder is UTF-8 decoder.
  // return 1, skipped the BOM, return 0, did nothing
  inline int skipUTF8BOM() {
    while(bb.remaining() < 3) {
      int nread = doRead();
      if(nread == -1) {
        return 0;
      }
    }
    unsigned int p = bb.getPosition();
    char *buf = bb.getBuffer();
    int b1 = buf[p] & 0xff;
    int b2 = buf[p + 1] & 0xff;
    int b3 = buf[p + 2] & 0xff;

    int c = ((b1 << 16) | (b2 << 8) | b3);
    if (c == UTF8_BOM) {
      bb.setPosition(p + 3);
      return 1;
    } else {
      return 0;
    }
  }
};

#endif
