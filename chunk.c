#include <stdlib.h>
#include <stdio.h>

#include "chunk.h"
#include "memory.h"

void initLineArray(LineArray* array) {
    array->capacity = 0;
    array->count = 0;
    array->lines = NULL;
}

void writeLineArray(LineArray* array, int line, int offset) {
    int currLine = -1;
    if (array->capacity > 0) {
        currLine = array->lines[array->count];
    }

    if (currLine != line) {
        if (array->capacity < array->count + 3) {
            int oldCapacity = array->capacity;
            array->capacity = GROW_CAPACITY(oldCapacity);
            array->lines = GROW_ARRAY(int, array->lines, oldCapacity, array->capacity);
        }

        if (currLine != -1) {
            array->count+=2;        
        }
        
        array->lines[array->count] = line;
    } 

    array->lines[array->count+1] = offset;
}

void freeLineArray(LineArray* array) {
    FREE_ARRAY(int, array->lines, array->capacity);
    initLineArray(array);
}

int getLine(LineArray* array, int offset) {
    if (array->capacity == 0) {
        return -1;
    }

    int index = 0;
    int maxLineOffset = array->lines[index + 1];
    while (offset > maxLineOffset) {
        index+=2;
        maxLineOffset = array->lines[index + 1];
    }

    return array->lines[index];
} 

void initChunk(Chunk *chunk) {
    chunk->count = 0;
    chunk->lineCount = 0;
    chunk->capacity = 0;
    chunk->code = NULL;
    initLineArray(&chunk->lines);
    initValueArray(&chunk->constants);
}

void writeChunk(Chunk* chunk, uint8_t byte, int line) {
    if (chunk->capacity < chunk->count + 1) {
        int oldCapacity = chunk->capacity;
        chunk->capacity = GROW_CAPACITY(oldCapacity);
        chunk->code = GROW_ARRAY(uint8_t, chunk->code, oldCapacity, chunk->capacity);
    }

    chunk->code[chunk->count] = byte;
    writeLineArray(&chunk->lines, line, chunk->count);
    chunk->count++;
}

void freeChunk(Chunk* chunk) {
    FREE_ARRAY(uint8_t, chunk->code, chunk->capacity);
    freeLineArray(&chunk->lines);
    freeValueArray(&chunk->constants);
    initChunk(chunk);
}

int addConstant(Chunk* chunk, Value value) {
    writeValueArray(&chunk->constants, value);
    return chunk->constants.count - 1;
}

int writeConstant(Chunk* chunk, Value value, int line) {
    int constantIndex = addConstant(chunk, value);

    OpCode opCode = OP_CONSTANT;
    if (constantIndex >= 256) {
        opCode = OP_CONSTANT_LONG;
    }

    writeChunk(chunk, opCode, line);

    if (opCode == OP_CONSTANT_LONG) {
        writeChunk(chunk, (constantIndex&0xF0000000)>>28, line);
        writeChunk(chunk, (constantIndex&0xF000000)>>24, line);
        writeChunk(chunk, (constantIndex&0xF00000)>>20, line);
        writeChunk(chunk, (constantIndex&0xF0000)>>16, line);
        writeChunk(chunk, (constantIndex&0xF000)>>12, line);
        writeChunk(chunk, (constantIndex&0xF00)>>8, line);
        writeChunk(chunk, (constantIndex&0xF0)>>4, line);
        writeChunk(chunk, constantIndex&0xF, line);
    } else {
        writeChunk(chunk, constantIndex, line);
    }

    return constantIndex;
}
