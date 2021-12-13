#ifndef _A3_SHOP_H_
#define _A3_SHOP_H_

#include <stdio.h>
#include "user/user.h"

struct shop_operations;
struct online_shop_manager;

struct shop_operations
{
    void    (*shop_init)        (struct online_shop_manager *shop, const char *user_data_path, const char *item_data_path);
    char*   (*user_login)       (struct online_shop_manager *shop, const char *username, const char *password);
    void    (*user_logout)      (struct online_shop_manager *shop, char *cookie);
    int     (*user_register)    (struct online_shop_manager *shop, const char *username, const char *password);
    void    (*login_menu)       (struct online_shop_manager *shop);
    void    (*shop_menu)        (struct online_shop_manager *shop);
    void    (*set_prev_info)    (struct online_shop_manager *shop, char *prev_info, int type);
    void    (*item_add)         (struct online_shop_manager *shop, int item_id, const char *item_name, int item_nums, double item_price, char *cookie);
    void    (*item_show)        (struct online_shop_manager *shop, int item_id, char *cookie);
    void    (*item_change)      (struct online_shop_manager *shop, int item_id, const char *item_name, int item_nums, double item_price, char *cookie);
    void    (*item_del)         (struct online_shop_manager *shop, int item_id, char *cookie);
    void    (*save_data)        (struct online_shop_manager *shop, char *cookie);
    int     (*verify_cookie)    (struct online_shop_manager *shop, char *cookie);
};

struct item
{
    int             id;
    char            *name;
    int             nums;
    double          price;
    struct item     *next;
};

struct online_shop_manager
{
    const struct shop_operations    *s_ops;
    struct user_manager *user_manager;
    char            *user_data_path;
    char            *item_data_path;
    struct item     *item_list;
    struct item     *item_to_print;
    char            *prev_info;
    int             prev_info_type;
};

extern struct online_shop_manager* get_new_shop(void);
extern void shop_init(struct online_shop_manager *shop, const char *user_data_path, const char *item_data_path);
extern char* user_login(struct online_shop_manager *shop, const char *username, const char *password);
extern void user_logout(struct online_shop_manager *shop, char *cookie);
extern int  user_register(struct online_shop_manager *shop, const char *username, const char *password);
extern void login_menu(struct online_shop_manager *shop);
extern void shop_menu(struct online_shop_manager *shop);
extern void set_prev_info(struct online_shop_manager *shop, char *prev_info, int type);
extern void item_add(struct online_shop_manager *shop, int item_id, const char *item_name, int item_nums, double item_price, char *cookie);
extern void item_show(struct online_shop_manager *shop, int item_id, char *cookie);
extern void item_change(struct online_shop_manager *shop, int item_id, const char *item_name, int item_nums, double item_price, char *cookie);
extern void item_del(struct online_shop_manager *shop, int item_id, char *cookie);
extern void save_data(struct online_shop_manager *shop, char *cookie);
extern int  verify_cookie(struct online_shop_manager *shop, char *cookie);

static struct shop_operations shop_ops = 
{
    .shop_init = shop_init,
    .user_login = user_login,
    .user_logout = user_logout,
    .user_register = user_register,
    .login_menu = login_menu,
    .shop_menu = shop_menu,
    .set_prev_info = set_prev_info,
    .item_add = item_add,
    .item_show = item_show,
    .item_change = item_change,
    .item_del = item_del,
    .save_data = save_data,
    .verify_cookie = verify_cookie,
};

enum shop_action
{
    ADD_ITEM_ACTION = 1,
    SHOW_ITEM_ACTION,
    CHANGE_ITEM_ACTION,
    DELETE_ITEM_ACTION,
    SAVE_DATA_ACTION,
    LOGOUT_ACTION,
};

enum login_action
{
    LOGIN_ACTION = 1,
    REGISTER_ACTION,
    EXIT_ACTION,
};

enum prev_info_type
{
    PREV_INFO,
    PREV_SUCCESS,
    PREV_ERROR,
    PREV_ITEM_PRINT,
};

enum cookie_verify_type
{
    COOKIE_VERIFY_FAILED = 0,
    COOKIE_VERIFY_SUCCESS,
};

#endif