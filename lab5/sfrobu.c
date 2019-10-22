#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#include <errno.h>
#include <ctype.h>

const char SPACE = ' ';
const char NEWLINE = '\n';

int fflag = 0;

int decoden(char c) {
  return c ^ 42;
}

int decodef(char c) {
  char cd = c ^ 42;
  if (cd < 0 || cd > UCHAR_MAX)
    return cd;
  else
    return toupper(cd);
}

int frobcmp(const char *a, const char *b) {
  int (*decode) (char);
  if (fflag)
    decode = &decodef;
  else
    decode = &decoden;

  const char *ac = a;
  const char *bc = b;
  for (;; ac++, bc++) {
    if (*ac == ' ' && *bc == ' ') {
      return 0;
    } else if (*bc == ' ' || decode(*ac) > decode(*bc)) {
      return 1;
    } else if (*ac == ' ' || decode(*ac) < decode(*bc)) {
      return -1;
    }
  }
  return 0;
}

int cmp(const void *ap, const void *bp) {
  return frobcmp(*(const char**) ap, *(const char**) bp);
}

int main(int argc, char **argv) {
  // check operand error
  if (argc == 2) {
    int lenarg = 0;
    for (; argv[1][lenarg]; lenarg++);
    if (lenarg != 2) {
      fprintf(stderr, "invalid operand: %s", argv[1]);
      exit(1);
    }
    char *f = "-f";
    for (int i = 0; i < lenarg; i++)
      if (argv[1][i] != f[i]) {
	fprintf(stderr, "invalid operand: %s", argv[1]);
	exit(1);
      }
    fflag = 1;
  } else if (argc > 2) {
    fprintf(stderr, "invalid number of operands");
    exit(1);
  }
  
  struct stat statBuffer;
  if (fstat(STDIN_FILENO, &statBuffer) < 0) {
    fprintf(stderr, "fstat failure");
    exit(1);
  }
  int bufferSize = statBuffer.st_size;
  char *inputBuffer = malloc((bufferSize + 1) * sizeof(char));
  if (!inputBuffer) {
    fprintf(stderr, "malloc error");
    exit(1);
  }

  int inputLength = 0, strCounter = 0;

  // read whole file if regular file
  if (bufferSize) {
    int retf = read(STDIN_FILENO, inputBuffer, bufferSize);
    if (retf < 0) {
      fprintf(stderr, "system call read error\n");
      exit(1);
    }
    inputLength = bufferSize - 1;
  }
  
  char charBuffer;
  int ret;
  while ((ret = read(STDIN_FILENO, &charBuffer, 1)) == 1) {
    // reallocate memory if necessary
    if (inputLength == bufferSize) {
      if (bufferSize == 0)
	bufferSize++;
      else
	bufferSize *= 2;
      char *tempBuffer = realloc(inputBuffer, sizeof(char) * bufferSize);
      if (!tempBuffer) {
	free(inputBuffer);
	fprintf(stderr, "realloc error");
	exit(1);
      }
      inputBuffer = tempBuffer;      
    }
    
    inputBuffer[inputLength] = charBuffer;
    inputLength++;
  }
  if (ret < 0) {
    fprintf(stderr, "system call read error\n");
    exit(1);
  }
    
  // append space if not seen at the end
  if (inputBuffer[inputLength - 1] != ' ') {
    if (inputLength + 1 == bufferSize) {
      if (bufferSize == 0)
	bufferSize++;
      else
	bufferSize++;
      char *tempBuffer = realloc(inputBuffer, sizeof(char) * bufferSize);
      if (!tempBuffer) {
	free(inputBuffer);
	fprintf(stderr, "realloc error");
	exit(1);
      }
      inputBuffer = tempBuffer;      
    }
    inputBuffer[inputLength] = ' ';
    inputLength++;
  }

  // count spaces
  int spaceFlag = 0;
  int startFlag = 1;
  for (int i = 0; i < inputLength; i++) {
    if (inputBuffer[i] == ' ' || inputBuffer[i] == '\n') {
      if (spaceFlag || startFlag)
	continue;
      else {
	spaceFlag = 1;
	strCounter++;
      }
    } else {
      spaceFlag = 0;
      if (startFlag)
	startFlag = 0;
    }    
  }
  // form string list
  char **list = malloc(sizeof(char*) * strCounter);
  if (!list) {
    free(inputBuffer);
    fprintf(stderr, "malloc error");
    exit(1);
  }  
  int count = 0;
  char *start = NULL;
  spaceFlag = 0;
  startFlag = 1;
  for (int i = 0; i < inputLength; i++)
    if (inputBuffer[i] == ' ' || inputBuffer[i] == '\n') {
      if (spaceFlag || startFlag)
	continue;
      else
	spaceFlag = 1;
    } else {
      if (startFlag || spaceFlag) {
	startFlag = 0;
	list[count] = inputBuffer + i;
	count++;
	start = inputBuffer + i;	
      }
      spaceFlag = 0;
    }    

  // sort
  qsort(list, count, sizeof(char*), cmp);

  // test output after sort
  for (int i = 0; i < count; i++) {
    char *c = list[i];
    while (*c != ' ' && *c != '\n') {
      write(STDOUT_FILENO, c, 1);
      c++;
    }
    write(STDOUT_FILENO, &SPACE, 1);
  }

  // free memory
  free(inputBuffer);
  free(list);
  return 0;
}
