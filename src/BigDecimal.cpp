//
// Created by hadenfmar on 26.09.2017.
//

#include <iostream>
#include <cassert>
#include <cstring>
#include <sstream>
#include <bitset>
#include "../include/BigDecimal.h"

using namespace std;
using namespace WolverinDEV;

BigDecimal::BigDecimal() {
    this->buffer = (char *) malloc(this->bufferSize = 8);
}

BigDecimal::BigDecimal(BigDecimal &ref) {
    this->bufferSize = ref.bufferSize;
    this->buffer = (char *) malloc(this->bufferSize);
    memcpy(this->buffer, ref.buffer, this->bufferSize);

    this->negative = ref.negative;
    this->highestBit = ref.highestBit;
}

BigDecimal::BigDecimal(uint64_t num, bool negative) : BigDecimal() {
    this->negative = negative;

    int index = sizeof(num) * 8 - 1;
    while(index >= 0){
        setBit(index, (num >> index) & 0x01);
        index--;
    }
    cout << "Highest bit: " << this->highestBit << endl;
}

BigDecimal::~BigDecimal() {}

std::string BigDecimal::toString(int radex) {
    assert(radex >= 2 && radex <= 16);
    return this->toString(radex, "0123456789ABCDEF");
}

std::string BigDecimal::toString(int radex, char* radexChars) {
    stringstream ss;

    if(radex == 2){
        int index = max(1, (int) this->highestBit);
        while(index >= 0){
            ss << (testBit(index) ? "1" : "0");
            index--;
        }
    }

    return ss.str();
}

bool BigDecimal::testBit(size_t index) const {
    int arrIndex = index >> 3;
    if(arrIndex >= this->bufferSize) return false;
    return fastTestBit(index);
}

bool BigDecimal::fastTestBit(size_t index) const {
    return ((this->buffer[index >> 3]) & (1 << (index & 0x7))) > 0;
}

void BigDecimal::setBit(size_t index, bool flag) {
    int arrIndex = index >> 3;
    int bitOffset = index & 0x07;

    if(arrIndex >= this->bufferSize) allocateBit(index);

    cout << "Setting " << index << " to " << flag << endl;
    if(flag) {
        this->buffer[arrIndex] |= (1 << bitOffset);
        this->highestBit = max(this->highestBit, index);
    } else {
        this->buffer[arrIndex] &= ~(1 << bitOffset);
        if(index == this->highestBit)
            while(!testBit(this->highestBit) && this->highestBit > 0) this->highestBit--;
    }
}

void BigDecimal::allocateBit(size_t index) {
    int arrIndex = index >> 3;
    if(this->bufferSize < arrIndex){
        auto oldBuffer = this->buffer;
        auto oldBufferSize = this->bufferSize;

        this->buffer = (char *) malloc(this->bufferSize = arrIndex + 1);
        memset(this->buffer, 0, this->bufferSize);

        if(oldBuffer){
            memcpy(this->buffer, oldBuffer, oldBufferSize);
            free(oldBuffer);
        }
    }
}

//TODO maybe in assambler?
BigDecimal& BigDecimal::operator+=(const BigDecimal& other) {
    if(other.negative xor this->negative) return *this; //TODO implement minus

    int minIndex = min(other.highestBit, this->highestBit);
    int maxIndex = max(other.highestBit, this->highestBit) + 1;
    this->allocateBit(maxIndex); //Allocate buffer

    size_t index = 0;
    bool carry = false;

    bool bitRes;
    bool flagA, flagB;

    while(index <= minIndex || carry){
        flagA = fastTestBit(index);
        flagB = other.fastTestBit(index);

        if(flagA and flagB and carry){
            bitRes = true;
            carry = true;
        } else if((carry and flagA) || (carry and flagB) || (flagA and flagB)) {
            bitRes = false;
            carry = true;
        }  else {
            bitRes = (carry or flagA or flagB);
            carry = false;
        }

        setBit(index, bitRes);
        index++;
    }

    return *this;
}