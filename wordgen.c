#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>

#include "wordgen.h"

void
usage(char *name)
{
	fprintf(stderr, "%s - Generate random strings\n\n", name);
	fprintf(stderr, " -v\t\tPrint additional status information\n");
	fprintf(stderr, " -q\t\tDo not print status messages\n");
	fprintf(stderr, " -m [MIN]\tSet minimum char count\n");
	fprintf(stderr, " -i [MAX]\tSet maximum char count\n");
	fprintf(stderr, " -c [SET]\tSet user defined char set (max 255)\n");
	fprintf(stderr, " -a\t\tChar set contain letters (ignores -c)\n");
	fprintf(stderr, " -A\t\tChar set contain capital letters (ignores -c)\n");
	fprintf(stderr, " -n\t\tChar set contains numbers (ignores -c)\n");
	fprintf(stderr, " -s\t\tChar set contains special characters (ignores -c)\n");
	fprintf(stderr, " -o [FILE]\tOutput to file\n");
	fprintf(stderr, " -h \t\tShow this help\n");
}

struct arguments
{
	/*
	* Contains a pointer to the char set
	*/
	char *chrset;

	/*
	* Contains 1 if char set wanted.
	*/
	char letters, capitals, numbers, specials;

	/*
	* Contains a pointer to the output file name.
	*/
	char *output_file;

	/*
	* Is set to 1 on quiet mode.
	*/
	char quiet, verbose;

	/*
	* Contains the minimum and maximum word length.
	*/
	size_t minlen, maxlen;
};

static int argp_parse(int argc, char *arg[], struct arguments *args);

int main(int argc, char **argv)
{
	int ret;
	wordfield *word = NULL;
	FILE *out = NULL;
	struct arguments args = { 0 };

	/* Parse command line arguments */
	if (argp_parse(argc, argv, &args))
	{
		usage(argv[0]);
		return 1;
	}

	/* Initialize word field */
	word = wordfield_init(args.maxlen);
	if (word == NULL)
	{
		if (!args.quiet)
			fprintf(stderr, "Unexpected program error.\n");
		exit(1);
	}

	/*
	* Copy char set into memory.
	*/
	if (args.chrset != NULL)
	{
		ret = validate_charset(args.chrset, strlen(args.chrset));
		if (ret != 0)
		{
			if (!args.quiet)
				fprintf(stderr, "Invalid char set.\n");
			exit(1);
		}

		ret = wordfield_set_charset(word, args.chrset, strlen(args.chrset));
		if (ret != 0)
		{
			if (!args.quiet)
				fprintf(stderr, "Could not load char set.\n");
			exit(1);
		}
	}
	else
	{
		if ((!args.letters) && (!args.capitals) && (!args.numbers) && (!args.specials))
			args.letters = 1;

		ret = wordfield_generate_charset(word, args.letters, args.capitals, args.numbers, args.specials);
		if (ret != 0)
		{
			if (!args.quiet)
				fprintf(stderr, "Could not generate char set.\n");
			exit(1);
		}
	}

	if (args.output_file != NULL)
		out = fopen(args.output_file, "w");
	else
		out = stdout;

	if (out == NULL)
	{
		if (!args.quiet)
			fprintf(stderr, "%s\n", "Could not open file.");
		exit(0);
	}

	while (1)
	{
		if (word->len < args.minlen)
			ret = wordfield_set_length(word, args.minlen);
		else if (word->len < args.maxlen)
			ret = wordfield_set_length(word, word->len + 1);
		else
			break;

		if (ret != 0)
		{
			if (!args.quiet)
				fprintf(stderr, "Could not set word length.\n");
			exit(1);
		}

		do
		{
			ret = wordfield_generate(word);
			if (ret != 0)
			{
				if (!args.quiet)
					fprintf(stderr, "Could not generate char set.\n");
				exit(1);
			}

			fputs(word->word, out);
			fputc('\n', out);

			ret = wordfield_increment(word);
		} while (ret == 0);
	}

	/* Free word field */
	wordfield_free(word);

	if (out != stdout)
		fclose(out);

	exit(0);
}

static int argp_parse(int argc, char *argv[], struct arguments *args)
{
	int i = 1;
	for (; argc > i; ++i)
	{
		char *argp = argv[i];
		if (argp[0] == '-')
		{
			switch (argp[1])
			{
				case 'q':
					args->quiet = 1;
					break;

				case 'v':
					args->verbose = 1;
					break;

				case 'm':
					if (argc > i + 1)
					{
						char *argpv = argv[i + 1];
						args->minlen = (size_t)strtoul(argpv, NULL, 10);
						++i;
					}
					else
					{
						return 1;
					}
					break;

				case 'i':
					if (argc > i + 1)
					{
						char *argpv = argv[i + 1];
						args->maxlen = (size_t)strtoul(argpv, NULL, 10);
						++i;
					}
					else
					{
						return 1;
					}
					break;

				case 'c':
					if (argc > i + 1)
					{
						char *argpv = argv[i + 1];
						if (strlen(argpv) > 255)
							return 1;
						args->chrset = argpv;
						++i;
					}
					else
					{
						return 1;
					}
					break;

				case 'a':
					args->letters = 1;
					break;

				case 'A':
					args->capitals = 1;
					break;

				case 'n':
					args->numbers = 1;
					break;

				case 's':
					args->specials = 1;
					break;

				case 'o':
					if (argc > i + 1)
					{
						char *argpv = argv[i + 1];
						args->output_file = argpv;
						++i;
					}
					else
					{
						return 1;
					}
					break;

				default:
					return 1;
			}
		}
		else
		{
			return 1;
		}
	}

	return 0;
}

