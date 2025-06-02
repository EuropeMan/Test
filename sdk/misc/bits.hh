#ifndef BITS_H
#define BITS_H

#include <cstdint>

struct matrix4x2_t
{
public:
    float* operator[](const int index) {
        return data[index];
    }

    const float* operator[](const int index) const {
        return data[index];
    }

    union {
        struct {
            float x, y, z, w;
            float x2, y2, z2, w2;
        };
        float data[4][2];
    };
};

struct bits_t
{
private:
	std::uint32_t m_bits;
public:
	bits_t() : m_bits(0) {}
	bits_t(const std::uint32_t& bit) : m_bits(bit) {}

	bool has(const std::uint32_t& bit) { return m_bits & bit; }
	void set(const std::uint32_t& bit) { m_bits |= bit; }
	void remove(const std::uint32_t& bit) { m_bits &= ~bit; }
};

#endif