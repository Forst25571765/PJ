#include <cstdint>
//Instructions
const uint8_t I_HALT = 0;
const uint8_t I_NOP = 1;
const uint8_t I_CMOVEXX = 2;
const uint8_t I_IRMOVEQ = 3;
const uint8_t I_RMMOVEQ = 4;
const uint8_t I_MRMOVQ = 5;
const uint8_t I_OPQ = 6;
const uint8_t I_JXX = 7;
const uint8_t I_CALL = 8;
const uint8_t I_RET = 9;
const uint8_t I_PUSHQ = 0XA;
const uint8_t I_POPQ = 0XB;
const uint8_t I_IOPQ = 0XC;
const uint8_t I_OPQN = 0XD;
const uint8_t I_IOPQN = 0XE;
//Register
const uint8_t R_RAX = 0;
const uint8_t R_RCX = 1;
const uint8_t R_RDX = 2;
const uint8_t R_RBX = 3;
const uint8_t R_RSP = 4;
const uint8_t R_RBP = 5;
const uint8_t R_RSI = 6;
const uint8_t R_RDI = 7;
const uint8_t R_R8 = 8;
const uint8_t R_R9 = 9;
const uint8_t R_R10 = 10;
const uint8_t R_R11 = 11;
const uint8_t R_R12 = 12;
const uint8_t R_R13 = 13;
const uint8_t R_R14 = 14;
const uint8_t R_NONE = 15;
//Condition Code
const uint8_t C_ZF = 0;
const uint8_t C_SF = 1;
const uint8_t C_OF = 2;
//Branch
const uint8_t B_ALWAYS = 0;
const uint8_t B_LE = 1;
const uint8_t B_L = 2;
const uint8_t B_E = 3;
const uint8_t B_NE = 4;
const uint8_t B_GE = 5;
const uint8_t B_G = 6;
//Bsize
const uint8_t R_QU = 0;
const uint8_t R_LU = 1;
const uint8_t R_WU = 2;
const uint8_t R_BU = 3;
const uint8_t R_LS = 4;
const uint8_t R_WS = 5;
const uint8_t R_BS = 6;
//State
const uint8_t S_AOK = 1;
const uint8_t S_HLT = 2;
const uint8_t S_ADR = 3;
const uint8_t S_INS = 4;
const uint8_t S_BUB = 5;
//Control
const uint8_t C_NORMAL = 0;
const uint8_t C_BUBBLE = 1;
const uint8_t C_STALL = 2;