// Global variables and helper functions
int var_Test11, *var_Test12;
int var_Test14;int TestHelperFunction14(int z) { return var_Test14+z;}
int var_Test15[3];int TestHelperFunction15(int z) { return var_Test15[2]+z;}
int var_Test16;int hepler_Test16() {int a; a=3; return a;}
int var_Test17;int TestHelperFunction17() {int a; a=3; return a;}
int var_Test18;int TestHelperFunction18() {return var_Test18;}
int var_Test19;int TestHelperFunction19() {return var_Test19;}

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
  int a[2];var_Test12=&a;*var_Test12 = 1;*(a + 1) = 2;return *a;
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

  return 0;
}