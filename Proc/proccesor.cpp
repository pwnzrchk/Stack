#include "proccesor.h"

#define SwagAdr (refSpu -> Swag)     //почему они бесполезны?
#define MakeErr(code)  code##_ERR   //ПОЧЕМУ?
#define REPLACE(command) (error = Swag##command(&(SwagAdr))) != NO_ERRS

//=================================================================================================================================================

#define CASE_TEMPLATE_ERROR_HANDLER(error_code)                 \
        fprintf(stderr, #error_code " error in processing\n");  \
        break;

//=================================================================================================================================================

#define REPLACEMENT(comand)                                         \
        if (REPLACE(comand)) {                                      \
            SwagDump(&(SwagAdr), error);                            \
            fprintf(stderr, #comand " error in processing\n");      \
            return MakeErr(comand);                                 \
        }                                                           \
        break;

//=================================================================================================================================================

#define JMP_TEMPLATE(operation)                                                                             \
        assert(refSpu);                                                                                     \
        if (index < 1 || index >= refSpu->ByteCodeBuf[0]) {                                                 \
            return INCORECT_SIZE;                                                                           \
        }                                                                                                   \
        swagElem_t compared1, compared2 = 0;                                                                \
        if ((SwagPop(&SwagAdr, &compared1) != NO_ERRS) || (SwagPop(&SwagAdr, &compared2) != NO_ERRS)) {     \
            return JMP_ERR;                                                                                 \
        }                                                                                                   \
        if (compared1 operation compared2) refSpu->pc = index;                                          \
        return WITHOUT_ERRS;

//=================================================================================================================================================

#define JMP_TEMP_CASE(jmp_var)                                                                              \
        if ((Proc_err = jmp_var(refSpu, refSpu -> ByteCodeBuf[++(refSpu -> pc)])) != WITHOUT_ERRS) {        \
            return JMP_ERR;                                                                                 \
        }                                                                                                   \
        break;

//=================================================================================================================================================
// FIXME - asserts
ProcErr_t Proccesing(spu_t* refSpu) {
    assert((refSpu != NULL) && (refSpu->ByteCodeBuf != NULL) && (refSpu->regs != NULL) && (refSpu->Swag.data != NULL));

    if (refSpu -> ByteCodeBuf == NULL)  return  NULL_PTR_ERR;
    if ((refSpu -> regs) == NULL)       return  NULL_PTR_ERR;
    if (refSpu -> Swag.data == NULL)    return  NULL_PTR_ERR;

    bool       FlagOfExit     = false;
    swagErr_t  error          = NO_ERRS;
    ProcErr_t  Proc_err       = WITHOUT_ERRS;
    swagElem_t BC_Buffer_Size = refSpu->size_of_bytecode_buffer;

    for(refSpu -> pc = 0; refSpu -> pc < BC_Buffer_Size && !FlagOfExit;) {
        const swagElem_t comand = refSpu -> ByteCodeBuf[refSpu ->pc];
        swagElem_t arg = 0;

        //DEBUG -
        fprintf(stderr, "%zu Comand in buffer-byte-code is - [%d]\n", refSpu->pc, refSpu -> ByteCodeBuf[(refSpu ->pc)]);
        if (refSpu->pc + 1 == refSpu->size_of_bytecode_buffer) return kErrorSize;

        if (!comand) {
            fprintf(stderr, "Null comand pointer in %zu ell in ARR\n", refSpu -> ByteCodeBuf[refSpu ->pc] + 1);
            return NULL_PTR_ERR;
        }
        // ErrorHandler - функция для обработки ошибок (принимает номер ошибки и выводит сообщение)
        switch(comand) {
            case PUSH:
                if ((error = SwagPush(&(SwagAdr), refSpu -> ByteCodeBuf[++(refSpu ->pc)])) != NO_ERRS) {
                    SwagDump(&(SwagAdr), error);
                    return PUSH_ERR;
                }
                break;

            case POP:
                if ((error = SwagPop(&(refSpu -> Swag), &arg)) != NO_ERRS) {
                    SwagDump(&(refSpu -> Swag), error);
                    return POP_ERR;
                }
                break;

            case SUM:
                REPLACEMENT(Sumn);

            case SUB:
                REPLACEMENT(Sub);

            case MUL:
                REPLACEMENT(Mul);

            case DIV:
                REPLACEMENT(Div);
            //REVIEW - same, its clearlier isnt it?
            case DUMP:
                if (REPLACE(Verify)) {
                    SwagDump(&(SwagAdr), error);
                    return DUMP_ERR;
                } else if (error == NO_ERRS) {
                    SwagDump(&(SwagAdr), NO_ERRS);
                }
                break;

            case HLT:
                FlagOfExit = true;
                return WITHOUT_ERRS;

            case PUSHR:
                if ((Proc_err = RegPush(refSpu, refSpu -> ByteCodeBuf[++(refSpu -> pc)])) != WITHOUT_ERRS) {
                    return Proc_err;
                }
                break;

            case POPR:
                if ((Proc_err = RegPop(refSpu, refSpu -> ByteCodeBuf[++(refSpu -> pc)])) != WITHOUT_ERRS) {
                    return Proc_err;
                }
                break;

            case JMP:   JMP_TEMP_CASE(Jmp);

            case JB:    JMP_TEMP_CASE(Jb);

            case JBE:   JMP_TEMP_CASE(Jbe);

            case JA:    JMP_TEMP_CASE(Ja);

            case JAE:   JMP_TEMP_CASE(Jae);

            case JE:    JMP_TEMP_CASE(Je);

            case JNE:   JMP_TEMP_CASE(Jne);

            case CALL:
                if (CallFunction(refSpu, refSpu -> ByteCodeBuf[++(refSpu ->pc)]) != WITHOUT_ERRS) return kCallError;
                break;

            case RET:
                if (RetFunction(refSpu) != WITHOUT_ERRS) return kRetError;
                break;

            case PUSHM:
                if (PushMemory(refSpu, refSpu -> ByteCodeBuf[++(refSpu ->pc)]) != WITHOUT_ERRS) return kPushMemoryError;
                break;

            case POPM:
                if (PopMemory(refSpu, refSpu -> ByteCodeBuf[++(refSpu ->pc)]) != WITHOUT_ERRS) return kPopMemoryError;
                break;

            case DRAW:
                DrawMemory(refSpu);
                break;


            case ERROR_COM:
            default:
                return UNKNW_CMD;

        }
    }
    return WITHOUT_ERRS;
}

//=================================================================================================================================================

fileFunErr_t BCFileToArr(fileInfo* refBCFile, swagElem_t** refArr) {
    assert(refArr != NULL);

    size_t size_of_memory = 0;
    if (MemoryCalculator(refBCFile, &size_of_memory) != WITHOUT_ERRS) return NULL_PTR_PLUM;
    //DEBUG fprintf(stderr, "Size of memory : %zu\n", size_of_memory);
    *refArr = (swagElem_t*)calloc((size_t)(size_of_memory + 1), sizeof(**refArr));
    if (*refArr == NULL) {
        fprintf(stderr, "Calloc error for BC_buffer\n");
        return NULL_PTR_PLUM;
    }

    (*refArr)[0] = size_of_memory;

    size_t arrIndex = 0;
    size_t strings_amount = refBCFile->str_count - 1;
    for(size_t j = 0; j < strings_amount && arrIndex < size_of_memory + 1; j++) {

        const char* workline = refBCFile -> pointerBuffer[j];
        if (!workline) {
            fprintf(stderr, "Null line pointer at %zu\n", j+1);
            break;
        }

        swagElem_t arg = 0;
        swagElem_t comand = 0;
        int scanned = 0;

        if ((scanned = sscanf(workline, "%zd %zd", &comand, &arg)) <= 0) {
            fprintf(stderr, "Error scanf BYTECODE file\nString No %zu\n", j + 1);
            break;
        } else if (scanned == 1) {
            (*refArr)[++arrIndex] = comand;
        } else if (scanned == 2) {
            (*refArr)[++arrIndex] = comand;
            (*refArr)[++arrIndex] = arg;
        } else {
            fprintf(stderr, "Wrong line No%zu\n", j+1);
            break;
        }
    }
    return NO_PLUM_ERR;
}

//=================================================================================================================================================

ProcErr_t MemoryCalculator(fileInfo* reference_byte_code_file, size_t* calculated_size) {
    assert(reference_byte_code_file != NULL && calculated_size != NULL);

    Plenumation(reference_byte_code_file);
    Distributor(reference_byte_code_file);

    size_t strings_amount = (reference_byte_code_file->str_count - 1);
    size_t memory_size =    0;

    for (size_t i = 0; i < strings_amount; i++) {
        const char* workline = reference_byte_code_file->pointerBuffer[i];
        if (!workline) {
            // fprintf(stderr, "Null line pointer at %zu\n", i+1);
            return NULL_PTR_ERR;
        }
        int cmd = 0;
        sscanf(workline, "%d", &cmd);
        if (cmd == ERROR_COM) return UNKNW_CMD;
        switch (cmd) {
            case POP:   case SUM:   case SUB: case MUL:
            case DIV:   case DUMP:  case HLT: case RET:
            case DRAW:
                memory_size += 1;
                break;
            default:
                memory_size += 2;
                break;
        }
    }

    *calculated_size = memory_size;

    return WITHOUT_ERRS;
}

//=================================================================================================================================================

ProcErr_t SpuConstructor(spu_t* refSpu, fileInfo* binary_file) {
    assert(binary_file != NULL && refSpu != NULL);

    swag_t SpuSwag = {};
    SwagInit(&SpuSwag, kInitialSize);
    refSpu -> Swag = SpuSwag;

    for (int i = 0; i < kRegistersAmount; i++) {
        refSpu -> regs[i] = 0;
    }
    refSpu -> pc = 0;

    swag_t spu_return_stack = {};
    SwagInit(&spu_return_stack, kReturnAddressesStackSize);
    refSpu->stack_return_addresses = spu_return_stack;

    FILE* binary = fopen(binary_file->file_name, "rb");
    if (binary == NULL) return NULL_PTR_ERR;

    long bytes_count = FileSize(binary);
    if (bytes_count == -1) return kErrorSize;
    size_t memory_size = (size_t)bytes_count/sizeof(int);

    //DEBUG
    fprintf(stderr, "memory size - [%zu]\n", memory_size);

    refSpu->ByteCodeBuf = (int*)calloc(memory_size, sizeof(int));

    ssize_t read_count = (ssize_t)fread(refSpu->ByteCodeBuf, sizeof(int), memory_size, binary);
    fclose(binary);
    if (bytes_count % sizeof(int) != 0) return INCORECT_SIZE;
    refSpu->size_of_bytecode_buffer = memory_size;

    //DEBUG
    for (int i = 0; i < memory_size; i++) {
        fprintf(stderr, "%d element in buffer is - [%d]\n", i, refSpu->ByteCodeBuf[i]);
    }

    refSpu->ram = (swagElem_t*) calloc(kSizeOfRam, sizeof(swagElem_t));
    return WITHOUT_ERRS;
}

//=================================================================================================================================================

ProcErr_t RegPush(spu_t* refSpu, size_t i) {
    assert(i < 5 && i > 0);
    assert(refSpu != NULL);

    if (!(i < 5 && i > 0)) return INCORECT_SIZE;

    swagElem_t temp = 0;
    swagErr_t ErrRegPush = SwagPop(&(refSpu -> Swag), &temp);
    if (ErrRegPush != NO_ERRS) {
        return REG_PUSH_ERR;
    }
    refSpu -> regs[i - 1] = temp;
    return WITHOUT_ERRS;
}

//=================================================================================================================================================

ProcErr_t RegPop (spu_t* refSpu, size_t i) {
    assert(refSpu != NULL);
    if (!(i < 5 && i > 0)) return INCORECT_SIZE;

    swagErr_t ErrPopReg = SwagPush(&(refSpu -> Swag), refSpu -> regs[i - 1]);
    if (ErrPopReg != NO_ERRS) return REG_POP_ERR;

    return WITHOUT_ERRS;

}

//=================================================================================================================================================

ProcErr_t Jmp(spu_t* refSpu, size_t index) {
    assert(refSpu != NULL);

    if (index < 1 || index >= refSpu->ByteCodeBuf[0]) {
        return INCORECT_SIZE;
    }
    refSpu ->pc = index;
    return WITHOUT_ERRS;
}

//=================================================================================================================================================

ProcErr_t Jbe(spu_t* refSpu, size_t index) {
    JMP_TEMPLATE(<=);
}

ProcErr_t Jb(spu_t* refSpu, size_t index) {
    JMP_TEMPLATE(<);
}

ProcErr_t Jae(spu_t* refSpu, size_t index) {
    JMP_TEMPLATE(>=);
}

ProcErr_t Ja(spu_t* refSpu, size_t index) {
    JMP_TEMPLATE(>);
}

ProcErr_t Je(spu_t* refSpu, size_t index) {
    JMP_TEMPLATE(==);
}

ProcErr_t Jne(spu_t* refSpu, size_t index) {
    JMP_TEMPLATE(!=);
}

//=================================================================================================================================================

void ErrorHandler(int error_code) {
    switch(error_code) {
        case NULL_PTR_ERR:
            CASE_TEMPLATE_ERROR_HANDLER(Null_pointer)

        case REG_POP_ERR:
            CASE_TEMPLATE_ERROR_HANDLER(Pop_register)

        case REG_PUSH_ERR:
            CASE_TEMPLATE_ERROR_HANDLER(Push_register)

        case PUSH_ERR:
            CASE_TEMPLATE_ERROR_HANDLER(Push)

        case POP_ERR:
            CASE_TEMPLATE_ERROR_HANDLER(Pop)

        case Sumn_ERR:
            CASE_TEMPLATE_ERROR_HANDLER(Sum)

        case Sub_ERR:
            CASE_TEMPLATE_ERROR_HANDLER(Sub)

        case Mul_ERR:
            CASE_TEMPLATE_ERROR_HANDLER(Mul)

        case Div_ERR:
            CASE_TEMPLATE_ERROR_HANDLER(Div)

        case DUMP_ERR:
            CASE_TEMPLATE_ERROR_HANDLER(Dumping)

        case JMP_ERR:
            CASE_TEMPLATE_ERROR_HANDLER(Jump)

        case INCORECT_SIZE:
            CASE_TEMPLATE_ERROR_HANDLER(Size)

        case UNKNW_CMD:
            CASE_TEMPLATE_ERROR_HANDLER(Comand)

        case kRetError:
            CASE_TEMPLATE_ERROR_HANDLER(ReturnSwag)

        case kCallError:
            CASE_TEMPLATE_ERROR_HANDLER(Call)

        default:
            fprintf(stderr, "ErrorHandler mistake");
    }
}

//=================================================================================================================================================
//Ассерты
ProcErr_t CallFunction(spu_t* spu, size_t new_pc) {
    assert(spu);

    if (SwagPush(&spu->stack_return_addresses, spu->pc + 1) != NO_ERRS) return kCallError;
    spu->pc = new_pc;
    return WITHOUT_ERRS;
}

//=================================================================================================================================================
//Ассерты
ProcErr_t RetFunction(spu_t* spu) {
    assert(spu);

    size_t return_pc = 0;
    SwagPop(&spu->stack_return_addresses, &return_pc);
    if (return_pc == 0) return kRetError;
    spu->pc = return_pc;
    return WITHOUT_ERRS;
}

//=================================================================================================================================================

ProcErr_t PushMemory(spu_t* spu, swagElem_t register_number) {
    assert(spu);

    swagElem_t inserted_value = spu->ram[spu->regs[register_number]];
    if (SwagPush(&(spu->Swag), inserted_value) != NO_ERRS) return kPushMemoryError;
    return WITHOUT_ERRS;
}

//=================================================================================================================================================

ProcErr_t PopMemory(spu_t* spu, swagElem_t register_number) {
    assert(spu);

    swagElem_t inserted_value = (swagElem_t)kTrashValue;
    if ((SwagPop(&(spu->Swag), &inserted_value) != NO_ERRS) || inserted_value == (swagElem_t)kTrashValue) return kPopMemoryError;
    spu->ram[spu->regs[register_number]] = inserted_value;
    return WITHOUT_ERRS;
}

//=================================================================================================================================================

ProcErr_t DrawMemory(spu_t* spu) {
    assert(spu);

    for (int i = 0; i < kSizeOfRam; i++) {
        printf("%c", spu->ram[i]);
        if (i%10 == 9) printf("\n"); // FIXME - 10?
    }
    return WITHOUT_ERRS;
}

//=================================================================================================================================================

ProcErr_t SpuDestroyer(spu_t* spu) {
    assert(spu);

    spu->pc = (size_t)kTrashValue;
    for(int i = 0; i < kRegistersAmount; i++) {
        spu->regs[i] = (swagElem_t)kTrashValue;
    }
    if (SwagDestroy(&(spu->Swag)) != NO_ERRS)                   return kSpuDestroyError;
    if (SwagDestroy(&(spu->stack_return_addresses)) != NO_ERRS) return kSpuDestroyError;
    free(spu->ByteCodeBuf);
    free(spu->ram);
    return WITHOUT_ERRS;
}

//=================================================================================================================================================

#undef SwagAdr
#undef MakeErr
#undef REPLACE
#undef REPLACEMENT
#undef JMP_TEMPLATE
#undef JMP_TEMP_CASE

// ProcErr_t SpuDump(spu_t* refSpu) {
    // printf();
// }
//
