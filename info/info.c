#include "info.h"
#include <stdio.h>
#include <stdlib.h>

void infoMsg(char *msg)
{
    printf("[\033[34m\033[1m*\033[0m] %s\n", msg);
}

void successMsg(char *msg)
{
    printf("[\033[32m\033[1m+\033[0m] %s\n", msg);
}

void errMsg(char *msg)
{
    printf("[\033[31m\033[1mx\033[0m] %s\n", msg);
}

void infoMsgMore(char *msg, char *append_msg)
{
    printf("[\033[34m\033[1m*\033[0m] %s %s\n", msg, append_msg);
}

void successMsgMore(char *msg, char *append_msg)
{
    printf("[\033[32m\033[1m+\033[0m] %s %s\n", msg, append_msg);
}

void errMsgMore(char *msg, char *append_msg)
{
    printf("[\033[31m\033[1mx\033[0m] %s %s\n", msg, append_msg);
}

void errExit(char *msg)
{
    printf("\033[31m\033[1m[x] FATAL ERROR:\033[0m ");
    puts(msg);
    exit(-1);
}