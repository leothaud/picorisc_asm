#ifndef PICORISC_LEXER_H__
#define PICORISC_LEXER_H__

#include <stdint.h>
#include <stdio.h>

typedef struct
{

  enum {
    IMM = 0, //
    ADD = 1, //
    ADDI = 2, 
    SUB = 3, //
    XOR = 4, //
    OR = 5, //
    AND = 6, //
    SHIFT = 7,
    ORI = 8,
    ORHI = 9,
    LOAD = 10, //
    STORE = 11, //
    BLT = 12,
    BE = 13,
    R0 = 14, //
    R1 = 15,//
    R2 = 16,//
    PC = 17,//
    IMMEDIATE
  } type;
  uint16_t imm;
} token_t;

typedef struct
{
  token_t *tokens;
  uint32_t n_token;
} tokens_t;

tokens_t tokenize(FILE *f);

#endif
