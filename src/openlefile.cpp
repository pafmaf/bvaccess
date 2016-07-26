
#define UNICODE

#include <iostream>
#include <string>
#include "BVFile.hpp"

#include "platform.hpp"

using namespace std;
using namespace soci;

int main() {
  const string fpath(show_file_picker());
  const string driver = "";
  BVBoard board(&fpath, &driver);

  return 0;
}
