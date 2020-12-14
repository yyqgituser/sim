#ifndef _SURROGATE_H
#define _SURROGATE_H

// surrogate charater ranges
#define MIN_HIGH 0xD800
#define MAX_HIGH 0xDBFF
#define MIN_LOW  0xDC00
#define MAX_LOW  0xDFFF
#define SURROGATE_MIN MIN_HIGH
#define SURROGATE_MAX MAX_LOW

#define UCS4_MIN 0x10000
#define UCS4_MAX ((1 << 20) + UCS4_MIN - 1)

// unsigned int c: code point
#define isHigh(uc) ((MIN_HIGH <= (uc)) && ((uc) <= MAX_HIGH))

// int c: code point
#define isLow(uc) ((MIN_LOW <= (uc)) && ((uc) <= MAX_LOW))

// unsigned int h, unsigned int l
#define toUCS4(h, l) (((((h) & 0x3ff) << 10) | ((l) & 0x3ff)) + 0x10000)

// unsigned int uc: code point
#define isSurrogate(uc) ((SURROGATE_MIN <= (uc)) && ((uc) <= SURROGATE_MAX))

/**
 * Tells whether or not the given UCS-4 character must be represented as a
 * surrogate pair in UTF-16.
 */
// unsigned int uc: code point
#define neededFor(uc) (((uc) >= UCS4_MIN) && ((uc) <= UCS4_MAX))

/**
 * Returns the high UTF-16 surrogate for the given UCS-4 character.
 */
// unsigned int uc: code point
#define high(uc) ((unsigned int)(0xd800 | ((((uc) - UCS4_MIN) >> 10) & 0x3ff)))

/**
 * Returns the low UTF-16 surrogate for the given UCS-4 character.
 */
// unsigned int uc: code point
#define low(uc) ((unsigned int)(0xdc00 | (((uc) - UCS4_MIN) & 0x3ff)))

#endif
