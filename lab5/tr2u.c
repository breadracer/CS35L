#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

const int MIN_OPERANDS = 3;

void tr2u(char *f, char *t, char* buf, int lenf) {
  for (int i = 0; i < lenf; i++)
    if (*buf == f[i])
      *buf = t[i];
}

int main(int argc, char **argv) {
  // check operand errors
  if (argc < MIN_OPERANDS) {
    printf("operand error: too few arguments\n");
    exit(1);
  }
  char *from = argv[1];
  char *to = argv[2];
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

  // get input
  char *buffer = malloc(sizeof(char));
  int ret;
  while ((ret = read(STDIN_FILENO, buffer, 1)) == 1) {
    tr2u(from, to, buffer, lenfrom);
    write(STDOUT_FILENO, buffer, 1);
  }
  if (ret < 0) {
    printf("system call read error\n");
    exit(1);
  }
}
