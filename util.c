/*
* -------------------------------  util.c  ----------------------------------
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>

#include "wordgen.h"

int validate_charset(char *charset, size_t len)
{
	size_t i, a;

	for (i = 0; i<len; i++)
	{
		for (a = 0; a<i; a++)
		{
			if (charset[i] == charset[a])
				return 1;
		}
	}

	return 0;
}

wordfield *
wordfield_init(size_t len)
{
	wordfield * word;

	if (len < 1)
		return NULL;

	/* Allocate space for word */
	word = (wordfield *)calloc(1, sizeof(wordfield));

	/* Set buffer lengths */
	word->wordlen = len + 1;
	word->incrementlen = len;

	/* Allocate word buffer */
	word->word = (char *)malloc(word->wordlen * sizeof(char));

	/* Allocate increment buffer */
	word->increment = (char *)malloc(word->incrementlen * sizeof(char));

	if ((word->word == NULL) || (word->increment == NULL))
	{
		free(word);
		return NULL;
	}

	return word;
}

void
wordfield_free(wordfield *word)
{
	if (word == NULL)
		return;

	/* Free buffers */
	if (word->word != NULL)
		free(word->word);
	if (word->increment != NULL)
		free(word->increment);
	if (word->charset != NULL)
		free(word->charset);

	free(word);
}

int
wordfield_set_charset(wordfield *word, char *charset, size_t len)
{
	if (word == NULL)
		return 1;

	if (len < 1)
		return 2;

	if (len > strlen(charset))
		len = strlen(charset);

	word->charsetlen = len + 1;
	word->chars = len;
	word->charset = (char *)calloc(word->charsetlen, sizeof(char));

	/* copy char set buffer into structure */
	memcpy(word->charset, charset, len);

	return 0;
}

int
wordfield_generate_charset(wordfield *word, char letters, char capitals, char numbers, char specials)
{
	size_t req = 0;
	char *pos = NULL;

	if (word == NULL)
		return 1;

	if (word->charset != NULL)
		return 2;

	if (letters)
		req += 26;
	if (capitals)
		req += 26;
	if (numbers)
		req += 10;
	if (specials)
		req += 30;

	/* Allocate buffer of required length */
	word->charsetlen = req + 1;
	word->chars = req;

	/* Allocate buffer */
	word->charset = (char *)calloc(word->charsetlen, sizeof(char));

	pos = word->charset;
	if (letters)
		pos = memcpy(pos, "abcdefghijklmnopqrstuvwxyz", 26);
	if (capitals)
		pos = memcpy(pos, "ABCDEFGHIJKLMNOPQRSTUVWXYZ", 26);
	if (numbers)
		pos = memcpy(pos, "0123456789", 10);
	if (specials)
		pos = memcpy(pos, ".,:;+*-/\\!\"§$%&/()=?`´'#*-_<>|", 30);

	return 0;
}

int
wordfield_free_charset(wordfield *word)
{
	if (word == NULL)
		return 1;

	if (word->charset == NULL)
		return 2;

	free(word->charset);
	word->charsetlen = 0;
	word->chars = 0;

	return 0;
}

int
wordfield_set_length(wordfield *word, size_t len)
{
	if (word == NULL)
		return 1;

	if ((len < 1) || (len > word->wordlen - 1))
		return 2;

	/* re-zero buffers */
	memset(word->word, 0, word->wordlen * sizeof(char));
	memset(word->increment, 0, word->incrementlen * sizeof(char));

	/* set length */
	word->len = len;

	/* reset affected variable */
	word->affected = 0;

	return 0;
}

int
wordfield_increment(wordfield *word)
{
	size_t pos;

	if (word == NULL)
		return 1;

	if ((word->charset == NULL) || (word->len == 0))
		return 2;

	pos = word->len - 1;
	while (1)
	{
		if (word->increment[pos] < word->chars - 1)
		{
			word->increment[pos]++;
			word->affected = word->len - pos;
			return 0;
		}
		else
		{
			word->increment[pos] = 0;
			if (pos == 0)
				break;
			else
				pos--;
		}
	}

	word->affected = 0;
	return 1;
}

int
wordfield_generate(wordfield *word)
{
	if (word == NULL)
		return 1;

	if ((word->charset == NULL) || (word->len == 0))
		return 2;

	size_t start, pos;

	if (word->affected != 0)
		start = word->len - word->affected;
	else
		start = 0;

	for (pos = start; pos < word->len; pos++)
	{
		word->word[pos] = word->charset[word->increment[pos]];
	}
	word->word[pos] = '\0';

	return 0;
}

