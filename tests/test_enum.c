/* extern declared functions and variables */
extern void printf();

// Global variables and helper functions
enum enum1 {
  a1, b1, c1
} enum1_inst;

enum enum2 {
  a2, b2=0, c2
} enum2_inst;

typedef enum {
  x, y, z
} enum4;

int enumtest_01_1() {
  return a1;
}

int enumtest_01_2() {
  return b1;
}

int enumtest_01_3() {
  return c1;
}

int enumtest_01_4() {
  enum enum1 z;
  z=a1;
  return z;
}

int enumtest_01_5() {
  int x;
  enum enum1 z;
  z=a1;
  x = z;
  return x;
}

int enumtest_02_1() {
  return a2;
}

int enumtest_02_2() {
  return b2;
}

int enumtest_02_3() {
  return c2;
}

int enumtest_02_4() {
  enum2_inst=b2;
  return enum2_inst;
}

int enumtest_03_1() {
  enum enum3 {
    a3=1, b3=0, c3=20
  };
  return a3;
}

int enumtest_03_2() {
  enum enum3 {
    a3=1, b3=0, c3=20
  };
  return b3;
}

int enumtest_03_3() {
  enum enum3 {
    a3=1, b3=0, c3=1<<4
  };
  return c3;
}

int enumtest_03_4() {
  enum enum3 {
    a3=1, b3=0, c3=20
  };
  enum enum3 z;
  z=a3;
  return z;
}

int enumtest_04_1() {
  enum4 v = x;
  return v;
}

int enumtest_04_2() {
  typedef enum {
    x,y,z
  } enum4l;
  enum4l v = x;
  return v;
}

int run_enum_tests() {
  int expected;
  int actual;

  expected=0;
  actual=enumtest_01_1();
  if (actual != expected)
  {
    printf("enumtest_01_1 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }

  expected=1;
  actual=enumtest_01_2();
  if (actual != expected)
  {
    printf("enumtest_01_2 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }

  expected=2;
  actual=enumtest_01_3();
  if (actual != expected)
  {
    printf("enumtest_01_3 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }

  expected=0;
  actual=enumtest_01_4();
  if (actual != expected)
  {
    printf("enumtest_01_4 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }

  expected=0;
  actual=enumtest_01_5();
  if (actual != expected)
  {
    printf("enumtest_01_5 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }

  expected=0;
  actual=enumtest_02_1();
  if (actual != expected)
  {
    printf("enumtest_02_1 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }

  expected=0;
  actual=enumtest_02_2();
  if (actual != expected)
  {
    printf("enumtest_02_2 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }

  expected=1;
  actual=enumtest_02_3();
  if (actual != expected)
  {
    printf("enumtest_02_3 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }

  expected=0;
  actual=enumtest_02_4();
  if (actual != expected)
  {
    printf("enumtest_02_4 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }

  expected=1;
  actual=enumtest_03_1();
  if (actual != expected)
  {
    printf("enumtest_03_1 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }

  expected=0;
  actual=enumtest_03_2();
  if (actual != expected)
  {
    printf("enumtest_03_2 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }

  expected=16;
  actual=enumtest_03_3();
  if (actual != expected)
  {
    printf("enumtest_03_3 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }

  expected=1;
  actual=enumtest_03_4();
  if (actual != expected)
  {
    printf("enumtest_03_4 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }

  expected=0;
  actual=enumtest_04_1();
  if (actual != expected)
  {
    printf("enumtest_04_1 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }

  expected=0;
  actual=enumtest_04_2();
  if (actual != expected)
  {
    printf("enumtest_04_2 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }

  return 0;
}