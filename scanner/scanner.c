#include <string.h>

#include "scanner.h"
#include "info/info.h"

#define is_upper(chr) \
    (chr >= 'A' && chr <= 'Z')

#define is_lower(chr) \
    (chr >= 'a' && chr <= 'z')

#define is_alpha(chr) \
    (is_upper(chr) || is_lower(chr))

#define is_digit(chr) \
    (chr >= '0' && chr <= '9') 

#define is_alnum(chr) \
    (is_alpha(chr) || is_digit(chr))

#define to_upper(chr) \
    (is_lower(chr) ? (chr ^ ' ') : chr)

#define is_space(chr) \
    (chr == ' ' || chr == '\t' || chr == '\r' || chr == '\n' || chr == '\v' || chr == '\f')

#define add_to_token_buffer(chr) \
do \
{ \
    if (token_buffer_loc == MAX_TOKEN_LEN) \
        errExit("Token too long! Check your config file!"); \
    token_buffer[token_buffer_loc++] = chr; \
}while(0)

#define valid_chr(chr) \
    (is_alpha(chr) || chr == '_' || chr == '.' || chr == '/')

int get_token(FILE *file, char *dst)
{
    char    per_chr;

    // init buffer
    memset(token_buffer, 0, sizeof(token_buffer));
    token_buffer_loc = 0;

    // clear the space
    while (1)
    {
        per_chr = getc(file);
        if (per_chr == '\n')
            line_no++;
        else if (per_chr == EOF)
            return -1;
        else if (!is_space(per_chr)) // token start
            break;
    }
    add_to_token_buffer(per_chr);   //  first non-space char, add to token buffer as first char

    // check for alpha
    if (valid_chr(per_chr))
    {
        while (1)
        {
            per_chr = getc(file);
            if (!(valid_chr(per_chr) || is_digit(per_chr)))
                break;
            add_to_token_buffer(per_chr);
        }
        ungetc(per_chr, file);  // not the char we seek for, push it back
    }
    else if (is_digit(per_chr))
    {
        while (1)
        {
            per_chr = getc(file);
            if (!is_digit(per_chr))
                break;
            add_to_token_buffer(per_chr);
        }
        ungetc(per_chr, file);
    }
    else
    {
        switch (per_chr)
        {
            case '=':
                    break;
            default:
                    errExit("Unexpected char!");
                    break;
        }
    }

    // copy to user
    add_to_token_buffer('\0');
    strcpy(dst, token_buffer);

    return 0;
}

void fetch_token(FILE *file, char *obj)
{
    char    tmp_token[MAX_TOKEN_LEN];
    get_token(file, tmp_token);

    if (strcmp(tmp_token, obj))
        errExit("Failed to fetch correct token in file!");
}