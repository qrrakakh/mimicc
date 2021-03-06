/* extern declared functions and variables */
extern void printf(char *fmt, ...);

char str_gvar_init[4] = "abc";
char global_string_array_variable_length[][5] = {"ab", "abc", "abcd"};

// Test functions
int Test2() 
{
  char *x; x = "abc"; return x[0];
}
int Test3() 
{
  char *x; x = "abc"; return x[1];
}
int Test4() 
{
  char *x = "abc"; return x[2];
}
int Test5() 
{
  char a; a='a'; return a;
}
int Test6() 
{
  char a; a='b'; return a;
}
int Test7() 
{
  char a; a='1'; return a;
}
int Test8() 
{
  char a; a='9'; return a;
}
int Test9() 
{
  char a; return sizeof(a);
}
char Test134() {
  char a='a', b='b';
  return b;
}

int escape_char_test1() {
  char e0 = '\\';
  char e1 = '\'';
  char e2 = '\E';
  return (((e0 << 8) + e1) << 8) + e2;
}

int escape_char_test2() {
  char e0 = '\0';
  char e1 = '\1';
  char e2 = '\2';
  char e3 = '\3';
  return (((((e0 << 8) + e1) << 8) + e2) << 8) + e3;
}

int escape_char_test3() {
  char e0 = '\4';
  char e1 = '\5';
  char e2 = '\6';
  char e3 = '\7';
  return (((((e0 << 8) + e1) << 8) + e2) << 8) + e3;
}

int escape_char_test4() {
  char e0 = '\a';
  char e1 = '\b';
  char e2 = '\e';
  char e3 = '\f';
  return (((((e0 << 8) + e1) << 8) + e2) << 8) + e3;
}

int escape_char_test5() {
  char e0 = '\n';
  char e1 = '\r';
  char e2 = '\t';
  char e3 = '\v';
  return (((((e0 << 8) + e1) << 8) + e2) << 8) + e3;
}

int escape_string_lvar_a_test1() {
  char z[] = "\\\"\E";
  return (((z[0] << 8) + z[1]) << 8) + z[2];
}

int escape_string_lvar_a_test2() {
  char z[] = "\0\1\2\3";
  return (((((z[0] << 8) + z[1]) << 8) + z[2]) << 8) + z[3];
}

int escape_string_lvar_a_test3() {
  char z[] = "\4\5\6\7";
  return (((((z[0] << 8) + z[1]) << 8) + z[2]) << 8) + z[3];
}

int escape_string_lvar_a_test4() {
  char z[] = "\a\b\e\f";
  return (((((z[0] << 8) + z[1]) << 8) + z[2]) << 8) + z[3];
}

int escape_string_lvar_a_test5() {
  char z[] = "\n\r\t\v";
  return (((((z[0] << 8) + z[1]) << 8) + z[2]) << 8) + z[3];
}

int escape_string_lvar_b_test1() {
  char *z;
  z = "\\\"\E";
  return (((z[0] << 8) + z[1]) << 8) + z[2];
}

int escape_string_lvar_b_test2() {
  char *z;
  z = "\0\1\2\3";
  return (((((z[0] << 8) + z[1]) << 8) + z[2]) << 8) + z[3];
}

int escape_string_lvar_b_test3() {
  char *z;
  z = "\4\5\6\7";
  return (((((z[0] << 8) + z[1]) << 8) + z[2]) << 8) + z[3];
}

int escape_string_lvar_b_test4() {
  char *z;
  z = "\a\b\e\f";
  return (((((z[0] << 8) + z[1]) << 8) + z[2]) << 8) + z[3];
}

int escape_string_lvar_b_test5() {
  char *z;
  z = "\n\r\t\v";
  return (((((z[0] << 8) + z[1]) << 8) + z[2]) << 8) + z[3];
}

char esga1[] = "\\\"\E";
char esga2[] = "\0\1\2\3";
char esga3[] = "\4\5\6\7";
char esga4[] = "\a\b\e\f";
char esga5[] = "\n\r\t\v";

char *esgb1 = "\\\"\E";
char *esgb2 = "\0\1\2\3";
char *esgb3 = "\4\5\6\7";
char *esgb4 = "\a\b\e\f";
char *esgb5 = "\n\r\t\v";

int escape_string_gvar_a_test1() {
  return (((esga1[0] << 8) + esga1[1]) << 8) + esga1[2];
}

int escape_string_gvar_a_test2() {
  return (((((esga2[0] << 8) + esga2[1]) << 8) + esga2[2]) << 8) + esga2[3];
}

int escape_string_gvar_a_test3() {
  return (((((esga3[0] << 8) + esga3[1]) << 8) + esga3[2]) << 8) + esga3[3];
}

int escape_string_gvar_a_test4() {
  return (((((esga4[0] << 8) + esga4[1]) << 8) + esga4[2]) << 8) + esga4[3];
}

int escape_string_gvar_a_test5() {
  return (((((esga5[0] << 8) + esga5[1]) << 8) + esga5[2]) << 8) + esga5[3];
}

int escape_string_gvar_b_test1() {
  return (((esgb1[0] << 8) + esgb1[1]) << 8) + esgb1[2];
}

int escape_string_gvar_b_test2() {
  return (((((esgb2[0] << 8) + esgb2[1]) << 8) + esgb2[2]) << 8) + esgb2[3];
}

int escape_string_gvar_b_test3() {
  return (((((esgb3[0] << 8) + esgb3[1]) << 8) + esgb3[2]) << 8) + esgb3[3];
}

int escape_string_gvar_b_test4() {
  return (((((esgb4[0] << 8) + esgb4[1]) << 8) + esgb4[2]) << 8) + esgb4[3];
}

int escape_string_gvar_b_test5() {
  return (((((esgb5[0] << 8) + esgb5[1]) << 8) + esgb5[2]) << 8) + esgb5[3];
}

char string_initializer_test() {
  char a[7] = "abcdef";
  return a[3];
}

char string_array_variable_length_test() {
  char a[][5] = {"ab", "abc", "abcd"};
  return a[2][2];
}

int run_char_strings_tests() {
  int expected;
  int actual;
  expected=97;
  actual=Test2();
  if (actual != expected)
  {
    printf("Test2 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=98;
  actual=Test3();
  if (actual != expected)
  {
    printf("Test3 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=99;
  actual=Test4();
  if (actual != expected)
  {
    printf("Test4 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=97;
  actual=Test5();
  if (actual != expected)
  {
    printf("Test5 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=98;
  actual=Test6();
  if (actual != expected)
  {
    printf("Test6 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=49;
  actual=Test7();
  if (actual != expected)
  {
    printf("Test7 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=57;
  actual=Test8();
  if (actual != expected)
  {
    printf("Test8 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=1;
  actual=Test9();
  if (actual != expected)
  {
    printf("Test9 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=98;
  actual=Test134();
  if (actual != expected)
  {
    printf("Test134 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=98;
  actual=str_gvar_init[1];
  if (actual != expected)
  {
    printf("str_gvar_init test faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=100;
  actual=string_initializer_test();
  if (actual != expected)
  {
    printf("string_initializer_test faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=99;
  actual=string_array_variable_length_test();
  if (actual != expected)
  {
    printf("string_array_variable_length_test faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }

  expected=99;
  actual=global_string_array_variable_length[2][2];
  if (actual != expected)
  {
    printf("string_array_variable_length_test faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }

  expected=(((92 << 8) + 39)<< 8) + 27;
  actual=escape_char_test1();
  if (actual != expected)
  {
    printf("escape_char_test1 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }

  expected=(((((0 << 8) + 1)<< 8) + 2)<<8) + 3;
  actual=escape_char_test2();
  if (actual != expected)
  {
    printf("escape_char_test2 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }

  expected=(((((4 << 8) + 5)<< 8) + 6)<<8) + 7;
  actual=escape_char_test3();
  if (actual != expected)
  {
    printf("escape_char_test3 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }

  expected=(((((7 << 8) + 8)<< 8) + 27)<<8) + 12;
  actual=escape_char_test4();
  if (actual != expected)
  {
    printf("escape_char_test4 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }

  expected=(((((10 << 8) + 13)<< 8) + 9)<<8) + 11;
  actual=escape_char_test5();
  if (actual != expected)
  {
    printf("escape_char_test5 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }

  expected=(((92 << 8) + 34)<< 8) + 27;
  actual=escape_string_lvar_a_test1();
  if (actual != expected)
  {
    printf("escape_string_lvar_a_test1 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }

  expected=(((((0 << 8) + 1)<< 8) + 2)<<8) + 3;
  actual=escape_string_lvar_a_test2();
  if (actual != expected)
  {
    printf("escape_string_lvar_a_test2 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }

  expected=(((((4 << 8) + 5)<< 8) + 6)<<8) + 7;
  actual=escape_string_lvar_a_test3();
  if (actual != expected)
  {
    printf("escape_string_lvar_a_test3 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }

  expected=(((((7 << 8) + 8)<< 8) + 27)<<8) + 12;
  actual=escape_string_lvar_a_test4();
  if (actual != expected)
  {
    printf("escape_string_lvar_a_test4 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }

  expected=(((((10 << 8) + 13)<< 8) + 9)<<8) + 11;
  actual=escape_string_lvar_a_test5();
  if (actual != expected)
  {
    printf("escape_string_lvar_a_test5 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }

  expected=(((92 << 8) + 34)<< 8) + 27;
  actual=escape_string_lvar_b_test1();
  if (actual != expected)
  {
    printf("escape_string_lvar_b_test1 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }

  expected=(((((0 << 8) + 1)<< 8) + 2)<<8) + 3;
  actual=escape_string_lvar_b_test2();
  if (actual != expected)
  {
    printf("escape_string_lvar_b_test2 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }

  expected=(((((4 << 8) + 5)<< 8) + 6)<<8) + 7;
  actual=escape_string_lvar_b_test3();
  if (actual != expected)
  {
    printf("escape_string_lvar_b_test3 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }

  expected=(((((7 << 8) + 8)<< 8) + 27)<<8) + 12;
  actual=escape_string_lvar_b_test4();
  if (actual != expected)
  {
    printf("escape_string_lvar_b_test4 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }

  expected=(((((10 << 8) + 13)<< 8) + 9)<<8) + 11;
  actual=escape_string_lvar_b_test5();
  if (actual != expected)
  {
    printf("escape_string_lvar_b_test5 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }

 expected=(((92 << 8) + 34)<< 8) + 27;
  actual=escape_string_gvar_a_test1();
  if (actual != expected)
  {
    printf("escape_string_gvar_a_test1 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }

  expected=(((((0 << 8) + 1)<< 8) + 2)<<8) + 3;
  actual=escape_string_gvar_a_test2();
  if (actual != expected)
  {
    printf("escape_string_gvar_a_test2 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }

  expected=(((((4 << 8) + 5)<< 8) + 6)<<8) + 7;
  actual=escape_string_gvar_a_test3();
  if (actual != expected)
  {
    printf("escape_string_gvar_a_test3 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }

  expected=(((((7 << 8) + 8)<< 8) + 27)<<8) + 12;
  actual=escape_string_gvar_a_test4();
  if (actual != expected)
  {
    printf("escape_string_gvar_a_test4 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }

  expected=(((((10 << 8) + 13)<< 8) + 9)<<8) + 11;
  actual=escape_string_gvar_a_test5();
  if (actual != expected)
  {
    printf("escape_string_gvar_a_test5 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }

 expected=(((92 << 8) + 34)<< 8) + 27;
  actual=escape_string_gvar_b_test1();
  if (actual != expected)
  {
    printf("escape_string_gvar_b_test1 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }

  expected=(((((0 << 8) + 1)<< 8) + 2)<<8) + 3;
  actual=escape_string_gvar_b_test2();
  if (actual != expected)
  {
    printf("escape_string_gvar_b_test2 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }

  expected=(((((4 << 8) + 5)<< 8) + 6)<<8) + 7;
  actual=escape_string_gvar_b_test3();
  if (actual != expected)
  {
    printf("escape_string_gvar_b_test3 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }

  expected=(((((7 << 8) + 8)<< 8) + 27)<<8) + 12;
  actual=escape_string_gvar_b_test4();
  if (actual != expected)
  {
    printf("escape_string_gvar_b_test4 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }

  expected=(((((10 << 8) + 13)<< 8) + 9)<<8) + 11;
  actual=escape_string_gvar_b_test5();
  if (actual != expected)
  {
    printf("escape_string_gvar_b_test5 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }

}