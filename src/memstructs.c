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

#include "memstructs.h"
#include "utils.h"
#include <string.h>
#ifdef _DEBUG
#include <assert.h>
#endif //_DEBUG

struct WordBuffer
{
	/// The string of the word.
	char *letters;
	/// The current position of the cursor.
	uint32_t curPosition;
	/// The current capacity of the buffer.
	uint32_t capacity;
};

RetStatus WordBuffer_init(WordBuffer *wbuf, const uint32_t initLen)
{
	if(initLen < 2)
	{
		fprintf(stderr,"Initialization of Word Buffer failed. "
				"Specified initial length %d, smaller than 2.\n", initLen);
		return GEN_FAIL;
	}

	WordBuffer newBuffer = {0};

	newBuffer.capacity = initLen;
	newBuffer.letters = (char*) calloc(newBuffer.capacity, sizeof(char));
	if(newBuffer.letters == NULL)
	{
		fprintf(stderr,"Initial string allocation failed "
				"for a %d characters long Word Buffer.\n", newBuffer.capacity);
		return GEN_FAIL;
	}
	newBuffer.curPosition = 0;
	/// ensures that the string is null-terminated and printable.
	newBuffer.letters[0] = '\0';

	*wbuf = newBuffer;

	return SUCCESS;
}

WordBuffer* WordBuffer_create(const uint32_t initLen)
{
	if(initLen < 2)
	{
		fprintf(stderr,"Creation of Word Buffer failed. "
				"Specified initial length %d, smaller than 2.\n", initLen);
		return NULL;
	}

	WordBuffer* wbuf = (WordBuffer*) calloc(1, sizeof(WordBuffer));
	if(wbuf == NULL)
	{
		fprintf(stderr,"Initial allocation for the Word Buffer failed.\n");
		return NULL;
	}

	wbuf->capacity = initLen;
	wbuf->letters = (char*) calloc(wbuf->capacity, sizeof(char));
	if(wbuf->letters == NULL)
	{
		fprintf(stderr,"Initial string allocation failed "
				"for a %d characters long Word Buffer.\n", wbuf->capacity);
		free(wbuf);
		return NULL;
	}
	wbuf->curPosition = 0;
	/// ensures that the string is null-terminated and printable.
	wbuf->letters[0] = '\0';

	return wbuf;
}

RetStatus WordBuffer_push_char(WordBuffer *wbuf, const int newChar)
{
	/// Doubles the size of the buffer if there is no space for the new char.
	if(wbuf->curPosition >= wbuf->capacity-1)
	{
		const uint32_t len = wbuf->capacity*2;
		char *ext_letters = (char*)realloc(wbuf->letters, len * sizeof(char));
		if(ext_letters != NULL)
		{
			wbuf->letters = ext_letters;
			wbuf->capacity = len;
		}
		else
		{
			fprintf(stderr,"String Expansion failed "
					"for a %d characters long Word Buffer.\n", len);
			return GEN_FAIL;
		}
	}

	wbuf->letters[wbuf->curPosition] =(char) newChar;
	wbuf->curPosition++;
	/// ensures that the string is null-terminated and printable.
	wbuf->letters[wbuf->curPosition] = '\0';

	return SUCCESS;
}

void WordBuffer_backspace(WordBuffer *wbuf)
{
	/// checks whether there are characters to erase.
	if(wbuf->curPosition >= 1)
	{
		wbuf->curPosition--;
		wbuf->letters[wbuf->curPosition] = '\0';
	}
}

void WordBuffer_clear(WordBuffer *wbuf)
{
	wbuf->curPosition = 0;
	wbuf->letters[0] = '\0';
}

void WordBuffer_print(const WordBuffer *wbuf)
{
	printf("%d bytes allocated and %d used for Word Buffer: %s\n",
			wbuf->capacity, wbuf->curPosition+1, wbuf->letters);
}

void WordBuffer_free(WordBuffer *wbuf)
{
	free(wbuf->letters);
}

void WordBuffer_destroy(WordBuffer **wbuf)
{
	WordBuffer_free(*wbuf);
	free(*wbuf);
}

struct WordBufferVector
{
	/// The array of Word Buffers of the vector.
	WordBuffer *buffers;
	/// The position for any new word to be inserted.
	size_t curPosition;
	/// The current capacity of the vector.
	size_t capacity;
};

WordBufferVector* WordBufferVector_create(const size_t initLen)
{
	WordBufferVector* newVec =
			(WordBufferVector*) calloc(1, sizeof(WordBufferVector));
	if(newVec == NULL)
	{
		fprintf(stderr,"Initial allocation for the Word Buffer Vector failed.\n");
		return NULL;
	}

	newVec->capacity = initLen;
	newVec->curPosition = 0;
	newVec->buffers = (WordBuffer*) calloc(newVec->capacity, sizeof(WordBuffer));
	if(newVec->buffers == NULL)
	{
		fprintf(stderr,"Initial array allocation failed "
				"for a Vector of %ld Word Buffers.\n", newVec->capacity);
		free(newVec);
		return NULL;
	}

	return newVec;
}

RetStatus WordBufferVector_init(WordBufferVector *vec, const size_t initLen)
{
	WordBufferVector newVec = {0};

	newVec.capacity = initLen;
	newVec.curPosition = 0;
	newVec.buffers = (WordBuffer*) calloc(newVec.capacity, sizeof(WordBuffer));
	if(newVec.buffers == NULL)
	{
		fprintf(stderr, "Initial array allocation failed "
				"for a Vector of %ld Word Buffers.\n", newVec.capacity);
		return GEN_FAIL;
	}

	*vec = newVec;

	return SUCCESS;
}

RetStatus WordBufferVector_push(WordBufferVector *vec, const WordBuffer *wbuf)
{
	/// Doubles the size of the vector if there is no space for the new word.
	if(vec->curPosition >= vec->capacity-1)
	{
		const size_t newLen = vec->capacity*2;
		WordBuffer* extVec =
				(WordBuffer*)realloc(vec->buffers, newLen * sizeof(WordBuffer));
		if(extVec != NULL)
		{
			vec->buffers = extVec;
			vec->capacity = newLen;
		}
		else
		{
			fprintf(stderr,"Expansion failed "
					"for a Vector of %ld Word Buffers.\n", newLen);
			return GEN_FAIL;
		}
	}

	/// Initializes the buffer at the new position of the array.
	if(WordBuffer_init(&(vec->buffers[vec->curPosition]), wbuf->curPosition+1)
			!= SUCCESS)
	{
		fprintf(stderr,"Allocation for the element %ld of "
				"the input Word Buffer Vector, failed for "
				"a %d characters long Word Buffer.\n",
				vec->curPosition, wbuf->curPosition+1);

		return GEN_FAIL;
	}

	/// Copies the contents of the input buffer
	/// to the buffer at the new position of the array.
	vec->buffers[vec->curPosition].capacity = wbuf->curPosition+1;
	vec->buffers[vec->curPosition].curPosition = wbuf->curPosition;
	if(!string_copy(vec->buffers[vec->curPosition].letters, wbuf->letters,
			vec->buffers[vec->curPosition].capacity))
	{
		fprintf(stderr, "Failed to copy new Word Buffer's string \"%s\" "
				"into Word Buffer Vector's position %ld.\n",
				wbuf->letters, vec->curPosition);
	}
	vec->curPosition++;

	return SUCCESS;
}

WordBuffer* WordBufferVector_at(const WordBufferVector *vec, const size_t index)
{
	return &(vec->buffers[index]);
}

char* WordBufferVector_word_at(const WordBufferVector *vec, const size_t index)
{
	return vec->buffers[index].letters;
}

size_t WordBufferVector_get_size(const WordBufferVector *vec)
{
	return vec->curPosition;
}

void WordBufferVector_print(const WordBufferVector *vec)
{
	printf("%ld entries allocated and %ld used for Word Buffer Vector. "
			"Contents:\n", vec->capacity, vec->curPosition);
	for(size_t i=0; i<vec->curPosition; i++)
	{
		printf("\t%ld:\t", i);
		WordBuffer_print(&(vec->buffers[i]));
	}
	printf("\n");
}

void WordBufferVector_free(WordBufferVector *vec)
{
	for(size_t i=0; i<vec->curPosition; i++)
	{
		WordBuffer_free(&(vec->buffers[i]));
	}
	free(vec->buffers);
}

void WordBufferVector_destroy(WordBufferVector **vec)
{
	WordBufferVector_free(*vec);
	free(*vec);
}

struct MemoryPool
{
	/// Pointer to the initial address of the memory space allocated for the pool.
	char *memSpace;
	/// the index of the next Character to be allocated.
	size_t nextChar;
	/// The current capacity of the pool.
	size_t capacity;
};


MemoryPool* MemoryPool_create(const size_t initCapacity)
{
	MemoryPool* memp = (MemoryPool*) calloc(1, sizeof(MemoryPool));
	if(memp == NULL)
	{
		fprintf(stderr,"Initial allocation for Memory Pool failed.\n");
		return NULL;
	}

	memp->capacity = initCapacity;

	memp->memSpace = (char*) calloc(memp->capacity, sizeof(char));
	if(memp->memSpace == NULL)
	{
		fprintf(stderr,"Memory region allocation of %ld bytes for the Memory Pool.\n",
			memp->capacity);
		free(memp);
		return NULL;
	}

	return memp;
}

RetStatus MemoryPool_init(MemoryPool *memp ,const size_t initCapacity)
{
	MemoryPool m = {0};

	m.capacity = initCapacity;

	m.memSpace = (char*) calloc(m.capacity, sizeof(char));
	if(m.memSpace == NULL)
	{
		fprintf(stderr,"Memory region allocation of %ld bytes for the Memory Pool.\n",
			m.capacity);
		return GEN_FAIL;
	}

	*memp = m;

	return SUCCESS;
}

char* MemoryPool_alloc_block(MemoryPool *memp, const size_t numChars)
{
	/// Checks whether there enough space left in the pool for the new block.
	if(memp->nextChar + numChars < memp->capacity)
	{
		char* alcdSpace = memp->memSpace + memp->nextChar*sizeof(char);
		memp->nextChar += numChars;

		return alcdSpace;
	}
	else return NULL;
}

bool MemoryPool_size_below(const MemoryPool *memp, const uint32_t limitPrc)
{
	return (memp->nextChar < memp->capacity * limitPrc / 100);
}

RetStatus MemoryPool_expand(MemoryPool *memp)
{
	const size_t newCapacity = 2*memp->capacity;
	char* extPtr = (char*) realloc(memp->memSpace, newCapacity);
	if(extPtr == NULL)
	{
		fprintf(stderr,"Expansion of Memory Pool to %ld bytes failed.\n",
			newCapacity);
		return GEN_FAIL;
	}

	memp->capacity = newCapacity;
	memp->memSpace = extPtr;

	return SUCCESS;
}

void MemoryPool_free(MemoryPool *memp)
{
	free(memp->memSpace);
}

/// @brief Statistics related to the performance of the hashing function.
typedef struct
{
	/// The total number of insertions to the Hash Table
	uint64_t totalInsertions;
	/// The total number of collisions occurred when inserting new entries.
	uint64_t totalCollisions;
	/// The mean displacement of the entries currently in the table.
	double meanDisplacement;
	/// The median displacement of the entries currently in the table.
	double medianDisplacement;
}HashStats;

/// @brief Statistics kept to help formating the output.
typedef struct
{
	/// The index of the word with maximum length inserted to the table.
	size_t maxLengthWordIndex;
	/// The index of the most frequently occurring word inserted to the table.
	size_t maxCountWordIndex;
}PrintFormatStats;

/// @brief Hash table entries
typedef struct
{
	/// The string of the word.
	char *letters;
	/// The number of occurrences of the word in the text.
	size_t count;
	/// The length of the string.
	uint32_t length;
	/// The relative displacement to the initial value of its hash index.
	int displacement;
}WordHashTabEntry;

struct WordHashTable
{
	/// The array of entries of the Hash Table.
	WordHashTabEntry *entries;
	/// The array of indices sorted in alphabetical word order
	size_t *alphOrderArray;
	/// The current capacity of the table.
	size_t capacity;
	/// The current size of the table.
	size_t size;
	/// A pool of characters for the strings of the words
	/// to avoid frequent dynamic allocations
	MemoryPool stringsPool;
	/// Statistics related to the hash performance of the table.
	HashStats hstats;
	/// Statistics related to the output format of the table.
	PrintFormatStats pfstats;
};

WordHashTable* WordHashTable_create(const size_t initCapacity)
{
	WordHashTable *newTable = (WordHashTable*) calloc(1, sizeof(WordHashTable));
	if(newTable == NULL)
	{
		fprintf(stderr,"Initial allocation for the Word Hash Table failed.\n");
		return NULL;
	}

	newTable->capacity = initCapacity;
	newTable->size = 0;
	newTable->entries = (WordHashTabEntry*)
			calloc(newTable->capacity, sizeof(WordHashTabEntry));
	if(newTable->entries == NULL)
	{
		fprintf(stderr, "Failed to initialize Word Hash Table "
				"for a capacity of %ld words\n", newTable->capacity);
		return NULL;
	}

	newTable->alphOrderArray = (size_t*)
			calloc(newTable->capacity, sizeof(size_t));
	if(newTable->alphOrderArray == NULL)
	{
		fprintf(stderr, "Failed to initialize Order Table "
				"for a capacity of %ld words\n", newTable->capacity);
		free(newTable->entries);
		return NULL;
	}

	/// Estimated that each string is a bit more than 8 characters long on average,
	/// though as only the 70% of the table will be used,
	/// the 70% of this number is used.
	if(MemoryPool_init(&(newTable->stringsPool), 6 * newTable->capacity) != SUCCESS)
	{
		fprintf(stderr, "Failed to allocate WordHashTable's strings pool\n");
		free(newTable->alphOrderArray);
		free(newTable->entries);
		return NULL;
	}

	return newTable;
}

RetStatus WordHashTable_init(WordHashTable* whtab, const size_t initCapacity)
{
	WordHashTable newTable = {0};

	newTable.capacity = initCapacity;
	newTable.size = 0;
	newTable.entries = (WordHashTabEntry*) calloc(newTable.capacity, sizeof(WordHashTabEntry));
	if(newTable.entries == NULL)
	{
		fprintf(stderr, "Failed to initialize Word Hash Table for a capacity of %ld words\n",
			newTable.capacity);
		return GEN_FAIL;
	}

	newTable.alphOrderArray = (size_t*) calloc(newTable.capacity, sizeof(size_t));
	if(newTable.alphOrderArray == NULL)
	{
		fprintf(stderr, "Failed to initialize Order Table for a capacity of %ld words\n",
			newTable.capacity);
		free(newTable.entries);
		return GEN_FAIL;
	}

	/// Estimated that each string is a bit more than 8 characters long on average,
	/// though as only the 70% of the table will be used,
	/// the 70% of this number is used.
	if(MemoryPool_init(&(newTable.stringsPool), 6 * newTable.capacity) != SUCCESS)
	{
		fprintf(stderr, "Failed to allocate WordHashTable's strings pool\n");
		free(newTable.alphOrderArray);
		free(newTable.entries);
		return GEN_FAIL;
	}

	*whtab = newTable;

	return SUCCESS;
}

/**
 * @brief Compares the new word alphabetically with those already in the table
 * and places the index of the corresponding entry in the appropriate position.
 * @details Insertion sort is used to keep the array alphabetically sorted.
 *
 * @param[in, out]	whtab	Pointer to the Hash table.
 * @param[in]		fp		Pointer to the string of the word to be inserted.
 * @param[in]		ind		The index of the corresponding Hash table entry.
 * @return	Void
 */
static void orderArray_insert(WordHashTable *whtab, const char *newWord, const size_t ind)
{
	int64_t i;
	if(whtab->size == 0) whtab->alphOrderArray[0] = ind;
	else
	{
		/// Backwards iteration of the array, moving each index
		/// to the next position until an index corresponding to a word,
		/// the string of each is alphabetical precedent of the new word
		/// is found.
		for(i =(int64_t)whtab->size - 1; i >= 0 
			&& (strcmp(newWord, whtab->entries[whtab->alphOrderArray[i]].letters) <= 0); i--)
		{
			whtab->alphOrderArray[i + 1] = whtab->alphOrderArray[i];
		}
		/// The new word is placed to the next position
		/// of the one holding the index of the word which stopped the iteration.
		whtab->alphOrderArray[i + 1] = ind;
	}
}

RetStatus WordHashTable_add_word(WordHashTable* whtab, const WordBuffer* buf)
{
	WordHashTabEntry* curEntry = NULL;

	/// The hash index computed is shortened to the capacity of the table
	const int64_t hashIndex = (int64_t)
			(fnvhash((uint8_t*) buf->letters, buf->curPosition + 1) % whtab->capacity);
	int newDispl = 0;
	do
	{
		/// Starting from the shortened hash index, the table is iterated
		/// in both directions until either the word is found in the table,
		/// if it exists already, or an empty slot is found.
		size_t curIndex =(size_t)(hashIndex + newDispl);
		if(curIndex < whtab->capacity)
		{
			curEntry = &(whtab->entries[curIndex]);
			if(curEntry->count == 0)
			{
				curEntry->letters =
					MemoryPool_alloc_block(&(whtab->stringsPool),
							(buf->curPosition + 1) * sizeof(char));
				if(curEntry->letters == NULL)
				{
#ifdef _DEBUG
					printf("String Pool is full. "
							"Allocation for new entry failed!\n");
#endif //_DEBUG
					return DATA_STRUCT_FULL;
				}
				curEntry->length = buf->curPosition + 1;
				if(!string_copy(curEntry->letters, buf->letters, curEntry->length))
				{
					fprintf(stderr, "Failed to copy word \"%s\" "
							"to a new entry", buf->letters);
					return GEN_FAIL;
				}
				curEntry->count++;
				curEntry->displacement = newDispl;

				/// The index is also appended to the alphabetically order array.
				orderArray_insert(whtab, curEntry->letters, curIndex);

				whtab->hstats.totalInsertions++;
				/// Absolute displacements larger than 0 for new entries
				/// mean that collisions occurred.
				if(newDispl>0) whtab->hstats.totalCollisions +=
						(uint64_t)newDispl;

				if(whtab->size == 0)
				{
					/// If the table is empty, the word is both the
					/// longest and the most frequently occurring.
					whtab->pfstats.maxCountWordIndex = curIndex;
					whtab->pfstats.maxLengthWordIndex = curIndex;
				}
				else
				{
					/// if the table is not empty, it is not possible
					/// for the word to be the most frequently occurring.
					WordHashTabEntry* maxLengthWord =
							&(whtab->entries[whtab->pfstats.maxLengthWordIndex]);
					if(curEntry->length > maxLengthWord->length)
						whtab->pfstats.maxLengthWordIndex = curIndex;
				}

				whtab->size++;
				return SUCCESS;
			}
			else
			{
				if((curEntry->length == buf->curPosition + 1) &&
					(curEntry->displacement == newDispl) &&
					(curEntry->letters[0] == buf->letters[0])
					&& curEntry->letters[curEntry->length - 2]
					== buf->letters[buf->curPosition - 1])
				{
					curEntry->count++;

					WordHashTabEntry* maxCountWord =
							&(whtab->entries[whtab->pfstats.maxCountWordIndex]);
					/// If the word is already on the table, its length
					/// was already evaluated and thus only its count is
					/// compared to the max.
					if(curEntry->count > maxCountWord->count)
					{
						whtab->pfstats.maxCountWordIndex = curIndex;
					}

					return SUCCESS;
				}
			}
		}

		if((hashIndex >= newDispl) &&(newDispl > 0))
		{
			curIndex =(size_t)(hashIndex - newDispl);
			curEntry = &(whtab->entries[curIndex]);
			if(curEntry->count == 0)
			{
				curEntry->letters =
					MemoryPool_alloc_block(&(whtab->stringsPool),
							(buf->curPosition + 1) * sizeof(char));
				if(curEntry->letters == NULL)
				{
#ifdef _DEBUG
					printf("String Pool is full. Allocation for new entry failed!\n");
#endif //_DEBUG
					return DATA_STRUCT_FULL;
				}
				curEntry->length = buf->curPosition + 1;
				if(!string_copy(curEntry->letters, buf->letters, curEntry->length))
				{
					fprintf(stderr, "Failed to copy word \"%s\" to a new entry",
						buf->letters);
					return GEN_FAIL;
				}
				curEntry->count++;
				curEntry->displacement = -newDispl;

				/// The index is also appended to the alphabetically order array.
				orderArray_insert(whtab, curEntry->letters, curIndex);

				whtab->hstats.totalInsertions++;
				/// Absolute displacements larger than 0 for new entries
				/// mean that collisions occurred.
				if(newDispl>0) whtab->hstats.totalCollisions +=
						(uint64_t)newDispl;

				if(whtab->size == 0)
				{
					/// If the table is empty, the word is both the
					/// longest and the most frequently occurring.
					whtab->pfstats.maxCountWordIndex = curIndex;
					whtab->pfstats.maxLengthWordIndex = curIndex;
				}
				else
				{
					/// if the table is not empty, it is not possible
					/// for the word to be the most frequently occurring.
					WordHashTabEntry* maxLengthWord =
							&(whtab->entries[whtab->pfstats.maxLengthWordIndex]);
					if(curEntry->length > maxLengthWord->length)
					{
						whtab->pfstats.maxLengthWordIndex = curIndex;
					}
				}

				whtab->size++;
				return SUCCESS;
			}
			else
			{
				if((curEntry->length == buf->curPosition + 1) &&
					(curEntry->displacement == -newDispl) &&
					(curEntry->letters[0] == buf->letters[0]) &&
					curEntry->letters[curEntry->length - 2]
					== buf->letters[buf->curPosition - 1])
				{
					curEntry->count++;

					WordHashTabEntry* maxCountWord =
						&(whtab->entries[whtab->pfstats.maxCountWordIndex]);
					/// If the word is already on the table, its length
					/// was already evaluated and thus only its count is
					/// compared to the max.
					if(curEntry->count > maxCountWord->count)
					{
						whtab->pfstats.maxCountWordIndex = curIndex;
					}

					return SUCCESS;
				}
			}
		}
		newDispl++;

	} while(((size_t)(hashIndex + newDispl) < whtab->capacity) ||(hashIndex >= newDispl));

	/// If the table is exhausted without finding the entry
	/// or an empty slot to insert it, the process fails.
	/// This should not be possible under normal execution.

	fprintf(stderr, "Entry %s is not on the table and there is no slot to add it\n",
		buf->letters);

	return GEN_FAIL;
}

bool WordHashTable_size_below(const WordHashTable* whtab, const uint32_t limitPrc)
{
	return (whtab->size < whtab->capacity * limitPrc / 100);
}

/**
 * @brief Updates the string pointers of the Hash Table, possibly moved
 * by a pool expansion.
 *
 * @param[in, out]	whtab			Pointer to the Hash table.
 * @param[in]		prevPoolBasePtr	Pointer to the previous base address
 * 									of the string pool.
 * @return	Void
 */
static void stringPointers_update(WordHashTable* whtab, const char* prevPoolBasePtr)
{
	for(size_t i = 0; i < whtab->size; i++)
	{
		/// Only the indices stored in the Order Array are valid.
		size_t validIndex = whtab->alphOrderArray[i];
		WordHashTabEntry* oldEntry = &(whtab->entries[validIndex]);
		oldEntry->letters =(oldEntry->letters - prevPoolBasePtr) +
				whtab->stringsPool.memSpace;
	}
}

RetStatus WordHashTable_MemoryPool_expand(WordHashTable* whtab)
{
	char* prevPoolPtr = whtab->stringsPool.memSpace;
	if(MemoryPool_expand(&(whtab->stringsPool)) != SUCCESS)
	{
		fprintf(stderr, "Failed to expand Hash Table memory pool\n");
		return GEN_FAIL;
	}
	if(prevPoolPtr != whtab->stringsPool.memSpace)
	{
		stringPointers_update(whtab, prevPoolPtr);
	}

	return SUCCESS;
}

/**
 * @brief Rehashes the entries of the old Hash Table to the expanded one.
 *
 * @param[in, out]	whtab			Pointer to the Hash table.
 * @param[in, out]	extEntTab		Pointer to the extended array of entries.
 * @return	Void
 */
static void WordHashTable_migrate(WordHashTable* whtab, WordHashTabEntry* extEntTab)
{
	WordHashTabEntry* curEntry = NULL;
	for(size_t i = 0; i < whtab->size; i++)
	{
		/// Only the indices stored in the Order Array are valid.
		size_t oldIndex = whtab->alphOrderArray[i];
		WordHashTabEntry* oldEntry = &(whtab->entries[whtab->alphOrderArray[i]]);

		const int64_t hashIndex =
			(int64_t) (fnvhash((uint8_t*) oldEntry->letters, oldEntry->length) % whtab->capacity);
		int newDispl = 0;
#ifdef _DEBUG
		RetStatus st = GEN_FAIL;
#endif //_DEBUG
		/// Searches only for empty slots as each entry was inserted only once
		/// in the old table.
		do
		{
			size_t curIndex =(size_t)(hashIndex + newDispl);
			if(curIndex < whtab->capacity)
			{
				curEntry = &(extEntTab[curIndex]);
				if(curEntry->count == 0)
				{
					curEntry->letters = oldEntry->letters;
					curEntry->length = oldEntry->length;
					curEntry->count = oldEntry->count;
					curEntry->displacement = newDispl;

					/// The order does not change when rehashing,
					/// so the old index is replaced with the
					/// rehashed one on the order array.
					whtab->alphOrderArray[i] = curIndex;

					/// If the word is the one with the max occurencies
					/// or count, the print format stats should be updated
					/// as well.
					if(oldIndex == whtab->pfstats.maxCountWordIndex)
					{
						whtab->pfstats.maxCountWordIndex = curIndex;
					}

					if(oldIndex == whtab->pfstats.maxLengthWordIndex)
					{
						whtab->pfstats.maxLengthWordIndex = curIndex;
					}

					/// Insertions and Collisions are counted for the rehashing
					/// process as well.
					whtab->hstats.totalInsertions++;
					if(newDispl>0) whtab->hstats.totalCollisions +=
							(uint64_t)newDispl;
#ifdef _DEBUG
					st = SUCCESS;
#endif //_DEBUG
					break;
				}
			}

			if((hashIndex >= newDispl) &&(newDispl > 0))
			{
				curIndex =(size_t)(hashIndex - newDispl);
				curEntry = &(extEntTab[curIndex]);

				if(curEntry->count == 0)
				{
					curEntry->letters = oldEntry->letters;
					curEntry->length = oldEntry->length;
					curEntry->count = oldEntry->count;
					curEntry->displacement = -newDispl;

					/// The order does not change when rehashing,
					/// so the old index is replaced with the
					/// rehashed one on the order array.
					whtab->alphOrderArray[i] = curIndex;

					/// If the word is the one with the max occurencies
					/// or count, the print format stats should be updated
					/// as well.
					if(oldIndex == whtab->pfstats.maxCountWordIndex)
					{
						whtab->pfstats.maxCountWordIndex = curIndex;
					}

					if(oldIndex == whtab->pfstats.maxLengthWordIndex)
					{
						whtab->pfstats.maxLengthWordIndex = curIndex;
					}

					/// Insertions and Collisions are counted for the rehashing
					/// process as well.
					whtab->hstats.totalInsertions++;
					if(newDispl > 0) whtab->hstats.totalCollisions +=
							(uint64_t)newDispl;

#ifdef _DEBUG
					st = SUCCESS;
#endif //_DEBUG
					break;
				}
			}
			newDispl++;

		} while(((size_t)(hashIndex + newDispl) < whtab->capacity) || (hashIndex >= newDispl));

#ifdef _DEBUG
		if(st != SUCCESS)
		{
			assert(false);
			fprintf(stderr, "Failed to migrate the word '%s' to a table containing %ld words\n",
					oldEntry->letters, whtab->capacity);
			break;
		}
#endif //_DEBUG
	}

	return;

}

RetStatus WordHashTable_expand(WordHashTable* whtab)
{
	WordHashTabEntry* extEntries = NULL;
	size_t* extOutOrder = NULL;

	const size_t newCapacity = whtab->capacity * 2;

	extEntries = (WordHashTabEntry*) calloc(newCapacity, sizeof(WordHashTabEntry));
	if(extEntries == NULL)
	{
		fprintf(stderr, "Failed to expand Word Hash Table Entries' "
				"array into %ld words\n", newCapacity);
		return GEN_FAIL;
	}

	extOutOrder = realloc(whtab->alphOrderArray, newCapacity * sizeof(size_t));
	if(extOutOrder == NULL)
	{
		fprintf(stderr, "Failed to expand print table "
				"for %ld words\n", newCapacity);
		free(extEntries);
		return GEN_FAIL;
	}

	whtab->capacity = newCapacity;
	whtab->alphOrderArray = extOutOrder;

	/// If the size of the pool approaches its capacity,
	/// it is expanded as well.
	if(!MemoryPool_size_below(&(whtab->stringsPool), 80))
	{
		if(WordHashTable_MemoryPool_expand(whtab) != SUCCESS)
		{
			fprintf(stderr, "Failed to expand Hash table's string pool");
			free(extEntries);
			return GEN_FAIL;
		}
#ifdef _DEBUG
		printf("Table expanded with Pool expansion.\n");
#endif //_DEBUG
	}

	WordHashTable_migrate(whtab, extEntries);
	free(whtab->entries);
	whtab->entries = extEntries;

#ifdef _DEBUG
	printf("Table expansion from %ld to %ld entries. Current size: %ld.\n",
			whtab->capacity / 2, whtab->capacity, whtab->size);
#endif //_DEBUG

	return SUCCESS;
}

/**
 * @brief Computes the number of digits of a decimal number.
 * @details Computes the number of characters needed to represent a decimal
 * number in a string.
 *
 * @param[in]	num	The decimal number.
 * @return	The number of digits
 */
static inline uint32_t num_of_digits(const size_t num)
{
	return (uint32_t)snprintf(NULL, 0, "%ld", num);
}

/**
 * @brief Prints the specified number of dashes.
 *
 * @param[in]	dashNum	The number of dashes to be printed.
 * @return	Void
 */
static inline void print_dash_line(const uint32_t dashNum)
{
	for(uint32_t i = 0; i < dashNum; i++)
		printf("%c", '-');
	printf("\n");
}

void WordHashTable_count_print(const WordHashTable* whtab)
{
	if(whtab->size == 0) return;

	const uint32_t maxWordLength =
			whtab->entries[whtab->pfstats.maxLengthWordIndex].length;
	const uint32_t maxDigitsCount =
			num_of_digits(whtab->entries[whtab->pfstats.maxCountWordIndex].count);

	printf("Number of appearances of each word:\n");
	printf("    %-*s    %s\n", maxWordLength, "Word", "Count");

	const uint32_t numOfDashes =
		(uint32_t)snprintf(NULL, 0, "    %-*s    %s\n",
				maxWordLength, "Word", "Count") + 3;
	print_dash_line(numOfDashes);

	for(size_t i = 0; i < whtab->size; i++)
	{
		WordHashTabEntry* curEntry =
				&(whtab->entries[whtab->alphOrderArray[i]]);
		printf("    %-*s    %*ld\n",
				maxWordLength, curEntry->letters, maxDigitsCount, curEntry->count);
	}
	print_dash_line(numOfDashes);

#ifdef _STATS
	printf("Most common word: \"%s\", appearing %ld time(s)",
		whtab->entries[whtab->pfstats.maxCountWordIndex].letters,
		whtab->entries[whtab->pfstats.maxCountWordIndex].count);
#endif //_STATS
}

void WordHashTable_hstats_update(WordHashTable* whtab)
{
	if(whtab->size == 0) return;

	int* displOrderedArray = calloc(whtab->size, sizeof(int));
	if(displOrderedArray == NULL)
	{
		fprintf(stderr, "Failed to allocate an array of %ld integers"
				"to calculate mean and median displacements.\n",
			whtab->size);
		return;
	}
	int64_t j = 0;
	whtab->hstats.meanDisplacement = 0;

	/// Uses insertion sort to create an array
	/// with the indices of the entries of the hash table
	/// sorted based on absolute displacement.
	for(int64_t i = 0; i<(int64_t)whtab->size; i++)
	{
		const int newAbsDispl =
				abs(whtab->entries[whtab->alphOrderArray[i]].displacement);
		whtab->hstats.meanDisplacement += newAbsDispl;
		for(j = i - 1; j >= 0 && newAbsDispl>displOrderedArray[j]; j--)
		{
			displOrderedArray[j + 1] = displOrderedArray[j];
		}
		displOrderedArray[j + 1] = newAbsDispl;
	}

	whtab->hstats.meanDisplacement /= (double)whtab->size;

	if(whtab->size % 2 == 0)
	{
		whtab->hstats.medianDisplacement =
			((double)(displOrderedArray[whtab->size / 2 - 1] +
					displOrderedArray[whtab->size / 2])) / 2;
	}
	else
	{
		whtab->hstats.medianDisplacement = displOrderedArray[whtab->size / 2];
	}

	free(displOrderedArray);

}

void WordHashTable_hstats_print(const WordHashTable* whtab)
{
	const double collisPerIns = (double)whtab->hstats.totalCollisions /
			(double)whtab->hstats.totalInsertions;
	const double occupancy = ((double)whtab->size) / (double)whtab->capacity;

	printf("\nHash Table statistics:\n");
	printf("\tCurrent Table size is %ld with a capacity of %ld(%.2f%% used)\n",
		whtab->size, whtab->capacity, occupancy * 100);

	if(whtab->size == 0) return;

	printf("\tTotal Insertions: %ld\n", whtab->hstats.totalInsertions);
	printf("\tAverage Collisions per Insertion: %.4f\n", collisPerIns);
	printf("\tMean and Median Displacements: %.4f and %.2f\n",
		whtab->hstats.meanDisplacement, whtab->hstats.medianDisplacement);
}

void WordHashTable_free(WordHashTable* whtab)
{
	MemoryPool_free(&(whtab->stringsPool));
	free(whtab->entries);
	free(whtab->alphOrderArray);
}

void WordHashTable_destroy(WordHashTable **whtab)
{
	WordHashTable_free(*whtab);
	free(*whtab);
}
