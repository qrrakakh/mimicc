/* extern declared functions and variables */
extern int Alloc4(int **p, int a, int b, int c, int d);
extern int PrintInt(int a);
extern int Foo();
extern void printf(char *fmt, ...);

// Global variables and helper functions
int fib(int a);
int fib(int a){if(a==0) return 0;else if(a==1) return 1; else return fib(a-2)+fib(a-1);}
int TestHelperFunction58(int a){return a*2;}
int TestHelperFunction61(){int a;a=1;return a;}
int TestHelperFunction62(){int a;a=1;return a;}
int TestHelperFunction65_Test72(int a,int b,int c,int d,int e,int f){return a+b*2+c*4+d*8+e*16+f*32;}
void TestHelperFunction138_voidfunc() {printf("void func runs correctly\n"); return;}

// Test functions
int Test55() 
{
  int *p;Alloc4(&p, 1, 2, 4, 8);int *q;q = p + 2;*q;q = p + 3;return *q;
}
int Test57() 
{
  PrintInt(fib(10)); return fib(10);
}
int Test58() 
{
  return TestHelperFunction58(1);
}
int Test61() 
{
  int a; int b; a=0;a=TestHelperFunction61();return a;
}
int Test62() 
{
  int a; int b; a=0;b=TestHelperFunction62();return a;
}
int Test60() 
{
  18%7; return Foo();
}
int Test63() 
{
  18%7;return Foo();
}
int Test64() 
{
  Foo();return 18%7;
}
int Test65() 
{
  return TestHelperFunction65_Test72(0,0,0,0,0,0);
}
int Test66() 
{
  return TestHelperFunction65_Test72(1,0,0,0,0,0);
}
int Test67() 
{
  return TestHelperFunction65_Test72(0,1,0,0,0,0);
}
int Test68() 
{
  return TestHelperFunction65_Test72(0,0,1,0,0,0);
}
int Test69() 
{
  return TestHelperFunction65_Test72(0,0,0,1,0,0);
}
int Test70() 
{
  return TestHelperFunction65_Test72(0,0,0,0,1,0);
}
int Test71() 
{
  return TestHelperFunction65_Test72(0,0,0,0,0,1);
}
int Test72() 
{
  return TestHelperFunction65_Test72(TestHelperFunction65_Test72(0,0,0,0,0,1),0,0,0,0,0);
}
int Test138() {
  TestHelperFunction138_voidfunc();
  return 1;
}

int run_funccall_tests() {
  int expected;
  int actual;

  expected=8;
  actual=Test55();
  if (actual != expected)
  {
    printf("Test55 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=55;
  actual=Test57();
  if (actual != expected)
  {
    printf("Test57 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=2;
  actual=Test58();
  if (actual != expected)
  {
    printf("Test58 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=1;
  actual=Test61();
  if (actual != expected)
  {
    printf("Test61 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=0;
  actual=Test62();
  if (actual != expected)
  {
    printf("Test62 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=0;
  actual=Test60();
  if (actual != expected)
  {
    printf("Test60 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=0;
  actual=Test63();
  if (actual != expected)
  {
    printf("Test63 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=4;
  actual=Test64();
  if (actual != expected)
  {
    printf("Test64 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=0;
  actual=Test65();
  if (actual != expected)
  {
    printf("Test65 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=1;
  actual=Test66();
  if (actual != expected)
  {
    printf("Test66 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=2;
  actual=Test67();
  if (actual != expected)
  {
    printf("Test67 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=4;
  actual=Test68();
  if (actual != expected)
  {
    printf("Test68 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=8;
  actual=Test69();
  if (actual != expected)
  {
    printf("Test69 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=16;
  actual=Test70();
  if (actual != expected)
  {
    printf("Test70 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=32;
  actual=Test71();
  if (actual != expected)
  {
    printf("Test71 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=32;
  actual=Test72();
  if (actual != expected)
  {
    printf("Test72 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=1;
  actual=Test138();
  if (actual != expected)
  {
    printf("Test138 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }

  return 0;
}