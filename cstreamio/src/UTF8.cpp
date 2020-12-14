#include "UTF8.h"
#include "surrogate.h"

/*
 * # Bits   Bit pattern
 * 1    7   0xxxxxxx
 * 2   11   110xxxxx 10xxxxxx
 * 3   16   1110xxxx 10xxxxxx 10xxxxxx
 * 4   21   11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
 * 5   26   111110xx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx
 * 6   31   1111110x 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx
 *
 * UCS-2 uses 1-3, UTF-16 uses 1-4, UCS-4 uses 1-6
 */

#define REPLACE_CHAR 0xFFFD

// int b: byte
#define isContinuation(b) (((b) & 0xc0) == 0x80)

void utf8_decoder (
  ByteBuffer *src,
  char32_t *dest_buf,
  unsigned int offset,
  unsigned int count,
  unsigned int *ndecoded) {

  char *sa = src->getBuffer();
  unsigned int sp = src->getPosition();
  unsigned int sl = src->getLimit();
  unsigned int dp = offset;
  unsigned int dl = offset + count;

  while (sp < sl && dp < dl) {
    int b1 = sa[sp];
    int b2, b3;
    switch ((b1 >> 4) & 0x0f) {
      case 0: case 1: case 2: case 3:
      case 4: case 5: case 6: case 7:
        // 1 byte, 7 bits: 0xxxxxxx
        dest_buf[dp++] = (char32_t)(b1 & 0x7f);
        sp++;
        continue;

      case 12: case 13:
        // 2 bytes, 11 bits: 110xxxxx 10xxxxxx
        if (sl - sp < 2) {
          goto return_exit;
        }
        if (!isContinuation(b2 = sa[sp + 1])) {
          // ingore error
          dest_buf[dp++] = REPLACE_CHAR;
          sp += 2;
          continue;
        }
        dest_buf[dp++] = ((char32_t)(((b1 & 0x1f) << 6) |
          ((b2 & 0x3f) << 0)));
        sp += 2;
        continue;

      case 14:
        // 3 bytes, 16 bits: 1110xxxx 10xxxxxx 10xxxxxx
        if (sl - sp < 3) {
          goto return_exit;
        }
        if ((!isContinuation(b2 = sa[sp + 1])) || (!isContinuation(b3 = sa[sp + 2]))) {
          dest_buf[dp++] = REPLACE_CHAR;
          sp += 3;
          continue;
        }
        dest_buf[dp++] = ((unsigned int)(((b1 & 0x0f) << 12) |
          ((b2 & 0x3f) << 06) |
          ((b3 & 0x3f) << 0)));
        sp += 3;
        continue;

      case 15:
        // 4, 5, or 6 bytes

        int b4, b5, b6, uc, nbyte;
        switch (b1 & 0x0f) {
          case 0: case 1: case 2: case 3:
          case 4: case 5: case 6: case 7:
          // 4 bytes, 21 bits
          if (sl - sp < 4) {
            goto return_exit;
          }
          if ((!isContinuation(b2 = sa[sp + 1])) ||
              (!isContinuation(b3 = sa[sp + 2])) ||
              (!isContinuation(b4 = sa[sp + 3]))) {
              dest_buf[dp++] = REPLACE_CHAR;
              sp += 4;
              continue;
          }
          uc = (((b1 & 0x07) << 18) |
            ((b2 & 0x3f) << 12) |
            ((b3 & 0x3f) << 06) |
            ((b4 & 0x3f) << 00));
          nbyte = 4;
          break;

          case 8: case 9: case 10: case 11:
            // 5 bytes, 26 bits
            if (sl - sp < 5) {
              goto return_exit;
            }
            if((!isContinuation(b2 = sa[sp + 1])) ||
               (!isContinuation(b3 = sa[sp + 2])) ||
               (!isContinuation(b4 = sa[sp + 3])) ||
               (!isContinuation(b5 = sa[sp + 4]))) {
              dest_buf[dp++] = REPLACE_CHAR;
              sp += 4;
              continue;
            }

            uc = (((b1 & 0x03) << 24) |
              ((b2 & 0x3f) << 18) |
              ((b3 & 0x3f) << 12) |
              ((b4 & 0x3f) << 06) |
              ((b5 & 0x3f) << 00));
            nbyte = 5;
            break;

          case 12: case 13:
            // 6 bytes, 31 bits
            if (sl - sp < 6) {
              goto return_exit;
            }
            if((!isContinuation(b2 = sa[sp + 1])) ||
               (!isContinuation(b3 = sa[sp + 2])) ||
               (!isContinuation(b4 = sa[sp + 3])) ||
               (!isContinuation(b5 = sa[sp + 4])) ||
               (!isContinuation(b6 = sa[sp + 5]))) {
              dest_buf[dp++] = REPLACE_CHAR;
              sp += 4;
              continue;
            }
            uc = (((b1 & 0x01) << 30) |
              ((b2 & 0x3f) << 24) |
              ((b3 & 0x3f) << 18) |
              ((b4 & 0x3f) << 12) |
              ((b5 & 0x3f) << 06) |
              ((b6 & 0x3f)));
            nbyte = 6;
            break;

          default:
            dest_buf[dp++] = REPLACE_CHAR;
            sp += 1;
            continue;
        }

        if (uc <= 0xffff) {
          if (isSurrogate(uc)) {
            dest_buf[dp++] = REPLACE_CHAR;
            sp += nbyte;
            continue;
          }
          dest_buf[dp++] = (unsigned int)uc;
          continue;
        }
        if (uc <= UCS4_MAX) {
          dest_buf[dp++] = (unsigned int)uc;
          sp += nbyte;
          continue;
        }
        dest_buf[dp++] = REPLACE_CHAR;
        sp += nbyte;
        continue;

      default:
        dest_buf[dp++] = REPLACE_CHAR;
        sp += 1;
        break;
    } // switch
  } // while

return_exit:
  src->setPosition(sp);
  *ndecoded = (dp - offset);  
}

void utf8_encoder (
  const char32_t *src_buf,
  unsigned int offset,
  unsigned int count,
  ByteBuffer *dest,
  unsigned int *nencoded) {

  unsigned int sp = offset;
  unsigned int sl = offset + count;
  char* da = dest->getBuffer();
  unsigned int dp = dest->getPosition();
  unsigned int dl = dest->getLimit();

  while (sp < sl) {
    unsigned int uc = src_buf[sp];

    if (uc < 0x80) {
      if (dp >= dl) {
        break;
      }
      da[dp++] = (char)uc;
      sp++;
      continue;
    }

    // 2 bytes, 11 bits
    if (uc < 0x800) {
      if (dl - dp < 2){
        break;
      }
      da[dp++] = (char)(0xc0 | ((uc >> 06)));
      da[dp++] = (char)(0x80 | ((uc >> 00) & 0x3f));
      sp++;
      continue;
    }
    if (uc <= 0xFFFF) {
      // 3 bytes, 16 bits
      if (dl - dp < 3) {
        break;
      }
      da[dp++] = (char)(0xe0 | ((uc >> 12)));
      da[dp++] = (char)(0x80 | ((uc >> 06) & 0x3f));
      da[dp++] = (char)(0x80 | ((uc >> 00) & 0x3f));
      sp++;
      continue;
    }

    if (uc < 0x110000) {
      if (dl - dp < 4) {
        break;
      }
      da[dp++] = (char)(0xf0 | ((uc >> 18)));
      da[dp++] = (char)(0x80 | ((uc >> 12) & 0x3f));
      da[dp++] = (char)(0x80 | ((uc >> 06) & 0x3f));
      da[dp++] = (char)(0x80 | ((uc >> 00) & 0x3f));
      sp++;
      continue;
    }

    da[dp++] = '?';
    sp++;
    continue;
  }

  dest->setPosition(dp);
  *nencoded = (sp - offset);
}
