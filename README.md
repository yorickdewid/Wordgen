# Wordgen

Generate a set of words based on input parameters. This can be used to feed tools that take credential lists an an input.
```
usage: wordgen [OPTIONS]:

 -v             Print additional status information
 -q             Do not print status messages
 -m [MIN]       Set minimum char count
 -i [MAX]       Set maximum char count (default: 10)
 -c [SET]       Set user defined char set (max 255)
 -a             Char set contain letters (ignores -c)
 -A             Char set contain capital letters (ignores -c)
 -n             Char set contains numbers (ignores -c)
 -s             Char set contains special characters (ignores -c)
 -o [FILE]      Output to file
 -h             Show this help
```

## Example
This example generates a set of lowercase words between the 8 and 10 characters and outputs it to a file.
```
wordgen -m 8 -i 10 -a -o passwords.txt
```
To use the input directly a pipe could be used to feed the output into the second program `wordgen -i 5 | hydra ...`
## License
See [LICENSE](LICENSE)
