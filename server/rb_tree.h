#ifndef RB_TREE_H_
#define RB_TREE_H_

#include "translatlib.h"

typedef enum { RED, BLACK } rb_color_t;

typedef struct node {
    int data;
    struct node *parent;
    struct node *left;
    struct node *right;
    rb_color_t color;
}rb_node_t;


typedef struct {
    int length;
    rb_node_t *root;
}tree_head_t;

#define rb_isred(r) ((r)->color == RED)
#define rb_isblack(r) ((r)->color == BLACK)
#define rb_setred(r) ((r)->color = RED)
#define rb_setblack(r) ((r)->color = BLACK)
#define rb_setallnull(r) do { (r)->left = NULL; (r)->right = NULL; } while(0)

extern void init_tree_header(tree_head_t *head);
extern void rb_left_rotate(tree_head_t *head, rb_node_t *x);
extern void rb_right_rotate(tree_head_t *head, rb_node_t *y);
extern void rb_insert(tree_head_t *head, rb_node_t *target);
extern void rb_inst_fixer(tree_head_t *head, rb_node_t *target);
extern int rb_remove(tree_head_t *head, rb_node_t *target);
extern void rb_translant(tree_head_t *head, rb_node_t *ltarget, rb_node_t *rtarget);
extern void rb_dele_fixer(tree_head_t *head, rb_node_t *target);
extern rb_node_t *rb_minimum(rb_node_t *r);
extern rb_node_t *rb_maximum(rb_node_t *r);
extern void rb_destroy(tree_head_t *head);
extern void rb_destroy_process(rb_node_t *target);

#endif
