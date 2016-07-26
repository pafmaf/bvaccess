
#ifdef _WIN32
	#include <Windows.h>
	#include <tchar.h>
#endif

char *show_file_picker();

char *wide_to_utf8(const wchar_t *s);
