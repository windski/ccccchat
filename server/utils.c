#include "utils.h"
#include "rb_tree.h"
#include "translatlib.h"


char **apply_2d_arr(int m, int n)
{
    char **tmp = (char **)malloc(sizeof(char *) * m);

    for(int i = 0; i < m; i++) {
        tmp[i] = (char *)malloc(sizeof(char) * n);
    }
    return tmp;
}

void free_2d_arr(char **target, int m)
{
    for(int i = 0; i < m; i++) {
        free(target[i]);
    }
    free(target);
}

int split_str(char *source, const char ch,
        char **target, int array_len)
{
    char *tmp = strchr(source, ch);
    char *tmp_sr = source;

    int i = 0;
    while(tmp) {
        if(i >= array_len) {
            // array_len = i;
            return -1;
        }

        strncpy(target[i], tmp_sr, get_str_len(tmp_sr, tmp));
        tmp_sr = tmp + 1;
        tmp = strchr(tmp_sr, ch);
        i++;
    }
    return i;
}


pair_t *make_kv_pair(char **info, int fir)
{
    if(fir % 2) {
        fputs("make info to pair failure..\n", stderr);
        return NULL;
    }

    pair_t *head = (pair_t *)malloc(sizeof(pair_t));

    head->length = fir / 2;

    struct elem *data = (struct elem *)malloc(sizeof(struct elem) * (head->length));

    for(int i = 0; i < fir; i += 2) {
        strncpy(data->key, info[i], sizeof(data->key));
        strncpy(data->value, info[i + 1], sizeof(data->value));
    }
    
    return head;
}


void free_kv_pair(pair_t *target)
{
    free(target->data);
    free(target);
}


pair_t *parse_info(const char *msg)
{
    int fir_len = 6;

    char *body = get_request_body(msg);
    char **first_info = apply_2d_arr(fir_len, MAX_MSG_INFO);

    int f_tmp;
    if((f_tmp = split_str(body, ';', first_info, fir_len)) < 0) {
        printf("protocol has a error..\n");
        free_2d_arr(first_info, fir_len);
        return NULL;
    }

    int sec_len = fir_len * 2;
    char **second_info = apply_2d_arr(sec_len, MAX_MSG_INFO);

    int s_tmp;
    for(int i = 0; i < f_tmp; i++) {
        if((s_tmp = split_str(first_info[i], ':', second_info, sec_len)) < 0) {
            printf("protocol info has a error..\n");
            free_2d_arr(first_info, fir_len);
            free_2d_arr(second_info, sec_len);
            return NULL;
        }
    }
    free_2d_arr(first_info, fir_len);

    pair_t *kv_data = make_kv_pair(second_info, s_tmp);
    free_2d_arr(second_info, sec_len);

    return kv_data;
}


int search_user(tree_head_t *user_table, pair_t *data)
{
    char *name = get_kv_value(data, "u_name");
    return rb_search(user_table->root, name);
}


char *get_kv_value(pair_t *kv_data, char *target)
{
    for(int i = 0; i < kv_data->length; i++) {
        if(strcmp(kv_data->data->key, target) == 0) {
            return kv_data->data->value;
        }
    }
    return NULL;
}


void insert_user(tree_head_t *user_table, pair_t *data)
{
    rb_node_t *user = (rb_node_t *)malloc(sizeof(rb_node_t));
    init_tree_node(user);

    user_info_t *u_info = (user_info_t *)malloc(sizeof(user_info_t));
    memset(u_info, 0, sizeof(user_info_t));

    char *u_name = get_kv_value(data, "u_name");
    /* char *u_time = get_kv_value(data, "u_time"); */
    char *u_pawd = get_kv_value(data, "u_passwd");

    strncpy(u_info->user_name, u_name, sizeof(u_info->user_name));

    encryption(u_pawd, ELEMSIZE);
    strncpy(u_info->user_passwd, u_pawd, sizeof(u_info->user_passwd));

    /* tm_t tmp_tm;
     * if(strptime(u_time, "", &tmp_tm) == NULL) {
     *     sprintf(stderr, "The Time string of procotol has an error..\n");
     * } */

    user->data = u_info;
    rb_insert(user_table, user);
}


int str_lt(char *l, char *r)
{
    while(l && r) {
        if(*l == *r) {
            l++;
            r++;
        } else {
            return *l < *r;
        }
    }
    return -1;
}


int str_bt(char *l, char *r)
{
    return !str_lt(l, r);
}

// I'm too lazy to write encryption algorithm, just use the system functions..
void encryption(char *target, int len)
{
    char buff[MAXLEN];
    memset(buff, 0, sizeof(buff));

    sprintf(buff, "echo -n \"%s\" | sha1sum > demo.dat", target);

    system(buff);

    FILE *fp = fopen("demo.dat", "r");

    memset(buff, 0, sizeof(buff));
    char *test = fgets(buff, sizeof(buff), fp);

    fclose(fp);
    system("rm -r ./demo.dat");

    assert(test != NULL);

    strncpy(target, buff, len);
}

