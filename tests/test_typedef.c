/* extern declared functions and variables */
extern void printf();

typedef int IntType, *IntPtrType;
typedef char CharType, **CharPtrPtrType;

struct st1 {
  int x;
};

typedef struct st1 St1Type;

typedef struct {
  int y;
} St2Type;

// typedef struct st3 St3Type;

// struct st3 {
//   int z;
// };

int int_typedef_test() {
  IntType x = 1;
  return 1;
}

int char_typedef_test() {
  CharType x = 1;
  return 1;
}

int int_ptr_typedef_test() {
  IntType x = 1;
  IntPtrType y = &x;
  *y = 2;
  return x;
}

int char_ptrptr_typedef_test() {
  CharType x = 1;
  CharPtrPtrType y;
  char *z = &x;
  y = &z;
  **y = 2;
  return x;
}

int st1_test() {
  St1Type s;
  s.x = 1;
  return s.x;
}

int st2_test() {
  St2Type s;
  s.y = 1;
  return s.y;
}

// int st3_test() {
//   St3Type s;
//   s.z = 1;
//   return s.z;
// }

int run_typedef_tests() {
  int expected;
  int actual;

  expected=1;
  actual=int_typedef_test();
  if (actual != expected)
  {
    printf("int_typedef_test faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }

  expected=1;
  actual=char_typedef_test();
  if (actual != expected)
  {
    printf("char_typedef_test faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }

  expected=2;
  actual=int_ptr_typedef_test();
  if (actual != expected)
  {
    printf("int_ptr_typedef_test faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }

  expected=2;
  actual=char_ptrptr_typedef_test();
  if (actual != expected)
  {
    printf("char_ptrptr_typedef_test faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }

  expected=1;
  actual=st1_test();
  if (actual != expected)
  {
    printf("st1_test faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }

  expected=1;
  actual=st2_test();
  if (actual != expected)
  {
    printf("st2_test faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }

  // expected=1;
  // actual=st3_test();
  // if (actual != expected)
  // {
  //   printf("st3_test faild, %d expected, but got %d\n", expected, actual);
  //   return 1;
  // }

  return 0;
}