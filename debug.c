#include <stdio.h>

#include "debug.h"
#include "value.h"

void disassembleChunk(Chunk* chunk, const char* name) {
    printf("== %s ==\n", name);

    for (int offset = 0; offset < chunk->count;) {
        offset = disassembleInstruction(chunk, offset);
    }
}

static int constantInstruction(const char* name, Chunk* chunk, int offset) {
    uint8_t constant = chunk->code[offset + 1];
    printf("%-16s %4d '", name, constant);
    printValue(chunk->constants.values[constant]);
    printf("'\n");
    
    return offset + 2;
}

static int constantLongInstruction(const char* name, Chunk* chunk, int offset) {
    int index = 0;
    index += (chunk->code[offset + 1]<<28);
    index += (chunk->code[offset + 2]<<24);
    index += (chunk->code[offset + 3]<<20);
    index += (chunk->code[offset + 4]<<16);
    index += (chunk->code[offset + 5]<<12);
    index += (chunk->code[offset + 6]<<8);
    index += (chunk->code[offset + 7]<<4);
    index += chunk->code[offset + 8];
    
    printf("%-16s %4d '", name, index);
    printValue(chunk->constants.values[index]);
    printf("'\n");
    
    return offset + 9;
}

static int simpleInstruction(const char* name, int offset) {
    printf("%s\n", name);
    return offset + 1;
}

int disassembleInstruction(Chunk* chunk, int offset) {
    printf("%04d ", offset);

    int currLine = getLine(&chunk->lines, offset);
    int lastLine = getLine(&chunk->lines, offset - 1);
    if (offset > 0 && currLine == lastLine) {
        printf("%8s ", "|");
    } else {
       printf("%8d ", currLine); 
    }

    uint8_t instruction = chunk->code[offset];
    switch(instruction) {
        case OP_CONSTANT:
            return constantInstruction("OP_CONSTANT", chunk, offset);
        case OP_CONSTANT_LONG:
            return constantLongInstruction("OP_CONSTANT_LONG", chunk, offset);
        case OP_NEGATE:
            return simpleInstruction("OP_NEGATE", offset);
        case OP_RETURN: 
            return simpleInstruction("OP_RETURN", offset);
        case OP_ADD:
            return simpleInstruction("OP_ADD", offset);
        case OP_SUBTRACT:
            return simpleInstruction("OP_SUBTRACT", offset);
        case OP_MULTIPLY:
            return simpleInstruction("OP_MULTIPLY", offset);
        case OP_DIVIDE:
            return simpleInstruction("OP_DIVIDE", offset);
        case OP_TERNARY:
            return simpleInstruction("OP_TERNARY", offset);
        default:
            printf("Unknown instruction %d\n", instruction);
            return offset + 1;
    }
}
