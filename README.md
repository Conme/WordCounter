# WordCounter

A straightforward C program counting the frequency each word appears in a piece of text. Initially the text, which is passed either from a text file as an argument or through the standard input, is partitioned into a vector of words. These words are subsequently counted and printed alphabetically along with their frequency on standard output.

The base of this project was a simple [programming self-test](http://www.st.ewi.tudelft.nl/koen/cs4140/test.pdf) taken during my Masters, the output format, the supported word structures(special characters like '-', '.', '@' etc are allowed inside words) and the performance of which were upgraded to make it more user-friendly, useful and well-performing.

## Getting Started

As a first step you can get a copy of the code by either cloning the repository to an existing folder in a system featuring git using the following command:
```
git clone https://github.com/Conme/WordCounter
```
Or by using a git interface like [github](https://help.github.com/en/github/creating-cloning-and-archiving-repositories/cloning-a-repository).

### Prerequisites

To use the build system included in the repository, [CMake](https://cmake.org) is essential for both Unix and Windows systems, while for Windows the MSBuild compiler included in all the recent versions of Visual Studio suites is needed. For Unix systems any CMake-supported and C11-compatible compiler is suitable. Make sure that both the location of CMake and the location of the compiler to be used are in the system's PATH variable.

### Compiling

Running the [winCompile](winCompile.bat) script for Windows and the [unixCompile](unixCompile.sh) script for Unix systems will produce a slower binary including more debug information when "Debug" argument is passed or a faster binary, optimized for the machine used to build it, when "Release" argument is passed. The binaries produced by the CMake-based build system can be found in "./build/Debug" and "./build/Release" for Debug and Release configurations respectively.

In case the CMake-based build system is not chosen to be used, you can use your prefered C11-compatible compiler to compile the files in the [source](src) and the [include](include) folders.

## Running the program

The produced binary can be run by either passing the input text through a file:

```
./WordCounter [INFILE]		for Unix
```
```
WordCounter.exe [INFILE]	for Windows
```
Or in an interactive mode running the program and typing your input in the command line ending it with an 'EOF' character.

The output is printed in the standard output and can be redirected into a file:
```
./WordCounter [INFILE] > [OUTFILE]		for Unix
```
```
WordCounter.exe [INFILE] > [OUTFILE]	for Windows
```
## Tested on

Ubuntu 18.04LTS with gcc 8.3

and on

Windows 7 with Visual Studio 14

## Contributing

In case you are interested in contributing in any way and for any remarks regarding this code, feel free to contact me on konpmetaxas@gmail.com

## Authors

* **Konstantinos Metaxas** - *Initial work* - [Conme](https://github.com/Conme)

## License

This project is licensed under the version 3 of the GNU General Public License (GPLv3) - see the [LICENSE.md](LICENSE.md) file for details.

## Acknowledgments

Special thanks to Sean Eron Anderson for his [Bit Twiddling Hacks code](https://graphics.stanford.edu/~seander/bithacks.html) and to Professor Koen Langendoen for the assignment which inspired me.
