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
