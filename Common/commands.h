#ifndef commands_h
#define commands_h

//Codes of commands for assembling
enum AsmCom_t {
    PUSH =      1,
    POP =       2,
    SUM =       3,
    SUB =       4,
    MUL =       5,
    DIV =       6,
    DUMP =      7,
    HLT =       8,
    PUSHR =     9,
    POPR =      10,
    JMP =       11,
    JB =        12,
    JBE =       13,
    JA =        14,
    JAE =       15,
    JE =        16,
    JNE =       17,
    CALL =      18,
    RET =       19,
    LABEL =     25,
    ERROR_COM = 99
};
//Codes of registers
enum AsmReg_t {
    RAX =       1,
    RBX =       2,
    RCX =       3,
    RDX =       4,
    ERROR_REG = 99
};

static const int kRegistersAmount = 4;
static const int kReturnAddressesStackSize = 10;
static const int kBufferMultiplier = 2;

#endif //commands_h
