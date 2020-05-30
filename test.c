int a1, *z1;
int a3[2];
int a4;int hoge1(int z) { return a4+z;}
int a5[3];int hoge2(int z) { return a5[2]+z;}
int a6;int hoge3() {int a; a=3; return a;}
int a7;int hoge4() {int a; a=3; return a;}
int a8;int hoge5() {return a8;}
int a9;int hoge6() {return a9;}
int fib(int a){if(a==0) return 0;else if(a==1) return 1; else return fib(a-2)+fib(a-1);}
int hoge_11(int a){return a*2;}
int hoge_12(){int a;a=1;return 0;}
int hoge_13(){int a;a=1;return a;}
int hoge_14(){int a;a=1;return a;}
int foo2_1(int a,int b,int c,int d,int e,int f){return a+b*2+c*4+d*8+e*16+f*32;}
int foo2_2(int a,int b,int c,int d,int e,int f){return a+b*2+c*4+d*8+e*16+f*32;}
int foo2_3(int a,int b,int c,int d,int e,int f){return a+b*2+c*4+d*8+e*16+f*32;}
int foo2_4(int a,int b,int c,int d,int e,int f){return a+b*2+c*4+d*8+e*16+f*32;}
int foo2_5(int a,int b,int c,int d,int e,int f){return a+b*2+c*4+d*8+e*16+f*32;}
int foo2_6(int a,int b,int c,int d,int e,int f){return a+b*2+c*4+d*8+e*16+f*32;}
int foo2_7(int a,int b,int c,int d,int e,int f){return a+b*2+c*4+d*8+e*16+f*32;}
int foo2_8(int a,int b,int c,int d,int e,int f){return a+b*2+c*4+d*8+e*16+f*32;}
int a10; int *hoge7(int x) { a10=x; return &a10; }
extern int a_test_func;
extern int arr_test_func[4];
extern int foo();
extern int printint(int a);
extern int alloc4(int **p, int a, int b, int c, int d);
extern int printf();  // invalid but pass right now
int test1() 
{
  int a, b;a=1;b=0; {int a;a=2;b=b+a;} {int a;a=4;{int a;a=8;b=b+a;}} {int a;a=16;{int a;a=32;}b=b+a;} return b;
}
int test2() 
{
  char *x; x = "abc"; return x[0];
}
int test3() 
{
  char *x; x = "abc"; return x[1];
}
int test4() 
{
  char *x; x = "abc"; return x[2];
}
int test5() 
{
  char a; a='a'; return a;
}
int test6() 
{
  char a; a='b'; return a;
}
int test7() 
{
  char a; a='1'; return a;
}
int test8() 
{
  char a; a='9'; return a;
}
int test9() 
{
  char a; return sizeof(a);
}
int test10() 
{
  char x[3]; x[0]=-1;x[1]=2;int y;y=4;return x[0]+y;
}
int test11() 
{
  a1 = 1;return a1;
}
int test12() 
{
  int a[2];z1=&a;*z1 = 1;*(a + 1) = 2;return *a;
}
int test13() 
{
  *a3 = 1;*(a3 + 1) = 2;return *a3;
}
int test14() 
{
  a4=3; return hoge1(20);
}
int test15() 
{
  a5[0]=1;a5[1]=2;a5[2]=3; return hoge2(20);
}
int test16() 
{
  a6=1; return hoge3();
}
int test17() 
{
  a7=1; hoge4();return a7;
}
int test18() 
{
  a8=1; int a8; a8=2;return hoge5();
}
int test19() 
{
  a9=1; int a9; a9=2;hoge6();return a9;
}
int test20() 
{
  int a[2];*a = 1;*(a + 1) = 2;return *a;
}
int test21() 
{
  int a[2];*a = 1;*(a + 1) = 2;return *(a+1);
}
int test22() 
{
  int a[2];*a = 1;*(a + 1) = 2;int *p;p = a;return *p + *(p + 1);
}
int test23() 
{
  int a[2];int *p;p = a;*p = 1;*(p + 1) = 2;return *a + *(a + 1);
}
int test24() 
{
  int a[2];a[0] = 1;a[1] = 2;return a[0];
}
int test25() 
{
  int a[2];a[0] = 1;a[1] = 2;return a[1];
}
int test26() 
{
  int a[2];a[0] = 1;a[1] = 2;int *p;p = a;return *p + *(p + 1);
}
int test27() 
{
  int a[2];a[0] = 1;a[1] = 2;int *p;p = a;return p[0] + p[1];
}
int test28() 
{
  int a[2];int *p;p = a;*p = 1;*(p + 1) = 2;return a[0] + a[1];
}
int test29() 
{
  int a[2];int b;a[0]=1;a[1]=2;b=0;return a[b];
}
int test30() 
{
  int a[2];int b;a[0]=1;a[1]=2;b=0;return a[b+1];
}
int test31() 
{
  int a[2];int b;a[0]=1;a[1]=2;b=1;return a[0];
}
int test32() 
{
  int a[2];int b;a[0]=1;a[1]=2;b=1;return a[1];
}
int test33() 
{
  int a[100];int b;a[0]=1;a[1]=2;b=1;return a[1];
}
int test34() 
{
  int a[3];int b;*a=1;*(a+1)=2;*(a+2)=3;b=1;return *a;
}
int test35() 
{
  int a[3];int b;a[0]=1;a[1]=2;a[2]=3;b=1;return a[0];
}
int test36() 
{
  int a[3];int b;a[0]=1;a[1]=2;a[2]=3;b=1;return a[1];
}
int test37() 
{
  int a[3];int b;a[0]=1;a[1]=2;a[2]=3;b=1;return a[2];
}
int test38() 
{
  int a[3];int b;a[0]=1;a[1]=2;a[2]=3;b=1;return a[b*2];
}
int test39() 
{
  int a[4];int b;a[0]=1;a[1]=2;a[2]=3;a[3]=4;b=1;return a[0];
}
int test40() 
{
  int a[4];int b;a[0]=1;a[1]=2;a[2]=3;a[3]=4;b=1;return a[1];
}
int test41() 
{
  int a[4];int b;a[0]=1;a[1]=2;a[2]=3;a[3]=4;b=1;return a[2];
}
int test42() 
{
  int a[4];int b;a[0]=1;a[1]=2;a[2]=3;a[3]=4;b=1;return a[3];
}
int test43() 
{
  int a; return sizeof a ;
}
int test44() 
{
  int a; return sizeof(a);
}
int test45() 
{
  int *a; return sizeof(a);
}
int test46() 
{
  int *a; return sizeof a ;
}
int test47() 
{
  int a; return sizeof(a+1);
}
int test48() 
{
  int a; return sizeof a+1 ;
}
int test49() 
{
  int *a; return sizeof(a+1);
}
int test50() 
{
  int *a; return sizeof a+1 ;
}
int test51() 
{
  int *a; return sizeof *a;
}
int test52() 
{
  int *a; return sizeof(*a);
}
int test53() 
{
  return sizeof(1);
}
int test54() 
{
  return sizeof(sizeof(1));
}
int test55() 
{
  int *p;alloc4(&p, 1, 2, 4, 8);int *q;q = p + 2;*q;q = p + 3;return *q;
}
int test56() 
{
  int x; int *y;y=&x;*y=3;return x;
}
int test57() 
{
  printint(fib(10)); return fib(10);
}
int test58() 
{
  return hoge_11(1);
}
int test59() 
{
  int a;int *b;a=1;b=&a;return *b;
}
int test60() 
{
  18%7; return foo();
}
int test61() 
{
  int a; int b; a=0;a=hoge_13();return a;
}
int test62() 
{
  int a; int b; a=0;b=hoge_14();return a;
}
int test63() 
{
  18%7;return foo();
}
int test64() 
{
  foo();return 18%7;
}
int test65() 
{
  return foo2_1(0,0,0,0,0,0);
}
int test66() 
{
  return foo2_2(1,0,0,0,0,0);
}
int test67() 
{
  return foo2_3(0,1,0,0,0,0);
}
int test68() 
{
  return foo2_4(0,0,1,0,0,0);
}
int test69() 
{
  return foo2_5(0,0,0,1,0,0);
}
int test70() 
{
  return foo2_6(0,0,0,0,1,0);
}
int test71() 
{
  return foo2_7(0,0,0,0,0,1);
}
int test72() 
{
  return foo2_8(foo2_8(0,0,0,0,0,1),0,0,0,0,0);
}
int test73() 
{
  return 18%7;
}
int test74() 
{
  return 23%4;
}
int test75() 
{
  return 94%11;
}
int test76() 
{
  {} return 3;
}
int test77() 
{
  return 3;
}
int test78() 
{
  int a; int b; a=0;b=1;if(a>1) b=2;else b=3;return b;
}
int test79() 
{
  int a; int b; a=0;b=1;if(a>1) b=2;return b;
}
int test80() 
{
  int a; int b; a=2;b=1;if(a>1) b=2;else b=3;return b;
}
int test81() 
{
  int a; int b; a=2;b=1;if(a>1) b=2;return b;
}
int test82() 
{
  int a; int b; b=0;for(a=0;a<10;a=a+1) b=b+2; return b;
}
int test83() 
{
  int a;a=12;while (a<=10) a=a+1;return a;
}
int test84() 
{
  int a;a=0;while (a<10) a=a+1; return a;
}
int test85() 
{
  int a;a=12;while (a<=10) a=a+1; return a;
}
int test86() 
{
  int a;a=0;while (a<10) a=a+1;return a;
}
int test87() 
{
  int a;a=1;int b;for(b=0;b<10;b=b+1) while(a<10) a=a+1; if(a==10) return 1;else return 2;
}
int test88() 
{
  int a;a=1;int b;for(b=0;b<10;b=b+1) while(a<9) a=a+1; if(a==10) return 1;else return b;
}
int test89() 
{
  int a; int c; int b;a=1;c=0;for(b=0;b<10;b=b+1) { a=a+ 1;c=c+2;} if(a==10) return 1;else return a+c;
}
int test90() 
{
  int a;a=11;return a<10;
}
int test91() 
{
  int a;a=2;return a<10;
}
int test92() 
{
  int a;a=10;return a<10;
}
int test93() 
{
  int a;a=10;return a<=10;
}
int test94() 
{
  int a;a=4;return a;
}
int test95() 
{
  int a;a=12;return a;
}
int test96() 
{
  return 0;
}
int test97() 
{
  return 42;
}
int test98() 
{
  return 5+20-4;
}
int test99() 
{
  return  12 + 34 - 5 ;
}
int test100() 
{
  return 5+6*7;
}
int test101() 
{
  return 5*(9-6);
}
int test102() 
{
  return (3+5)/2;
}
int test103() 
{
  return (+3+5)/2;
}
int test104() 
{
  return (-3+5)/2;
}
int test105() 
{
  return (5+1)==6;
}
int test106() 
{
  return 8==(4-2);
}
int test107() 
{
  return (5+1)!=6;
}
int test108() 
{
  return 8!=(4-2);
}
int test109() 
{
  return 4<4*2;
}
int test110() 
{
  return 4<=4*1;
}
int test111() 
{
  return 4<4+1;
}
int test112() 
{
  return 4*2<4;
}
int test113() 
{
  return 4<=2+1;
}
int test114() 
{
  return 4>4*2;
}
int test115() 
{
  return 4>=4*1;
}
int test116() 
{
  return 4>4+1;
}
int test117() 
{
  return 4*2>4;
}
int test118() 
{
  return 4>=2+1;
}
int test119() 
{
  int a; int b;a=3;b=5*6-8;return a+b/2;
}
int test120() 
{
  int a; int b; int c;a=1;b=2;c=4;return (b+c)/2+a;
}
int test121() 
{
  int f2o; int bar_a; int b1zz;f2o=1;bar_a=2;b1zz=4;return (bar_a+b1zz)/2+f2o;
}
int test122() 
{
  int f2o; int bar_a; int b1zz;f2o=1;bar_a=2;b1zz=4;(bar_a+b1zz)/2+f2o;return (f2o+b1zz)*bar_a;
}
int test123() 
{
  int a; a=1;
  return ++a;
}
int test124() 
{
  int a; a=1;
  ++a;
  return a;
}
int test125() 
{
  int a; a=1;
  return --a;
}
int test126() 
{
  int a; a=1;
  --a;
  return a;
}
int test127() 
{
  int a; a=1;
  return a++;
}
int test128() 
{
  int a; a=1;
  a++;
  return a;
}
int test129() 
{
  int a; a=1;
  return a--;
}
int test130() 
{
  int a; a=1;
  a--;
  return a;
}
int test131() {
  int a=1;
  return a;
}
int test132() {
  int a=1,b=2;
  return a+b;
}
char test133() {
  int a='a';
  return a;
}
char test134() {
  char a='a', b='b';
  return b;
}
int test135() {
  a_test_func=1;
  return a_test_func;
}
int test136() {
  arr_test_func[0]=0;
  arr_test_func[1]=2;
  arr_test_func[2]=4;
  return arr_test_func[2];
}
int test137() {
  int *z = hoge7(10);
  return *z;
}
int main() {
  int expected;
  int actual;

  expected=26;
  actual=test1();
  if (actual != expected)
  {
    printf("test1 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=97;
  actual=test2();
  if (actual != expected)
  {
    printf("test2 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=98;
  actual=test3();
  if (actual != expected)
  {
    printf("test3 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=99;
  actual=test4();
  if (actual != expected)
  {
    printf("test4 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=97;
  actual=test5();
  if (actual != expected)
  {
    printf("test5 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=98;
  actual=test6();
  if (actual != expected)
  {
    printf("test6 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=49;
  actual=test7();
  if (actual != expected)
  {
    printf("test7 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=57;
  actual=test8();
  if (actual != expected)
  {
    printf("test8 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=1;
  actual=test9();
  if (actual != expected)
  {
    printf("test9 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=3;
  actual=test10();
  if (actual != expected)
  {
    printf("test10 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=1;
  actual=test11();
  if (actual != expected)
  {
    printf("test11 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=1;
  actual=test12();
  if (actual != expected)
  {
    printf("test12 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=1;
  actual=test13();
  if (actual != expected)
  {
    printf("test13 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=23;
  actual=test14();
  if (actual != expected)
  {
    printf("test14 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=23;
  actual=test15();
  if (actual != expected)
  {
    printf("test15 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=3;
  actual=test16();
  if (actual != expected)
  {
    printf("test16 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=1;
  actual=test17();
  if (actual != expected)
  {
    printf("test17 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=1;
  actual=test18();
  if (actual != expected)
  {
    printf("test18 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=2;
  actual=test19();
  if (actual != expected)
  {
    printf("test19 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=1;
  actual=test20();
  if (actual != expected)
  {
    printf("test20 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=2;
  actual=test21();
  if (actual != expected)
  {
    printf("test21 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=3;
  actual=test22();
  if (actual != expected)
  {
    printf("test22 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=3;
  actual=test23();
  if (actual != expected)
  {
    printf("test23 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=1;
  actual=test24();
  if (actual != expected)
  {
    printf("test24 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=2;
  actual=test25();
  if (actual != expected)
  {
    printf("test25 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=3;
  actual=test26();
  if (actual != expected)
  {
    printf("test26 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=3;
  actual=test27();
  if (actual != expected)
  {
    printf("test27 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=3;
  actual=test28();
  if (actual != expected)
  {
    printf("test28 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=1;
  actual=test29();
  if (actual != expected)
  {
    printf("test29 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=2;
  actual=test30();
  if (actual != expected)
  {
    printf("test30 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=1;
  actual=test31();
  if (actual != expected)
  {
    printf("test31 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=2;
  actual=test32();
  if (actual != expected)
  {
    printf("test32 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=2;
  actual=test33();
  if (actual != expected)
  {
    printf("test33 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=1;
  actual=test34();
  if (actual != expected)
  {
    printf("test34 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=1;
  actual=test35();
  if (actual != expected)
  {
    printf("test35 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=2;
  actual=test36();
  if (actual != expected)
  {
    printf("test36 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=3;
  actual=test37();
  if (actual != expected)
  {
    printf("test37 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=3;
  actual=test38();
  if (actual != expected)
  {
    printf("test38 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=1;
  actual=test39();
  if (actual != expected)
  {
    printf("test39 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=2;
  actual=test40();
  if (actual != expected)
  {
    printf("test40 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=3;
  actual=test41();
  if (actual != expected)
  {
    printf("test41 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=4;
  actual=test42();
  if (actual != expected)
  {
    printf("test42 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=4;
  actual=test43();
  if (actual != expected)
  {
    printf("test43 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=4;
  actual=test44();
  if (actual != expected)
  {
    printf("test44 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=8;
  actual=test45();
  if (actual != expected)
  {
    printf("test45 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=8;
  actual=test46();
  if (actual != expected)
  {
    printf("test46 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=4;
  actual=test47();
  if (actual != expected)
  {
    printf("test47 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=5;
  actual=test48();
  if (actual != expected)
  {
    printf("test48 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=8;
  actual=test49();
  if (actual != expected)
  {
    printf("test49 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=9;
  actual=test50();
  if (actual != expected)
  {
    printf("test50 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=4;
  actual=test51();
  if (actual != expected)
  {
    printf("test51 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=4;
  actual=test52();
  if (actual != expected)
  {
    printf("test52 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=4;
  actual=test53();
  if (actual != expected)
  {
    printf("test53 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=4;
  actual=test54();
  if (actual != expected)
  {
    printf("test54 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=8;
  actual=test55();
  if (actual != expected)
  {
    printf("test55 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=3;
  actual=test56();
  if (actual != expected)
  {
    printf("test56 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=55;
  actual=test57();
  if (actual != expected)
  {
    printf("test57 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=2;
  actual=test58();
  if (actual != expected)
  {
    printf("test58 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=1;
  actual=test59();
  if (actual != expected)
  {
    printf("test59 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=0;
  actual=test60();
  if (actual != expected)
  {
    printf("test60 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=1;
  actual=test61();
  if (actual != expected)
  {
    printf("test61 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=0;
  actual=test62();
  if (actual != expected)
  {
    printf("test62 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=0;
  actual=test63();
  if (actual != expected)
  {
    printf("test63 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=4;
  actual=test64();
  if (actual != expected)
  {
    printf("test64 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=0;
  actual=test65();
  if (actual != expected)
  {
    printf("test65 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=1;
  actual=test66();
  if (actual != expected)
  {
    printf("test66 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=2;
  actual=test67();
  if (actual != expected)
  {
    printf("test67 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=4;
  actual=test68();
  if (actual != expected)
  {
    printf("test68 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=8;
  actual=test69();
  if (actual != expected)
  {
    printf("test69 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=16;
  actual=test70();
  if (actual != expected)
  {
    printf("test70 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=32;
  actual=test71();
  if (actual != expected)
  {
    printf("test71 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=32;
  actual=test72();
  if (actual != expected)
  {
    printf("test72 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=4;
  actual=test73();
  if (actual != expected)
  {
    printf("test73 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=3;
  actual=test74();
  if (actual != expected)
  {
    printf("test74 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=6;
  actual=test75();
  if (actual != expected)
  {
    printf("test75 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=3;
  actual=test76();
  if (actual != expected)
  {
    printf("test76 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=3;
  actual=test77();
  if (actual != expected)
  {
    printf("test77 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=3;
  actual=test78();
  if (actual != expected)
  {
    printf("test78 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=1;
  actual=test79();
  if (actual != expected)
  {
    printf("test79 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=2;
  actual=test80();
  if (actual != expected)
  {
    printf("test80 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=2;
  actual=test81();
  if (actual != expected)
  {
    printf("test81 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=20;
  actual=test82();
  if (actual != expected)
  {
    printf("test82 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=12;
  actual=test83();
  if (actual != expected)
  {
    printf("test83 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=10;
  actual=test84();
  if (actual != expected)
  {
    printf("test84 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=12;
  actual=test85();
  if (actual != expected)
  {
    printf("test85 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=10;
  actual=test86();
  if (actual != expected)
  {
    printf("test86 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=1;
  actual=test87();
  if (actual != expected)
  {
    printf("test87 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=10;
  actual=test88();
  if (actual != expected)
  {
    printf("test88 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=31;
  actual=test89();
  if (actual != expected)
  {
    printf("test89 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=0;
  actual=test90();
  if (actual != expected)
  {
    printf("test90 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=1;
  actual=test91();
  if (actual != expected)
  {
    printf("test91 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=0;
  actual=test92();
  if (actual != expected)
  {
    printf("test92 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=1;
  actual=test93();
  if (actual != expected)
  {
    printf("test93 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=4;
  actual=test94();
  if (actual != expected)
  {
    printf("test94 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=12;
  actual=test95();
  if (actual != expected)
  {
    printf("test95 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=0;
  actual=test96();
  if (actual != expected)
  {
    printf("test96 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=42;
  actual=test97();
  if (actual != expected)
  {
    printf("test97 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=21;
  actual=test98();
  if (actual != expected)
  {
    printf("test98 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=41;
  actual=test99();
  if (actual != expected)
  {
    printf("test99 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=47;
  actual=test100();
  if (actual != expected)
  {
    printf("test100 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=15;
  actual=test101();
  if (actual != expected)
  {
    printf("test101 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=4;
  actual=test102();
  if (actual != expected)
  {
    printf("test102 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=4;
  actual=test103();
  if (actual != expected)
  {
    printf("test103 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=1;
  actual=test104();
  if (actual != expected)
  {
    printf("test104 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=1;
  actual=test105();
  if (actual != expected)
  {
    printf("test105 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=0;
  actual=test106();
  if (actual != expected)
  {
    printf("test106 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=0;
  actual=test107();
  if (actual != expected)
  {
    printf("test107 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=1;
  actual=test108();
  if (actual != expected)
  {
    printf("test108 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=1;
  actual=test109();
  if (actual != expected)
  {
    printf("test109 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=1;
  actual=test110();
  if (actual != expected)
  {
    printf("test110 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=1;
  actual=test111();
  if (actual != expected)
  {
    printf("test111 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=0;
  actual=test112();
  if (actual != expected)
  {
    printf("test112 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=0;
  actual=test113();
  if (actual != expected)
  {
    printf("test113 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=0;
  actual=test114();
  if (actual != expected)
  {
    printf("test114 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=1;
  actual=test115();
  if (actual != expected)
  {
    printf("test115 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=0;
  actual=test116();
  if (actual != expected)
  {
    printf("test116 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=1;
  actual=test117();
  if (actual != expected)
  {
    printf("test117 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=1;
  actual=test118();
  if (actual != expected)
  {
    printf("test118 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=14;
  actual=test119();
  if (actual != expected)
  {
    printf("test119 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=4;
  actual=test120();
  if (actual != expected)
  {
    printf("test120 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=4;
  actual=test121();
  if (actual != expected)
  {
    printf("test121 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=10;
  actual=test122();
  if (actual != expected)
  {
    printf("test122 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=2;
  actual=test123();
  if (actual != expected)
  {
    printf("test123 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=2;
  actual=test124();
  if (actual != expected)
  {
    printf("test124 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=0;
  actual=test125();
  if (actual != expected)
  {
    printf("test125 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=0;
  actual=test126();
  if (actual != expected)
  {
    printf("test126 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=1;
  actual=test127();
  if (actual != expected)
  {
    printf("test127 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=2;
  actual=test128();
  if (actual != expected)
  {
    printf("test128 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=1;
  actual=test129();
  if (actual != expected)
  {
    printf("test129 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=0;
  actual=test130();
  if (actual != expected)
  {
    printf("test130 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=1;
  actual=test131();
  if (actual != expected)
  {
    printf("test131 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=3;
  actual=test132();
  if (actual != expected)
  {
    printf("test132 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=97;
  actual=test133();
  if (actual != expected)
  {
    printf("test133 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=98;
  actual=test134();
  if (actual != expected)
  {
    printf("test134 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=1;
  actual=test135();
  if (actual != expected)
  {
    printf("test135 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=4;
  actual=test136();
  if (actual != expected)
  {
    printf("test136 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  expected=10;
  actual=test137();
  if (actual != expected)
  {
    printf("test137 faild, %d expected, but got %d\n", expected, actual);
    return 1;
  }
  return 0;
}
