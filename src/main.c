
#include "hex2dec.h"

int is_valid_num(char c) {
    return (c >= '0' && c <= '9');
}

int is_valid_hex_alpha_upper(char c) {
    return (c >= 'A' && c <= 'F');
}

int is_valid_hex_alpha_lower(char c) {
    return (c >= 'a' && c <= 'f');
}

int is_valid_hex_alpha(char c) {
    return is_valid_hex_alpha_upper(c) || is_valid_hex_alpha_lower(c);
}

int is_valid_hex(char c) {
    return is_valid_num(c) || is_valid_hex_alpha(c);
}

int main(int argc, char *argv[])
{
    if (argc == 1) {
        fprintf(stderr, "No input(s).\n");

        return EXIT_FAILURE;
    }
    
    while (*++argv) {
        mpz_t n;
        mpz_init(n);

        for (size_t i = 0; i < strlen(*argv); ++i) {
            /** Check for the initial prefix hex values sometimes have: '0x'.
             *  
             *  The check is performed by testing whether we're at the start
             *  of the string and the current character is a zero. If both
             *  conditions are true, the third check makes sure the next char
             *  is a lowercase 'x'. This check thus ensures that we're not
             *  skipping over a number that is just prefixed with a zero. While
             *  this program does not handle numbers in octal notation in its
             *  current form, implementing this would not be difficult, and in
             *  any case, it's just good practice to verify the entire prefix
             *  is there. 
             *
             *  If the number is prefixed by a zero but the next char
             *  is not a lower case 'x', the current value will be multiplied
             *  by the valid hex character '0', resulting in a non-update to
             *  the value, after which the index counter 'i' will be 
             *  incremented, an invalid value will be found if it's there, and
             *  an error message will be posted.
             *
             *  If a number is entered in 
             *  octal notation, it will be interpreted as hex in the current
             *  version of the program.
             *
             */
            if (i == 0 && (*argv)[i] == '0' && (*argv)[i+1] == 'x') {
                ++i;
                continue;
            }

            if ((*argv)[i] == 'h' || (*argv)[i] == 'H') {
                // Skip over the hex specifier
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

            // If the number has invalid hex characters, exit with ERROR.
            fprintf(stderr, "[Error] Invalid value in number.\n");
            return EXIT_SUCCESS;
        }

        gmp_printf("%Zd\n", n);
        mpz_clear(n);
    }

    return EXIT_SUCCESS;
}

