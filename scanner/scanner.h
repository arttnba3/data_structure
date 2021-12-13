#ifndef _A3_SCANNER_H_
#define _A3_SCANNER_H_

#include <stdio.h>

#define MAX_TOKEN_LEN 0x200

static char token_buffer[MAX_TOKEN_LEN];
static int token_buffer_loc = 0;
static int line_no = 1;

extern int get_token(FILE *file, char *dst);
extern void fetch_token(FILE *file, char *obj);

#endif