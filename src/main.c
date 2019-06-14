
#include "hex2dec.h"

int is_valid_num(char c) {
    return ((c >= '0') && (c <= '9'));
}

int is_valid_hex_alpha_upper(char c) {
    return ((c >= 'A') && (c <= 'F'));
}

int is_valid_hex_alpha_lower(char c) {
    return ((c >= 'a') && (c <= 'f'));
}

int is_valid_hex_alpha(char c) {
    return is_valid_hex_alpha_upper(c) || is_valid_hex_alpha_lower(c);
}

int is_valid_hex(char c) {
    return is_valid_num(c) || is_valid_hex_alpha(c);
}

void print_license_info(void) {
    printf("This program is free software; you may redistribute it under the terms of\n");
    printf("the GNU General Public License version 3 or (at your option) a later version.\n");
    printf("This program has absolutely no warranty.\n\n");
}

void print_version_info(void) {
    printf("hex2dec Version 3.0.0\n");
    printf("Copyright (C) 2019, Jose Fernando Lopez Fernandez.\n\n");

    print_license_info();
}

void print_help(void) {
    print_version_info();
    printf("\n");
    printf("Usage: hex2dec [OPTIONS] NUMBER [NUMBERS...]\n\n");
    printf("    -h, --help        Print this help menu and exit\n");
    printf("        --version     Print program version information and exit\n");
    printf("    -v, --verbose     Print detailed info during execution\n\n");
}

int main(int argc, char *argv[])
{
    // When the program starts, verify some arguments were passed in. If the user
    // did not supply any arguments when executing the program, argc will equal 1,
    // meaning argv contains only the program name.
    //
    // As we cannot convert a hexadecimal number to decimal without the aforementioned
    // hexadecimal number, we will then simply notify the user he failed to provide
    // any arguments and print the help menu.
    if (argc == 1) {
        fprintf(stderr, "No input(s).\n");

        print_help();

        return EXIT_FAILURE;
    }

    // Locale info struct for pretty-printing using locale-specific formatting.
    // It is not initialized unless the user specifically asks for pretty-printed
    // output via the '-p' or '--pretty-print' options.
    struct lconv* lc = NULL;

    // Available program options
    int option_print_with_locale_formatting = FALSE;
    int option_verbose_output = FALSE;
    
    // Check for options
    // TODO: Add option to print in custom locale as specified via command line.
    for (int i = 1; i < argc; ++i) {
        // Since any options will begin with a dash, there is no need to process
        // any input strings that do not contain a leading dash. Any invalid 
        // input strings will be handled by the numeric parser proper. Therefore,
        // Check if the input string begins if a dash. If it does, look for the
        // corresponding match. Otherwise, skip it.
        if (argv[i][0] != '-') {
            // Input string does not begin with a dash, so it is not an option.
            // Skip input string by prematurely ending the current iteration.
            continue;
        }

        // If the input string is a valid option, set the corresponding flag and
        // continue to the next input string. Otherwise, alert the user about the
        // invalid flag and exit with error.
        if ((strcmp(argv[i],"-h") == 0) || (strcmp(argv[i],"--help") == 0)) {
            // The '--help' or '-h' options will short-circuit execution, and will
            // immediately print the help menu and exit the program with a 
            // return code of 0 (EXIT_SUCCESS).
            print_help();

            return EXIT_SUCCESS;
        } else if (strcmp(argv[i],"--version") == 0) {
            // The '--version' option will also short-circuit the program execution
            // immediately, resulting in the program's version information being
            // printed to standard out, and the program subsequently exiting 
            // with a return code of 0 (EXIT_SUCCESS).
            print_version_info();

            return EXIT_SUCCESS;
        } else if ((strcmp(argv[i],"-p") == 0) || (strcmp(argv[i],"--pretty-print") == 0)) {
            option_print_with_locale_formatting = TRUE;
        } else if ((strcmp(argv[i],"-v") == 0) || (strcmp(argv[i],"--verbose") == 0)) {
            option_verbose_output = TRUE;
        } else {
            // To allow the user to specify options wherever they wish (i.e., before
            // or after the inputs), we consider any input begining with a dash 
            // an option. Therefore, we must handle any invalid options here.
            fprintf(stderr, "[Error] Invalid option: %s\n", argv[i]);

            return EXIT_FAILURE;
        }
    }

    // If pretty-printing enabled, set up locale-specific info
    if (option_print_with_locale_formatting == TRUE) {
        // Check the configured system locale by passing 0 as the locale category.
        char* system_locale = setlocale(0, "");
        
        // Handle situation where system_locale is NULL.
        if (system_locale == NULL) {
            // If the user requested verbose output, notify them that the system
            // locale is NULL. Do not do this if the user did not explicitly request
            // verbose output, as this may unnecessarily scare them into thinking
            // this is more serious than it is.
            if (option_verbose_output == TRUE) {
                printf("System locale is NULL. Will attempt to use default.\n");
            }
        } else {
            // If the user requested verbose output, print the system locale to 
            // standard output. It is safe to do so because we have verified that
            // the system_locale pointer is not NULL.
            if (option_verbose_output == TRUE) {
                printf("System locale: %s\n", system_locale);
            }
        }

        // To set program locale settings in accordance with the configured
        // system locale, we need to call 'setlocale' with an empty string for
        // the locale, which conventionally means "Get locale configuration from
        // the environment"
        //
        // If you would like to compile the program to use a different locale,
        // rather than the system locale, you may directly define the LOCALE
        // variable when invoking the compiler, like so:
        //
        //      -DLOCALE=\"en_US.UTF-8\"
        //
        // Simply replace the example locale with the one you wish to use. Keep
        // in mind that the locale you select will not be overridden unless the
        // system cannot find it on your local environment, in which case it will
        // attempt to use the locale specified by the DEFAULT_LOCALE setting below.

        #ifndef LOCALE
        #define LOCALE ""
        #endif // LOCALE

        char* locale = setlocale(LC_ALL, LOCALE);

        // The return value of 'setlocale' will be the name of the locale that
        // was set. If there was an error of any kind, however, and the program
        // locale could therefore not be successfully set, the return value will
        // be NULL.
        if (locale == NULL) {
            // If the system locale could not be queried to set the program
            // locale, default to a predefined locale.
            
            // Note: The current default is United States English with UTF-8
            // character encoding. If you wish to use a different default, 
            // modify this accordingly. You may also directly define the 
            // DEFAULT_LOCALE variable when compiling like so:
            //      
            //      -DDEFAULT_LOCALE=\"en_US.UTF-8\"
            //
            // Simply replace the example locale with the one you wish to use.

            #ifndef DEFAULT_LOCALE
            #define DEFAULT_LOCALE "en_US.UTF-8"
            #endif // DEFAULT_LOCALE

            char* default_locale = DEFAULT_LOCALE;

            // If the user requested verbose output, notify them about the 
            // failure to set locale from the system default. Only do this if 
            // the user specifically requested verbose output, as the casual 
            // user may be scared into thinking this is a bigger issue than it 
            // actually is.
            if (option_verbose_output == TRUE) {
                printf("Could not set locale: %s\n", LOCALE);
                printf("Defaulting to '%s'\n", default_locale);
            }

            // Retry setting the locale with the specified default.
            locale = setlocale(LC_ALL, default_locale);

            // TODO: Use default separator and numeric groups setting to more
            // robustly handle failure.
            //
            // If the call to 'setlocale' failed again, notify the user of the
            // error and simply turn off the 'pretty-print' option.
            if (locale == NULL) {
                fprintf(stderr, "[Error] Failed to get locale information for locale: %s\n", default_locale);

                option_print_with_locale_formatting = FALSE;

                // Having disabled pretty-printing, skip the next part of the
                // locale setting setup, as a NULL locale pointer will trigger
                // a segmentation fault because of the NULL pointer dereference.
                goto SKIP;
            } // End second locale == NULL check
        } // End first locale == NULL check

        // If the user requested verbose output, notify them about the locale
        // that was set.
        if (option_verbose_output == TRUE) {
            // This will crash the program if locale = NULL if we don't skip it
            // because we would then be dereferencing a NULL pointer.
            printf("Locale set: %s\n", locale);
        }

        // Call 'localeconv' to retrieve locale settings such as thousands 
        // separator or decimal point character for the locale set with the 
        // 'setlocale' function call.
        //
        // The locale settings are stored in a heap-allocated structure pointed
        // to by the 'lc' pointer. Documentation for the 'struct lconv' data type
        // may be found at https://en.cppreference.com/w/c/locale/lconv.
        //
        // This will also crash we do not skip this when locale = NULL because 
        // if locale is NULL, no locale was set, and therefore the localeconv 
        // function cannot function as expected.
        lc = localeconv();
    }

SKIP: /* Safely prevented dereferencing NULL locale pointer */ ;

    // Allocate and initialize arbitrary-precision integer to hold decimal
    // value for conversion of any size hexadecimal number.
    mpz_t n;
    mpz_init(n);

    // Process input
    while (*++argv) {
        // Before beginning to process the input in earnest, check the first 
        // character. If it's a dash, consider it an option and skip it.
        if ((*argv)[0] == '-') {
            // Avoid processing the current input by simply skipping the execution
            // of the current iteration. There is no need to manually advance the
            // argument pointer, as the loop stop condition does this itself. 
            // Otherwise, we would cause the program to skip two arguments.
            continue;
        }

        // To avoid allocating and deallocating the output variable on each
        // iteration, simply reset its value to 0 at the start of each input-
        // processing step.
        mpz_set_ui(n, 0);

        for (size_t i = 0; i < strlen(*argv); ++i) {
            // Check for the initial prefix hex values sometimes have: '0x' or '0X'.
            //  
            // The check is performed by testing whether we're at the start
            // of the string and the current character is a zero. If both
            // conditions are true, the third check makes sure the next char
            // is a lowercase 'x'. This check thus ensures that we're not
            // skipping over a number that is just prefixed with a zero. While
            // this program does not handle numbers in octal notation in its
            // current form, implementing this would not be difficult, and in
            // any case, it's just good practice to verify the entire prefix
            // is there. 
            //
            // If the number is prefixed by a zero but the next char
            // is not a lower case 'x', the current value will be multiplied
            // by the valid hex character '0', resulting in a non-update to
            // the value, after which the index counter 'i' will be 
            // incremented, an invalid value will be found if it's there, and
            // an error message will be posted.
            //
            // If a number is entered in octal notation, it will be interpreted
            // as hex in the current version of the program.
            if ((i == 0) && ((*argv)[i] == '0') && (((*argv)[i+1] == 'x') || ((*argv)[i+1] == 'X'))) {
                ++i;
                continue;
            }

            if ((*argv)[i] == 'h' || (*argv)[i] == 'H') {
                // If we hit the hex specifier we're done.
                break;
            }

            if (is_valid_hex_alpha_upper((*argv)[i])) {
                mpz_mul_si(n, n, 16);
                mpz_add_ui(n, n, (unsigned long int) (*argv)[i] - 'A' + 10);

                continue;
            }

            if (is_valid_hex_alpha_lower((*argv)[i])) {
                mpz_mul_si(n, n, 16);
                mpz_add_ui(n, n, (unsigned long int) (*argv)[i] - 'a' + 10);

                continue;
            }

            if (is_valid_num((*argv)[i])) {
                mpz_mul_si(n, n, 16);
                mpz_add_ui(n, n, (unsigned long int) (*argv)[i] - '0');

                continue;
            }

            // Every valid character value explicitly continues the loop, so if 
            // we hit this point, there are invalid characters, in the input. 
            // More robust error handling would be nice, or maybe the option to
            // simply skip invalid characters but for now simply exit with an
            // error status.
            fprintf(stderr, "[Error] Invalid value in number.\n");
            return EXIT_FAILURE;
        }

        // Pretty print if specified via command-line option
        if (option_print_with_locale_formatting == TRUE) {
            
            // Get number string by calling 'mpz_get_str'. The 'NULL' parameter
            // means we are not supplying our own string. Instead, the function
            // allocates the string on its own, but we must remember to free it,
            // or else leak the allocated memory.
            char* num_str = mpz_get_str(NULL, 10, n);
            
            // Get number of digits per digit group
            errno = 0;

            // In case something goes wrong or the locale setting specified a
            // weird number of digits per group, set the number of digits per
            // group to whatever the default is, as specified by the 
            // DEFAULT_DIGITS_PER_GROUP variable.

            #ifndef DEFAULT_DIGITS_PER_GROUP
            #define DEFAULT_DIGITS_PER_GROUP 3
            #endif // DEFAULT_DIGITS_PER_GROUP

            long digits_per_group = strtol(lc->grouping, NULL, 10);

            // Check for various possible errors that could have occurred during
            // 'strtol' call.
            if (((errno == ERANGE) && ((digits_per_group == LONG_MAX) || (digits_per_group == LONG_MIN))) || ((errno != 0) && (digits_per_group == 0))) {
                // On failure, print the system error message by calling 'strerror'
                // and print the 'lc->grouping' string so the user can see what the
                // original string was.
                fprintf(stderr, "[Error] Failed to get locale-specific number of digits per group: %s (%s) Defaulting to %d\n", lc->grouping, strerror(errno), DEFAULT_DIGITS_PER_GROUP);

                // Set the number of digits_per_group to 0 so it can be fixed 
                // down the line.
                digits_per_group = 0;
            }

            // Set digits_per_group to the previously established default if
            // something went wrong.
            if (digits_per_group == 0) {
                digits_per_group = DEFAULT_DIGITS_PER_GROUP;
            }
            
            // Since we're parsing from left to right and adding separators, we
            // need to make sure the left-most group only has a separator after
            // the digits in a group if the whole number has a number of digits
            // divisible by the number of digits in a group. Otherwise, with 
            // numbers like 8022, we want the first group from left-to-right to 
            // have a separator after the 8, not after the 802, yielding 802,2. 
            // To do this, we set the initial number of digits in the current 
            // group to the length of the string modulo three, giving us the 
            // expected output.
            long digits_in_group = strlen(num_str) % digits_per_group;

            // Print the original input string first
            printf("%32s = ", *argv);

            // Then print the converted string
            for (size_t i = 0; i < strlen(num_str); ++i) {
                
                // Print a separator character after the requisite number of 
                // digits in a group has been printed. Then reset the counter of
                // digits in the current group, and continue printing digits,
                // making sure to increase the number of digits in the current
                // group.
                if (digits_in_group == digits_per_group) {
                    printf("%s", lc->thousands_sep);
                    digits_in_group = 0;
                }

                printf("%c", num_str[i]);
                ++digits_in_group;
            }

            // Free the memory allocated by 'mpz_get_str'.
            free(num_str);
        } else if (option_print_with_locale_formatting == FALSE) {
            // Since the user did not request locale-specific formatting, simply
            // use the built-in GMP printf function to print the number with no
            // pomp and circumstance.
            gmp_printf("%Zu", n);
        }

        // Print a newline character after the number.
        printf("\n");
    }
        
    // Deallocate the output number only after all inputs have been processed.
    // This is an execution optimization that simply resets the value of the 
    // number at the start of each iteration, requiring only a single allocation
    // and deallocation at the start and end of the program, respectively.
    mpz_clear(n);

    return EXIT_SUCCESS;
}

