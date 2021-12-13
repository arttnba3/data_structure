#ifndef _A3_USER_H_
#define _A3_USER_H_

#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

static const char encrypt_salt[0x100] = "$6$arttnba3$";

struct user_manager;

struct user_info
{
    char    *username;
    char    *password;
    char    *cookie;
    struct user_info    *next;
};

struct user_operations
{
    char*   (*user_login)   (struct user_manager *user_manager, const char *username, const char *password);
    void    (*user_logout)  (struct user_manager *user_manager, const char *cookie);
    int     (*user_register)(struct user_manager *user_manager, const char *username, const char *password);
};

struct user_manager
{
    const struct user_operations    *u_ops;
    struct user_info    *user_list;
};

extern struct user_manager* get_new_user_manager(void);
extern char* inner_user_login(struct user_manager *user_manager, const char *username, const char *password);
extern void inner_user_logout(struct user_manager *user_manager, const char *cookie);
extern int inner_user_register(struct user_manager *user_manager, const char *username, const char *password);

static struct user_operations user_ops = 
{
    .user_login = inner_user_login,
    .user_logout = inner_user_logout,
    .user_register = inner_user_register,
};

enum register_action
{
    REGISTER_FAILED = 0,
    REGISTER_SUCCESS,
};

#endif