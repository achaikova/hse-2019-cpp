# Huffman Archiver
- Archiver based on the Huffman algorithm that can compress and decompress both binary and text files.
## Command-Line Options
- Four command line flags:
    - `-c` - compress
    - `-u` - uncompress
    - `-f`, `--file <path>` - input file name
    - `-o`, `--output <path>`- result file name

- The value of the parameter (if any) is specified through a space.
- Parameters can run in any order, extra parameters are not allowed.
##  Display
### Example
- Source file size (source data): 15,678 bytes, compressed data size: 6,172 bytes, additional data size: 482 bytes. Size of all compressed file: 6172 + 482 = 6654 bytes.
```
./HuffmanArchiver -c -f myfile.txt -o result.bin
15678
6172
482
```
- Size of extracted file (received data): 15,678 bytes, size of compressed data: 6,172 bytes, size of additional data: 482 bytes. Size of all original compressed file: 6172 + 482 = 6654 bytes.
```
./HuffmanArchiver -u -f result.bin -o myfile_new.txt
6172
15678
482
```
## Runtime Output Example
```
257034822
102341525
58

real	2m29.767s
user	2m12.850s
sys	0m2.006s
```
## Usage of tests
```
make
./test_HuffmanArchiver 
```
- Expected Outcome
```
[doctest] doctest version is "2.3.7"
[doctest] run with "--help" for options
===============================================================================
[doctest] test cases:      3 |      3 passed |      0 failed |      0 skipped
[doctest] assertions:     20 |     20 passed |      0 failed |
[doctest] Status: SUCCESS!
```
