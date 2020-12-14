#ifndef _ASCII_H
#define _ASCII_H

#include "ByteBuffer.h"

// ASCII decoder
inline void ascii_decoder (
  ByteBuffer *src,
  char32_t *dest_buf,
  unsigned int offset,
  unsigned int count,
  unsigned int *ndecoded) {
  char *sa = src->getBuffer();
  int sp = src->getPosition();
  int sl = src->getLimit();
	int dp = offset;
	int dl = offset + count;

  while((sp < sl) && (dp < dl)) {
    dest_buf[dp++] = (char32_t)(sa[sp++] & 0xFF);
  }
  src->setPosition(sp);
  *ndecoded = (dp - offset);  
};

// ASCII encoder
inline void ascii_encoder (
  char32_t *src_buf,
  unsigned int offset,
  unsigned int count,
  ByteBuffer *dest,
  unsigned int *nencoded) {

  unsigned int sp = offset;
  unsigned int sl = offset + count;
  char* da = dest->getBuffer();
  int dp = dest->getPosition();
  int dl = dest->getLimit();

  while((sp < sl) && dp < dl) {
    da[dp++] = (char)src_buf[sp++];
  }
  
  dest->setPosition(dp);
  *nencoded = (sp - offset); 
};


#endif
