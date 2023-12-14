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
template <typename T, typename... List>
bool In(T lhs, T first, List... list) {
    return lhs == first || In(lhs, list...);
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
    f.icode = ReadHigh4Bits(f.pc);
    f.ifun = ReadLow4Bits(f.pc);
    f.stat = SelectFStat();
    bool need_regids = In(f.icode, I_CMOVEXX, I_PUSHQ, I_POPQ, I_IRMOVEQ, I_RMMOVEQ, I_MRMOVQ, I_OPQ, I_IOPQ, I_OPQN, I_IOPQN);//是否需要寄存器
    bool need_valC = In(f.icode, I_IRMOVEQ, I_RMMOVEQ, I_MRMOVQ, I_JXX, I_CALL, I_IOPQ, I_IOPQN);//是否有立即数
    if (need_regids) {
        if (IfAddrExecutable(f.pc + 1)) {
            f.rA = ReadHigh4Bits(f.pc + 1);
            f.rB = ReadLow4Bits(f.pc + 1); 
        }
        else f.stat = S_INS;
    }
    if (need_valC) {
        if (IfAddrExecutable(f.pc + need_regids + 1)) {
            f.valC = Read8Bytes(f.pc + need_regids + 1);
        }
        else f.stat = S_INS;
    }
    f.valP = f.pc + 1 + need_regids + 8 * need_valC;
    if (f.stat == S_HLT) F.predPC = f.pc;
    else F.predPC = GetFPredPC();
}
void Device::F2D() {
    if (F.control == C_STALL) return;
    D.stat = f.stat;
    D.icode = f.icode;
    D.ifun = f.ifun;
    D.rA = f.rA;
    D.rB = f.rB;
    D.valC = f.valC;
    D.valP = f.valP;
    D.Jump = f.Jump;
    D.predPC = f.pc;
}
void Device::Decode() {
    if (D.control == C_STALL) return;
    if (D.control == C_BUBBLE) D.reset();
    d.srcA = SelectSrcA();
    d.srcB = SelectSrcB();
    d.dstE = SelectDstE();
    d.dstM = SelectDstM();
    if (d.srcA != R_NONE) d.valA = ReadReg(d.srcA, R_QU);
    else d.valA = 0;
    if (d.srcB != R_NONE) d.valB = ReadReg(d.srcB, R_QU);
    else d.valB = 0;
    d.stat = D.stat;
    d.icode = D.icode;
    d.ifun = D.ifun;
    d.valC = D.valC;
    d.Jump = D.Jump;
    d.predPC = D.predPC;
}
void Device::D2E() {
    if (D.control == C_STALL) return;
    E.stat = d.stat;
    E.icode = d.icode;
    E.ifun = d.ifun;
    E.valC = d.valC;
    E.valA = d.valA;
    E.valB = d.valB;
    E.dstE = d.dstE;
    E.dstM = d.dstM;
    E.srcA = d.srcA;
    E.srcB = d.srcB;
    E.Jump = d.Jump;
    E.predPC = d.predPC;
}
void Device::Execute() {
    if (E.control == C_STALL) return;
    if (E.control == C_BUBBLE) E.reset();
    uint8_t cnt = 0;
    uint8_t fun;
    uint64_t tepvalE;
    uint64_t aluA = SelectAluA();
    uint64_t aluB = SelectAluB();
    if (In(E.icode, I_OPQ, I_IOPQ, I_OPQN, I_IOPQN)) fun = E.ifun;
    else fun = ALU_ADD;
    bool need_cc = In(E.icode, I_OPQ, I_IOPQ, I_OPQN, I_IOPQN) && In(m.stat, S_AOK, S_BUB) && In(W.stat, S_AOK, S_BUB);
    e.valE = GetALUFunc(fun)(aluA, aluB);
    //乘除模运算一般要10个周期
    if (In(E.icode, I_OPQ, I_IOPQ, I_OPQN, I_IOPQN) && In(E.ifun, ALU_MULQ, ALU_DIVQ, ALU_REMQ) && !cnt) {
        cnt = 10;
        tepvalE = e.valE;
    }
    if (cnt > 1) {
        cnt--;
        E.Done = false;
        return;
    }
    else {
        if (cnt ==1) {
            cnt--;
            E.Done = true;
            e.valE = tepvalE;
        }
    }
    E.Done = true; //简单运算
    if (need_cc) {
        e.Ccode[C_ZF] = (e.valE == 0);
        e.Ccode[C_SF] = (((int64_t)e.valE) < 0);
        e.Ccode[C_OF] = false;
        int64_t x = (int64_t) aluA;
        int64_t y = (int64_t) aluB;
        int64_t z = (int64_t) e.valE;
        if (x <= 0 && y <= 0 && z > 0 && E.ifun == ALU_SUB) e.Ccode[C_OF] = true;
        if (x >= 0 && y >= 0 && z < 0 && E.ifun == ALU_ADD) e.Ccode[C_OF] = true;
        //e.Ccode[C_OF] = (((((int64_t)aluA < 0) ^ (E.ifun == ALU_SUB)) == (int64_t)aluB < 0) && ((int64_t)e.valE < 0 != (int64_t)aluB < 0));
    } 
    e.Cnd = CalcCond(e.Ccode);
    if (E.icode == I_CMOVEXX && !e.Cnd) e.dstE = R_NONE;
    else e.dstE = E.dstE;
    e.stat = E.stat;
    e.icode = E.icode;
    e.ifun = E.ifun;
    e.Jump = E.Jump;
    e.valA = E.valA;
    e.dstM = E.dstM;
    e.valC = E.valC;
    e.predPC = E.predPC;
}
void Device::E2M() {
    if (E.control == C_STALL) return;
    if (!IfExecuteDone()) return;
    M.stat = e.stat;
    M.icode = e.icode;
    M.ifun = e.ifun;
    M.Cnd = e.Cnd;
    M.valE = e.valE;
    M.valA = e.valA;
    M.dstE = e.dstE;
    M.dstM = e.dstM;
    M.Jump = e.Jump;
    M.predPC = e.predPC;
}
void Device::Memory() {
    if (M.control == C_STALL) return;
    if (M.control == C_BUBBLE) M.reset();
    m.stat = M.stat;
    uint64_t tep_addr;

}