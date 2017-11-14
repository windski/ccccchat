#include "rb_tree.h"

/* left-rotate: x is target
 *      px                              px
 *     /                               /
 *    x                               y
 *   /  \    -(left rotate)->        / \
 *  lx   y                          x  ry
 *     /   \                       /  \
 *    ly   ry                     lx  ly
 *
 */

void left_rotate(tree_head_t *head, rb_node_t *x)
{
    rb_node_t *y = x->right;
    x->right = y->left;

    if(y->left != NULL) {
        y->left->parent = x;
    }

    y->parent = x->parent;

    if(x->parent == NULL) {
        head->root = y;
    } else if(x == x->parent->left) {
        x->parent->left = y;
    } else {
        x->parent->right = y;
    }
    y->left = x;
    x->parent = y;
}

/*
 *           py                               py
 *           /                                /
 *          y                                x
 *         /  \     -(right rotate)->       /  \
 *        x   ry                           lx   y
 *       / \                                   / \
 *      lx  rx                                rx  ry
 */

void right_rotate(tree_head_t *head, rb_node_t *y)
{
    rb_node_t *x = y->left;
    y->left = x->right;

    if(x->right != NULL) {
        x->right->parent = y;
    }

    x->parent = y->parent;

    if(y->parent == NULL) {
        head->root = x;
    } else if(y == y->parent->right) {
        y->parent->right = x;
    } else {
        y->parent->left = x;
    }
    x->right = y;
    y->parent = x;
}


void insert(tree_head_t *head, rb_node_t *target)
{
    rb_node_t *y = NULL;
    rb_node_t *x = head->root;

    while(x != NULL) {
        y = x;
        if(target->data < x->data) {         // Revise according to specific conditions
            x = x->left;
        } else {
            x = x->right;
        }
    }
    target->parent = y;

    if(y == NULL) {
        head->root = target;
    } else if(target->data < y->data) {
        y->left = target;
    } else {
        y->right = target;
    }

    rb_setallnull(target);
    rb_setred(target);

    rb_fixer(head, target);
}

void rb_fixer(tree_head_t *head, rb_node_t *target)
{
    rb_node_t *pa, *gpa;

    while((pa = rb_parent(target)) && rb_isred(pa)) {
        gpa = rb_parent(pa);
        if(pa == gpa->left) {
            // case 1
            {
                rb_node_t *uncle = gpa->right;        // 23333
                if(uncle && rb_isred(uncle)) {
                    rb_setblack(uncle);
                    rb_setblack(pa);
                    rb_setred(gpa);
                    target = gpa;
                    continue;
                }
            }

            // case 2
            if(target == pa->right) {
                rb_node_t *tmp;
                left_rotate(head, pa);
                tmp = pa;
                pa = target;
                target = tmp;
            }

            // case 3
            rb_setblack(pa);
            rb_setred(gpa);
            right_rotate(head, gpa);
        } else {
            // case 1
            {
                rb_node_t *uncle = gpa->left;
                if(uncle && rb_isred(uncle)) {
                    rb_setblack(uncle);
                    rb_setblack(pa);
                    rb_setred(gpa);
                    target = pa;
                    continue;
                }
            }

            // case 2
            if(target == pa->left) {
                rb_node_t *tmp;
                right_rotate(head, pa);
                tmp = pa;
                pa = target;
                target = tmp;
            }

            // case 3
            rb_setblack(pa);
            rb_setred(gpa);
            left_rotate(head, gpa);
        }
    }

    rb_setblack(head->root);
}

