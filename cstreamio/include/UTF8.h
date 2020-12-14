#ifndef _UTF8_H
#define _UTF8_H

#include "ByteBuffer.h"

// UTF8 decoder
void utf8_decoder (
  ByteBuffer *src,
  char32_t *dest_buf,
  unsigned int offset,
  unsigned int count,
  unsigned int *ndecoded);

// UTF8 encoder
void utf8_encoder (
  const char32_t *src_buf,
  unsigned int offset,
  unsigned int count,
  ByteBuffer *dest,
  unsigned int *nencoded);

#endif
