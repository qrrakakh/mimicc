#include <stdio.h>
#include <stdlib.h>
int a_test_func;
int arr_test_func[3];
int foo() { printf("OK\n"); return 0;}
int printint(int a) { printf("%d\n", a); return 0;}
int alloc4(int **p, int a, int b, int c, int d) {
  *p = (int*)malloc(sizeof(int)*4);
  *(*p+0)=a;*(*p+1)=b;*(*p+2)=c;*(*p+3)=d;
  return 0;
}