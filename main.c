/*
 * code by arttnba3
 * 2021.12
 * Data Structure - Big Homework
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <pthread.h>
#include <string.h>

#include "info/info.h"
#include "user/user.h"
#include "shop/shop.h"
#include "scanner/scanner.h"

static struct online_shop_manager *shop;
static FILE *f_config;
static char *user_data_path;
static char *item_data_path;

void read_config(void);

int main(int argc, char **argv, char **envp)
{
    char    input_buf[MAX_TOKEN_LEN];
    char    username[MAX_TOKEN_LEN];
    char    *password;
    int     action;
    int     item_id;
    char    item_name[MAX_TOKEN_LEN];
    int     item_nums;
    double  item_price;
    char    *cookie;

    setvbuf(stdin, NULL, _IONBF, 0);
    setvbuf(stdout, NULL, _IONBF, 0);
    setvbuf(stderr, NULL, _IONBF, 0);
    cookie = NULL;

    successMsg("Welcome to arttnba3\'s Online Shop!");
    infoMsg("Initializing the system now...");
    
    // read config from specific file
    if (argc < 2)
        f_config = fopen("config.ini", "r");
    else
        f_config = fopen(argv[1], "r");

    if (!f_config)
        errExit("Unable to open the config file!");
    
    infoMsg("Reading config...");
    read_config();
    fclose(f_config);
    successMsg("Reading config done!");

    // initialize the shop
    infoMsg("Loading data from database...");
    shop = get_new_shop();
    shop->s_ops->shop_init(shop, user_data_path, item_data_path);
    successMsg("Loading database done!");

    // start the system
    infoMsg("Entering system in 3 seconds...");
    sleep(3);
    while (1)
    {
        if (cookie)   // for those user that logged in
        {
            shop->s_ops->shop_menu(shop);
            printf("Your choice: ");
            fgets(input_buf, MAX_TOKEN_LEN, stdin);
            input_buf[strlen(input_buf) - 1] = '\0'; // clear \n
            switch(atoi(input_buf))
            {
                case ADD_ITEM_ACTION:
                        printf("Please input item id: ");
                        fgets(input_buf, MAX_TOKEN_LEN, stdin);
                        input_buf[strlen(input_buf) - 1] = '\0'; // clear \n
                        item_id = atoi(input_buf);
                        printf("Please input item name: ");
                        fgets(item_name, MAX_TOKEN_LEN, stdin);
                        item_name[strlen(item_name) - 1] = '\0'; // clear \n
                        printf("Please input item nums: ");
                        fgets(input_buf, MAX_TOKEN_LEN, stdin);
                        input_buf[strlen(input_buf) - 1] = '\0'; // clear \n
                        item_nums = atoi(input_buf);
                        printf("Please input item price: ");
                        fgets(input_buf, MAX_TOKEN_LEN, stdin);
                        input_buf[strlen(input_buf) - 1] = '\0'; // clear \n
                        item_price = atof(input_buf);
                        
                        shop->s_ops->item_add(shop, item_id, item_name, item_nums, item_price, cookie);
                        break;
                case SHOW_ITEM_ACTION:
                        printf("Please input item id: ");
                        fgets(input_buf, MAX_TOKEN_LEN, stdin);
                        input_buf[strlen(input_buf) - 1] = '\0'; // clear \n
                        item_id = atoi(input_buf);
                        shop->s_ops->item_show(shop, item_id, cookie);
                        break;
                case CHANGE_ITEM_ACTION:
                        printf("Please input item id: ");
                        fgets(input_buf, MAX_TOKEN_LEN, stdin);
                        input_buf[strlen(input_buf) - 1] = '\0'; // clear \n
                        item_id = atoi(input_buf);
                        printf("Please input item name: ");
                        fgets(item_name, MAX_TOKEN_LEN, stdin);
                        item_name[strlen(item_name) - 1] = '\0'; // clear \n
                        printf("Please input item nums: ");
                        fgets(input_buf, MAX_TOKEN_LEN, stdin);
                        input_buf[strlen(input_buf) - 1] = '\0'; // clear \n
                        item_nums = atoi(input_buf);
                        printf("Please input item price: ");
                        fgets(input_buf, MAX_TOKEN_LEN, stdin);
                        input_buf[strlen(input_buf) - 1] = '\0'; // clear \n
                        item_price = atof(input_buf);
                        
                        shop->s_ops->item_change(shop, item_id, item_name, item_nums, item_price, cookie);
                        break;
                case DELETE_ITEM_ACTION:
                        printf("Please input item id: ");
                        fgets(input_buf, MAX_TOKEN_LEN, stdin);
                        input_buf[strlen(input_buf) - 1] = '\0'; // clear \n
                        item_id = atoi(input_buf);
                        shop->s_ops->item_del(shop, item_id, cookie);
                        break;
                case SAVE_DATA_ACTION:
                        shop->s_ops->save_data(shop, cookie);
                        break;
                case LOGOUT_ACTION:
                        shop->s_ops->user_logout(shop, cookie);
                        cookie = NULL;
                        break;
                default:
                        shop->s_ops->set_prev_info(shop, "Invalid choice!", PREV_ERROR);
                        break;
            }
        }
        else    // for those user that hadn't logged in
        {
            shop->s_ops->login_menu(shop);
            printf("Your choice: ");
            fgets(input_buf, MAX_TOKEN_LEN, stdin);
            input_buf[strlen(input_buf) - 1] = '\0'; // clear \n
            action = atoi(input_buf);
            switch(action)
            {
                case LOGIN_ACTION:
                case REGISTER_ACTION:
                        printf("Please input your username: ");
                        fgets(username, MAX_TOKEN_LEN, stdin);
                        username[strlen(username) - 1] = '\0'; // clear \n
                        if (strlen(username) == 0)
                        {
                            shop->s_ops->set_prev_info(shop, "NULL username!", PREV_ERROR);
                            break;
                        }
                        password = getpass("Please input your password: ");
                        if (strlen(password) == 0)
                        {
                            shop->s_ops->set_prev_info(shop, "NULL password!", PREV_ERROR);
                            break;
                        }
                        if (action == LOGIN_ACTION)
                        {
                            infoMsg("Loginning now...");
                            sleep(1);
                            cookie = shop->s_ops->user_login(shop, username, password);
                            if (!cookie)
                                shop->s_ops->set_prev_info(shop, "Invalid username or password!", PREV_ERROR);
                        }
                        else
                        {
                            infoMsg("Registering now...");
                            sleep(1);
                            if (!(shop->s_ops->user_register(shop, username, password)))
                                shop->s_ops->set_prev_info(shop, "Username already existed!", PREV_ERROR);
                            else
                                shop->s_ops->set_prev_info(shop, "Done.", PREV_SUCCESS);
                        }
                        break;
                case EXIT_ACTION:
                        successMsg("See you next time!");
                        return 0;
                default:
                        shop->s_ops->set_prev_info(shop, "Invalid choice!", PREV_ERROR);
            }
        }
    }
}

void read_config(void)
{
    char    tmp_token[MAX_TOKEN_LEN];

    get_token(f_config, tmp_token);
    if (!strcmp(tmp_token, "USER_DATA_PATH"))
    {
        fetch_token(f_config, "=");
        user_data_path = (char*) malloc(MAX_TOKEN_LEN);
        get_token(f_config, user_data_path);

        fetch_token(f_config, "ITEM_DATA_PATH");
        fetch_token(f_config, "=");
        item_data_path = (char*) malloc(MAX_TOKEN_LEN);
        get_token(f_config, item_data_path);
    }
    else if (!strcmp(tmp_token, "ITEM_DATA_PATH"))
    {
        fetch_token(f_config, "=");
        item_data_path = (char*) malloc(MAX_TOKEN_LEN);
        get_token(f_config, item_data_path);

        fetch_token(f_config, "USER_DATA_PATH");
        fetch_token(f_config, "=");
        user_data_path = (char*) malloc(MAX_TOKEN_LEN);
        get_token(f_config, user_data_path);
    }
    else
    {
        errExit("Invalid config file!");
    }

    infoMsgMore("User data path: ", user_data_path);
    infoMsgMore("item data path: ", item_data_path);
}