/* extern declared functions and variables */
extern void printf(char *fmt, ...);

// Test functions
int and_test_true()
{
  int a = 1;
  int b = 1;
  return a && b;
}

int and_test_false1()
{
  int a = 0;
  int b = 1;
  return a && b;
}

int and_test_false2()
{
  int a = 1;
  int b = 0;
  return a && b;
}

int and_test_false3()
{
  int a = 0;
  int b = 0;
  return a && b;
}

int or_test_true1()
{
  int a = 1;
  int b = 1;
  return a || b;
}

int or_test_true2()
{
  int a = 0;
  int b = 1;
  return a || b;
}

int or_test_true3()
{
  int a = 1;
  int b = 0;
  return a || b;
}

int or_test_false()
{
  int a = 0;
  int b = 0;
  return a || b;
}

int not_test_true() {
  int a = 1;
  return !a;
}

int not_test_negative() {
  int a = 0;
  return !a;
}

int run_logical_ops_tests() {
  int expected;
  int actual;

  expected=1;
  actual=and_test_true();
  if (actual != expected)
  {
    printf("and_test_true faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }

  expected=0;
  actual=and_test_false1();
  if (actual != expected)
  {
    printf("and_test_false1 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }

  expected=0;
  actual=and_test_false2();
  if (actual != expected)
  {
    printf("and_test_false2 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }

  expected=0;
  actual=and_test_false3();
  if (actual != expected)
  {
    printf("and_test_false3 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }

  expected=1;
  actual=or_test_true1();
  if (actual != expected)
  {
    printf("or_test_true1 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }

  expected=1;
  actual=or_test_true2();
  if (actual != expected)
  {
    printf("or_test_true2 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }

  expected=1;
  actual=or_test_true3();
  if (actual != expected)
  {
    printf("or_test_true3 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }

  expected=0;
  actual=or_test_false();
  if (actual != expected)
  {
    printf("or_test_false faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }

  expected=0;
  actual=not_test_true();
  if (actual != expected)
  {
    printf("not_test_true faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }

  expected=1;
  actual=not_test_negative();
  if (actual != expected)
  {
    printf("not_test_negative faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }

  return 0;
}