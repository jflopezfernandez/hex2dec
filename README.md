
# hex2dec

`hex2dec` is a hexadecimal to decimal command line converter utility. It supports multiple precision arithmetic as well as locale-dependent formatting via the `--pretty-print` option.

# Usage

Assuming it's in your path, the converter can be called using `hex2dec`, or if it is not in your path, it can be called from the same directory by relative addressing like so:

```bash
$ ./hex2dec
```

With no input, the help menu is printed.

```bash
hex2dec Version 3.0.0
Copyright (C) 2019, Jose Fernando Lopez Fernandez.

This program is free software; you may redistribute it under the terms of
the GNU General Public License version 3 or (at your option) a later version.
This program has absolutely no warranty.


Usage: hex2dec [OPTIONS] NUMBER [NUMBERS...]

    -h, --help        Print this help menu and exit
        --version     Print program version information and exit
    -v, --verbose     Print detailed info during execution
```

To convert a hexadecimal number of arbitrary size, simply pass it in as a command-line argument.

```bash
$ ./hex2dec 0xABCDEFABCDEFABCDEFABCDEF
53170898287292916380478459375
```

The output can be pretty printed using the `--pretty-print` option, as well as the `-p` short option, like so:

```bash
$ ./hex2dec --pretty-print 0xABCDEFABCDEFABCDEFABCDEF
0xABCDEFABCDEFABCDEFABCDEF = 53,170,898,287,292,916,380,478,459,375
```

For locale-specific output, the application currently queries the system configuration for the locale. The locale can be manually overridden by defining the `LOCALE` preprocessor macro with the desired locale.

> Note: The requested locale must be installed on your system in order to function as expected.

# Building

The project makefile is pretty straightforward; all of the usual configuration variables may be overridden to configure the build as you see fit. Specifically, you may configure the C compiler to use with `CC`, the compiler flags with `CFLAGS`, preprocessor flags with `CPPFLAGS`, and linker flags with `LDFLAGS`.

Below is a sample build with the `CC` and `CFLAGS` being overridden.

```bash
$ make -B CC=gcc-9.1.0 CFLAGS="-std=c17 -Wall -Wextra -Wpedantic -Werror -O3 -mtune=intel -march=skylake"
gcc-9.1.0 -I include -D_POSIX_C_SOURCE -D_GNU_SOURCE  -std=c17 -Wall -Wextra -Wpedantic -Werror -O3 -mtune=intel -march=skylake -c -o main.o src/main.c
gcc-9.1.0 -std=c17 -Wall -Wextra -Wpedantic -Werror -O3 -mtune=intel -march=skylake  -lm -lgmp  -o hex2dec main.o
```

Keep in mind that whatever compiler you set with `CC` is itself called as the linker driver, which you would then pass flags to with the usual `-Wl,-s` flag for example, if you were using `gcc`. So keep that in mind, as the compiler will call whatever the default linker is unless you explicitly change it, or you modify the makefile to directly call a linker like `gold` or `ld` yourself.
