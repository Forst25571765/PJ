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
