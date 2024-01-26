#include <stdio.h>
#include <stdlib.h>
#include "lexer.h"
#include "parser.h"


int main(int argc, char **argv)
{
  if (argc != 3)
  {
    fprintf(stderr, "Usage: %s [input file] [output file].", argv[0]);
    exit(1);
  }
  FILE *f_in = fopen(argv[1], "r");
  if (f_in == NULL)
  {
    fprintf(stderr, "Error openning input file.");
    exit(1);
  }
  FILE *f_out = fopen(argv[2], "wb");
  if (f_out == NULL)
  {
    fprintf(stderr, "Error openning output file.");
    fclose(f_in);
    exit(1);
  }

  parse(f_out, tokenize(f_in));
  uint16_t tmp = ~((uint16_t)0);
  fwrite(&tmp, sizeof(uint16_t), 1, f_out); // add undefined instruction at the end to stop simulator
  fclose(f_in);
  fclose(f_out);
  return 0;
}
