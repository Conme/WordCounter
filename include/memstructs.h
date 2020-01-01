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

#ifndef MEMSTRUCTS_H_
#define MEMSTRUCTS_H_

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/// @brief The return status of a routine,
/// stating the reason of a potential error.
typedef enum
{
	/// The routine was completed successfully.
	SUCCESS,
	/// The routine failed because an expandable data structure is out of space.
	DATA_STRUCT_FULL,
	/// The routine failed in a way preventing the completion of the program.
	GEN_FAIL
}RetStatus;

/// @brief A Word Buffer containing a single word and its length.
typedef struct WordBuffer WordBuffer;

/**
 * @brief Allocates a new Word Buffer with the specified capacity.
 * @details The new buffer is null-terminated.
 *
 * @param[in]	initLen	The value of the initial capacity of the buffer.
 * @return	Return a pointer to the allocated buffer.
 */
WordBuffer* WordBuffer_create(const uint32_t initLen);

/**
 * @brief Initializes a new Word Buffer to the specified capacity.
 * @details The initialized buffer is null-terminated.
 *
 * @param[in, out]	wbuf	Pointer to the buffer to be initialized.
 * @param[in]		initLen	The value of the initial capacity of the buffer.
 * @return	Returns the status of the routine.
 */
RetStatus WordBuffer_init(WordBuffer *wbuf, const uint32_t initLen);

/**
 * @brief Pushes a new character to the Word Buffer.
 * @details Expands the word's string if needed.
 *
 * @param[in, out]	wbuf	Pointer to the buffer.
 * @param[in]		newChar	The value of the character to be pushed.
 * @return	Returns the status of the routine.
 */
RetStatus WordBuffer_push_char(WordBuffer *wbuf, const int newChar);

/**
 * @brief Deletes the last non-null character of the Word Buffer.
 *
 * @param[in, out]	wbuf	Pointer to the buffer.
 * @return	Void
 */
void WordBuffer_backspace(WordBuffer *wbuf);

/**
 * @brief Clears the Word Buffer.
 * @details Ensures that the buffer remains null-terminated.
 *
 * @param[in, out]	wbuf	Pointer to the buffer.
 * @return	Void
 */
void WordBuffer_clear(WordBuffer *wbuf);

/**
 * @brief Prints in a user-readable way the state of the Word Buffer.
 *
 * @param[in]	wbuf	Pointer to the buffer.
 * @return	Void
 */
void WordBuffer_print(const WordBuffer *wbuf);

/**
 * @brief Frees the memory allocated for the structs of the Word Buffer.
 *
 * @param[in, out]	wbuf	Pointer to the buffer.
 * @return	Void
 */
void WordBuffer_free(WordBuffer *wbuf);

/**
 * @brief Frees the memory allocated for the structs of the Word Buffer
 * and the memory allocated for the Word Buffer itself.
 *
 * @param[in, out]	wbuf	Pointer to the pointer of the buffer.
 * @return	Void
 */
void WordBuffer_destroy(WordBuffer **wbuf);


/// @brief A Vector of Word Buffers
typedef struct WordBufferVector WordBufferVector;

/**
 * @brief Allocates a new Word Buffer Vector with the specified capacity.
 *
 * @param[in]	initLen	The value of the initial capacity of the vector.
 * @return	Return a pointer to the allocated vector.
 */
WordBufferVector* WordBufferVector_create(const size_t initLen);

/**
 * @brief Initializes a Word Buffer Vector to the specified capacity.
 *
 * @param[in]	initLen	The value of the initial capacity of the vector.
 * @return	Returns the status of the routine.
 */
RetStatus WordBufferVector_init(WordBufferVector *vec, const size_t initLen);

/**
 * @brief Pushes a new Word Buffer to the vector.
 * @details Expands the vector if needed.
 *
 * @param[in, out]	vec		Pointer to the vector.
 * @param[in]		wbuf	Pointer to the vector to be pushed.
 * @return	Returns the status of the routine.
 */
RetStatus WordBufferVector_push(WordBufferVector *vec, const WordBuffer *wbuf);

/**
 * @brief Returns the Word Buffer at the specified position.
 *
 * @param[in]	vec	Pointer to the vector.
 * @param[in]	index	Index of the element.
 * @return	Returns a pointer to the indexed element.
 */
WordBuffer* WordBufferVector_at(const WordBufferVector *vec, const size_t index);

/**
 * @brief Returns the string of the Word Buffer at the specified position.
 *
 * @param[in]	vec	Pointer to the vector.
 * @param[in]	index	Index of the element.
 * @return	Returns a pointer to the indexed element.
 */
char* WordBufferVector_word_at(const WordBufferVector *vec, const size_t index);

/**
 * @brief Returns the number of elements in the Word Buffer Vector.
 *
 * @param[in]	vec		Pointer to the vector.
 * @return	Returns the size of the Word Buffer Vector.
 */
size_t WordBufferVector_get_size(const WordBufferVector *vec);

/**
 * @brief Prints in a user-readable way the elements of the Word Buffer Vector.
 *
 * @param[in]	vec	Pointer to the vector.
 * @return	Void
 */
void WordBufferVector_print(const WordBufferVector *vec);

/**
 * @brief Frees the memory allocated for the structs of the Word Buffer Vector.
 *
 * @param[in, out]	vec	Pointer to the vector.
 * @return	Void
 */
void WordBufferVector_free(WordBufferVector *vec);

/**
 * @brief Frees the memory allocated for the structs of the Word Buffer Vector
 * and the memory allocated for the vector itself.
 *
 * @param[in, out]	vec	Pointer to the pointer of the vector.
 * @return	Void
 */
void WordBufferVector_destroy(WordBufferVector **vec);


/// @brief Simple memory pool of characters
typedef struct MemoryPool MemoryPool;

/**
 * @brief Allocates a new Memory Pool with the specified capacity.
 *
 * @param[in]	initCapacity	The value of the initial capacity of the pool.
 * @return	Return a pointer to the allocated pool.
 */
MemoryPool* MemoryPool_create(const size_t initCapacity);

/**
 * @brief Initializes a Memory Pool to the specified capacity.
 *
 * @param[in, out]	memp		Pointer to the pool.
 * @param[in]		initCapacity	The value of the initial capacity of the pool.
 * @return	Returns the status of the routine.
 */
RetStatus MemoryPool_init(MemoryPool *memp, const size_t initCapacity);

/**
 * @brief Allocates a block with the specified number of bytes from the pool.
 *
 * @param[in, out]	memp		Pointer to the pool.
 * @param[in]		numChars	The number of characters to be allocated.
 * @return	Returns a pointer to the allocated memory block.
 */
char* MemoryPool_alloc_block(MemoryPool *memp, const size_t numChars);

/**
 * @brief Checks whether the size of the pool is smaller than the specified
 * capacity limit percentage.
 *
 * @param[in]	memp		Pointer to the pool.
 * @param[in]	limitPrc	The limit percentage set.
 * @return	Returns true if the limit is not reached.
 */
bool MemoryPool_size_below(const MemoryPool *memp, const uint32_t limitPrc);

/**
 * @brief Doubles the size of the Memory Pool.
 *
 * @param[in, out]	memp		Pointer to the pool.
 * @return	Returns the status of the routine.
 */
RetStatus MemoryPool_expand(MemoryPool *memp);

/**
 * @brief Frees the memory allocated for the structs of the Memory Pool.
 *
 * @param[in, out]	memp	Pointer to the pool.
 * @return	Void
 */
void MemoryPool_free(MemoryPool *memp);



/// @brief A hash table used to count the words
typedef struct WordHashTable WordHashTable;

/**
 * @brief Allocates a new Word Hash Table with the specified capacity.
 *
 * @param[in]	initCapacity	The value of the initial capacity of the table.
 * @return	Return a pointer to the allocated table.
 */
WordHashTable* WordHashTable_create(const size_t initCapacity);

/**
 * @brief Initializes a Word Hash Table to the specified capacity.
 *
 * @param[in, out] 	whtab		Pointer to the Hash table.
 * @param[in]		initCapacity	The value of the initial capacity of the table.
 * @return	Returns the status of the routine.
 */
RetStatus WordHashTable_init(WordHashTable *whtab, const size_t initCapacity);

/**
 * @brief Hashes a new word to the Hash table or increase its counter
 * if the word already exists.
 * @details Collisions are handled using linear probing.
 * If there is not enough space in the strings pool for the word,
 * it fails with DATA_STRUCT_FULL
 *
 * @param[in, out]	whtab	Pointer to the Hash table.
 * @param[in]		wbuf	The Word Buffer of the word to be added.
 * @return	Returns the status of the routine.
 */
RetStatus WordHashTable_add_word(WordHashTable *whtab, const WordBuffer *wbuf);

/**
 * @brief Checks whether the size of the Hash Table is smaller than
 * the specified capacity limit percentage.
 *
 * @param[in]	whtab		Pointer to the table.
 * @param[in]	limitPrc	The limit percentage set.
 * @return	Returns true if the limit is not reached.
 */
bool WordHashTable_size_below(const WordHashTable* whtab, const uint32_t limitPrc);

/**
 * @brief Doubles the size of the strings pool of the table.
 * @details Updates the string pointers of the active entries
 * with the new location of the strings.
 *
 * @param[in, out]	whtab		Pointer to the table.
 * @return	Returns the status of the routine.
 */
RetStatus WordHashTable_MemoryPool_expand(WordHashTable *whtab);

/**
 * @brief Doubles the size of the Hash table and rehashes its entries.
 * @details Expands the table's strings pool if almost full.
 *
 * @param[in, out]	whtab		Pointer to the table.
 * @return	Returns the status of the routine.
 */
RetStatus WordHashTable_expand(WordHashTable *whtab);

/**
 * @brief Prints the word in the Hash table in alphabetical order
 * and their count
 *
 * @param[in]	whtab	Pointer to the table.
 * @return	Void
 */
void WordHashTable_count_print(const WordHashTable *whtab);

/**
 * @brief Update the hashing statistics of the table.
 *
 * @param[in, out]	whtab	Pointer to the table.
 * @return	Void
 */
void WordHashTable_hstats_update(WordHashTable *whtab);

/**
 * @brief Prints in a human-readable way the hashing statistics of the table.
 *
 * @param[in]	whtab	Pointer to the table.
 * @return	Void
 */
void WordHashTable_hstats_print(const WordHashTable *whtab);

/**
 * @brief Frees the memory allocated for the structs of the Word Hash Table.
 *
 * @param[in, out]	whtab	Pointer to the table.
 * @return	Void
 */
void WordHashTable_free(WordHashTable *whtab);

/**
 * @brief Frees the memory allocated for the structs of the Word Hash Table
 * and the memory allocated for the table itself.
 *
 * @param[in, out]	whtab	Pointer to the pointer of the table.
 * @return	Void
 */
void WordHashTable_destroy(WordHashTable **whtab);

#endif /* MEMSTRUCTS_H_ */
