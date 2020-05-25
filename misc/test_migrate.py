import re
if __name__=='__main__':
  in_file='./test.sh'

  test_case_match = r'^assert (\d+) \'(.*)int main\(\)\s*\{(.*)\}.*\''
  
  case_count=1
  case_dict = {}
  global_list = []
  with open(in_file, 'r') as rh:
    for line in rh:
      m = re.match(test_case_match, line.strip())
      if m:
        expected = int(m[1].strip())
        global_def = m[2].strip().replace('\\\'', '')
        test_code = m[3].strip().replace('\\\'', '')
        case_dict[case_count] = (expected, test_code)
        if len(global_def)>0:
          global_list.append(global_def)
        case_count += 1

  for g in global_list:
    print(g)
  for k in case_dict.keys():
    print("int test{}() ".format(k))
    print("{")
    print("  "+case_dict[k][1])
    print("}")
  
  print("int main() {")
  print("  int expected;")
  print("  int actual;")
  print("")

  for k in case_dict.keys():
    print("  expected={};".format(case_dict[k][0]))
    print("  actual=test{}();".format(k))
    print("  if (actual != expected)")
    print("  {")
    print("    printf(\"test{} faild, %d expected, but got %d\\n\", expected, actual);".format(k))
    print("    return 1;")
    print("  }")
  print("  return 0;")
  print("}")
