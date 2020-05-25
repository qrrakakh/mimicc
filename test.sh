#!/bin/bash
compile_test_func() {
  cc -c -o test_func.o test_func.c
}

assert() {
  expected="$1"
  input="$2"

  echo -e "$input" > tmp.c
  ./9cc ./tmp.c > tmp.s
  cc -c -o tmp.o tmp.s
  cc -o tmp tmp.o test_func.o
  ./tmp
  actual="$?"

  if [ "$actual" = "$expected" ]; then
    echo "$input => $actual"
  else
    echo "$input => $expected expected, but got $actual"
    exit 1
  fi
}

run_test_c_code() {
  test_c_code=$1
  ./9cc ${test_c_code} > tmp.s
  cc -c -o tmp.o tmp.s
  cc -o tmp tmp.o test_func.o
  ./tmp
  if [ "0" -ne "$?" ];then
    echo "[TEST FAILED]"
    exit 1
  fi
}

compile_test_func
run_test_c_code "./test.c"

echo "[TEST PASSED]"
exit 0

# obsoleted

assert 26 'int main() {int a;int b;a=1;b=0; {int a;a=2;b=b+a;} {int a;a=4;{int a;a=8;b=b+a;}} {int a;a=16;{int a;a=32;}b=b+a;} return b;}'
assert 26 'int main() ///HOGEHOGE\n/*hagehge */{\n  int a;\n  /*hogehoge \nhagehage*/int b;\n  a=1;\n  b=0;\n  {\n    int a;\n    a=2;\n     //a=1\n    b=b+a;\n  }\n  {\n    int a;\n    a=4;\n    {\n      int a;\n      a=8;\n      b=b+a;\n    }\n  }\n  {\n    int a;\n    a=16;\n    {\n      int a;\n      a=32;\n    }\n    b=b+a;\n  }\n  return b;\n}\n'
assert 97 'int main() { char *x; x = "abc"; return x[0]; }'
assert 98 'int main() { char *x; x = "abc"; return x[1]; }'
assert 99 'int main() { char *x; x = "abc"; return x[2]; }'
assert 97 'int main() { char a; a='\'a\''; return a;} '
assert 98 'int main() { char a; a='\'b\''; return a;} '
assert 49 'int main() { char a; a='\'1\''; return a;} '
assert 57 'int main() { char a; a='\'9\''; return a;} '
assert 1 'char a2;char main() {a2 = 1;return a2;}'
assert 1 'int main() { char a; return sizeof(a); }'
assert 3 'int main() {char x[3]; x[0]=-1;x[1]=2;int y;y=4;return x[0]+y;}'
assert 1 'int a1;int main() {a1 = 1;return a1;}'
assert 1 'int *z1;int main() {int a[2];z1=&a;*z1 = 1;*(a + 1) = 2;return *a;}'
assert 1 'int a3[2];int main() {*a3 = 1;*(a3 + 1) = 2;return *a3;}'
assert 23 'int a4;int hoge1(int z) { return a4+z;} int main() {a4=3; return hoge1(20);}'
assert 23 'int a5[3];int hoge2(int z) { return a5[2]+z;} int main() {a5[0]=1;a5[1]=2;a5[2]=3; return hoge2(20);}'
assert 3 'int a6;int hoge3() {int a; a=3; return a;} int main() {a6=1; return hoge3();}'
assert 1 'int a7;int hoge4() {int a; a=3; return a;} int main() {a7=1; hoge4();return a7;}'
assert 1 'int a8;int hoge5() {return a8;} int main() {a8=1; int a8; a8=2;return hoge5();}'
assert 2 'int a9;int hoge6() {return a9;} int main() {a9=1; int a9; a9=2;hoge6();return a9;}'
assert 1 'int main() {int a[2];*a = 1;*(a + 1) = 2;return *a;}'
#assert 1 'int main() {int a[2];*a = 1;*(a + 1) = 2;*a;}'
assert 2 'int main() {int a[2];*a = 1;*(a + 1) = 2;return *(a+1);}'
assert 3 'int main() {int a[2];*a = 1;*(a + 1) = 2;int *p;p = a;return *p + *(p + 1);}'
assert 3 'int main() {int a[2];int *p;p = a;*p = 1;*(p + 1) = 2;return *a + *(a + 1);}'

assert 1 'int main() {int a[2];a[0] = 1;a[1] = 2;return a[0];}'
#assert 1 'int main() {int a[2];a[0] = 1;a[1] = 2;a[0];}'
assert 2 'int main() {int a[2];a[0] = 1;a[1] = 2;return a[1];}'
assert 3 'int main() {int a[2];a[0] = 1;a[1] = 2;int *p;p = a;return *p + *(p + 1);}'
assert 3 'int main() {int a[2];a[0] = 1;a[1] = 2;int *p;p = a;return p[0] + p[1];}'
assert 3 'int main() {int a[2];int *p;p = a;*p = 1;*(p + 1) = 2;return a[0] + a[1];}'

assert 1 'int main() {int a[2];int b;a[0]=1;a[1]=2;b=0;return a[b];}'
assert 2 'int main() {int a[2];int b;a[0]=1;a[1]=2;b=0;return a[b+1];}'

assert 1 'int main() {int a[2];int b;a[0]=1;a[1]=2;b=1;return a[0];}'
assert 2 'int main() {int a[2];int b;a[0]=1;a[1]=2;b=1;return a[1];}'

assert 2 'int main() {int a[100];int b;a[0]=1;a[1]=2;b=1;return a[1];}'
#assert 1 'int main() {int a3;int a2;int a1;int b;*(&a1)=1;*((&a1)+1)=2;*((&a1)+2)=3;b=1;return a1;}'
assert 1 'int main() {int a[3];int b;*a=1;*(a+1)=2;*(a+2)=3;b=1;return *a;}'

assert 1 'int main() {int a[3];int b;a[0]=1;a[1]=2;a[2]=3;b=1;return a[0];}'
assert 2 'int main() {int a[3];int b;a[0]=1;a[1]=2;a[2]=3;b=1;return a[1];}'
assert 3 'int main() {int a[3];int b;a[0]=1;a[1]=2;a[2]=3;b=1;return a[2];}'
assert 3 'int main() {int a[3];int b;a[0]=1;a[1]=2;a[2]=3;b=1;return a[b*2];}'

assert 1 'int main() {int a[4];int b;a[0]=1;a[1]=2;a[2]=3;a[3]=4;b=1;return a[0];}'
assert 2 'int main() {int a[4];int b;a[0]=1;a[1]=2;a[2]=3;a[3]=4;b=1;return a[1];}'
assert 3 'int main() {int a[4];int b;a[0]=1;a[1]=2;a[2]=3;a[3]=4;b=1;return a[2];}'
assert 4 'int main() {int a[4];int b;a[0]=1;a[1]=2;a[2]=3;a[3]=4;b=1;return a[3];}'


assert 4 'int main() {int a; return sizeof a ;}'
assert 4 'int main() {int a; return sizeof(a);}'
assert 8 'int main() {int *a; return sizeof(a);}'
assert 8 'int main() {int *a; return sizeof a ;}'
assert 4 'int main() {int a; return sizeof(a+1);}'
assert 5 'int main() {int a; return sizeof a+1 ;}'
assert 8 'int main() {int *a; return sizeof(a+1);}'
assert 9 'int main() {int *a; return sizeof a+1 ;}'
assert 4 'int main() {int *a; return sizeof *a;}'
assert 4 'int main() {int *a; return sizeof(*a);}'
assert 4 'int main() {return sizeof(1);}'
assert 4 'int main() {return sizeof(sizeof(1));}'
assert 8 'int main() {int *p;alloc4(&p, 1, 2, 4, 8);int *q;q = p + 2;*q;q = p + 3;return *q;}'
assert 3 'int main(){int x; int *y;y=&x;*y=3;return x;}'
assert 55 'int fib(int a){if(a==0) return 0;else if(a==1) return 1; else return fib(a-2)+fib(a-1);}int main(){printint(fib(10)); return fib(10);}'
assert 2 'int hoge_11(int a){return a*2;}int main(){return hoge_11(1);}'
assert 1 'int main(){int a;int *b;a=1;b=&a;return *b;}'
#assert 3 'int main(){int a; int b; int c; a=3;b=5;c=&b+8;return *c;}'
assert 0 'int hoge_12(){int a;a=1;return 0;}int main(){18%7; return foo();}'
assert 1 'int hoge_13(){int a;a=1;return a;}int main(){int a; int b; a=0;a=hoge_13();return a;}'
assert 0 'int hoge_14(){int a;a=1;return a;}int main(){int a; int b; a=0;b=hoge_14();return a;}'
assert 0 'int main(){18%7;return foo();}'
assert 4 'int main(){foo();return 18%7;}'
assert 0 'int foo2_1(int a,int b,int c,int d,int e,int f){return a+b*2+c*4+d*8+e*16+f*32;}int main(){return foo2_1(0,0,0,0,0,0);}'
assert 1 'int foo2_2(int a,int b,int c,int d,int e,int f){return a+b*2+c*4+d*8+e*16+f*32;}int main(){return foo2_2(1,0,0,0,0,0);}'
assert 2 'int foo2_3(int a,int b,int c,int d,int e,int f){return a+b*2+c*4+d*8+e*16+f*32;}int main(){return foo2_3(0,1,0,0,0,0);}'
assert 4 'int foo2_4(int a,int b,int c,int d,int e,int f){return a+b*2+c*4+d*8+e*16+f*32;}int main(){return foo2_4(0,0,1,0,0,0);}'
assert 8 'int foo2_5(int a,int b,int c,int d,int e,int f){return a+b*2+c*4+d*8+e*16+f*32;}int main(){return foo2_5(0,0,0,1,0,0);}'
assert 16 'int foo2_6(int a,int b,int c,int d,int e,int f){return a+b*2+c*4+d*8+e*16+f*32;}int main(){return foo2_6(0,0,0,0,1,0);}'
assert 32 'int foo2_7(int a,int b,int c,int d,int e,int f){return a+b*2+c*4+d*8+e*16+f*32;}int main(){return foo2_7(0,0,0,0,0,1);}'
assert 32 'int foo2_8(int a,int b,int c,int d,int e,int f){return a+b*2+c*4+d*8+e*16+f*32;}int main(){return foo2_8(foo2_8(0,0,0,0,0,1),0,0,0,0,0);}'
assert 4 'int main(){return 18%7;}'
assert 3 'int main(){return 23%4;}'
assert 6 'int main(){return 94%11;}'
assert 3 'int main(){{} return 3;}'
assert 3 'int main(){return 3;}'
assert 3 'int main(){int a; int b; a=0;b=1;if(a>1) b=2;else b=3;return b;}'
assert 1 'int main(){int a; int b; a=0;b=1;if(a>1) b=2;return b;}'
assert 2 'int main(){int a; int b; a=2;b=1;if(a>1) b=2;else b=3;return b;}'
assert 2 'int main(){int a; int b; a=2;b=1;if(a>1) b=2;return b;}'
assert 20 'int main(){int a; int b; b=0;for(a=0;a<10;a=a+1) b=b+2; return b;}'
assert 12 'int main(){int a;a=12;while (a<=10) a=a+1;return a;}'
assert 10 'int main(){int a;a=0;while (a<10) a=a+1; return a;}'
assert 12 'int main(){int a;a=12;while (a<=10) a=a+1; return a;}'
assert 10 'int main(){int a;a=0;while (a<10) a=a+1;return a;}'
assert 1 'int main(){int a;a=1;int b;for(b=0;b<10;b=b+1) while(a<10) a=a+1; if(a==10) return 1;else return 2;}'
assert 10 'int main(){int a;a=1;int b;for(b=0;b<10;b=b+1) while(a<9) a=a+1; if(a==10) return 1;else return b;}'
assert 31 'int main(){int a; int c; int b;a=1;c=0;for(b=0;b<10;b=b+1) { a=a+ 1;c=c+2;} if(a==10) return 1;else return a+c;}'

assert 0 'int main(){int a;a=11;return a<10;}'
assert 1 'int main(){int a;a=2;return a<10;}'
assert 0 'int main(){int a;a=10;return a<10;}'
assert 1 'int main(){int a;a=10;return a<=10;}'

assert 4 'int main(){int a;a=4;return a;}'
assert 12 'int main(){int a;a=12;return a;}'

assert 0 'int main(){return 0;}'
assert 42 'int main(){return 42;}'
assert 21 'int main(){return 5+20-4;}'
assert 41 'int main(){return  12 + 34 - 5 ;}'
assert 47 'int main(){return 5+6*7;}'
assert 15 'int main(){return 5*(9-6);}'
assert 4 'int main(){return (3+5)/2;}'
assert 4 'int main(){return (+3+5)/2;}'
assert 1 'int main(){return (-3+5)/2;}'
assert 1 'int main(){return (5+1)==6;}'
assert 0 'int main(){return 8==(4-2);}'
assert 0 'int main(){return (5+1)!=6;}'
assert 1 'int main(){return 8!=(4-2);}'
assert 1 'int main(){return 4<4*2;}'
assert 1 'int main(){return 4<=4*1;}'
assert 1 'int main(){return 4<4+1;}'
assert 0 'int main(){return 4*2<4;}'
assert 0 'int main(){return 4<=2+1;}'
assert 0 'int main(){return 4>4*2;}'
assert 1 'int main(){return 4>=4*1;}'
assert 0 'int main(){return 4>4+1;}'
assert 1 'int main(){return 4*2>4;}'
assert 1 'int main(){return 4>=2+1;}'
assert 14 'int main(){int a; int b;a=3;b=5*6-8;return a+b/2;}'
assert 4 'int main(){int a; int b; int c;a=1;b=2;c=4;return (b+c)/2+a;}'
assert 4 'int main(){int f2o; int bar_a; int b1zz;f2o=1;bar_a=2;b1zz=4;return (bar_a+b1zz)/2+f2o;}'
assert 10 'int main(){int f2o; int bar_a; int b1zz;f2o=1;bar_a=2;b1zz=4;(bar_a+b1zz)/2+f2o;return (f2o+b1zz)*bar_a;}'
assert 1 'int main(){return 1;}'
