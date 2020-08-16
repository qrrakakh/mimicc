/* extern declared functions and variables */
extern int a_test_func;
extern int arr_test_func[4];
extern void printf(char *fmt, ...);

int Test73(void) 
{
  return 18%7;
}
int Test74(void) 
{
  return 23%4;
}
int Test75() 
{
  return 94%11;
}
int Test76() 
{
  {} return 3;
}
int Test77() 
{
  return 3;
}
int Test90() 
{
  int a;a=11;return a<10;
}
int Test91() 
{
  int a;a=2;return a<10;
}
int Test92() 
{
  int a;a=10;return a<10;
}
int Test93() 
{
  int a;a=10;return a<=10;
}
int Test94() 
{
  int a;a=4;return a;
}
int Test95() 
{
  int a;a=12;return a;
}
int Test96() 
{
  return 0;
}
int Test97() 
{
  return 42;
}
int Test98() 
{
  return 5+20-4;
}
int Test99() 
{
  return  12 + 34 - 5 ;
}
int Test100() 
{
  return 5+6*7;
}
int Test101() 
{
  return 5*(9-6);
}
int Test102() 
{
  return (3+5)/2;
}
int Test103() 
{
  return (+3+5)/2;
}
int Test104() 
{
  return (-3+5)/2;
}
int Test105() 
{
  return (5+1)==6;
}
int Test106() 
{
  return 8==(4-2);
}
int Test107() 
{
  return (5+1)!=6;
}
int Test108() 
{
  return 8!=(4-2);
}
int Test109() 
{
  return 4<4*2;
}
int Test110() 
{
  return 4<=4*1;
}
int Test111() 
{
  return 4<4+1;
}
int Test112() 
{
  return 4*2<4;
}
int Test113() 
{
  return 4<=2+1;
}
int Test114() 
{
  return 4>4*2;
}
int Test115() 
{
  return 4>=4*1;
}
int Test116() 
{
  return 4>4+1;
}
int Test117() 
{
  return 4*2>4;
}
int Test118() 
{
  return 4>=2+1;
}
int Test119() 
{
  int a; int b;a=3;b=5*6-8;return a+b/2;
}
int Test120() 
{
  int a; int b; int c;a=1;b=2;c=4;return (b+c)/2+a;
}
int Test121() 
{
  int f2o; int bar_a; int b1zz;f2o=1;bar_a=2;b1zz=4;return (bar_a+b1zz)/2+f2o;
}
int Test122() 
{
  int f2o; int bar_a; int b1zz;f2o=1;bar_a=2;b1zz=4;(bar_a+b1zz)/2+f2o;return (f2o+b1zz)*bar_a;
}
int Test123() 
{
  int a; a=1;
  return ++a;
}
int Test124() 
{
  int a; a=1;
  ++a;
  return a;
}
int Test125() 
{
  int a; a=1;
  return --a;
}
int Test126() 
{
  int a; a=1;
  --a;
  return a;
}
int Test127() 
{
  int a; a=1;
  return a++;
}
int Test128() 
{
  int a; a=1;
  a++;
  return a;
}
int Test129() 
{
  int a; a=1;
  return a--;
}
int Test130() 
{
  int a; a=1;
  a--;
  return a;
}
int Test131() {
  int a=1;
  return a;
}
int Test132() {
  int a=1,b=2;
  return a+b;
}
char Test133() {
  int a='a';
  return a;
}
int Test135() {
  a_test_func=1;
  return a_test_func;
}
int Test136() {
  arr_test_func[0]=0;
  arr_test_func[1]=2;
  arr_test_func[2]=4;
  return arr_test_func[2];
}
int Test143() {
  int a=1, b=2, c=1, d=5, e=5;
  a+=1; b-=1; c*=3; d/=2; e%=2;
  return (((e*4+d)*4+c)*4+b)*4+a; // 2 + 1*4 + 3*16 + 2*64 + 1*256 = 438
}

_Bool boolean_test_true() {
  _Bool a = 1;
  return a;
}

_Bool boolean_test_false() {
  _Bool a = 0;
  return a;
}

int and_test() { // 1
  int x = 1 + 0 + 4;
  int y = 1 + 2 + 0;
  return x & y;
}

int or_test() { // 1+2+4 = 7
  int x = 1 + 0 + 4;
  int y = 1 + 2 + 0;
  return x | y;
}

int xor_test() { // 0 + 2 + 4 = 6
  int x = 1 + 0 + 4;
  int y = 1 + 2 + 0;
  return x ^ y;
}

int and_test2() { // 1
  int x = 1 + 0 + 4;
  int y = 1 + 2 + 0;
  x &= y;
  return x;
}

int or_test2() { // 1+2+4 = 7
  int x = 1 + 0 + 4;
  int y = 1 + 2 + 0;
  x |= y;
  return x;
}

int xor_test2() { // 0 + 2 + 4 = 6
  int x = 1 + 0 + 4;
  int y = 1 + 2 + 0;
  x ^= y;
  return x;
}

int not_test() { // ((1<<31)-1) - 1 - 4 - (1<<31) = -6
  int x = 1 + 0 + 4;
  int y = 1 + 2 + 0;
  return ~x;
}

int lshift_test1() { // 12
  int x = 3;
  return x << 2;
}

int lshift_test2() { // 12
  int x = 3;
  x <<= 2;
  return x;
}

int rshift_test1() { // 3
  int x = 12;
  return x >> 2;
}

int rshift_test2() { // 3
  int x = 12;
  x >>= 2;
  return x;
}

int run_basic_tests() {
  int expected;
  int actual;
  
  expected=4;
  actual=Test73();
  if (actual != expected)
  {
    printf("Test73 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=3;
  actual=Test74();
  if (actual != expected)
  {
    printf("Test74 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=6;
  actual=Test75();
  if (actual != expected)
  {
    printf("Test75 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=3;
  actual=Test76();
  if (actual != expected)
  {
    printf("Test76 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=3;
  actual=Test77();
  if (actual != expected)
  {
    printf("Test77 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=0;
  actual=Test90();
  if (actual != expected)
  {
    printf("Test90 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=1;
  actual=Test91();
  if (actual != expected)
  {
    printf("Test91 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=0;
  actual=Test92();
  if (actual != expected)
  {
    printf("Test92 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=1;
  actual=Test93();
  if (actual != expected)
  {
    printf("Test93 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=4;
  actual=Test94();
  if (actual != expected)
  {
    printf("Test94 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=12;
  actual=Test95();
  if (actual != expected)
  {
    printf("Test95 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=0;
  actual=Test96();
  if (actual != expected)
  {
    printf("Test96 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=42;
  actual=Test97();
  if (actual != expected)
  {
    printf("Test97 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=21;
  actual=Test98();
  if (actual != expected)
  {
    printf("Test98 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=41;
  actual=Test99();
  if (actual != expected)
  {
    printf("Test99 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=47;
  actual=Test100();
  if (actual != expected)
  {
    printf("Test100 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=15;
  actual=Test101();
  if (actual != expected)
  {
    printf("Test101 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=4;
  actual=Test102();
  if (actual != expected)
  {
    printf("Test102 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=4;
  actual=Test103();
  if (actual != expected)
  {
    printf("Test103 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=1;
  actual=Test104();
  if (actual != expected)
  {
    printf("Test104 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=1;
  actual=Test105();
  if (actual != expected)
  {
    printf("Test105 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=0;
  actual=Test106();
  if (actual != expected)
  {
    printf("Test106 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=0;
  actual=Test107();
  if (actual != expected)
  {
    printf("Test107 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=1;
  actual=Test108();
  if (actual != expected)
  {
    printf("Test108 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=1;
  actual=Test109();
  if (actual != expected)
  {
    printf("Test109 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=1;
  actual=Test110();
  if (actual != expected)
  {
    printf("Test110 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=1;
  actual=Test111();
  if (actual != expected)
  {
    printf("Test111 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=0;
  actual=Test112();
  if (actual != expected)
  {
    printf("Test112 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=0;
  actual=Test113();
  if (actual != expected)
  {
    printf("Test113 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=0;
  actual=Test114();
  if (actual != expected)
  {
    printf("Test114 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=1;
  actual=Test115();
  if (actual != expected)
  {
    printf("Test115 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=0;
  actual=Test116();
  if (actual != expected)
  {
    printf("Test116 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=1;
  actual=Test117();
  if (actual != expected)
  {
    printf("Test117 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=1;
  actual=Test118();
  if (actual != expected)
  {
    printf("Test118 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=14;
  actual=Test119();
  if (actual != expected)
  {
    printf("Test119 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=4;
  actual=Test120();
  if (actual != expected)
  {
    printf("Test120 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=4;
  actual=Test121();
  if (actual != expected)
  {
    printf("Test121 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=10;
  actual=Test122();
  if (actual != expected)
  {
    printf("Test122 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=2;
  actual=Test123();
  if (actual != expected)
  {
    printf("Test123 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=2;
  actual=Test124();
  if (actual != expected)
  {
    printf("Test124 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=0;
  actual=Test125();
  if (actual != expected)
  {
    printf("Test125 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=0;
  actual=Test126();
  if (actual != expected)
  {
    printf("Test126 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=1;
  actual=Test127();
  if (actual != expected)
  {
    printf("Test127 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=2;
  actual=Test128();
  if (actual != expected)
  {
    printf("Test128 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=1;
  actual=Test129();
  if (actual != expected)
  {
    printf("Test129 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=0;
  actual=Test130();
  if (actual != expected)
  {
    printf("Test130 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=1;
  actual=Test131();
  if (actual != expected)
  {
    printf("Test131 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=3;
  actual=Test132();
  if (actual != expected)
  {
    printf("Test132 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=97;
  actual=Test133();
  if (actual != expected)
  {
    printf("Test133 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=1;
  actual=Test135();
  if (actual != expected)
  {
    printf("Test135 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=4;
  actual=Test136();
  if (actual != expected)
  {
    printf("Test136 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=438;
  actual=Test143();
  if (actual != expected)
  {
    printf("Test143 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }

  expected=1;
  actual=boolean_test_true();
  if (actual != expected)
  {
    printf("boolean_test_true faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=0;
  actual=boolean_test_false();
  if (actual != expected)
  {
    printf("boolean_test_false faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }

  expected=1;
  actual=and_test();
  if (actual != expected)
  {
    printf("and_test faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }

  expected=7;
  actual=or_test();
  if (actual != expected)
  {
    printf("or_test faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }

 expected=6;
  actual=xor_test();
  if (actual != expected)
  {
    printf("xor_test faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }

  expected=1;
  actual=and_test2();
  if (actual != expected)
  {
    printf("and_test2 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }

  expected=7;
  actual=or_test2();
  if (actual != expected)
  {
    printf("or_test2 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }

  expected=6;
  actual=xor_test2();
  if (actual != expected)
  {
    printf("xor_test2 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }

  expected=-6;
  actual=not_test();
  if (actual != expected)
  {
    printf("not_test faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }

  expected=12;
  actual=lshift_test1();
  if (actual != expected)
  {
    printf("lshift_test1 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }

  expected=12;
  actual=lshift_test2();
  if (actual != expected)
  {
    printf("lshift_test2 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }

  expected=3;
  actual=rshift_test1();
  if (actual != expected)
  {
    printf("rshift_test1 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }

  expected=3;
  actual=rshift_test2();
  if (actual != expected)
  {
    printf("rshift_test2 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }

  return 0;
}