# !/bin/bash

assert() {
  expected="$1"
  input="$2"
  ./smth "$input" > tmp.s || exit
  gcc -static -o tmp tmp.s
  ./tmp
  actual="$?"
  if [ "$actual" = "$expected" ]; then
    echo "$input => $actual"
  else
    echo "$input => $expected expected, but got $actual"
    exit 1
  fi
}

echo "=== TESTS ==="

assert 0 '{0;}'
assert 42 '{42;}'
assert 21 '{5+20-4;}'
assert 41 '{ 12 + 34 - 5 ;}'
assert 47 '{5+6*7;}'
assert 15 '{5*(9-6);}'
assert 4 '{(3+5)/2;}'
assert 10 '{-10+20;}'
assert 10 '{+ - 10 + 20;}'
assert 10 '{- -10;}'
assert 10 '{- - +10;}'

assert 1 '{1 == 1;}'
assert 0 '{12 == 34;}'
assert 1 '{12 != 34;}'
assert 0 '{1 != 1;}'
assert 1 '{12 < 34;}'
assert 1 '{12 <= 34;}'
assert 0 '{34 <= 12;}'
assert 0 '{34 < 12;}'
assert 1 '{34 > 12;}'
assert 1 '{34 >= 12;}'
assert 0 '{12 > 34;}'
assert 0 '{12 >= 34;}'

assert 12 '{a=12;return a;}'
assert 30 '{a=42-12;return a;}'
assert 30 '{a=42;return a-12;}'
assert 15 '{a = 7; z = 8; c=a+z; return c;}'

assert 42 '{hoge=42;return hoge;}'
assert 42 '{hoge=21;fuga=2;piyo=hoge*fuga;return piyo;}'

assert 42 '{hoge=42; return hoge;}'
assert 126 '{hoge=42; return hoge* 3;}'

assert 42 '{hoge = 42; if (hoge) return 42;}'
assert 21 '{hoge = 0; if (hoge) return 1; return 21;}'
assert 42 '{hoge = 42; if (hoge == 42) return hoge; else return 0;}'
assert 42 '{hoge = 0; if (hoge == 42) return 12; else return 42;'} 

assert 42 '{hoge = 0; while (hoge < 42) hoge = hoge + 1; return hoge;}'
assert 15 '{hoge = 0; for (i = 0; i < 5; i = i + 1) hoge = hoge + 3; return hoge;}'
assert 15 '{hoge = 0; for (i = 0; i < 5; i = i + 1) for (j = 0; j < 3; j = j + 1) hoge = hoge + 1; return hoge;}'
assert 15 '{hoge = 0; for (i = 0; i < 5; i = i + 1) { hoge = hoge + 3; } return hoge;}'

echo "=== ALL TESTS PASSED ==="
echo