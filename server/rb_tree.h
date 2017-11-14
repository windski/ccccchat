#ifndef RB_TREE_H_
#define RB_TREE_H_

#include "translatlib.h"

typedef enum { RED, BLACK } color_t;

typedef struct node {
    int data;
    struct node *parent;
    struct node *left;
    struct node *right;
    color_t color;
}rb_node_t;


typedef struct {
    int length;
    rb_node_t *root;
}tree_head_t;

#define rb_parent(r) ((r)->parent)
#define rb_isred(r) ((r)->color == RED)
#define rb_isblack(r) ((r)->color == BLACK)
#define rb_setred(r) ((r)->color = RED)
#define rb_setblack(r) ((r)->color = BLACK)
#define rb_setallnull(r) do { (r)->left = NULL; (r)->right = NULL; } while(0)

extern void left_rotate(tree_head_t *head, rb_node_t *x);
extern void right_rotate(tree_head_t *head, rb_node_t *y);
extern void insert(tree_head_t *head, rb_node_t *target);
extern void rb_fixer(tree_head_t *head, rb_node_t *target);

#endif
