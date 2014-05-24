// Test static storage duration

#include <cassert>

int dummy = 0;
int make_calls = 0;
int get_calls = 0;

int* make_global() {
  static int** g1 = new int*(&dummy);
  make_calls++;
  return *g1;
}

int* get_global() {
  static int* g2 = make_global();
  get_calls++;
  return g2;
}

int main(int, char**) {
  assert(make_calls == 0);
  assert(get_calls == 0);
  int* a1 = get_global();
  assert(make_calls == 1);
  assert(get_calls == 1);
  int* a2 = get_global();
  assert(make_calls == 1);
  assert(get_calls == 2);
  assert(a1 == &dummy);
  assert(a2 == &dummy);
  return 0;
}

