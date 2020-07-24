/* extern declared functions and variables */

typedef struct {
  int gp_offset;
  int fp_offset;
  void *overflow_arg_area;
  void *reg_save_area;
} __va_elem;

typedef __va_elem va_list[1];

void va_end(__va_elem *ap) {}

extern void printf(char *fmt, ...);
extern void vprintf(char *fmt, __va_elem *ap, ...);

void printf_wrap(char *fmt, ...);

void printf_wrap(char *fmt, ...) {
  va_list ap;
  va_start(ap,fmt);
  vprintf(fmt, ap);
  va_end(ap);
}

int va_func_test() {
  printf_wrap("va_func_test %d %c %s\n", 42, '$', "OK!");
  return 0;
}

int run_va_func_tests() {
  int expected;
  int actual;

  expected=0;
  actual=va_func_test();
  if (actual != expected)
  {
    printf("va_func_test faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }

  return 0;
}
