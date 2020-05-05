#include <stdio.h>
int foo() { printf("OK\n"); return 0;}
int foo2(int a, int b, int c, int d, int e, int f) {
  return a+b*2+c*4+d*8+e*16+f*32;
}