/*
* -------------------------------  wordgen.h  -------------------------------
*
* Copyright (c) 2015, Yorick de Wid <yorick17 at outlook dot com>
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions are met:
*
*   * Redistributions of source code must retain the above copyright notice,
*     this list of conditions and the following disclaimer.
*   * Redistributions in binary form must reproduce the above copyright
*     notice, this list of conditions and the following disclaimer in the
*     documentation and/or other materials provided with the distribution.
*   * Neither the name of Redis nor the names of its contributors may be used
*     to endorse or promote products derived from this software without
*     specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
* AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
* IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
* ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
* LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
* CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
* SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
* INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
* CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
* ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
* POSSIBILITY OF SUCH DAMAGE.
*
*/

typedef struct
{
	/*
	* Buffers used
	*/
	char *word, *increment, *charset;

	/*
	* The overall length and length of each buffer.
	*/
	size_t len, affected, chars, wordlen, incrementlen, charsetlen;
} wordfield;

/*
* Checks for char duplicates in a char set
*/
int validate_charset(char *charset, size_t len);

/*
* Initialize word field.
* Specify maximum word length without zero character as len.
*/
wordfield *
wordfield_init(size_t len);

void
wordfield_free(wordfield *word);

int
wordfield_set_charset(wordfield *word, char *charset, size_t len);

int
wordfield_generate_charset(wordfield *word, char letters, char capitals, char numbers, char specials);

int
wordfield_free_charset(wordfield *word);

int
wordfield_set_length(wordfield *word, size_t len);

int
wordfield_increment(wordfield *word);

int
wordfield_generate(wordfield *word);
