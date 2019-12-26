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
#include "memstructs.h"

/**
 * @brief Tokenization of the input text to a vector of Word Buffers.
 * @details Reads the stream input on a character basis to construct words,
 * which are then pushed as Word Buffers in a vector.
 *
 * @param[out]	vec	Pointer to the Word Buffer Vector to be filled.
 * @param[in]	fp	Pointer to the input file, NULL if the stdin is to be used.
 * @return	Return the status of the routine.
 */
RetStatus get_input(WordBufferVector *vec, FILE *fp);


#define INITIAL_WORD_VECTOR_LENGTH 128
/**
 * @brief Uses a Hash Table of to count the occurrences of each unique word
 * and prints the result in alphabetical order.
 */
int main(int argc, char *argv[])
{
	FILE* inpf;
	/// At most one argument expected, which is the name of the input text file.
	switch(argc)
	{
		case 1:
		{
			inpf = NULL;
			break;
		}
		case 2:
		{
			if(!file_open(&inpf, argv[1], "r"))
			{
				printf("Failed to open file: %s. Exiting...\n", argv[1]);
				return EXIT_FAILURE;
			}
			break;
		}
		default:
		{
			printf("Too many arguments. %d were passed with a maximum of 1 "
					"accepted. Exiting...\n", argc - 1);
			return EXIT_FAILURE;
		}
	}

	/// Creates a Vector of WordBuffers of a predefined initial length
	/// to host the words of the text.
	WordBufferVector *inputVector =
			WordBufferVector_create(INITIAL_WORD_VECTOR_LENGTH);
	if(inputVector == NULL)
	{
		if(inpf != NULL) fclose(inpf);
		fprintf(stderr, "Failed to create Word Buffer Vector "
				"for input processing. Exiting...\n");
		return EXIT_FAILURE;
	}

	/// Converts the text passed on input to a Vector of WordBuffers.
	if(get_input(inputVector, inpf) != SUCCESS)
	{
		fprintf(stderr, "Failed to read input. Exiting...\n");
		WordBufferVector_destroy(&inputVector);
		if(inpf != NULL) fclose(inpf);
		return EXIT_FAILURE;
	}
	if(inpf != NULL) fclose(inpf);

	/// Based on the total number of words appearing in the input, selects as
	/// inital size for the Hash Table the closest power of 2.
	const size_t inputSize = WordBufferVector_get_size(inputVector);
	const size_t ceilSize = next_2power(inputSize);
	const size_t floorSize = ceilSize / 2;
	const size_t wtabInitSize = (inputSize - floorSize >= floorSize / 2)
			? ceilSize : floorSize;
#ifdef _STATS
	printf("Input Length: %ld words\n", inputSize);
#endif
#ifdef _DEBUG
	printf("Initial table size: %ld slots\n", wtabInitSize);
#endif //_DEBUG

	WordHashTable *hashTable = WordHashTable_create(wtabInitSize);
	if(hashTable == NULL)
	{
		fprintf(stderr, "Insufficient memory for creating "
				"the Hash Table. Exiting...\n");
		WordBufferVector_destroy(&inputVector);
		return EXIT_FAILURE;
	}

	/// Iterating over the WordBuffers in the vector,
	/// each word is added to the Hash Table or
	/// its counter is incremented if it already exists,
	for(size_t i = 0; i<inputSize; i++)
	{
		RetStatus rst = SUCCESS;
		/// The memory pool used by the Table to allocate new strings,
		/// keeps expanding if the insertion process failed due to
		/// limited pool space.
		while((rst = WordHashTable_add_word(hashTable, WordBufferVector_at(inputVector,i)))
				== DATA_STRUCT_FULL)
		{
			if(WordHashTable_MemoryPool_expand(hashTable) != SUCCESS)
			{
				fprintf(stderr, "Hash Table's String Pool is out of memory.\n");
				WordHashTable_destroy(&hashTable);
				WordBufferVector_destroy(&inputVector);
				return EXIT_FAILURE;
			}
		}

		if(rst != SUCCESS)
		{
			fprintf(stderr, "Failed to insert word '%s' in the table. "
					"Exiting...\n",	WordBufferVector_word_at(inputVector,i));
			WordHashTable_destroy(&hashTable);
			WordBufferVector_destroy(&inputVector);
			return EXIT_FAILURE;
		}

		/// If the Hash table reaches an occupancy percentage of at least 70%,
		/// the table expands to avoid an increased collision rate
		/// slowing down the insertions.
		if(!WordHashTable_size_below(hashTable, 70))
		{
			if(WordHashTable_expand(hashTable) != SUCCESS)
			{
				fprintf(stderr, "Hash Table Expansion failed! Exiting...\n");
				WordHashTable_destroy(&hashTable);
				WordBufferVector_destroy(&inputVector);
				return EXIT_FAILURE;
			}
		}
	}

	/// After all words are counted, they are printed in alphabetical order.
	WordHashTable_count_print(hashTable);
#ifdef _STATS
	WordHashTable_hstats_update(hashTable);
	WordHashTable_hstats_print(hashTable);
#endif //_STATS

	WordHashTable_destroy(&hashTable);
	WordBufferVector_destroy(&inputVector);

	return EXIT_SUCCESS;
}


/**
 * @brief Converts a Latin Alphabet letter to its lowercase form.
 *
 * @param[in]	let	Letter to be converted
 * @return	Corresponding lowercase letter
 */
static inline int to_lowercase(const int let)
{
	return (let | 0x20);
}

/**
 * @brief Evaluates whether the input character is a Latin Alphabet letter
 *
 * @param[in]	ch	Character to to be evaluated
 * @return	True if the character is a Latin Alphabet letter
 */
static inline bool is_letter(const int ch)
{
	int lowch = to_lowercase(ch);

	return (lowch >= 'a' && lowch <= 'z');
}

/**
 * @brief Evaluates whether the input character is a number
 *
 * @param[in]	ch	Character to to be evaluated
 * @return	True if the character is a number
 */
static inline bool is_number(const int ch)
{
	return (ch >= '0' && ch <= '9');
}

/**
 * @brief Evaluates whether the input character is among those
 * which can possibly appear inside a word.
 *
 * @param[in]	ch	Character to to be evaluated
 * @return	True if the character is an inside word number
 */
static inline bool is_inword_symbol(const int ch)
{
	static const char inWrdSymbols[] = {'-', '\'', '%', ',', '.', '@'};

	bool isSym = false;
	for(uint32_t i = 0; i<sizeof(inWrdSymbols); i++)
	{
		isSym |= (ch == inWrdSymbols[i]);
	}
	return isSym;
}

/// @brief The type of the processed character
typedef enum
{
	/// The processed character is a Latin alphabet letter.
	LETTER,
	/// The processed character is a number.
	NUMBER,
	/// The processed character is among those
	/// which can possibly appear inside a word.
	IN_WORD_SYMBOL,
	/// The processed character is not used in words.
	OTHER_SYMBOL
}InputCharType;

/**
 * @brief Categorizes a character on the available InputCharTypes
 *
 * @param[in]	ch	Character to to be evaluated
 * @return	The InputCharType of the character
 */
static inline InputCharType get_char_type(const int ch)
{
	if(is_letter(ch)) return LETTER;
	else
	{
		if(is_number(ch)) return NUMBER;
		else
		{
			if(is_inword_symbol(ch)) return IN_WORD_SYMBOL;
			else return OTHER_SYMBOL;
		}
	}
}

/// @brief The state of the input processor. Where the processing cursor is.
typedef enum
{
	/// Input Processing Cursor is between words.
	BETWEEN_WORDS,
	/// Input Processing Cursor is inside a word, after an Alpharithmetic.
	IN_WORD_AFTER_ALPHARITH,
	/// Input Processing Cursor is inside a word, after an In Word character.
	IN_WORD_AFTER_SYMBOL
}InputState;

#define INITIAL_WORD_BUFFER_LENGTH 16

RetStatus get_input(WordBufferVector* vec, FILE* fp)
{
	WordBuffer* wbuf = WordBuffer_create(INITIAL_WORD_BUFFER_LENGTH);
	if(wbuf == NULL)
	{
		fprintf(stderr, "Failed to initialize word buffer for input "
				"processing.\n");
		return GEN_FAIL;
	}

	FILE* source;
	/// If no file is passed, the input text is read from the standard input.
	if(fp == NULL)
	{
		/// The user provides the input using an 'EOF' to signify its end.
		printf("Enter input followed by an 'EOF'([Enter - Ctrl+D] for Unix "
				"and [Enter - Ctrl+Z - Enter] for Windows)\n");
		source = stdin;
	}
	else source = fp;


	int newChar;
	InputState state = BETWEEN_WORDS;
	/// The input is processed on a character basis
	while((newChar = fgetc(source)) != EOF)
	{
		InputCharType inpType = get_char_type(newChar);
		switch(state)
		{
			case BETWEEN_WORDS:
			{
				/// While being between words only Alpharithmetic characters
				/// change the input's state. Other characters can't be in
				/// the beginning of a word.
				switch(inpType)
				{
					case LETTER:
					{
						/// Letters are converted to lowercase
						/// before being appended to the buffer
						if(WordBuffer_push_char(wbuf, to_lowercase(newChar))
								!= SUCCESS)
						{
							WordBuffer_destroy(&wbuf);
							return GEN_FAIL;
						}
						state = IN_WORD_AFTER_ALPHARITH;
						break;
					}
					case NUMBER:
					{
						if(WordBuffer_push_char(wbuf, newChar) != SUCCESS)
						{
							WordBuffer_destroy(&wbuf);
							return GEN_FAIL;
						}
						state = IN_WORD_AFTER_ALPHARITH;
						break;
					}
					default:
					{
						state = BETWEEN_WORDS;
						break;
					}
				}
				break;
			}
			case IN_WORD_AFTER_ALPHARITH:
			{
				switch(inpType)
				{
					/// After an Alpharithmetic, a new one signifies
					/// the continuation of the word
					case LETTER:
					{
						if(WordBuffer_push_char(wbuf, to_lowercase(newChar))
								!= SUCCESS)
						{
							WordBuffer_destroy(&wbuf);
							return GEN_FAIL;
						}
						state = IN_WORD_AFTER_ALPHARITH;
						break;
					}
					case NUMBER:
					{
						if(WordBuffer_push_char(wbuf, newChar) != SUCCESS)
						{
							WordBuffer_destroy(&wbuf);
							return GEN_FAIL;
						}
						state = IN_WORD_AFTER_ALPHARITH;
						break;
					}

					/// After an Alpharithmetic, an In Word Symbol signifies
					/// that the word possibly ended so the state changes.
					case IN_WORD_SYMBOL:
					{
						if(WordBuffer_push_char(wbuf, newChar) != SUCCESS)
						{
							WordBuffer_destroy(&wbuf);
							return GEN_FAIL;
						}
						state = IN_WORD_AFTER_SYMBOL;
						break;
					}

					/// After an Alpharithmetic, any other symbol signifies
					/// the definite end of the word, which is subsequently
					/// pushed to the vector.
					case OTHER_SYMBOL:
					{
						if(WordBufferVector_push(vec, wbuf) != SUCCESS)
						{
							WordBuffer_destroy(&wbuf);
							return GEN_FAIL;
						}
						WordBuffer_clear(wbuf);
						state = BETWEEN_WORDS;
						break;
					}
				}
				break;
			}
			case IN_WORD_AFTER_SYMBOL:
			{
				/// After an In Word symbol only Alpharithmetic characters
				/// signify the continuation of the word.
				switch(inpType)
				{
					case LETTER:
					{
						if(WordBuffer_push_char(wbuf, to_lowercase(newChar))
								!= SUCCESS)
						{
							WordBuffer_destroy(&wbuf);
							return GEN_FAIL;
						}
						state = IN_WORD_AFTER_ALPHARITH;
						break;
					}
					case NUMBER:
					{
						if(WordBuffer_push_char(wbuf, newChar) != SUCCESS)
						{
							WordBuffer_destroy(&wbuf);
							return GEN_FAIL;
						}
						state = IN_WORD_AFTER_ALPHARITH;
						break;
					}
					/// Any symbol signifies that the word had already ended
					/// before the previous symbol, as 2 consecutive symbols
					/// are not allowed inside words.
					default:
					{
						/// The extra symbol is discarded and the word buffer
						/// is pushed to the vector.
						WordBuffer_backspace(wbuf);
						if(WordBufferVector_push(vec, wbuf) != SUCCESS)
						{
							WordBuffer_destroy(&wbuf);
							return GEN_FAIL;
						}
						WordBuffer_clear(wbuf);
						state = BETWEEN_WORDS;
						break;
					}
				}
				break;
			}
		}
	}

	switch(state)
	{
		/// After the 'EOF' is reached, if the last character was alpharithmetic
		/// the word was concluded and the containing buffer is pushed
		/// to the vector.
		case IN_WORD_AFTER_ALPHARITH:
		{
			if(WordBufferVector_push(vec, wbuf) != SUCCESS)
			{
				WordBuffer_destroy(&wbuf);
				return GEN_FAIL;
			}
			break;
		}
		/// After the 'EOF' is reached, if the last character was an In Word
		/// symbol, the word was concluded before that, so the extra character
		/// is discarded and the containing buffer is pushed to the vector.
		case IN_WORD_AFTER_SYMBOL:
		{
			WordBuffer_backspace(wbuf);
			if(WordBufferVector_push(vec, wbuf) != SUCCESS)
			{
				WordBuffer_destroy(&wbuf);
				return GEN_FAIL;
			}
			break;
		}
		default:
		{
			break;
		}
	}

	WordBuffer_destroy(&wbuf);

	return SUCCESS;
}
