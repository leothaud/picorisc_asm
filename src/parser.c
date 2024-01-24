#include "parser.h"
#include <stdlib.h>
#include <arpa/inet.h>

void print_binary(uint16_t x)
{
  for (int i = 0; i < 16; ++i)
  {
    printf("%d", (x & 1) ? 1 : 0);
    x = x >> 1;
  }
  printf("\n");
}


void parse(FILE *fp, tokens_t tokens)
{
  instruction_t instr;
  uint32_t position = 0;
  while (position < tokens.n_token)
  {
    instr.opcode = tokens.tokens[position].type;
    switch (tokens.tokens[position].type)
    {
    case IMM:
    case ORI:
    case ORHI:
      ++position;
      if ((position >= tokens.n_token) ||
	  ((tokens.tokens[position].type != R0) &&
	   (tokens.tokens[position].type != R1) &&
	   (tokens.tokens[position].type != R2) &&
	   (tokens.tokens[position].type != R3)))
      {
	fprintf(stderr, "Unexpected token.");
	exit(1);
      }
    
      instr.rs = 0;
      instr.rd = tokens.tokens[position].type - R0;
      ++position;
      if ((position >= tokens.n_token) ||
	  (tokens.tokens[position].type != IMMEDIATE))
      {
	fprintf(stderr, "Unexpected token.");
	exit(1);
      }
      instr.imm = tokens.tokens[position].imm;
      break;

    case ADD:
    case SUB:
    case XOR:
    case OR:
    case AND:
    case LOAD:
    case STORE:
      ++position;
      if ((position >= tokens.n_token) ||
	  ((tokens.tokens[position].type != R0) &&
	   (tokens.tokens[position].type != R1) &&
	   (tokens.tokens[position].type != R2) &&
	   (tokens.tokens[position].type != R3)))
      {
	fprintf(stderr, "Unexpected token.");
	exit(1);
      }
    
      instr.rd = tokens.tokens[position].type - R0;
      ++position;
      if ((position >= tokens.n_token) ||
	  ((tokens.tokens[position].type != R0) &&
	   (tokens.tokens[position].type != R1) &&
	   (tokens.tokens[position].type != R2) &&
	   (tokens.tokens[position].type != R3)))
      {
	fprintf(stderr, "Unexpected token.");
	exit(1);
      }
      instr.rs = tokens.tokens[position].type - R0;
      instr.imm = 0;
      fwrite(&instr, sizeof(instruction_t), 1, fp);
      break;

    case ADDI:
    case BLT:
    case BE:
      ++position;
      if ((position >= tokens.n_token) ||
	  ((tokens.tokens[position].type != R0) &&
	   (tokens.tokens[position].type != R1) &&
	   (tokens.tokens[position].type != R2) &&
	   (tokens.tokens[position].type != R3)))
      {
	fprintf(stderr, "Unexpected token.");
	exit(1);
      }
    
      instr.rd = tokens.tokens[position].type - R0;
      ++position;
      if ((position >= tokens.n_token) ||
	  ((tokens.tokens[position].type != R0) &&
	   (tokens.tokens[position].type != R1) &&
	   (tokens.tokens[position].type != R2) &&
	   (tokens.tokens[position].type != R3)))
      {
	fprintf(stderr, "Unexpected token.");
	exit(1);
      }
      instr.rs = tokens.tokens[position].type - R0;
      ++position;
      if ((position >= tokens.n_token) ||
	  (tokens.tokens[position].type != IMMEDIATE))
      {
	fprintf(stderr, "Unexpected token.");
	exit(1);
      }
      instr.imm = tokens.tokens[position].imm;
      break;

    case SHIFT:
      ++position;
      if ((position >= tokens.n_token) ||
	  ((tokens.tokens[position].type != R0) &&
	   (tokens.tokens[position].type != R1) &&
	   (tokens.tokens[position].type != R2) &&
	   (tokens.tokens[position].type != R3)))
      {
	fprintf(stderr, "Unexpected token.");
	exit(1);
      }
      instr.rd = tokens.tokens[position].type - R0;
      instr.rs = 0;
      instr.imm = 0;
      break;

    default:
      fprintf(stderr, "Unexpected token.");
      exit(1);
    }
    print_binary(* (uint16_t*)&instr);
    fwrite(&instr, sizeof(uint16_t), 1, fp);
    ++position;
  }
}