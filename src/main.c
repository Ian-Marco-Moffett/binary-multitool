#include <stdio.h>
#include <argp.h>
#include <stdint.h>
#include <hexdump.h>
#include <elfinfo.h>

#define FLAG_HEXDUMP (1 << 0)
#define FLAG_ELFINFO (1 << 1)


static const char* input_file = NULL;
uint16_t flags = 0;


static struct argp_option options[] = {
  {"in", 'i', "INPUT", 0, "Input binary"},
  {"hexdump", 'x', 0, 0, "For hex dumping the binary"},
  {"elfinfo", 'e', 0, 0, "For gathering information about an ELF"},
  {0}
};


static error_t parse_opt(int key, char* arg, struct argp_state* state) {
  switch (key) {
    case 'i':
      input_file = arg;
      break;
    case 'x':
      flags |= FLAG_HEXDUMP;
      break;
    case 'e':
      flags |= FLAG_ELFINFO;
      break;
    default:
      return ARGP_ERR_UNKNOWN;
  }

  return 0;
}


static struct argp argp = {options, parse_opt, 0, 0, 0, 0, 0};


int main(int argc, char** argv) {
  argp_parse(&argp, argc, argv, 0, 0, 0);
  
  if (input_file == NULL) {
    printf("Expected input file, please use --help\n");
    return 1;
  }

  FILE* fp = fopen(input_file, "r");
  if (fp == NULL) {
    perror("fopen");
    return 1;
  }

  if (flags & FLAG_HEXDUMP) hexdump(fp);
  if (flags & FLAG_ELFINFO) dump_elf(fp);
  
  fclose(fp);
  return 0;
}
