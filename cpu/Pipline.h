#include <cstdint>

#include "Map.h"
struct F_Reg {
    uint8_t control = C_NORMAL;
    uint8_t stat = S_AOK;
    uint64_t predPC = 0;
    void reset() {
        stat = S_BUB;
    } 
};
struct D_Reg {
    uint8_t control = C_NORMAL;
    uint8_t stat = S_AOK;
    uint8_t icode = 0;
    uint8_t ifun = 0;
    uint8_t rA = R_NONE;
    uint8_t rB = R_NONE;
    uint64_t valC = 0;
    uint64_t valP = 0;
    uint64_t predPC = 0;
    bool Jump = true;
    void reset() {
        stat = S_BUB;
        icode = I_NOP;
        rA = R_NONE;
        rB = R_NONE;
        Jump = true;
    }
};
struct E_Reg {
    uint8_t control = C_NORMAL;
    uint8_t stat = S_AOK;
    uint8_t icode = 0;
    uint8_t ifun = 0;
    uint64_t valC = 0;
    uint64_t valA = 0;
    uint64_t valB = 0;
    uint8_t dstE = R_NONE;
    uint8_t dstM = R_NONE;
    uint8_t srcA = R_NONE;
    uint8_t srcB = R_NONE;
    uint64_t predPC = 0;
    bool Jump = true;
    bool Done = true;
    void reset() {
        stat = S_BUB;
        icode = I_NOP;
        dstE = R_NONE;
        dstM = R_NONE;
        srcA = R_NONE;
        srcB = R_NONE;
        Done = true;
        Jump = true;
    }
};
struct M_Reg {
    uint8_t control = C_NORMAL;
    uint8_t stat = S_AOK;
    uint8_t icode = 0;
    uint8_t ifun = 0;
    uint64_t valE = 0;
    uint64_t valA = 0;
    uint8_t dstE = R_NONE;
    uint8_t dstM = R_NONE;
    uint64_t predPC = 0;
    bool Jump = true;
    bool Cnd = false;
    void reset() {
        stat = S_BUB;
        icode = I_NOP;
        dstE = R_NONE;
        dstM = R_NONE;
    }
};
struct W_Reg {
    uint8_t control = C_NORMAL;
    uint8_t stat = S_AOK;
    uint8_t icode = 0;
    uint8_t ifun = 0;
    uint64_t valE = 0;
    uint64_t valM = 0;
    uint8_t dstE = R_NONE;
    uint8_t dstM = R_NONE;
    uint64_t predPC = 0;
    void reset() {
        stat = S_BUB;
        icode = I_NOP;
        dstE = R_NONE;
        dstM = R_NONE;
    }
};
struct F_Wire {
    uint8_t stat = S_AOK;
    uint8_t icode = 0;
    uint8_t ifun = 0;
    uint8_t rA = R_NONE;
    uint8_t rB = R_NONE;
    uint64_t valC = 0;
    uint64_t valP = 0;
    uint64_t predPC = 0;
    uint64_t pc = 0;
    bool Jump = true;
};
struct D_Wire {
    uint8_t stat = S_AOK;
    uint8_t icode = 0;
    uint8_t ifun = 0;
    uint64_t valC = 0;
    uint64_t valA = 0;
    uint64_t valB = 0;
    uint8_t dstE = R_NONE;
    uint8_t dstM = R_NONE;
    uint8_t srcA = R_NONE;
    uint8_t srcB = R_NONE;
    uint64_t predPC = 0;
    uint64_t pc = 0;
    bool Jump = true;
};
struct E_Wire {
    uint8_t stat = S_AOK;
    uint8_t icode = 0;
    uint8_t ifun = 0;
    uint64_t valE = 0;
    uint64_t valA = 0;
    uint64_t valC = 0;
    uint8_t dstE = R_NONE;
    uint8_t dstM = R_NONE;
    uint64_t predPC = 0;
    uint64_t pc = 0;
    bool Jump = true;
    bool Cnd = false;
    bool Ccode[3] = {true,false,false};
};
struct M_Wire {
    uint8_t stat = S_AOK;
    uint8_t icode = 0;
    uint8_t ifun = 0;
    uint64_t valE = 0;
    uint64_t valM = 0;
    uint8_t dstE = R_NONE;
    uint8_t dstM = R_NONE;
    uint64_t predPC = 0;
};