#include "gd32f4xx.h"
#include "gd32f450i_eval.h"
#include <stdio.h>

int fputc(int ch, FILE *f);

int _write(int file, char* ptr, int len) {
  for (int i = 0; i < len; ++i) {
    fputc((int)ptr[i], NULL);
  }

  return len;
}
