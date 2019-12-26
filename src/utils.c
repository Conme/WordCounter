/**
 * WordCounter calculates the frequency each word appears in the text
 * passed as input through a text file or the standard input.
 * Copyright (C) 2019  Konstantinos Metaxas (konpmetaxas@gmail.com)

 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "utils.h"
#include <stdlib.h>
#include <string.h>


bool string_copy(char *dst, const char *src, const size_t cnt)
{
#ifndef _MSC_VER

	if((dst == NULL) || (src == NULL) || (cnt == 0)) return false;

	strncpy(dst, src, cnt);

	return true;
#else
	if(strcpy_s(dst, cnt, src) == 0) return true;
	else return false;
#endif //MSC_VER
}

bool file_open(FILE **filePointer, const char *path, const char *flags)
{
#ifndef _MSC_VER

	if ((path == NULL) || (flags == NULL)) return false;

	FILE *fp = fopen(path, flags);

	if (fp != NULL)
	{
		*filePointer = fp;
		return true;
	}
	else return false;

#else
	if (fopen_s(filePointer, path, flags) == 0) return true;
	else return false;
#endif //MSC_VER
}

size_t next_2power(const size_t num)
{
	if (num == 0) return 1;

	register size_t v = num;

	/// Rounds up the input to the next power of 2,
	/// by setting all the bits on the right of the most significant '1',
	/// to '1' and incrementing by 1 at the end.
	v--;
	v |= v >> 1;
	v |= v >> 2;
	v |= v >> 4;
	v |= v >> 8;
	v |= v >> 16;
	v |= v >> 32;
	v++;

	return v;
}

#define FNV_OFFSET 0xcbf29ce484222325
#define FNV_PRIME 0x100000001b3

uint64_t fnvhash(const uint8_t *hword, const uint32_t wlen)
{
	uint64_t hash = FNV_OFFSET;
	register const uint64_t fnv_prime = FNV_PRIME;
	uint32_t i = 0;

	for (i = 0; i < wlen; i++)
	{
		hash = hash ^ hword[i];
		hash = hash * fnv_prime;
	}

	return hash;
}
