#!/bin/bash
compile_test_func() {
  cc -c -o test_func.o test_func.c
}

assert() {
  expected="$1"
  input="$2"

  ./9cc "$input" > tmp.s
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
compile_test_func

assert 0 '18%7;foo();'
assert 4 'foo();18%7;'
assert 0 'foo2(0,0,0,0,0,0);'
assert 1 'foo2(1,0,0,0,0,0);'
assert 2 'foo2(0,1,0,0,0,0);'
assert 4 'foo2(0,0,1,0,0,0);'
assert 8 'foo2(0,0,0,1,0,0);'
assert 16 'foo2(0,0,0,0,1,0);'
assert 32 'foo2(0,0,0,0,0,1);'
assert 32 'foo2(foo2(0,0,0,0,0,1),0,0,0,0,0);'
assert 4 '18%7;'
assert 3 '23%4;'
assert 6 '94%11;'
assert 3 '{}{3;}'
assert 3 '3;'
assert 3 'a=0;b=1;if(a>1) b=2;else b=3;return b;'
assert 1 'a=0;b=1;if(a>1) b=2;return b;'
assert 2 'a=2;b=1;if(a>1) b=2;else b=3;return b;'
assert 2 'a=2;b=1;if(a>1) b=2;return b;'
assert 20 'b=0;for(a=0;a<10;a=a+1) b=b+2; return b;'
assert 12 'a=12;while (a<=10) a=a+1;a;'
assert 10 'a=0;while (a<10) a=a+1; a;'
assert 12 'a=12;while (a<=10) a=a+1; return a;'
assert 10 'a=0;while (a<10) a=a+1;return a;'
assert 1 'a=1;for(b=0;b<10;b=b+1) while(a<10) a=a+1; if(a==10) return 1;else return 2;'
assert 10 'a=1;for(b=0;b<10;b=b+1) while(a<9) a=a+1; if(a==10) return 1;else return b;'
assert 31 'a=1;c=0;for(b=0;b<10;b=b+1) { a=a+ 1;c=c+2;} if(a==10) return 1;else return a+c;'

assert 0 'a=11;a<10;'
assert 1 'a=2;a<10;'
assert 0 'a=10;a<10;'
assert 1 'a=10;a<=10;'

assert 4 'a=4;return a;'
assert 12 'a=12;a;'

assert 0 '0;'
assert 42 '42;'
assert 21 '5+20-4;'
assert 41 ' 12 + 34 - 5 ;'
assert 47 '5+6*7;'
assert 15 '5*(9-6);'
assert 4 '(3+5)/2;'
assert 4 '(+3+5)/2;'
assert 1 '(-3+5)/2;'
assert 1 '(5+1)==6;'
assert 0 '8==(4-2);'
assert 0 '(5+1)!=6;'
assert 1 '8!=(4-2);'
assert 1 '4<4*2;'
assert 1 '4<=4*1;'
assert 1 '4<4+1;'
assert 0 '4*2<4;'
assert 0 '4<=2+1;'
assert 0 '4>4*2;'
assert 1 '4>=4*1;'
assert 0 '4>4+1;'
assert 1 '4*2>4;'
assert 1 '4>=2+1;'
assert 14 'a=3;b=5*6-8;a+b/2;'
assert 4 'a=1;b=2;c=4;(b+c)/2+a;'
assert 4 'f2o=1;bar_a=2;b1zz=4;(bar_a+b1zz)/2+f2o;'
assert 10 'f2o=1;bar_a=2;b1zz=4;(bar_a+b1zz)/2+f2o;return (f2o+b1zz)*bar_a;'
assert 1 'return 1;'

echo OK