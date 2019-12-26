/**
 * This file is part of WordCounter.
 * Copyright (C) 2019  Konstantinos Metaxas (konpmetaxas@gmail.com)
 *
 * WordCounter is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * WordCounter is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with WordCounter.  If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef UTILS_H_
#define UTILS_H_

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

/**
 * @brief Copies string from source to destination pointers
 * @details Wrapper of strcpy_s for compilation on Microsoft
 * systems and a safer version of strncpy for the rest.
 *
 * @param[out]	dst	Pointer to the destination string.
 * @param[in]	src	Pointer to the source string.
 * @return	Returns true if the process succeeds.
 */
bool string_copy(char *dst, const char *src, const size_t cnt);

/**
 * @brief Opens a file
 * @details Wrapper of fopen_s for compilation on Microsoft
 * systems and a safer version of fopen for the rest.
 *
 * @param[out]	filePointer	Pointer to the pointer of the file.
 * @param[in]	src			Pointer to the string containing the path of the file.
 * @param[in]	flags		Pointer to the string containing the flags.
 * @return	Returns true if the process succeeds.
 */
bool file_open(FILE **filePointer, const char *path, const char *flags);

/**
 * @brief Computes a 64-bit hash index of a byte array.
 * @details The function uses the FNV-1a algorithm which except for its
 * simplicity, is proven to create a really good hash distribution
 * when tested for English words.
 *
 * @param[in]	hword	Pointer to the array of bytes.
 * @param[in]	src		The length of the array.
 * @return	Returns the hash index.
 */
uint64_t fnvhash(const uint8_t *hword, const uint32_t wlen);

/**
 * @brief Rounds up a number to the next power of 2.
 * @details Based on "Bit Twiddling Hacks" of Sean Eron Anderson
 * (https://graphics.stanford.edu/~seander/bithacks.html#RoundUpPowerOf2)
 *
 * @param[in]	num	Number to be rounded.
 * @return	Returns the rounded up number.
 */
size_t next_2power(const size_t num);

#endif /* UTILS_H_ */
