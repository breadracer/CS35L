#include <stdio.h>
#include <stdlib.h>

const int MIN_OPERANDS = 3;

void tr2b(char *f, char *t, char *buf, int lenb, int lenf) {
  for (int i = 0; i < lenb; i++)
    for (int j = 0; j < lenf; j++)
      if (buf[i] == f[j])
  	buf[i] = t[j];
}

int main(int argc, char **argv) {
  // check operand errors
  if (argc < MIN_OPERANDS) {
    printf("operand error: too few arguments\n");
    exit(1);
  }
  char *from = argv[1];
  char *to = argv[2];
  char *buffer = malloc(sizeof(char));
  int lenbuf = 0;

  // get buffer
  char curr = getchar();
  while (curr != EOF) {
    buffer[lenbuf] = curr;
    char *newbuf = realloc(buffer, (lenbuf + 2) * sizeof(char));
    if (newbuf) {
      buffer = newbuf;      
      lenbuf++;
    } else {
      printf("realloc error");
      exit(1);
    }
    curr = getchar();
  }

  // check operand errors
  int lenfrom = 0, lento = 0;
  for (; from[lenfrom]; lenfrom++);
  for (; to[lento]; lento++);
  if (lenfrom != lento) {
    printf("operand error: operands should be the same length\n");
    exit(1);
  }
  for (int i = 0; i < lenfrom; i++)
    for (int j = i + 1; j < lenfrom; j++)
      if (from[j] == from[i]) {
	printf("operand error: duplicate bytes in from\n");
	exit(1);
      }

  tr2b(from, to, buffer, lenbuf, lenfrom);

  for (int i = 0; i < lenbuf; i++)
    putchar(buffer[i]);

  free(buffer);
}
