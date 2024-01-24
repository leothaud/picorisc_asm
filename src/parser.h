#ifndef PICORISC_PARSER_H__
#define PICORISC_PARSER_H__

#include <stdio.h>
#include <stdint.h>
#include "lexer.h"

typedef struct
{
  uint8_t imm: 8;
  uint8_t rs: 2;
  uint8_t rd: 2;
  uint8_t opcode: 4;
} instruction_t;

void parse(FILE* fp, tokens_t tokens);


#endif
