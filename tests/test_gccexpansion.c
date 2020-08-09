/* extern declared functions and variables */
extern void printf(char *fmt, ...);

int statement_expression_test1() {
  int x = ({int y = 2; y*y;});
  return x;
}

int statement_expression_test2() {
  int x;
  x = ({int y = 2; y*y;});
  return x;
}

int statement_expression_test3_helper(int x) {
  return x;
}

int statement_expression_test3() {
  int x = statement_expression_test3_helper(({int y = 2; y*y;}));
  return x;
}

int run_gccexpansion_tests() {
  int expected;
  int actual;

  expected=4;
  actual=statement_expression_test1();
  if (actual != expected)
  {
    printf("statement_expression_test1 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }

  expected=4;
  actual=statement_expression_test2();
  if (actual != expected)
  {
    printf("statement_expression_test2 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }

  expected=4;
  actual=statement_expression_test3();
  if (actual != expected)
  {
    printf("statement_expression_test3 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }

  return 0;
}