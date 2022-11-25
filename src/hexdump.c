#include <hexdump.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <stdint.h>


static void dump(uint8_t* buf, size_t nmemb) {
  size_t cbuf_i = 0;
  size_t off = 0;
  char* cbuf = calloc(nmemb + 1, sizeof(char));      // Char buf.
  
  for (size_t i = 0; i < nmemb; ++i) {
    if (i % 15 == 0 && i != nmemb - 1) {
      printf("\t%s", cbuf);
      memset(cbuf, 0, nmemb);
      cbuf_i = 0;

      printf("\n%08X: ", off);
    }

    printf("%02X ", buf[i]);
    ++off;

    switch (buf[i]) {
      case '[':
      case ']':
      case '{':
      case '}':
      case '>':
      case '?':
      case '<':
      case '+':
      case '-':
        cbuf[i] = buf[i];
        break;
      default:
        if (buf[i] >= 'A' && buf[i] <= 'Z') {
          cbuf[cbuf_i++] = buf[i];
          break;
        }

        if (buf[i] >= 'a' && buf[i] <= 'z') {
          cbuf[cbuf_i++] = buf[i];
          break;
        }

        if (buf[i] >= '0' && buf[i] <= '9') {
          cbuf[cbuf_i++] = buf[i];
          break;
        }

        cbuf[cbuf_i++] = '.';
        break;
    }
  }
}


void hexdump(FILE* fp) {
  fseek(fp, 0, SEEK_END);
  size_t fsize = ftell(fp);
  fseek(fp, 0, SEEK_SET);

  char* buf = malloc(sizeof(char) * fsize);
  fread(buf, sizeof(char), fsize, fp);

  dump(buf, fsize);

  free(buf);
}
