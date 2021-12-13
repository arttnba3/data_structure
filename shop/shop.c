#include "shop.h"
#include "info/info.h"
#include "user/user.h"
#include <string.h>

struct online_shop_manager* get_new_shop(void)
{
    struct online_shop_manager  *shop;

    shop = (struct online_shop_manager*) malloc(sizeof(struct online_shop_manager));
    shop->s_ops = &shop_ops;
    shop->user_manager = get_new_user_manager();
    shop->item_list = NULL;
    shop->prev_info = NULL;

    return shop;
}

void shop_init(struct online_shop_manager *shop, const char *user_data_path, const char *item_data_path)
{
    FILE                *f_user_data;
    FILE                *f_item_data;
    char                buf[0x200];
    struct item        *per_item;
    struct user_info    *per_user;

    shop->user_data_path = (char*) malloc(sizeof(char) * (strlen(user_data_path) + 1));
    shop->item_data_path = (char*) malloc(sizeof(char) * (strlen(item_data_path) + 1));

    strcpy(shop->user_data_path, user_data_path);
    strcpy(shop->item_data_path, item_data_path);

    f_user_data = fopen(shop->user_data_path, "a+");
    f_item_data = fopen(shop->item_data_path, "a+");
    if ((!f_user_data) || (!f_item_data))
        errExit("Failed to open database file!");
    
    while (fscanf(f_item_data, "%511s", buf) != EOF)
    {
        per_item = (struct item*) malloc(sizeof(struct item));
        per_item->id = atoi(buf);

        if (fscanf(f_item_data, "%511s", buf) == EOF)
            errExit("Unexpected end of file in database!");
        per_item->name = (char*) malloc(sizeof(char) * (strlen(buf) + 1));
        strcpy(per_item->name, buf);

        if (fscanf(f_item_data, "%511s", buf) == EOF)
            errExit("Unexpected end of file in database!");
        per_item->nums = atoi(buf);
        
        if (fscanf(f_item_data, "%511s", buf) == EOF)
            errExit("Unexpected end of file in database!");
        per_item->price = atof(buf);

        per_item->next = shop->item_list;
        shop->item_list = per_item;
    }

    while (fscanf(f_user_data, "%511s", buf) != EOF)
    {
        per_user = (struct user_info*) malloc(sizeof(struct user_info));
        per_user->username = (char*) malloc(sizeof(char) * (strlen(buf) + 1));
        strcpy(per_user->username, buf);

        if (fscanf(f_user_data, "%511s", buf) == EOF)
            errExit("Unexpected end of file in database!");
        per_user->password = (char*) malloc(sizeof(char) * (strlen(buf) + 1));
        strcpy(per_user->password, buf);

        per_user->cookie = NULL;
        per_user->next = shop->user_manager->user_list;
        shop->user_manager->user_list = per_user;
    }

    fclose(f_item_data);
    fclose(f_user_data);
}

char* user_login(struct online_shop_manager *shop, const char *username, const char *password)
{
    return shop->user_manager->u_ops->user_login(shop->user_manager, username, password);
}

void user_logout(struct online_shop_manager *shop, char *cookie)
{
    shop->user_manager->u_ops->user_logout(shop->user_manager, cookie);
}

int user_register(struct online_shop_manager *shop, const char *username, const char *password)
{
    int     ret_val;
    char    *tmp_cookie;

    ret_val = shop->user_manager->u_ops->user_register(shop->user_manager, username, password);
    if (ret_val == REGISTER_SUCCESS) // automatically save the new user to file if success to register
    {
        tmp_cookie = shop->s_ops->user_login(shop, username, password);
        shop->s_ops->save_data(shop, tmp_cookie);
        shop->s_ops->user_logout(shop, tmp_cookie);
    }

    return ret_val;
}

void login_menu(struct online_shop_manager *shop)
{
    system("clear");
    puts("#######################################");
    puts("#######################################");
    puts("#####                             #####");
    puts("#####      Online Shop System     #####");
    puts("#####        version 0.0.1        #####");
    puts("#####    [PLEASE LOGIN FIRST!]    #####");
    puts("#####                             #####");
    puts("#####           1.login           #####");
    puts("#####           2.register        #####");
    puts("#####           3.exit            #####");
    puts("#####                             #####");
    puts("#######################################");
    puts("#######################################");
    if (shop->prev_info)
    {
        switch(shop->prev_info_type)
        {
            case PREV_INFO:
                    infoMsg(shop->prev_info);
                    break;
            case PREV_SUCCESS:
                    successMsg(shop->prev_info);
                    break;
            case PREV_ERROR:
                    errMsg(shop->prev_info);
                    break;
        }
        shop->prev_info = NULL;
    }
}

void shop_menu(struct online_shop_manager *shop)
{
    system("clear");
    puts("#######################################");
    puts("#######################################");
    puts("#####                             #####");
    puts("#####      Online Shop System     #####");
    puts("#####        version 0.0.1        #####");
    puts("#####                             #####");
    puts("#####       1.Add an item         #####");
    puts("#####       2.Show an item        #####");
    puts("#####       3.Change an item      #####");
    puts("#####       4.Delete an item      #####");
    puts("#####       5.Save the data       #####");
    puts("#####       6.logout              #####");
    puts("#####                             #####");
    puts("#######################################");
    puts("#######################################");
    if (shop->prev_info)
    {
        switch(shop->prev_info_type)
        {
            case PREV_INFO:
                    infoMsg(shop->prev_info);
                    break;
            case PREV_SUCCESS:
                    successMsg(shop->prev_info);
                    break;
            case PREV_ERROR:
                    errMsg(shop->prev_info);
                    break;
            case PREV_ITEM_PRINT:
                    puts("");
                    puts("#######################################");
                    printf("id: %d\n", shop->item_to_print->id);
                    printf("name: %s\n", shop->item_to_print->name);
                    printf("nums: %d\n", shop->item_to_print->nums);
                    printf("price: %lf\n", shop->item_to_print->price);
                    puts("#######################################");
                    puts("");
                    shop->item_to_print = NULL;
                    break;
        }
        shop->prev_info = NULL;
    }
}

void set_prev_info(struct online_shop_manager *shop, char *prev_info, int type)
{
    shop->prev_info = prev_info;
    shop->prev_info_type = type;
}

void item_add(struct online_shop_manager *shop, int item_id, const char *item_name, int item_nums, double item_price, char *cookie)
{
    struct item     *new_item;
    struct item     *old_item;

    if (shop->s_ops->verify_cookie(shop, cookie) == COOKIE_VERIFY_FAILED)
    {
        shop->s_ops->set_prev_info(shop, "Invalid cookie! Please try to relogin!", PREV_ERROR);
        return ;
    }

    old_item = shop->item_list;
    while (old_item)
    {
        if (old_item->id == item_id)
        {
            shop->s_ops->set_prev_info(shop, "Conflict item id!", PREV_ERROR);
            return ;
        }
        old_item = old_item->next;
    }

    new_item = (struct item*) malloc(sizeof(struct item));
    new_item->id = item_id;
    new_item->name = (char*) malloc(sizeof(char) * (strlen(item_name) + 1));
    strcpy(new_item->name, item_name);
    new_item->nums = item_nums;
    new_item->price = item_price;
    new_item->next = shop->item_list;
    shop->item_list = new_item;

    shop->s_ops->set_prev_info(shop, "Done.", PREV_SUCCESS);
}

void item_show(struct online_shop_manager *shop, int item_id, char *cookie)
{
    struct item     *old_item;

    if (shop->s_ops->verify_cookie(shop, cookie) == COOKIE_VERIFY_FAILED)
    {
        shop->s_ops->set_prev_info(shop, "Invalid cookie! Please try to relogin!", PREV_ERROR);
        return ;
    }

    old_item = shop->item_list;
    while (old_item)
    {
        if (old_item->id == item_id)
        {
            shop->item_to_print = old_item;
            shop->s_ops->set_prev_info(shop, "", PREV_ITEM_PRINT);
            return ;
        }
        old_item = old_item->next;
    }

    shop->s_ops->set_prev_info(shop, "Item not found!", PREV_ERROR);
}

void item_change(struct online_shop_manager *shop, int item_id, const char *item_name, int item_nums, double item_price, char *cookie)
{
    struct item     *old_item;

    if (shop->s_ops->verify_cookie(shop, cookie) == COOKIE_VERIFY_FAILED)
    {
        shop->s_ops->set_prev_info(shop, "Invalid cookie! Please try to relogin!", PREV_ERROR);
        return ;
    }

    old_item = shop->item_list;
    while (old_item)
    {
        if (old_item->id == item_id)
        {
            free(old_item->name);
            old_item->name = (char*) malloc(sizeof(char) * (strlen(item_name) + 1));
            strcpy(old_item->name, item_name);
            old_item->nums = item_nums;
            old_item->price = item_price;

            shop->s_ops->set_prev_info(shop, "Done.", PREV_SUCCESS);
            return ;
        }
        old_item = old_item->next;
    }

    shop->s_ops->set_prev_info(shop, "Item not found!", PREV_ERROR);
}

void item_del(struct online_shop_manager *shop, int item_id, char *cookie)
{
    struct item     *old_item;
    struct item     *prev_item;

    if (shop->s_ops->verify_cookie(shop, cookie) == COOKIE_VERIFY_FAILED)
    {
        shop->s_ops->set_prev_info(shop, "Invalid cookie! Please try to relogin!", PREV_ERROR);
        return ;
    }

    old_item = shop->item_list;
    prev_item = NULL;
    while (old_item)
    {
        if (old_item->id == item_id)
        {
            if (!prev_item)
                shop->item_list = old_item->next;
            else
                prev_item->next = old_item->next;
            free(old_item);
            shop->s_ops->set_prev_info(shop, "Done.", PREV_SUCCESS);
            return ;
        }
        prev_item = old_item;
        old_item = old_item->next;
    }

    shop->s_ops->set_prev_info(shop, "Item not found!", PREV_ERROR);
}

void save_data(struct online_shop_manager *shop, char *cookie)
{
    FILE                *f_user_data;
    FILE                *f_item_data;
    struct user_info    *per_user;
    struct item         *per_item;

    if (shop->s_ops->verify_cookie(shop, cookie) == COOKIE_VERIFY_FAILED)
    {
        shop->s_ops->set_prev_info(shop, "Invalid cookie! Please try to relogin!", PREV_ERROR);
        return ;
    }

    f_user_data = fopen(shop->user_data_path, "w");
    f_item_data = fopen(shop->item_data_path, "w");
    if ((!f_user_data) || (!f_item_data))
    {
        shop->s_ops->set_prev_info(shop, "Unable to open data file! Something\'s wrong!", PREV_ERROR);
        return ;
    }

    per_user = shop->user_manager->user_list;
    while (per_user)
    {
        fprintf(f_user_data, "%s %s\n", per_user->username, per_user->password);
        per_user = per_user->next;
    }

    per_item = shop->item_list;
    while (per_item)
    {
        fprintf(f_item_data, "%d %s %d %lf\n", per_item->id, per_item->name, per_item->nums, per_item->price);
        per_item = per_item->next;
    }

    fclose(f_user_data);
    fclose(f_item_data);

    shop->s_ops->set_prev_info(shop, "Done.", PREV_SUCCESS);
}

int verify_cookie(struct online_shop_manager *shop, char *cookie)
{
    struct user_info    *per_user;

    per_user = shop->user_manager->user_list;
    while (per_user)
    {
        if (per_user->cookie && !strcmp(cookie, per_user->cookie))
            return COOKIE_VERIFY_SUCCESS;
        
        per_user = per_user->next;
    }

    return COOKIE_VERIFY_FAILED;
}