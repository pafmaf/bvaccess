
#define UNICODE

#include <iostream>
#include <string>
#include "BVFile.hpp"

#include <Windows.h>
#include <tchar.h>

using namespace std;
using namespace soci;

char *wide_to_utf8(const wchar_t *s) {
	size_t len = 0;
	wchar_t c;
	for (const wchar_t *ss = s; 0 != (c = *ss); ++ss) {
		if (c < 0x80) {
			len += 1;
		} else if (c < 0x800) {
			len += 2;
		} else if (c >= 0xd800 && c < 0xdc00) {
			// surrogate pair
			len += 4;
			++ss;
		} else if (c >= 0xdc00 && c < 0xe000) {
			// pair end -- invalid sequence
			return nullptr;
		} else {
			len += 3;
		}
	}
	char *buf = (char *)malloc(1 + len);
	wcstombs(buf, s, len);
	buf[len] = 0;
	return buf;
}

char *show_file_picker() {
	OPENFILENAME ofn;
	wchar_t filename[1024];
	filename[0] = 0;
	memset(&ofn, 0, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = (HWND)0;
	ofn.lpstrFilter = L"All Files\0*.bv\0\0";
	ofn.lpstrFile = filename;
	ofn.nMaxFile = 1024;
	if (GetOpenFileName(&ofn)) {
		return wide_to_utf8(filename);
	}
	return nullptr;
}

int main() {
  const string fpath(show_file_picker());
  const string driver = "";
  BVBoard board(&fpath, &driver);

  return 0;
}
