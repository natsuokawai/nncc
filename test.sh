#!/bin/bash
# chmod a+x test.sh

assert() {
  expected="$1"
  input="$2"

  ./nncc "$input" > tmp.s || exit
  cc -o tmp tmp.s
  ./tmp
  actual="$?"

  if [ "$actual" = "$expected" ]; then
    echo "$input => $actual"
  else
    echo "$input => $expected expected, but got $actual"
    exit 1
  fi
}

assert 0 0
assert 42 42

echo OK