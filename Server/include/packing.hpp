#ifndef PACKING_HPP
#define PACKING_HPP

#include <utility>
#include <cstdint>

#define pack754_32(f) (pack754((f), 32, 8))
#define pack754_64(f) (pack754((f), 64, 11))
#define unpack754_32(i) (unpack754((i), 32, 8))
#define unpack754_64(i) (unpack754((i), 64, 11))


uint64_t pack754(long double f, unsigned bits, unsigned expbits);
long double unpack754(uint64_t i, unsigned bits, unsigned expbits);
void packu32(unsigned char *buf, uint32_t i);
uint32_t unpacku32(unsigned char *buf);

void encode_pos(unsigned char *buf, float x, float y);
std::pair<int16_t, int16_t> decode_input(unsigned char *buf);

#endif