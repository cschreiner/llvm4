struct S2 {
  int f0;
  int f1;
  int f2;
} a;

struct S2 fn1() {
  return a;
}

int main() { 
  fn1();
  return 0;
}

