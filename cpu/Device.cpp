#include <cstdint>
#include <cstring>
#include <functional>
#include <queue>
#include <stack>
#include <string>

#include "Map.h"
#include "Pipline.h"
#include "Device.h"
uint8_t Char2Uint8(char c) {
    if (c >= '0' && c <= '9') {
        return (c - '0');
    }
    if (c >= 'a' && c <= 'f') {
        return (c - 'a');
    }
    if (c >= 'A' && c <= 'F') {
        return (c - 'A');
    }
    throw std::domain_error("code error");
    exit(1);
}
Device::Device(std::string code) {
    memset(Reg, 0, REG_SIZE);
    memset(Mem, 0, MEM_SIZE);
    Stat = S_AOK;
    int length = code.length();
    if (length > 2 * MEM_SIZE) throw std::domain_error("code too long");
    if (length == 0) throw std::domain_error("no code");
    int cnt = 0;
    auto p = code.begin();
    while (p != code.end()) {
        Mem[cnt++] = 16 * Char2Uint8(*p++) + Char2Uint8(*p++);
    }
    addr = 0;
}
uint8_t Device::ReadHigh4Bits(uint64_t pos) {
    return (Mem[pos] & 0xf0u) >> 4u;
}
uint8_t Device::ReadLow4Bits(uint64_t pos) { 
    return (Mem[pos] & 0x0fu); 
}
uint64_t Device::Read8Bytes(uint64_t pos) {
    return *(uint64_t *)(Mem + pos);
}
uint64_t Device::Read4Bytes(uint64_t pos) {
    return *(uint32_t *)(Mem + pos);
}
uint64_t Device::Read2Bytes(uint64_t pos) {
    return *(uint16_t *)(Mem + pos);
}
uint64_t Device::Read1Bytes(uint64_t pos) {
    return *(uint8_t *)(Mem + pos);
}
void Device::Write8Bytes(uint64_t pos, uint64_t val) {
    *(uint64_t *)(Mem + pos) = val;
}
void Device::Write4Bytes(uint64_t pos, uint64_t val) {
    *(uint32_t *)(Mem + pos) = val;
}
void Device::Write2Bytes(uint64_t pos, uint64_t val) {
    *(uint16_t *)(Mem + pos) = val;
}
void Device::Write1Bytes(uint64_t pos, uint64_t val) { 
    *(uint8_t *)(Mem + pos) = val; 
}
void Device::Fetch() {
    if (F.control == C_STALL) {
        return; 
    }
    f.pc = SelectPC();
}
