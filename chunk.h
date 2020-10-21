#ifndef clox_chunk_h
#define clox_chunk_h

#include "common.h"
#include "value.h"

typedef enum {
    OP_CONSTANT,
    OP_CONSTANT_LONG,
    OP_RETURN
} OpCode;

typedef struct {
    int count;
    int capacity;
    int* lines;
} LineArray;

typedef struct {
    int count;
    int lineCount;
    int capacity;
    uint8_t* code;
    LineArray lines;
    ValueArray constants;
} Chunk;


void initLineArray(LineArray* array);
void writeLineArray(LineArray* array, int line, int offset);
void freeLineArray(LineArray* array);
int getLine(LineArray* array, int offset);
void initChunk(Chunk* chunk);
void writeChunk(Chunk* chunk, uint8_t byte, int line);
void freeChunk(Chunk* chunk);
int addConstant(Chunk* chunk, Value value);
void writeConstant(Chunk* chunk, Value value, int line);

#endif