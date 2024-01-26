#include "lexer.h"
#include <stdlib.h>

#define  BLOCK_SIZE 128

#define CHECK_EOF(eof)				\
  if (eof == EOF)				\
  {						\
    fprintf(stderr, "Unexpected end of file.");	\
    exit(1);					\
  }

#ifdef DEBUG

#define ADD_TOKEN(TKN, IMM)					\
  if (tokens.n_token == tokens_capacity * BLOCK_SIZE)		\
  {								\
    ++tokens_capacity;						\
    tokens.tokens = realloc(tokens.tokens,			\
			    (tokens_capacity * BLOCK_SIZE)	\
			    * sizeof(token_t));			\
  }								\
  tokens.tokens[tokens.n_token].type = TKN;			\
  tokens.tokens[tokens.n_token].imm = IMM;			\
  ++tokens.n_token;						\
  printf("Adding token %d\n", TKN)

#else

#define ADD_TOKEN(TKN, IMM)					\
  if (tokens.n_token == tokens_capacity * BLOCK_SIZE)		\
  {								\
    ++tokens_capacity;						\
    tokens.tokens = realloc(tokens.tokens,			\
			    (tokens_capacity * BLOCK_SIZE)	\
			    * sizeof(token_t));			\
  }								\
  tokens.tokens[tokens.n_token].type = TKN;			\
  tokens.tokens[tokens.n_token].imm = IMM;			\
  ++tokens.n_token

#endif

tokens_t tokenize(FILE *fp)
{
  tokens_t tokens;
  tokens.n_token = 0;
  uint32_t tokens_capacity = 1;
  tokens.tokens = malloc((tokens_capacity * BLOCK_SIZE) * sizeof(token_t));
  char c;

  uint32_t column = 0, line = 1;
  
  while ((c = fgetc(fp)) != EOF)
  {
  loopStart:
    c = ((c >= 'a') && (c <= 'z')) ? c + 'A' - 'a' : c;
    if (c == '\n')
    {
      ++line;
      column = 0;
    }
    else
    {
      ++column;
    }
    
    switch (c)
    {
      
    case ' ':
    case '\t':
    case '\n':
    case '\r':
      continue;

    case 'R':
    {
      int8_t eof = (c = fgetc(fp));
      CHECK_EOF(eof);
      c = ((c >= 'a') && (c <= 'z')) ? c + 'A' - 'a' : c;
      switch (c)
      {
      case '0':
        ADD_TOKEN(R0,0);
	break;
      case '1':
	ADD_TOKEN(R1,0);
	break;
      case '2':
	ADD_TOKEN(R2, 0);
	break;
      case '3':
	ADD_TOKEN(R3, 0);
	break;
      default:
	fprintf(stderr, "Unexpected character at line %d column %d. (1)", line, column);
	exit(1);
      }
      ++column;
      break;
    }

    case 'I':
    {
      int8_t eof = (c = fgetc(fp));
      CHECK_EOF(eof);
      c = ((c >= 'a') && (c <= 'z')) ? c + 'A' - 'a' : c;
      ++column;  
      if (c != 'M')
      {
	fprintf(stderr, "Unexpected character at line %d column %d. (2)", line, column);
	exit(1);
      }
      eof = (c = fgetc(fp));
      CHECK_EOF(eof);
      c = ((c >= 'a') && (c <= 'z')) ? c + 'A' - 'a' : c;
      ++column;  
      if (c != 'M')
      {
	fprintf(stderr, "Unexpected character at line %d column %d. (3)", line, column);
	exit(1);
      }
      ADD_TOKEN(IMM, 0);
      break;
    }

    case 'A':
    {
      int8_t eof = (c = fgetc(fp));
      CHECK_EOF(eof);
      c = ((c >= 'a') && (c <= 'z')) ? c + 'A' - 'a' : c;
      ++column;
      switch (c)
      {
      case 'D':
	eof = (c = fgetc(fp));
	CHECK_EOF(eof);
	c = ((c >= 'a') && (c <= 'z')) ? c + 'A' - 'a' : c;
	++column;  
	if (c != 'D')
	{
	  fprintf(stderr, "Unexpected character at line %d column %d. (4)", line, column);
	  exit(1);
	}
	eof = (c = fgetc(fp));
	CHECK_EOF(eof);
	c = ((c >= 'a') && (c <= 'z')) ? c + 'A' - 'a' : c;
	if (c == 'I')
	{
	  ++column;
	  ADD_TOKEN(ADDI, 0);
	}
	else
	{
	  ADD_TOKEN(ADD, 0);
	  goto loopStart;
	}
	break;
      
      case 'N':
	eof = (c = fgetc(fp));
	CHECK_EOF(eof);
	c = ((c >= 'a') && (c <= 'z')) ? c + 'A' - 'a' : c;
	++column;  
	if (c != 'D')
	{
	  fprintf(stderr, "Unexpected character at line %d column %d. (5)", line, column);
	  exit(1);
	}
	c = ((c >= 'a') && (c <= 'z')) ? c + 'A' - 'a' : c;
	ADD_TOKEN(AND, 0);
	break;
      
      default:
	fprintf(stderr, "Unexpected character at line %d column %d. (6)", line, column);
	exit(1);
      }
      break;
    }

    case 'S':
    {
      int8_t eof = (c = fgetc(fp));
      CHECK_EOF(eof);
      c = ((c >= 'a') && (c <= 'z')) ? c + 'A' - 'a' : c;
      ++column;
      switch (c)
      {
      case 'U':
	eof = (c = fgetc(fp));
	CHECK_EOF(eof);
	c = ((c >= 'a') && (c <= 'z')) ? c + 'A' - 'a' : c;
	++column;
	if (c != 'B')
	{
	  fprintf(stderr, "Unexpected character at line %d column %d. (7)", line, column);
	  exit(1);
	}
	ADD_TOKEN(SUB, 0);
	break;
	
      case 'H':
	eof = (c = fgetc(fp));
	CHECK_EOF(eof);
	c = ((c >= 'a') && (c <= 'z')) ? c + 'A' - 'a' : c;
	++column;
	if (c != 'I')
	{
	  fprintf(stderr, "Unexpected character at line %d column %d. (8)", line, column);
	  exit(1);
	}

	eof = (c = fgetc(fp));
	CHECK_EOF(eof);
	c = ((c >= 'a') && (c <= 'z')) ? c + 'A' - 'a' : c;
	++column;
	if (c != 'F')
	{
	  fprintf(stderr, "Unexpected character at line %d column %d. (9)", line, column);
	  exit(1);
	}

	eof = (c = fgetc(fp));
	CHECK_EOF(eof);
	c = ((c >= 'a') && (c <= 'z')) ? c + 'A' - 'a' : c;
	++column;
	if (c != 'T')
	{
	  fprintf(stderr, "Unexpected character at line %d column %d. (10)", line, column);
	  exit(1);
	}
	ADD_TOKEN(SHIFT, 0);
	break;
	
	
      case 'T':
	eof = (c = fgetc(fp));
	CHECK_EOF(eof);
	c = ((c >= 'a') && (c <= 'z')) ? c + 'A' - 'a' : c;
	++column;
	if (c != 'O')
	{
	  fprintf(stderr, "Unexpected character at line %d column %d. (11)", line, column);
	  exit(1);
	}
	eof = (c = fgetc(fp));
	CHECK_EOF(eof);
	c = ((c >= 'a') && (c <= 'z')) ? c + 'A' - 'a' : c;
	++column;
	if (c != 'R')
	{
	  fprintf(stderr, "Unexpected character at line %d column %d. (12)", line, column);
	  exit(1);
	}
	eof = (c = fgetc(fp));
	CHECK_EOF(eof);
	c = ((c >= 'a') && (c <= 'z')) ? c + 'A' - 'a' : c;
	++column;
	if (c != 'E')
	{
	  fprintf(stderr, "Unexpected character at line %d column %d. (13)", line, column);
	  exit(1);
	}
	ADD_TOKEN(STORE, 0);
	break;

      default:
	fprintf(stderr, "Unexpected character at line %d column %d. (14)", line, column);
	exit(1);
      }
      break;
    }

    case 'X':
    {
      int8_t eof = (c = fgetc(fp));
      CHECK_EOF(eof);
      c = ((c >= 'a') && (c <= 'z')) ? c + 'A' - 'a' : c;
      ++column;
      if (c != 'O')
      {
	fprintf(stderr, "Unexpected character at line %d column %d. (15)", line, column);
	exit(1);
      }
      eof = (c = fgetc(fp));
      CHECK_EOF(eof);
      c = ((c >= 'a') && (c <= 'z')) ? c + 'A' - 'a' : c;
      ++column;
      if (c != 'R')
      {
	fprintf(stderr, "Unexpected character at line %d column %d. (16)", line, column);
	exit(1);
      }
      ADD_TOKEN(XOR, 0);
      break;
    }

    case 'O':
    {
      int8_t eof = (c = fgetc(fp));
      CHECK_EOF(eof);
      c = ((c >= 'a') && (c <= 'z')) ? c + 'A' - 'a' : c;
      ++column;
      if (c != 'R')
      {
	fprintf(stderr, "Unexpected character at line %d column %d. (17)", line, column);
	exit(1);
      }

      eof = (c = fgetc(fp));
      c = ((c >= 'a') && (c <= 'z')) ? c + 'A' - 'a' : c;
      if (eof == EOF)
      {
	ADD_TOKEN(OR, 0);
	goto loopEnd;
      }
      switch (c)
      {
      case 'I':
	ADD_TOKEN(ORI, 0);
	break;
      case 'H':
	eof = (c = fgetc(fp));
	CHECK_EOF(eof);
	c = ((c >= 'a') && (c <= 'z')) ? c + 'A' - 'a' : c;
	++column;
	if (c != 'I')
	{
	  fprintf(stderr, "Unexpected character at line %d column %d. (18)", line, column);
	  exit(1);
	}
	ADD_TOKEN(ORHI, 0);
	break;

      default:
	ADD_TOKEN(OR, 0);
	goto loopStart;
      }
      break;
    }
    

    case 'L':
    {
      int8_t eof = (c = fgetc(fp));
      CHECK_EOF(eof);
      c = ((c >= 'a') && (c <= 'z')) ? c + 'A' - 'a' : c;
      ++column;
      if (c != 'O')
      {
	fprintf(stderr, "Unexpected character at line %d column %d. (19)", line, column);
	exit(1);
      }
      eof = (c = fgetc(fp));
      CHECK_EOF(eof);
      c = ((c >= 'a') && (c <= 'z')) ? c + 'A' - 'a' : c;
      ++column;
      if (c != 'A')
      {
	fprintf(stderr, "Unexpected character at line %d column %d. (20)", line, column);
	exit(1);
      }
      eof = (c = fgetc(fp));
      CHECK_EOF(eof);
      c = ((c >= 'a') && (c <= 'z')) ? c + 'A' - 'a' : c;
      ++column;
      if (c != 'D')
      {
	fprintf(stderr, "Unexpected character at line %d column %d. (21)", line, column);
	exit(1);
      }
      ADD_TOKEN(LOAD, 0);
      break;
    }

    case 'B':
    {
      int8_t eof = (c = fgetc(fp));
      CHECK_EOF(eof);
      c = ((c >= 'a') && (c <= 'z')) ? c + 'A' - 'a' : c;
      ++column;
      switch (c)
      {
      case 'L':
	eof = (c = fgetc(fp));
	CHECK_EOF(eof);
	c = ((c >= 'a') && (c <= 'z')) ? c + 'A' - 'a' : c;
	++column;
	if (c != 'T')
	{
	  fprintf(stderr, "Unexpected character at line %d column %d. (22)", line, column);
	  exit(1);
	}
	ADD_TOKEN(BLT, 0);
	break;
      case 'E':
	ADD_TOKEN(BE, 0);
	break;
      default:
	fprintf(stderr, "Unexpected character at line %d column %d. (23)", line, column);
	exit(1);
      }
      break;
    }

    case '0':
    {
      int8_t eof = (c = fgetc(fp));
      CHECK_EOF(eof);
      c = ((c >= 'a') && (c <= 'z')) ? c + 'A' - 'a' : c;
      ++column;
      uint16_t val = 0;
      switch (c)
      {
      case 'B':
      {
	uint8_t brk = 1;
	do
	{
	  eof = (c = fgetc(fp));
	  if (eof == EOF)
	  {
	    ADD_TOKEN(IMMEDIATE, val);
	    return tokens;
	  }
	  c = ((c >= 'a') && (c <= 'z')) ? c + 'A' - 'a' : c;
	  if ((c == '0') || (c == '1'))
	  {
	    val = val<<1;
	    if (c == '1')
	      val = val | 1;
	  }
	  else
	  {
	    brk = 0;
	  }
	}	while (brk);
	break;
      }

      case 'D':
      {
	uint8_t brk = 1;
	do
	{
	  eof = (c = fgetc(fp));
	  if (eof == EOF)
	  {
	    ADD_TOKEN(IMMEDIATE, val);
	    return tokens;
	  }
	  c = ((c >= 'a') && (c <= 'z')) ? c + 'A' - 'a' : c;
	  if ((c >= '0') && (c <= '9'))
	  {
	    val = val * 10 + (c - '0');
	  }
	  else
	  {
	    brk = 0;
	  }
        
	} while (brk);
	break;
      }
      case 'X':
      {
	uint8_t brk = 1;
	do
	{
	  eof = (c = fgetc(fp));
	  if (eof == EOF)
	  {
	    ADD_TOKEN(IMMEDIATE, val);
	    return tokens;
	  }
	  c = ((c >= 'a') && (c <= 'z')) ? c + 'A' - 'a' : c;

	  if (((c >= '0') && (c <= '9')) ||
	      ((c >= 'a') && (c <= 'f')) ||
	      ((c >= 'A') && (c <= 'F')))
	  {
	    uint16_t delta;
	    if ((c >= '0') && (c <= '9'))
	    {
	      delta = c - '0';
	    }
	    else
	    {
	      if ((c >= 'a') && (c <= 'f'))
	      {
		delta = c - 'a' + 10;
	      }
	      else
	      {
		delta = c - 'A' + 10;
	      }
	    }
	    val = val<<4;
	    val = val | delta;
	  }
	  else
	  {
	    brk = 0;
	  }
        
	} while (brk);
	break;
      }
      }
      ADD_TOKEN(IMMEDIATE, val);
      break;
    }

    default:
      fprintf(stderr, "Unexpected character at line %d column %d. (24 : %d)", line, column, c);
      exit(1);
  
    }
  }
  loopEnd:;
    return tokens;
  
}
