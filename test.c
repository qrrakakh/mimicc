// Global variables and helper functions
int var_Test11, *var_Test12;
int var_Test13[2];
int var_Test14;int TestHelperFunction14(int z) { return var_Test14+z;}
int var_Test15[3];int TestHelperFunction15(int z) { return var_Test15[2]+z;}
int var_Test16;int hepler_Test16() {int a; a=3; return a;}
int var_Test17;int TestHelperFunction17() {int a; a=3; return a;}
int var_Test18;int TestHelperFunction18() {return var_Test18;}
int var_Test19;int TestHelperFunction19() {return var_Test19;}
int fib(int a){if(a==0) return 0;else if(a==1) return 1; else return fib(a-2)+fib(a-1);}
int TestHelperFunction58(int a){return a*2;}
int TestHelperFunction61(){int a;a=1;return a;}
int TestHelperFunction62(){int a;a=1;return a;}
int TestHelperFunction65_Test72(int a,int b,int c,int d,int e,int f){return a+b*2+c*4+d*8+e*16+f*32;}
int var_Test137; int *TestHelperFunction137(int x) { var_Test137=x; return &var_Test137; }
void TestHelperFunction138_voidfunc() {printf("void func runs correctly\n"); return;}

/* extern declared functions and variables */
extern int a_test_func;
extern int arr_test_func[4];
extern int Foo();
extern int PrintInt(int a);
extern int Alloc4(int **p, int a, int b, int c, int d);

// Test functions
int Test1() 
{
  int a, b;a=1;b=0; {int a;a=2;b=b+a;} {int a;a=4;{int a;a=8;b=b+a;}} {int a;a=16;{int a;a=32;}b=b+a;} return b;
}
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
  char *x; x = "abc"; return x[2];
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
int Test10() 
{
  char x[3]; x[0]=-1;x[1]=2;int y;y=4;return x[0]+y;
}
int Test11() 
{
  var_Test11 = 1;return var_Test11;
}
int Test12() 
{
  int a[2];var_Test12=&a;*var_Test12 = 1;*(a + 1) = 2;return *a;
}
int Test13() 
{
  *var_Test13 = 1;*(var_Test13 + 1) = 2;return *var_Test13;
}
int Test14() 
{
  var_Test14=3; return TestHelperFunction14(20);
}
int Test15() 
{
  var_Test15[0]=1;var_Test15[1]=2;var_Test15[2]=3; return TestHelperFunction15(20);
}
int Test16() 
{
  var_Test16=1; return hepler_Test16();
}
int Test17() 
{
  var_Test17=1; TestHelperFunction17();return var_Test17;
}
int Test18() 
{
  var_Test18=1; int var_Test18; var_Test18=2;return TestHelperFunction18();
}
int Test19() 
{
  var_Test19=1; int var_Test19; var_Test19=2;TestHelperFunction19();return var_Test19;
}
int Test20() 
{
  int a[2];*a = 1;*(a + 1) = 2;return *a;
}
int Test21() 
{
  int a[2];*a = 1;*(a + 1) = 2;return *(a+1);
}
int Test22() 
{
  int a[2];*a = 1;*(a + 1) = 2;int *p;p = a;return *p + *(p + 1);
}
int Test23() 
{
  int a[2];int *p;p = a;*p = 1;*(p + 1) = 2;return *a + *(a + 1);
}
int Test24() 
{
  int a[2];a[0] = 1;a[1] = 2;return a[0];
}
int Test25() 
{
  int a[2];a[0] = 1;a[1] = 2;return a[1];
}
int Test26() 
{
  int a[2];a[0] = 1;a[1] = 2;int *p;p = a;return *p + *(p + 1);
}
int Test27() 
{
  int a[2];a[0] = 1;a[1] = 2;int *p;p = a;return p[0] + p[1];
}
int Test28() 
{
  int a[2];int *p;p = a;*p = 1;*(p + 1) = 2;return a[0] + a[1];
}
int Test29() 
{
  int a[2];int b;a[0]=1;a[1]=2;b=0;return a[b];
}
int Test30() 
{
  int a[2];int b;a[0]=1;a[1]=2;b=0;return a[b+1];
}
int Test31() 
{
  int a[2];int b;a[0]=1;a[1]=2;b=1;return a[0];
}
int Test32() 
{
  int a[2];int b;a[0]=1;a[1]=2;b=1;return a[1];
}
int Test33() 
{
  int a[100];int b;a[0]=1;a[1]=2;b=1;return a[1];
}
int Test34() 
{
  int a[3];int b;*a=1;*(a+1)=2;*(a+2)=3;b=1;return *a;
}
int Test35() 
{
  int a[3];int b;a[0]=1;a[1]=2;a[2]=3;b=1;return a[0];
}
int Test36() 
{
  int a[3];int b;a[0]=1;a[1]=2;a[2]=3;b=1;return a[1];
}
int Test37() 
{
  int a[3];int b;a[0]=1;a[1]=2;a[2]=3;b=1;return a[2];
}
int Test38() 
{
  int a[3];int b;a[0]=1;a[1]=2;a[2]=3;b=1;return a[b*2];
}
int Test39() 
{
  int a[4];int b;a[0]=1;a[1]=2;a[2]=3;a[3]=4;b=1;return a[0];
}
int Test40() 
{
  int a[4];int b;a[0]=1;a[1]=2;a[2]=3;a[3]=4;b=1;return a[1];
}
int Test41() 
{
  int a[4];int b;a[0]=1;a[1]=2;a[2]=3;a[3]=4;b=1;return a[2];
}
int Test42() 
{
  int a[4];int b;a[0]=1;a[1]=2;a[2]=3;a[3]=4;b=1;return a[3];
}
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
int Test55() 
{
  int *p;Alloc4(&p, 1, 2, 4, 8);int *q;q = p + 2;*q;q = p + 3;return *q;
}
int Test56() 
{
  int x; int *y;y=&x;*y=3;return x;
}
int Test57() 
{
  PrintInt(fib(10)); return fib(10);
}
int Test58() 
{
  return TestHelperFunction58(1);
}
int Test59() 
{
  int a;int *b;a=1;b=&a;return *b;
}
int Test60() 
{
  18%7; return Foo();
}
int Test61() 
{
  int a; int b; a=0;a=TestHelperFunction61();return a;
}
int Test62() 
{
  int a; int b; a=0;b=TestHelperFunction62();return a;
}
int Test63() 
{
  18%7;return Foo();
}
int Test64() 
{
  Foo();return 18%7;
}
int Test65() 
{
  return TestHelperFunction65_Test72(0,0,0,0,0,0);
}
int Test66() 
{
  return TestHelperFunction65_Test72(1,0,0,0,0,0);
}
int Test67() 
{
  return TestHelperFunction65_Test72(0,1,0,0,0,0);
}
int Test68() 
{
  return TestHelperFunction65_Test72(0,0,1,0,0,0);
}
int Test69() 
{
  return TestHelperFunction65_Test72(0,0,0,1,0,0);
}
int Test70() 
{
  return TestHelperFunction65_Test72(0,0,0,0,1,0);
}
int Test71() 
{
  return TestHelperFunction65_Test72(0,0,0,0,0,1);
}
int Test72() 
{
  return TestHelperFunction65_Test72(TestHelperFunction65_Test72(0,0,0,0,0,1),0,0,0,0,0);
}
int Test73() 
{
  return 18%7;
}
int Test74() 
{
  return 23%4;
}
int Test75() 
{
  return 94%11;
}
int Test76() 
{
  {} return 3;
}
int Test77() 
{
  return 3;
}
int Test78() 
{
  int a; int b; a=0;b=1;if(a>1) b=2;else b=3;return b;
}
int Test79() 
{
  int a; int b; a=0;b=1;if(a>1) b=2;return b;
}
int Test80() 
{
  int a; int b; a=2;b=1;if(a>1) b=2;else b=3;return b;
}
int Test81() 
{
  int a; int b; a=2;b=1;if(a>1) b=2;return b;
}
int Test82() 
{
  int a; int b; b=0;for(a=0;a<10;a=a+1) b=b+2; return b;
}
int Test83() 
{
  int a;a=12;while (a<=10) a=a+1;return a;
}
int Test84() 
{
  int a;a=0;while (a<10) a=a+1; return a;
}
int Test85() 
{
  int a;a=12;while (a<=10) a=a+1; return a;
}
int Test86() 
{
  int a;a=0;while (a<10) a=a+1;return a;
}
int Test87() 
{
  int a;a=1;int b;for(b=0;b<10;b=b+1) while(a<10) a=a+1; if(a==10) return 1;else return 2;
}
int Test88() 
{
  int a;a=1;int b;for(b=0;b<10;b=b+1) while(a<9) a=a+1; if(a==10) return 1;else return b;
}
int Test89() 
{
  int a; int c; int b;a=1;c=0;for(b=0;b<10;b=b+1) { a=a+ 1;c=c+2;} if(a==10) return 1;else return a+c;
}
int Test90() 
{
  int a;a=11;return a<10;
}
int Test91() 
{
  int a;a=2;return a<10;
}
int Test92() 
{
  int a;a=10;return a<10;
}
int Test93() 
{
  int a;a=10;return a<=10;
}
int Test94() 
{
  int a;a=4;return a;
}
int Test95() 
{
  int a;a=12;return a;
}
int Test96() 
{
  return 0;
}
int Test97() 
{
  return 42;
}
int Test98() 
{
  return 5+20-4;
}
int Test99() 
{
  return  12 + 34 - 5 ;
}
int Test100() 
{
  return 5+6*7;
}
int Test101() 
{
  return 5*(9-6);
}
int Test102() 
{
  return (3+5)/2;
}
int Test103() 
{
  return (+3+5)/2;
}
int Test104() 
{
  return (-3+5)/2;
}
int Test105() 
{
  return (5+1)==6;
}
int Test106() 
{
  return 8==(4-2);
}
int Test107() 
{
  return (5+1)!=6;
}
int Test108() 
{
  return 8!=(4-2);
}
int Test109() 
{
  return 4<4*2;
}
int Test110() 
{
  return 4<=4*1;
}
int Test111() 
{
  return 4<4+1;
}
int Test112() 
{
  return 4*2<4;
}
int Test113() 
{
  return 4<=2+1;
}
int Test114() 
{
  return 4>4*2;
}
int Test115() 
{
  return 4>=4*1;
}
int Test116() 
{
  return 4>4+1;
}
int Test117() 
{
  return 4*2>4;
}
int Test118() 
{
  return 4>=2+1;
}
int Test119() 
{
  int a; int b;a=3;b=5*6-8;return a+b/2;
}
int Test120() 
{
  int a; int b; int c;a=1;b=2;c=4;return (b+c)/2+a;
}
int Test121() 
{
  int f2o; int bar_a; int b1zz;f2o=1;bar_a=2;b1zz=4;return (bar_a+b1zz)/2+f2o;
}
int Test122() 
{
  int f2o; int bar_a; int b1zz;f2o=1;bar_a=2;b1zz=4;(bar_a+b1zz)/2+f2o;return (f2o+b1zz)*bar_a;
}
int Test123() 
{
  int a; a=1;
  return ++a;
}
int Test124() 
{
  int a; a=1;
  ++a;
  return a;
}
int Test125() 
{
  int a; a=1;
  return --a;
}
int Test126() 
{
  int a; a=1;
  --a;
  return a;
}
int Test127() 
{
  int a; a=1;
  return a++;
}
int Test128() 
{
  int a; a=1;
  a++;
  return a;
}
int Test129() 
{
  int a; a=1;
  return a--;
}
int Test130() 
{
  int a; a=1;
  a--;
  return a;
}
int Test131() {
  int a=1;
  return a;
}
int Test132() {
  int a=1,b=2;
  return a+b;
}
char Test133() {
  int a='a';
  return a;
}
char Test134() {
  char a='a', b='b';
  return b;
}
int Test135() {
  a_test_func=1;
  return a_test_func;
}
int Test136() {
  arr_test_func[0]=0;
  arr_test_func[1]=2;
  arr_test_func[2]=4;
  return arr_test_func[2];
}
int Test137() {
  int *z = TestHelperFunction137(10);
  return *z;
}
int Test138() {
  TestHelperFunction138_voidfunc();
  return 1;
}
int Test139() {
  int i=100, b=0;
  for(int i=0;i<10;++i)
    b=b+i;
  return i+b;
}
int Test140() {
  int i=50, b=0;
  for(int i=0;i<10;++i) {
    b=b+i;
  }
  return i+b;
}
int Test141() {
  int a=0;
  for(int i=0;i<100;++i) {
    if(i%2==0) continue;
    a=a+i;
  }
  return a; // 2500
}

int Test142() {
  int a=0, b=49, c=3;
  for(int i=0;i<100;++i) {
    a=i;
    if(i==b) break;
  }
  return a;  // 49
}

// main function
int main() {
  int expected;
  int actual;

  expected=26;
  actual=Test1();
  if (actual != expected)
  {
    printf("Test1 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
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
  expected=3;
  actual=Test10();
  if (actual != expected)
  {
    printf("Test10 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=1;
  actual=Test11();
  if (actual != expected)
  {
    printf("Test11 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=1;
  actual=Test12();
  if (actual != expected)
  {
    printf("Test12 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=1;
  actual=Test13();
  if (actual != expected)
  {
    printf("Test13 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=23;
  actual=Test14();
  if (actual != expected)
  {
    printf("Test14 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=23;
  actual=Test15();
  if (actual != expected)
  {
    printf("Test15 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=3;
  actual=Test16();
  if (actual != expected)
  {
    printf("Test16 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=1;
  actual=Test17();
  if (actual != expected)
  {
    printf("Test17 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=1;
  actual=Test18();
  if (actual != expected)
  {
    printf("Test18 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=2;
  actual=Test19();
  if (actual != expected)
  {
    printf("Test19 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=1;
  actual=Test20();
  if (actual != expected)
  {
    printf("Test20 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=2;
  actual=Test21();
  if (actual != expected)
  {
    printf("Test21 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=3;
  actual=Test22();
  if (actual != expected)
  {
    printf("Test22 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=3;
  actual=Test23();
  if (actual != expected)
  {
    printf("Test23 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=1;
  actual=Test24();
  if (actual != expected)
  {
    printf("Test24 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=2;
  actual=Test25();
  if (actual != expected)
  {
    printf("Test25 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=3;
  actual=Test26();
  if (actual != expected)
  {
    printf("Test26 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=3;
  actual=Test27();
  if (actual != expected)
  {
    printf("Test27 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=3;
  actual=Test28();
  if (actual != expected)
  {
    printf("Test28 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=1;
  actual=Test29();
  if (actual != expected)
  {
    printf("Test29 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=2;
  actual=Test30();
  if (actual != expected)
  {
    printf("Test30 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=1;
  actual=Test31();
  if (actual != expected)
  {
    printf("Test31 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=2;
  actual=Test32();
  if (actual != expected)
  {
    printf("Test32 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=2;
  actual=Test33();
  if (actual != expected)
  {
    printf("Test33 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=1;
  actual=Test34();
  if (actual != expected)
  {
    printf("Test34 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=1;
  actual=Test35();
  if (actual != expected)
  {
    printf("Test35 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=2;
  actual=Test36();
  if (actual != expected)
  {
    printf("Test36 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=3;
  actual=Test37();
  if (actual != expected)
  {
    printf("Test37 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=3;
  actual=Test38();
  if (actual != expected)
  {
    printf("Test38 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=1;
  actual=Test39();
  if (actual != expected)
  {
    printf("Test39 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=2;
  actual=Test40();
  if (actual != expected)
  {
    printf("Test40 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=3;
  actual=Test41();
  if (actual != expected)
  {
    printf("Test41 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=4;
  actual=Test42();
  if (actual != expected)
  {
    printf("Test42 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
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
  expected=4;
  actual=Test54();
  if (actual != expected)
  {
    printf("Test54 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=8;
  actual=Test55();
  if (actual != expected)
  {
    printf("Test55 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=3;
  actual=Test56();
  if (actual != expected)
  {
    printf("Test56 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=55;
  actual=Test57();
  if (actual != expected)
  {
    printf("Test57 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=2;
  actual=Test58();
  if (actual != expected)
  {
    printf("Test58 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=1;
  actual=Test59();
  if (actual != expected)
  {
    printf("Test59 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=0;
  actual=Test60();
  if (actual != expected)
  {
    printf("Test60 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=1;
  actual=Test61();
  if (actual != expected)
  {
    printf("Test61 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=0;
  actual=Test62();
  if (actual != expected)
  {
    printf("Test62 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=0;
  actual=Test63();
  if (actual != expected)
  {
    printf("Test63 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=4;
  actual=Test64();
  if (actual != expected)
  {
    printf("Test64 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=0;
  actual=Test65();
  if (actual != expected)
  {
    printf("Test65 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=1;
  actual=Test66();
  if (actual != expected)
  {
    printf("Test66 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=2;
  actual=Test67();
  if (actual != expected)
  {
    printf("Test67 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=4;
  actual=Test68();
  if (actual != expected)
  {
    printf("Test68 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=8;
  actual=Test69();
  if (actual != expected)
  {
    printf("Test69 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=16;
  actual=Test70();
  if (actual != expected)
  {
    printf("Test70 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=32;
  actual=Test71();
  if (actual != expected)
  {
    printf("Test71 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=32;
  actual=Test72();
  if (actual != expected)
  {
    printf("Test72 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=4;
  actual=Test73();
  if (actual != expected)
  {
    printf("Test73 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=3;
  actual=Test74();
  if (actual != expected)
  {
    printf("Test74 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=6;
  actual=Test75();
  if (actual != expected)
  {
    printf("Test75 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=3;
  actual=Test76();
  if (actual != expected)
  {
    printf("Test76 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=3;
  actual=Test77();
  if (actual != expected)
  {
    printf("Test77 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=3;
  actual=Test78();
  if (actual != expected)
  {
    printf("Test78 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=1;
  actual=Test79();
  if (actual != expected)
  {
    printf("Test79 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=2;
  actual=Test80();
  if (actual != expected)
  {
    printf("Test80 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=2;
  actual=Test81();
  if (actual != expected)
  {
    printf("Test81 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=20;
  actual=Test82();
  if (actual != expected)
  {
    printf("Test82 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=12;
  actual=Test83();
  if (actual != expected)
  {
    printf("Test83 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=10;
  actual=Test84();
  if (actual != expected)
  {
    printf("Test84 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=12;
  actual=Test85();
  if (actual != expected)
  {
    printf("Test85 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=10;
  actual=Test86();
  if (actual != expected)
  {
    printf("Test86 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=1;
  actual=Test87();
  if (actual != expected)
  {
    printf("Test87 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=10;
  actual=Test88();
  if (actual != expected)
  {
    printf("Test88 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=31;
  actual=Test89();
  if (actual != expected)
  {
    printf("Test89 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=0;
  actual=Test90();
  if (actual != expected)
  {
    printf("Test90 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=1;
  actual=Test91();
  if (actual != expected)
  {
    printf("Test91 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=0;
  actual=Test92();
  if (actual != expected)
  {
    printf("Test92 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=1;
  actual=Test93();
  if (actual != expected)
  {
    printf("Test93 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=4;
  actual=Test94();
  if (actual != expected)
  {
    printf("Test94 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=12;
  actual=Test95();
  if (actual != expected)
  {
    printf("Test95 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=0;
  actual=Test96();
  if (actual != expected)
  {
    printf("Test96 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=42;
  actual=Test97();
  if (actual != expected)
  {
    printf("Test97 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=21;
  actual=Test98();
  if (actual != expected)
  {
    printf("Test98 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=41;
  actual=Test99();
  if (actual != expected)
  {
    printf("Test99 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=47;
  actual=Test100();
  if (actual != expected)
  {
    printf("Test100 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=15;
  actual=Test101();
  if (actual != expected)
  {
    printf("Test101 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=4;
  actual=Test102();
  if (actual != expected)
  {
    printf("Test102 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=4;
  actual=Test103();
  if (actual != expected)
  {
    printf("Test103 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=1;
  actual=Test104();
  if (actual != expected)
  {
    printf("Test104 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=1;
  actual=Test105();
  if (actual != expected)
  {
    printf("Test105 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=0;
  actual=Test106();
  if (actual != expected)
  {
    printf("Test106 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=0;
  actual=Test107();
  if (actual != expected)
  {
    printf("Test107 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=1;
  actual=Test108();
  if (actual != expected)
  {
    printf("Test108 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=1;
  actual=Test109();
  if (actual != expected)
  {
    printf("Test109 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=1;
  actual=Test110();
  if (actual != expected)
  {
    printf("Test110 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=1;
  actual=Test111();
  if (actual != expected)
  {
    printf("Test111 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=0;
  actual=Test112();
  if (actual != expected)
  {
    printf("Test112 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=0;
  actual=Test113();
  if (actual != expected)
  {
    printf("Test113 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=0;
  actual=Test114();
  if (actual != expected)
  {
    printf("Test114 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=1;
  actual=Test115();
  if (actual != expected)
  {
    printf("Test115 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=0;
  actual=Test116();
  if (actual != expected)
  {
    printf("Test116 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=1;
  actual=Test117();
  if (actual != expected)
  {
    printf("Test117 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=1;
  actual=Test118();
  if (actual != expected)
  {
    printf("Test118 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=14;
  actual=Test119();
  if (actual != expected)
  {
    printf("Test119 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=4;
  actual=Test120();
  if (actual != expected)
  {
    printf("Test120 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=4;
  actual=Test121();
  if (actual != expected)
  {
    printf("Test121 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=10;
  actual=Test122();
  if (actual != expected)
  {
    printf("Test122 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=2;
  actual=Test123();
  if (actual != expected)
  {
    printf("Test123 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=2;
  actual=Test124();
  if (actual != expected)
  {
    printf("Test124 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=0;
  actual=Test125();
  if (actual != expected)
  {
    printf("Test125 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=0;
  actual=Test126();
  if (actual != expected)
  {
    printf("Test126 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=1;
  actual=Test127();
  if (actual != expected)
  {
    printf("Test127 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=2;
  actual=Test128();
  if (actual != expected)
  {
    printf("Test128 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=1;
  actual=Test129();
  if (actual != expected)
  {
    printf("Test129 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=0;
  actual=Test130();
  if (actual != expected)
  {
    printf("Test130 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=1;
  actual=Test131();
  if (actual != expected)
  {
    printf("Test131 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=3;
  actual=Test132();
  if (actual != expected)
  {
    printf("Test132 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=97;
  actual=Test133();
  if (actual != expected)
  {
    printf("Test133 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=98;
  actual=Test134();
  if (actual != expected)
  {
    printf("Test134 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=1;
  actual=Test135();
  if (actual != expected)
  {
    printf("Test135 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=4;
  actual=Test136();
  if (actual != expected)
  {
    printf("Test136 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=10;
  actual=Test137();
  if (actual != expected)
  {
    printf("Test137 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=1;
  actual=Test138();
  if (actual != expected)
  {
    printf("Test138 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=145;
  actual=Test139();
  if (actual != expected)
  {
    printf("Test139 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=95;
  actual=Test140();
  if (actual != expected)
  {
    printf("Test140 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=2500;
  actual=Test141();
  if (actual != expected)
  {
    printf("Test141 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=49;
  actual=Test142();
  if (actual != expected)
  {
    printf("Test142 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  return 0;
}
