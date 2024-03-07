#ifndef NETUTILS_HPP
#define NETUTILS_HPP

#include <utility>
#include <cstdint>
#include <string>
#include <stdexcept>
#include <cstring>


namespace pack {
#define pack754_32(f) (pack754((f), 32, 8))
#define pack754_64(f) (pack754((f), 64, 11))
#define unpack754_32(i) (unpack754((i), 32, 8))
#define unpack754_64(i) (unpack754((i), 64, 11))


uint64_t pack754(long double f, unsigned bits, unsigned expbits);
long double unpack754(uint64_t i, unsigned bits, unsigned expbits);
void packu32(unsigned char *buf, uint32_t i);
uint32_t unpacku32(unsigned char *buf);

void encode_pos(unsigned char *buf, float x, float y);
std::pair<float, float> decode_pos(unsigned char *buf);

void encode_input(unsigned char *buf, int16_t x, int16_t y);
std::pair<int16_t, int16_t> decode_input(unsigned char *buf);

} // namespace pack


class NetworkException : public std::runtime_error
{
public:
    NetworkException(const std::string &msg)
    : std::runtime_error(msg + ": " + strerror(errno))
    {}
};

#endif