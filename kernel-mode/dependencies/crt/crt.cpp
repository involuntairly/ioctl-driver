#pragma once

#include <dependencies/crt/crt.h>

namespace crt
{
	char chr_lower(char c) {
		if (c >= 'A' && c <= 'Z') return c - 'A' + 'a';
		return c;
	}

	int str_icmp(const char* cs, const char* ct) {
		if (cs && ct) {
			while (chr_lower(*cs) == chr_lower(*ct)) {
				if (*cs == 0 && *ct == 0) return 0;
				if (*cs == 0 || *ct == 0) break;
				cs++;
				ct++;
			}
			return chr_lower(*cs) - chr_lower(*ct);
		}
		return -1;
	}

	int lower(int c) {
		if (c >= 'A' && c <= 'Z')
			return c + 'a' - 'A';
		else
			return c;
	}

	int wcs_cmp(const wchar_t* s1, const wchar_t* s2) {
		while (*s1 == *s2++)
			if (*s1++ == '\0')
				return (0);

		return (*(const unsigned int*)s1 - *(const unsigned int*)--s2);
	}

	char* lower_str(char* str) {
		for (char* s = str; *s; ++s)
		{
			*s = (char)lower(*s);
		}
		return str;
	}

	size_t str_len(const char* str) {
		const char* s;
		for (s = str; *s; ++s);
		return (s - str);
	}

	int str_ncmp(const char* s1, const char* s2, size_t n) {
		if (n == 0)
			return (0);
		do {
			if (*s1 != *s2++)
				return (*(unsigned char*)s1 - *(unsigned char*)--s2);
			if (*s1++ == 0)
				break;
		} while (--n != 0);
		return (0);
	}

	int str_cmp(const char* s1, const char* s2) {
		while (*s1 == *s2++)
			if (*s1++ == 0)
				return (0);
		return (*(unsigned char*)s1 - *(unsigned char*)--s2);
	}

	char* str_str(const char* s, const char* find) {
		char c, sc;
		size_t len;
		if ((c = *find++) != 0)
		{
			len = str_len(find);
			do {
				do {
					if ((sc = *s++) == 0)
					{
						return (NULL);
					}
				} while (sc != c);
			} while (str_ncmp(s, find, len) != 0);
			s--;
		}
		return ((char*)s);
	}

	int mem_cmp(const void* s1, const void* s2, size_t n) {
		const unsigned char* p1 = (const unsigned char*)s1;
		const unsigned char* end1 = p1 + n;
		const unsigned char* p2 = (const unsigned char*)s2;
		int d = 0;
		for (;;) {
			if (d || p1 >= end1) break;
			d = (int)*p1++ - (int)*p2++;
			if (d || p1 >= end1) break;
			d = (int)*p1++ - (int)*p2++;
			if (d || p1 >= end1) break;
			d = (int)*p1++ - (int)*p2++;
			if (d || p1 >= end1) break;
			d = (int)*p1++ - (int)*p2++;
		}
		return d;
	}

	void* mem_cpy(void* dest, const void* src, size_t len) {
		char* d = (char*)dest;
		const char* s = (const char*)src;
		while (len--)
			*d++ = *s++;
		return dest;
	}

	void* mem_set(void* dest, uint8_t c, size_t count) {
		size_t block_idx;
		size_t blocks = count >> 3;
		size_t bytes_left = count - (blocks << 3);
		uint64_t c_ull =
			c
			| (((uint64_t)c) << 8)
			| (((uint64_t)c) << 16)
			| (((uint64_t)c) << 24)
			| (((uint64_t)c) << 32)
			| (((uint64_t)c) << 40)
			| (((uint64_t)c) << 48)
			| (((uint64_t)c) << 56);

		uint64_t* dest_ptr8 = (uint64_t*)dest;
		for (block_idx = 0; block_idx < blocks; block_idx++) dest_ptr8[block_idx] = c_ull;

		if (!bytes_left) return dest;

		blocks = bytes_left >> 2;
		bytes_left = bytes_left - (blocks << 2);

		uint32_t* dest_ptr4 = (uint32_t*)&dest_ptr8[block_idx];
		for (block_idx = 0; block_idx < blocks; block_idx++) dest_ptr4[block_idx] = (uint32_t)c_ull;

		if (!bytes_left) return dest;

		blocks = bytes_left >> 1;
		bytes_left = bytes_left - (blocks << 1);

		uint16_t* dest_ptr2 = (uint16_t*)&dest_ptr4[block_idx];
		for (block_idx = 0; block_idx < blocks; block_idx++) dest_ptr2[block_idx] = (uint16_t)c_ull;

		if (!bytes_left) return dest;

		uint8_t* dest_ptr1 = (uint8_t*)&dest_ptr2[block_idx];
		for (block_idx = 0; block_idx < bytes_left; block_idx++) dest_ptr1[block_idx] = (uint8_t)c_ull;

		return dest;
	}
}