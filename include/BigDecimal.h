#pragma once

#include <cstddef>
#include <string>

namespace WolverinDEV {
    class BigDecimal {
    public:
        BigDecimal();
        BigDecimal(BigDecimal& ref);
        BigDecimal(uint64_t num, bool negative = false);

        ~BigDecimal();

        std::string toString(int radex);
        std::string toString(int radex, char* radexChars);

        bool testBit(size_t index) const;
        void setBit(size_t index, bool flag);


        BigDecimal& operator+=(const BigDecimal&);
        BigDecimal operator+(BigDecimal);
        BigDecimal operator-=(BigDecimal);
        BigDecimal operator-(BigDecimal);
        BigDecimal operator*=(BigDecimal);
        BigDecimal operator*(BigDecimal);
        BigDecimal operator/=(BigDecimal);
        BigDecimal operator/(BigDecimal);

    private:
        char* buffer = nullptr;
        size_t bufferSize = 0;

        size_t highestBit = 0;
        bool negative = false;

        void allocateBit(size_t index);
        inline bool fastTestBit(size_t index) const;
    };
}