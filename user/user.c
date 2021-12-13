#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

#include "user.h"

struct user_manager* get_new_user_manager(void)
{
    struct user_manager     *user_manager;

    user_manager = (struct user_manager*) malloc(sizeof(struct user_manager));
    user_manager->u_ops = &user_ops;
    user_manager->user_list = NULL;

    return user_manager;
}

char* inner_user_login(struct user_manager *user_manager, const char *username, const char *password)
{
    struct user_info    *u_info;
    char                *encrypt_pw;
    char                time_salt[0x200];

    encrypt_pw = crypt(password, encrypt_salt);
    u_info = user_manager->user_list;
    while (u_info)
    {
        if (!strcmp(u_info->username, username))
        {
            if (strcmp(u_info->password, encrypt_pw))
            {
                return NULL;
            }
            else
            {
                sprintf(time_salt, "%ld", time(NULL));
                encrypt_pw = crypt(encrypt_pw, time_salt);
                u_info->cookie = (char*) malloc(sizeof(char) * (strlen(encrypt_pw) + 1));
                strcpy(u_info->cookie, encrypt_pw);
                return u_info->cookie;
            }
        }
        u_info = u_info->next;
    }

    // not found
    return NULL;
}

void inner_user_logout(struct user_manager *user_manager, const char *cookie)
{
    struct user_info    *u_info;

    u_info = user_manager->user_list;
    while (u_info)
    {
        if (u_info->cookie && !strcmp(u_info->cookie, cookie))
        {
            free(u_info->cookie);
            u_info->cookie = NULL;
            return ;
        }
        u_info = u_info->next;
    }
}

int inner_user_register(struct user_manager *user_manager, const char *username, const char *password)
{
    struct user_info    *u_info;
    char                *encrypt_pw;
    char                time_salt[0x200];

    
    u_info = user_manager->user_list;
    while (u_info)
    {
        if (!strcmp(u_info->username, username))
        {
            return REGISTER_FAILED;
        }
        u_info = u_info->next;
    }

    u_info = (struct user_info*) malloc(sizeof(struct user_info));
    u_info->username = (char*) malloc((strlen(username) + 1) * sizeof(char));
    strcpy(u_info->username, username);
    encrypt_pw = crypt(password, encrypt_salt);
    u_info->password = (char*) malloc(sizeof(char) * (strlen(encrypt_pw) + 1));
    strcpy(u_info->password, encrypt_pw);
    u_info->cookie = NULL;
    u_info->next = user_manager->user_list;
    user_manager->user_list = u_info;

    return REGISTER_SUCCESS;
}