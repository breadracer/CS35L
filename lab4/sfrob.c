#include <stdio.h>
#include <stdlib.h>

int decode(char c) {
  return c ^ 42;
}

int frobcmp(const char* a, const char* b) {
  const char* ac = a;
  const char* bc = b;
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

int cmp(const void* ap, const void* bp) {
  return frobcmp(*(const char**) ap, *(const char**) bp);
}

int main() {
  int lengthb = 0;
  int lengthl = 0;
  int lengthc = 0;
  int counter = 0;
  char* buffer = malloc(sizeof(char));
  char** list = malloc(sizeof(char*));
  int* counters = malloc(sizeof(int));
  if (!buffer || !list) {
    fprintf(stderr, "Error");
    exit(1);
  }
  list[lengthl] = buffer + counter;
  int spaceFlag = 0;
  char c = getchar();

  while (c == ' ' || c == '\n') {
    c = getchar();
  }

  while (c != EOF) {


    
    /*    for (int i = 0; i < lengthb; i++) {
      putchar(buffer[i]);
    }
    putchar('\n');

    for (int j = 0; j < lengthl; j++) {
      putchar(*(list[j]));
      putchar(' ');
    }
    putchar('\n');*/
    

    
    if (c == ' ' || c == '\n') {
      c = ' ';
      list[lengthl] = buffer + counter;
      
      counters[lengthc] = counter;
      int* tempc = realloc(counters, (lengthc + 2) * sizeof(int));
      if (!tempc) {
	free(buffer);
	free(list);
	fprintf(stderr, "Error");
	exit(1);	
      }
      counters = tempc;
      lengthc++;
      
      char** templ = realloc(list, (lengthl + 2) * sizeof(char*));
      if (!templ) {
	free(buffer);
	free(list);
	fprintf(stderr, "Error");
	exit(1);	
      }
      list = templ;

      counter = lengthb + 1;


      
      lengthl++;
    }
    buffer[lengthb] = c;

    //    printf("%d\n", lengthb);
    
    char* tempb = realloc(buffer, (lengthb + 2) * sizeof(char));
    if (!tempb) {
      free(buffer);
      free(list);
      fprintf(stderr, "Error");
      exit(1);
    }
    buffer = tempb;
    for (int k = 0; k < lengthl; k++) {
      list[k] = buffer + counters[k];
    }
    lengthb++;

    /*    for (int j = 0; j < lengthl; j++) {
      putchar(*(list[j]));
      putchar(' ');
    }
    putchar('\n');*/


    
    c = getchar();
    if (spaceFlag) {
      while (c == ' ' || c == '\n') {
	c = getchar();
      }
      spaceFlag = 0;
    } else if (c == ' ' || c == '\n') {
      spaceFlag = 1;
    }
    
    if (c == EOF) {
      buffer[lengthb] = ' ';
      list[lengthl] = buffer + counter;
      counters[lengthc] = counter;
      lengthl++;
    }

  }
  
  
  //  printf("%d\n", lengthl);
  qsort(list, lengthl, sizeof(char*), cmp);
  for (int i = 0; i < lengthl; i++) {
    char* c = list[i];
    while (*c != ' ') {
      putchar(*c);
      c++;
    }
    putchar(' ');
  }
 
  free(list);
  free(buffer);
  return 0;
}
