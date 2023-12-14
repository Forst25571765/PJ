#include <cstdint>
#include <cstring>
#include <functional>
#include <queue>
#include <stack>
#include <string>

#include "Map.h"
#include "Pipline.h"
const int MEM_SIZE = 8*1024;
const int REG_SIZE = 16;

class Device {
    public:
    F_Reg F;
    F_Wire f;
    D_Reg D;
    D_Wire d;
    E_Reg E;
    E_Wire e;
    M_Reg M;
    M_Wire m;
    W_Reg W;
    uint8_t Stat;
    uint64_t Reg[REG_SIZE] = {0};
    uint8_t Mem[MEM_SIZE] = {0};
    bool CCODE[3] = {true, false, false};
    Device (std::string code);
    uint8_t ReadHigh4Bits(uint64_t pos);
    uint8_t ReadLow4Bits(uint64_t pos);
    uint64_t Read1Bytes(uint64_t pos);
    uint64_t Read2Bytes(uint64_t pos);
    uint64_t Read4Bytes(uint64_t pos);
    uint64_t Read8Bytes(uint64_t pos);
    void Write1Bytes(uint64_t pos, uint64_t val);
    void Write2Bytes(uint64_t pos, uint64_t val);
    void Write4Bytes(uint64_t pos, uint64_t val);
    void Write8Bytes(uint64_t pos, uint64_t val);
    void Fetch();
    void Decode();
    void Execute();
    void Memory();
    void Writeback();
    void F2D();
    void D2E();
    void E2M();
    void M2W();
    uint64_t GetPC();
    void SetFcontrol();
    void SetDcontrol();
    void SetEcontrol();
    void SetMcontrol();
    void SetCC();
    void SetDSrcA();
    void SetDSrcB();

    private:
    uint64_t addr_end = 0;
    uint64_t badnumber = 0;
    uint64_t addr;
    uint64_t GetFPredPC();
    bool IfAddrReadable();
    bool IfAddrWriteable();
    bool IfAddrExecutable();
    bool IfInstValid();
    bool IfExecuteDone();
    bool CalcCond(bool CCODE[3]);
    bool IfLoadUseH();
    bool IfMispredicted();
    bool IfRet();
    std::function<uint64_t(uint64_t, uint64_t)> GetALUFunc(uint8_t ifun);
    uint8_t SelectSrcA() const;
    uint8_t SelectSrcB() const;
    uint8_t SelectDstE();
    uint8_t SelectDstM() const;
    uint64_t SelectPC();
    uint8_t SelectFStat() const;
    uint64_t SelectAluA() const;
    uint64_t SelectAluB() const;
    uint64_t ReadReg(uint8_t reg, uint8_t ifun);
    void WriteReg(uint8_t reg, uint8_t ifun, uint64_t val);
    void ReadMemToValM(uint64_t mem_addr);
    void WriteMemFromValA(uint64_t mem_addr);
};