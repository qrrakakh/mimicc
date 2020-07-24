/* extern declared functions and variables */
extern void printf(char *fmt, ...);

// Test functions
int Test43() 
{
  int a; return sizeof a ;
}
int Test44() 
{
  int a; return sizeof(a);
}
int Test45() 
{
  int *a; return sizeof(a);
}
int Test46() 
{
  int *a; return sizeof a ;
}
int Test47() 
{
  int a; return sizeof(a+1);
}
int Test48() 
{
  int a; return sizeof a+1 ;
}
int Test49() 
{
  int *a; return sizeof(a+1);
}
int Test50() 
{
  int *a; return sizeof a+1 ;
}
int Test51() 
{
  int *a; return sizeof *a;
}
int Test52() 
{
  int *a; return sizeof(*a);
}
int Test53() 
{
  return sizeof(1);
}
int Test54() 
{
  return sizeof(sizeof(1));
}
int Test_struct_sizeof1() {
  struct {
    int a;
    char b;
    int c;
  } a;
  return sizeof(a);
}
int Test_struct_sizeof2() {
  struct {
    int a;
    char b1;
    char b2;
    char b3;
    int c;
  } a;
  return sizeof(a);
}
int Test_struct_sizeof3() {
  struct {
    int a;
    char b1;
    int c;
    char b2;
    char b3;
  } a;
  return sizeof(a);
}

int run_sizeof_tests() {
  int expected;
  int actual;

  expected=4;
  actual=Test43();
  if (actual != expected)
  {
    printf("Test43 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=4;
  actual=Test44();
  if (actual != expected)
  {
    printf("Test44 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=8;
  actual=Test45();
  if (actual != expected)
  {
    printf("Test45 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=8;
  actual=Test46();
  if (actual != expected)
  {
    printf("Test46 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=4;
  actual=Test47();
  if (actual != expected)
  {
    printf("Test47 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=5;
  actual=Test48();
  if (actual != expected)
  {
    printf("Test48 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=8;
  actual=Test49();
  if (actual != expected)
  {
    printf("Test49 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=9;
  actual=Test50();
  if (actual != expected)
  {
    printf("Test50 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=4;
  actual=Test51();
  if (actual != expected)
  {
    printf("Test51 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=4;
  actual=Test52();
  if (actual != expected)
  {
    printf("Test52 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=4;
  actual=Test53();
  if (actual != expected)
  {
    printf("Test53 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=12;
  actual=Test_struct_sizeof1();
  if (actual != expected)
  {
    printf("Test struct_sizeof1 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=12;
  actual=Test_struct_sizeof2();
  if (actual != expected)
  {
    printf("Test struct_sizeof2 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
 expected=16;
  actual=Test_struct_sizeof3();
  if (actual != expected)
  {
    printf("Test struct_sizeof3 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }

  return 0;
}