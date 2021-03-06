/* extern declared functions and variables */
extern void printf(char *fmt, ...);

// Global variables and helper functions
int var_Test11, *var_Test12;
int var_Test14;int TestHelperFunction14(int z) { return var_Test14+z;}
int var_Test15[3];int TestHelperFunction15(int z) { return var_Test15[2]+z;}
int var_Test16;int hepler_Test16() {int a; a=3; return a;}
int var_Test17;int TestHelperFunction17() {int a; a=3; return a;}
int var_Test18;int TestHelperFunction18() {return var_Test18;}
int var_Test19;int TestHelperFunction19() {return var_Test19;}

// 10
int int_gvar_init = ((((((((((2+3)-3)*10)/5)<<1)>>2)+1)%2)+5)/6)*10;
// 97
char char_gvar_init = 'a';
// 1 + 0 + 4 + 8 + 0 + 32 = 45
int bit_gvar_init = (1&1) + (2&0) + (4|4) + (8|0) + (16^16) + (32^0);
// 1 + 0 + 4 + 0 + 16 + 32 + 0 + 0 + 256 = 309
int compare_gvar_init = ((1<2)<<0) + ((2<1)<<1) + ((1<=2)<<2) + ((2<=1)<<3)
                        + ((2<=2)<<4) + ((1==1)<<5) + ((1==2)<<6) + ((1!=1)<<7) + ((1!=2)<<8);
// 1 + 0
// - 4 - 16
// + 0 + 32
// + 64 + 0 + 0
// + 512 + 1024 + 0 = 1613
int logical_gvar_init = ((!0) <<0) + ((!1) <<1)
                        + ((~0) <<2) + ((~1) <<3)
                        + ((0?1:0) <<4) + ((1?1:0) <<5)
                        + ((1&&1) <<6) + ((0&&1) <<7) + ((0&&0) <<8)
                        + ((1||1) <<9) + ((0||1) <<10) + ((0||0) <<11);

const int const_gvar1 = 1;
int const const_gvar2 = 2;
const int *const_gvar3;

// Test functions
int Test1() 
{
  int a, b;a=1;b=0; {int a;a=2;b=b+a;} {int a;a=4;{int a;a=8;b=b+a;}} {int a;a=16;{int a;a=32;}b=b+a;} return b;
}
int Test11() 
{
  var_Test11 = 1;return var_Test11;
}
int Test12() 
{
  int a[2];var_Test12=a;*var_Test12 = 1;*(a + 1) = 2;return *a;
}
int Test14() 
{
  var_Test14=3; return TestHelperFunction14(20);
}
int Test15() 
{
  var_Test15[0]=1;var_Test15[1]=2;var_Test15[2]=3; return TestHelperFunction15(20);
}
int Test16() 
{
  var_Test16=1; return hepler_Test16();
}
int Test17() 
{
  var_Test17=1; TestHelperFunction17();return var_Test17;
}
int Test18() 
{
  var_Test18=1; int var_Test18; var_Test18=2;return TestHelperFunction18();
}
int Test19() 
{
  var_Test19=1; int var_Test19; var_Test19=2;TestHelperFunction19();return var_Test19;
}

int const_lvar_test1() {
  int const a = 1;
  return a;
}

int const_lvar_test2() {
  const int a = 2;
  return a;
}

int const_lvar_test3() {
  const int *p;
  int a = 1;
  p = &a;
  return *p;
}

int const_lvar_test4() {
  int a = 1;
  int * const p = &a;
  a = 2;
  return *p;
}

int const_gvar_test1() {
  return const_gvar1;
}

int const_gvar_test2() {
  return const_gvar2;
}

int const_gvar_test3() {
  int a = 1;
  const_gvar3 = &a;
  return *const_gvar3;
}

int run_basic_variables_tests() {
  int expected;
  int actual;

  expected=26;
  actual=Test1();
  if (actual != expected)
  {
    printf("Test1 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=1;
  actual=Test11();
  if (actual != expected)
  {
    printf("Test11 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=1;
  actual=Test12();
  if (actual != expected)
  {
    printf("Test12 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=23;
  actual=Test14();
  if (actual != expected)
  {
    printf("Test14 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=23;
  actual=Test15();
  if (actual != expected)
  {
    printf("Test15 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=3;
  actual=Test16();
  if (actual != expected)
  {
    printf("Test16 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=1;
  actual=Test17();
  if (actual != expected)
  {
    printf("Test17 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=1;
  actual=Test18();
  if (actual != expected)
  {
    printf("Test18 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=2;
  actual=Test19();
  if (actual != expected)
  {
    printf("Test19 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=10;
  actual=int_gvar_init;
  if (actual != expected)
  {
    printf("int_gvar_init_test faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=97;
  actual=char_gvar_init;
  if (actual != expected)
  {
    printf("char_gvar_init_test faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }

  expected=45;
  actual=bit_gvar_init;
  if (actual != expected)
  {
    printf("bit_gvar_init_test faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=309;
  actual=compare_gvar_init;
  if (actual != expected)
  {
    printf("compare_gvar_init_test faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=1613;
  actual=logical_gvar_init;
  if (actual != expected)
  {
    printf("logical_gvar_init_test faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }

  expected=1;
  actual=const_lvar_test1();
  if (actual != expected)
  {
    printf("const_lvar_test1_test faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=2;
  actual=const_lvar_test2();
  if (actual != expected)
  {
    printf("const_lvar_test2_test faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=1;
  actual=const_lvar_test3();
  if (actual != expected)
  {
    printf("const_lvar_test3_test faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=2;
  actual=const_lvar_test4();
  if (actual != expected)
  {
    printf("const_lvar_test4_test faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }

  expected=1;
  actual=const_gvar_test1();
  if (actual != expected)
  {
    printf("const_gvar_test1_test faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=2;
  actual=const_gvar_test2();
  if (actual != expected)
  {
    printf("const_gvar_test2_test faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=1;
  actual=const_gvar_test3();
  if (actual != expected)
  {
    printf("const_gvar_test3_test faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }

  return 0;
}