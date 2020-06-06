// Global variables and helper functions
int var_Test13[2];
int var_Test137; int *TestHelperFunction137(int x) { var_Test137=x; return &var_Test137; }
void TestHelperFunction148(int *a) { *a = 10; }

// Test functions
int Test10() 
{
  char x[3]; x[0]=-1;x[1]=2;int y;y=4;return x[0]+y;
}
int Test13() 
{
  *var_Test13 = 1;*(var_Test13 + 1) = 2;return *var_Test13;
}
int Test20() 
{
  int a[2];*a = 1;*(a + 1) = 2;return *a;
}
int Test21() 
{
  int a[2];*a = 1;*(a + 1) = 2;return *(a+1);
}
int Test22() 
{
  int a[2];*a = 1;*(a + 1) = 2;int *p;p = a;return *p + *(p + 1);
}
int Test23() 
{
  int a[2];int *p;p = a;*p = 1;*(p + 1) = 2;return *a + *(a + 1);
}
int Test24() 
{
  int a[2];a[0] = 1;a[1] = 2;return a[0];
}
int Test25() 
{
  int a[2];a[0] = 1;a[1] = 2;return a[1];
}
int Test26() 
{
  int a[2];a[0] = 1;a[1] = 2;int *p;p = a;return *p + *(p + 1);
}
int Test27() 
{
  int a[2];a[0] = 1;a[1] = 2;int *p;p = a;return p[0] + p[1];
}
int Test28() 
{
  int a[2];int *p;p = a;*p = 1;*(p + 1) = 2;return a[0] + a[1];
}
int Test29() 
{
  int a[2];int b;a[0]=1;a[1]=2;b=0;return a[b];
}
int Test30() 
{
  int a[2];int b;a[0]=1;a[1]=2;b=0;return a[b+1];
}
int Test31() 
{
  int a[2];int b;a[0]=1;a[1]=2;b=1;return a[0];
}
int Test32() 
{
  int a[2];int b;a[0]=1;a[1]=2;b=1;return a[1];
}
int Test33() 
{
  int a[100];int b;a[0]=1;a[1]=2;b=1;return a[1];
}
int Test34() 
{
  int a[3];int b;*a=1;*(a+1)=2;*(a+2)=3;b=1;return *a;
}
int Test35() 
{
  int a[3];int b;a[0]=1;a[1]=2;a[2]=3;b=1;return a[0];
}
int Test36() 
{
  int a[3];int b;a[0]=1;a[1]=2;a[2]=3;b=1;return a[1];
}
int Test37() 
{
  int a[3];int b;a[0]=1;a[1]=2;a[2]=3;b=1;return a[2];
}
int Test38() 
{
  int a[3];int b;a[0]=1;a[1]=2;a[2]=3;b=1;return a[b*2];
}
int Test39() 
{
  int a[4];int b;a[0]=1;a[1]=2;a[2]=3;a[3]=4;b=1;return a[0];
}
int Test40() 
{
  int a[4];int b;a[0]=1;a[1]=2;a[2]=3;a[3]=4;b=1;return a[1];
}
int Test41() 
{
  int a[4];int b;a[0]=1;a[1]=2;a[2]=3;a[3]=4;b=1;return a[2];
}
int Test42() 
{
  int a[4];int b;a[0]=1;a[1]=2;a[2]=3;a[3]=4;b=1;return a[3];
}
int Test56() 
{
  int x; int *y;y=&x;*y=3;return x;
}
int Test59() 
{
  int a;int *b;a=1;b=&a;return *b;
}
int Test137() {
  int *z = TestHelperFunction137(10);
  return *z;
}
int Test148() {
  int a;
  TestHelperFunction148(&a);
  return a*2;
}

int run_pointers_tests() {
  int expected;
  int actual;

  expected=3;
  actual=Test10();
  if (actual != expected)
  {
    printf("Test10 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=1;
  actual=Test13();
  if (actual != expected)
  {
    printf("Test13 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=1;
  actual=Test20();
  if (actual != expected)
  {
    printf("Test20 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=2;
  actual=Test21();
  if (actual != expected)
  {
    printf("Test21 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=3;
  actual=Test22();
  if (actual != expected)
  {
    printf("Test22 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=3;
  actual=Test23();
  if (actual != expected)
  {
    printf("Test23 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=1;
  actual=Test24();
  if (actual != expected)
  {
    printf("Test24 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=2;
  actual=Test25();
  if (actual != expected)
  {
    printf("Test25 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=3;
  actual=Test26();
  if (actual != expected)
  {
    printf("Test26 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=3;
  actual=Test27();
  if (actual != expected)
  {
    printf("Test27 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=3;
  actual=Test28();
  if (actual != expected)
  {
    printf("Test28 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=1;
  actual=Test29();
  if (actual != expected)
  {
    printf("Test29 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=2;
  actual=Test30();
  if (actual != expected)
  {
    printf("Test30 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=1;
  actual=Test31();
  if (actual != expected)
  {
    printf("Test31 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=2;
  actual=Test32();
  if (actual != expected)
  {
    printf("Test32 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=2;
  actual=Test33();
  if (actual != expected)
  {
    printf("Test33 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=1;
  actual=Test34();
  if (actual != expected)
  {
    printf("Test34 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=1;
  actual=Test35();
  if (actual != expected)
  {
    printf("Test35 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=2;
  actual=Test36();
  if (actual != expected)
  {
    printf("Test36 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=3;
  actual=Test37();
  if (actual != expected)
  {
    printf("Test37 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=3;
  actual=Test38();
  if (actual != expected)
  {
    printf("Test38 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=1;
  actual=Test39();
  if (actual != expected)
  {
    printf("Test39 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=2;
  actual=Test40();
  if (actual != expected)
  {
    printf("Test40 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=3;
  actual=Test41();
  if (actual != expected)
  {
    printf("Test41 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=4;
  actual=Test42();
  if (actual != expected)
  {
    printf("Test42 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=3;
  actual=Test56();
  if (actual != expected)
  {
    printf("Test56 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=1;
  actual=Test59();
  if (actual != expected)
  {
    printf("Test59 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=10;
  actual=Test137();
  if (actual != expected)
  {
    printf("Test137 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=20;
  actual=Test148();
  if (actual != expected)
  {
    printf("Test148 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }

  return 0;
}