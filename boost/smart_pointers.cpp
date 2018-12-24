#include <assert.h>
#include <boost/scoped_array.hpp>
#include <boost/scoped_ptr.hpp>
#include <iostream>


void testShared() {

}


void testScoped() {
  boost::scoped_ptr<int> int_ptr(new int(1));
  assert(*int_ptr == 1);

  boost::scoped_array<int> array_ptr(new int[10]);
  for (int i = 0; i < 10; ++i) {
    array_ptr[i] = i;
  }
  for (int i = 0; i < 10; ++i) {
    assert(array_ptr[i] == i);
  }
}

int main() {
  testScoped();
  testShared();
  return 0;
}

