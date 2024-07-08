The TASK:
Implement a grep-like utility in C++ that does a recursive search of a given query in all files in a directory.

High-level requirements:
- You should use only the C++ standard library.
- You should use CMake as a build system.
- There is no strict limitation on the version of the C++ standard. Feel free to use any version you need (including the latest ones).
- The utility accepts a query to search and a directory path and does a recursive search in a given directory.
- The utility outputs results to the console.
- The utility should work reasonably fast (e.g. utilize all cores during the search).
- The solution should contain a written design document (README) and also include tests. You may use test frameworks if you want.

Requests:
- Please send the resulting work as a ZIP archive. 
- Please do not put.git directory into the archive.
- Please do not put your name anywhere on your submitted assignment.

Usage:
```bash
$ ./my_grep "KEYWORD" "PATH"
```

Notes:
- Search is always case-sensitive
- first argument is always a `KEYWORD`
- second argument is always a `PATH`
- print of findings is not ordered
