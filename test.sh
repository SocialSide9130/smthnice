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

echo "=== PART 1 ==="
assert 0 'main() {return 0;}'
assert 42 'main() {return 42;}'
assert 21 'main() {return 5+20-4;}'
assert 41 'main() {return  12 + 34 - 5 ;}'
assert 47 'main() {return 5+6*7;}'
assert 15 'main() {return 5*(9-6);}'
assert 4 'main() {return (3+5)/2;}'
assert 10 'main() {return -10+20;}'
assert 10 'main() {return + - 10 + 20;}'
assert 10 'main() {return - -10;}'
assert 10 'main() {return - - +10;}'

echo "=== PART 2 ==="
assert 1 'main() {return 1 == 1;}'
assert 0 'main() {return 12 == 34;}'
assert 1 'main() {return 12 != 34;}'
assert 0 'main() {return 1 != 1;}'
assert 1 'main() {return 12 < 34;}'
assert 1 'main() {return 12 <= 34;}'
assert 0 'main() {return 34 <= 12;}'
assert 0 'main() {return 34 < 12;}'
assert 1 'main() {return 34 > 12;}'
assert 1 'main() {return 34 >= 12;}'
assert 0 'main() {return 12 > 34;}'
assert 0 'main() {return 12 >= 34;}'

echo "=== PART 3 ==="
assert 12 'main() {a=12;return a;}'
assert 30 'main() {a=42-12;return a;}'
assert 30 'main() {a=42;return a-12;}'
assert 15 'main() {a = 7; z = 8; c=a+z; return c;}'

echo "=== PART 4 ==="
assert 42 'main() {hoge=42;return hoge;}'
assert 42 'main() {hoge=21;fuga=2;piyo=hoge*fuga;return piyo;}'

echo "=== PART 5 ==="
assert 42 'main() {hoge=42; return hoge;}'
assert 126 'main() {hoge=42; return hoge* 3;}'

echo "=== PART 6 ==="
assert 42 'main() {hoge = 42; if (hoge) return 42;}'
assert 21 'main() {hoge = 0; if (hoge) return 1; return 21;}'
assert 42 'main() {hoge = 42; if (hoge == 42) return hoge; else return 0;}'
assert 42 'main() {hoge = 0; if (hoge == 42) return 12; else return 42;'} 

echo "=== PART 7 ==="
assert 42 'main() {hoge = 0; while (hoge < 42) hoge = hoge + 1; return hoge;}'
assert 15 'main() {hoge = 0; for (i = 0; i < 5; i = i + 1) hoge = hoge + 3; return hoge;}'
assert 15 'main() {hoge = 0; for (i = 0; i < 5; i = i + 1) for (j = 0; j < 3; j = j + 1) hoge = hoge + 1; return hoge;}'
assert 15 'main() {hoge = 0; for (i = 0; i < 5; i = i + 1) { hoge = hoge + 3; } return hoge;}'
assert 42 'three() {return 3;} main() {return three() * 14;}'
assert 120 'fact(n) {if (n == 1) return 1; return n * fact(n-1);} main() {return fact(5);}'
assert 50 'square(n) {return n * n;} dbl(n) {return 2 * n;} main() {return dbl(square(5));}'
assert 12 's() {n = 5; return a(3);} a(n) {return n;} main() {n = 44; return 4 * s();}'
assert 42 'main() {n = 42; return *(&n);}'
assert 42 'main() {a = 42; b = 24; return *(&b+8);}'
assert 42 'main() {a = 24; b = 42; return *(&a-8);}'

echo "=== ALL TESTS PASSED ==="
echo