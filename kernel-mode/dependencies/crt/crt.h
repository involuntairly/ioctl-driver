#pragma once

#include <dependencies/imports/imports.h>

namespace crt
{
	char chrlwr(
		char c);

	int stricmp(
		const char* cs,
		const char* ct);

	int lower(
		int c);

	int wcscmp(
		const wchar_t* s1,
		const wchar_t* s2);

	char* lowerstr(
		char* Str);

	size_t strlen(
		const char* str);

	int strncmp(
		const char* s1,
		const char* s2,
		size_t n);

	int strcmp(
		const char* s1,
		const char* s2);

	char* strstr(
		const char* s,
		const char* find);

	int memcmp(
		const void* s1,
		const void* s2,
		size_t n);

	void* memcpy(
		void* dest,
		const void* src,
		size_t len);

	void* memset(
		void* dest,
		UINT8 c,
		size_t count);
}