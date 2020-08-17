/* extern declared functions and variables */
extern void printf(char *fmt, ...);

// Global variables and helper functions
int TestHelperFunction144_146(int a) {
  int b=10;
  switch(a) {
    case 1:
      b = 1;
      break;
    case 2+1:
      b = 4;
      break;
    default:
      b = 0;
      break;
  }
  return b;
}
int TestHelperFunction147(int a) {
  int b=10;
  switch(a) {
    case 1:
      b = 1;
      break;
    case 2:
      b = 4;
      break;
  }
  return b;
}

// Test functions
int Test78() 
{
  int a; int b; a=0;b=1;if(a>1) b=2;else b=3;return b;
}
int Test79() 
{
  int a; int b; a=0;b=1;if(a>1) b=2;return b;
}
int if_else_test1() 
{
  int a; int b; a=2;b=1;if(a>1) b=2;else b=3;return b;
}
int if_else_test2() 
{
  int a; int b; a=0;b=1;if(a>1) b=2;else b=3;return b;
}
int conditional_expression_test1() 
{
  int a=2;
  return (a>1) ? 2: 3;
}
int conditional_expression_test2() 
{
  int a=0;
  return (a>1) ? 2: 3;
}
int Test81() 
{
  int a; int b; a=2;b=1;if(a>1) b=2;return b;
}
int Test82() 
{
  int a; int b; b=0;for(a=0;a<10;a=a+1) b=b+2; return b;
}
int Test83() 
{
  int a;a=12;while (a<=10) a=a+1;return a;
}
int Test84() 
{
  int a;a=0;while (a<10) a=a+1; return a;
}
int Test85() 
{
  int a;a=12;while (a<=10) a=a+1; return a;
}
int Test86() 
{
  int a;a=0;while (a<10) a=a+1;return a;
}
int Test87() 
{
  int a;a=1;int b;for(b=0;b<10;b=b+1) while(a<10) a=a+1; if(a==10) return 1;else return 2;
}
int Test88() 
{
  int a;a=1;int b;for(b=0;b<10;b=b+1) while(a<9) a=a+1; if(a==10) return 1;else return b;
}
int Test89() 
{
  int a; int c; int b;a=1;c=0;for(b=0;b<10;b=b+1) { a=a+ 1;c=c+2;} if(a==10) return 1;else return a+c;
}
int Test144() {
  return TestHelperFunction144_146(1);
}
int Test145() {
  return TestHelperFunction144_146(3);
}
int Test146() {
  return TestHelperFunction144_146(2);
}
int Test147() {
  return TestHelperFunction147(3);
}
int Test139() {
  int i=100, b=0;
  for(int i=0;i<10;++i)
    b=b+i;
  return i+b;
}
int Test140() {
  int i=50, b=0;
  for(int i=0;i<10;++i) {
    b=b+i;
  }
  return i+b;
}
int Test141() {
  int a=0;
  for(int i=0;i<100;++i) {
    if(i%2==0) continue;
    a=a+i;
  }
  return a; // 2500
}

int Test142() {
  int a=0, b=49, c=3;
  for(int i=0;i<100;++i) {
    a=i;
    if(i==b) break;
  }
  return a;  // 49
}
int run_control_tests() {
  int expected;
  int actual;

  expected=3;
  actual=Test78();
  if (actual != expected)
  {
    printf("Test78 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=1;
  actual=Test79();
  if (actual != expected)
  {
    printf("Test79 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=2;
  actual=if_else_test1();
  if (actual != expected)
  {
    printf("if_else_test1 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=3;
  actual=if_else_test2();
  if (actual != expected)
  {
    printf("if_else_test2 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=2;
  actual=conditional_expression_test1();
  if (actual != expected)
  {
    printf("conditional_expression_test1 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=3;
  actual=conditional_expression_test2();
  if (actual != expected)
  {
    printf("conditional_expression_test2 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=2;
  actual=Test81();
  if (actual != expected)
  {
    printf("Test81 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=20;
  actual=Test82();
  if (actual != expected)
  {
    printf("Test82 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=12;
  actual=Test83();
  if (actual != expected)
  {
    printf("Test83 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=10;
  actual=Test84();
  if (actual != expected)
  {
    printf("Test84 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=12;
  actual=Test85();
  if (actual != expected)
  {
    printf("Test85 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=10;
  actual=Test86();
  if (actual != expected)
  {
    printf("Test86 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=1;
  actual=Test87();
  if (actual != expected)
  {
    printf("Test87 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=10;
  actual=Test88();
  if (actual != expected)
  {
    printf("Test88 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=31;
  actual=Test89();
  if (actual != expected)
  {
    printf("Test89 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=145;
  actual=Test139();
  if (actual != expected)
  {
    printf("Test139 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=95;
  actual=Test140();
  if (actual != expected)
  {
    printf("Test140 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=2500;
  actual=Test141();
  if (actual != expected)
  {
    printf("Test141 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=49;
  actual=Test142();
  if (actual != expected)
  {
    printf("Test142 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
 expected=1;
  actual=Test144();
  if (actual != expected)
  {
    printf("Test144 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=4;
  actual=Test145();
  if (actual != expected)
  {
    printf("Test145 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=0;
  actual=Test146();
  if (actual != expected)
  {
    printf("Test146 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=10;
  actual=Test147();
  if (actual != expected)
  {
    printf("Test147 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  return 0;
}