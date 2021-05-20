# Econometrics
Labs for subject of econometrics
## Prerequisites
For Windows I use Cygwin. Therefore, you first need to install it (https://cygwin.com).

Requirements:
* gcc;
* CMake.
### GNU Scientific Library
Unpack the archive gsl-latest.tar.gz and install it.
(If you want the latest version download the archive here: https://www.gnu.org/software/gsl/).
As you might have guessed, I am running this script in Cygwin-terminal.
```shell script
$ ./configure
$ ./make
$ ./make install
```
For a more detailed installation of this library, you can refer to the `INSTALL` file supplied with the library.

Congratulations!
### Other solutions
If you have found a more elegant solution for connecting gsl, then please write to me about it.