#include <stdio.h>

#include "common.h"
#include "chunk.h"
#include "debug.h"

int main(int argc, const char* argv[]) {
    Chunk chunk;
    initChunk(&chunk);

    /*int constant = addConstant(&chunk, 1.2);
    writeChunk(&chunk, OP_CONSTANT, 1);
    writeChunk(&chunk, constant, 1);*/

    for (int i = 0; i < 256; i++) {
        writeConstant(&chunk, 1.2, 1);
    }

    writeConstant(&chunk, 2.3, 2);

    //writeChunk(&chunk, OP_RETURN, 4);

    disassembleChunk(&chunk, "test chunk");
    freeChunk(&chunk);

    return 0;
}
