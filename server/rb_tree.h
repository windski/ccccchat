#ifndef RB_TREE_H_
#define RB_TREE_H_

#include "comm_struct.h"

#define rb_isred(r) ((r)->color == RED)
#define rb_isblack(r) ((r)->color == BLACK)
#define rb_setred(r) ((r)->color = RED)
#define rb_setblack(r) ((r)->color = BLACK)
#define rb_setallnull(r) do { (r)->left = NULL; (r)->right = NULL; } while(0)

extern void init_tree_header(tree_head_t *head);
extern void init_tree_node(rb_node_t *node);
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
extern int rb_search(rb_node_t *root, char *target);

#endif
