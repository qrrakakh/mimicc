// Test functions
extern int run_basic_tests();
extern int run_basic_variables_tests();
extern int run_pointers_tests();
extern int run_char_strings_tests();
extern int run_control_tests();
extern int run_funccall_tests();
extern int run_struct_tests();
extern int run_sizeof_tests();
extern int run_enum_tests();
extern int run_typedef_tests();
extern int run_logical_ops_tests();

// main function
int main() {
  if(run_basic_tests()!=0) {
    return 1;
  }
  if(run_basic_variables_tests()!=0) {
    return 1;
  }
  if(run_pointers_tests()!=0) {
    return 1;
  }
  if(run_char_strings_tests()!=0) {
    return 1;
  }
  if(run_control_tests()!=0) {
    return 1;
  }
  if(run_funccall_tests()!=0) {
    return 1;
  }
  if(run_struct_tests()!=0) {
    return 1;
  }
  if(run_sizeof_tests()!=0) {
    return 1;
  }
  if(run_enum_tests()!=0) {
    return 1;
  }
  if(run_typedef_tests()!=0) {
    return 1;
  }
  if(run_logical_ops_tests()!=0) {
    return 1;
  }

  return 0;
}
