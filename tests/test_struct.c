struct structTest149_152 {
  int v1;
  char c1;
  char c2;
  int v2;
  int v3;
} var_structTest149;
int TestHelperFunction151(struct structTest149_152 *s) { return s->v1; }
char TestHelperFunction152(struct structTest149_152 *s) {return s->c1; }
int Test149() {
  struct structTest149_152 s;
  (&var_structTest149)->v3 = 4;
  (&s)->v1 = 0;
  (&s)->v2 = 2;
  return (&s)->v1 + (&s)->v2 + (&var_structTest149)->v3;
}
char Test150() {
  struct structTest149_152 {
    char c1, c2, c3, c4, c5, c6, c7;
  } s;
  (&s)->c1 = 'a';
  (&s)->c2 = 'b';
  (&s)->c3 = 'c';
  (&s)->c4 = 'd';
  (&s)->c5 = 'e';
  (&s)->c6 = 'f';
  (&s)->c7 = 'g';
  return (&s)->c7;
}
int Test151() {
  struct structTest149_152 s;
  (&s)->v1 = 8;
  return TestHelperFunction151(&s);
}
char Test152() {
  struct structTest149_152 s;
  (&s)->c1 = 'c';
  return TestHelperFunction152(&s);
}
int Test153() {
  struct structTest149_152 s;
  var_structTest149.v3 = 4;
  s.v1 = 0;
  s.v2 = 2;
  return s.v1 + s.v2 + var_structTest149.v3;
}
char Test154() {
  struct structTest149_152 {
    char c1, c2, c3, c4, c5, c6, c7;
  } s;
  s.c1 = 'a';
  s.c2 = 'b';
  s.c3 = 'c';
  s.c4 = 'd';
  s.c5 = 'e';
  s.c6 = 'f';
  s.c7 = 'g';
  return s.c7;
}
char Test155() {
  struct {
    char c1, c2, c3, c4, c5, c6, c7;
  } s;
  s.c1 = 'a';
  s.c2 = 'b';
  s.c3 = 'c';
  s.c4 = 'd';
  s.c5 = 'e';
  s.c6 = 'f';
  s.c7 = 'g';
  return s.c5;
}
int run_struct_tests() {
  int expected;
  int actual;

  expected=6;
  actual=Test149();
  if (actual != expected)
  {
    printf("Test149 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected='g';
  actual=Test150();
  if (actual != expected)
  {
    printf("Test150 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=8;
  actual=Test151();
  if (actual != expected)
  {
    printf("Test151 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected='c';
  actual=Test152();
  if (actual != expected)
  {
    printf("Test152 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=6;
  actual=Test153();
  if (actual != expected)
  {
    printf("Test153 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected='g';
  actual=Test154();
  if (actual != expected)
  {
    printf("Test154 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected='e';
  actual=Test155();
  if (actual != expected)
  {
    printf("Test155 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  return 0;
}