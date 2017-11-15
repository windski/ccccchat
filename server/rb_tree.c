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

void rb_left_rotate(tree_head_t *head, rb_node_t *x)
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

void rb_right_rotate(tree_head_t *head, rb_node_t *y)
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


void rb_insert(tree_head_t *head, rb_node_t *target)
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

    head->length++;
    rb_inst_fixer(head, target);
}

void rb_inst_fixer(tree_head_t *head, rb_node_t *target)
{
    while((target->parent != NULL) && rb_isred(target->parent)) {
        if(target->parent == target->parent->parent->left) {
            // case 1
            rb_node_t *uncle = target->parent->parent->right;        // 23333
            if(uncle && rb_isred(uncle)) {
                rb_setblack(uncle);
                rb_setblack(target->parent);
                rb_setred(target->parent->parent);
                target = target->parent->parent;
            } else {
                // case 2
                if(target == target->parent->right) {
                    target = target->parent;
                    rb_left_rotate(head, target);
                }

                // case 3
                rb_setblack(target->parent);
                rb_setred(target->parent->parent);
                rb_right_rotate(head, target->parent->parent);
            }
        } else {
            // case 1
            rb_node_t *uncle = target->parent->parent->left;
            if(uncle && rb_isred(uncle)) {
                rb_setblack(uncle);
                rb_setblack(target->parent);
                rb_setred(target->parent->parent);
                target = target->parent->parent;
            } else {
                // case 2
                if(target == target->parent->left) {
                    target = target->parent;
                    rb_right_rotate(head, target);
                }

                // case 3
                rb_setblack(target->parent);
                rb_setred(target->parent->parent);
                rb_left_rotate(head, target->parent->parent);
            }
        }
    }

    rb_setblack(head->root);
}


int rb_remove(tree_head_t *head, rb_node_t *target)
{
    if((head->length - 1) < 0) {
        return -1;
    }

    rb_node_t *tmp = target;
    rb_color_t tmp_color_ori = tmp->color;

    rb_node_t *child = NULL;

    if(target->left == NULL) {
        child = target->right;
        rb_translant(head, target, child);
    } else if(target->right == NULL) {
        child = target->left;
        rb_translant(head, target, child);
    } else {
        tmp = rb_minimum(target->right);
        tmp_color_ori = tmp->color;
        child = tmp->right;

        if(tmp->parent == target) {
            child->parent = tmp;
        } else {
            rb_translant(head, tmp, tmp->right);
            tmp->right = target->right;
            tmp->right->parent = tmp;
        }

        rb_translant(head, target, tmp);
        tmp->left = target->left;
        tmp->left->parent = tmp;
        tmp->color = target->color;
    }

    head->length--;
    if(tmp_color_ori == BLACK) {
        rb_dele_fixer(head, child);
    }
    free(target);

    return 0;
}


rb_node_t *rb_minimum(rb_node_t *r)
{
    while((r = r->left));
    return r;
}


rb_node_t *rb_maximum(rb_node_t *r)
{
    while((r = r->right));
    return r;
}


void rb_translant(tree_head_t *head, rb_node_t *ltarget, rb_node_t *rtarget)
{
    if(ltarget->parent == NULL) {
        head->root = rtarget;
    } else if(ltarget == ltarget->parent->left) {
        ltarget->parent->left = rtarget;
    } else {
        ltarget->parent->right = rtarget;
    }
    rtarget->parent = ltarget->parent;
}


void rb_dele_fixer(tree_head_t *head, rb_node_t *target)
{
    rb_node_t *tmp;

    while((target != head->root) && rb_isblack(target)) {

        if(target == target->parent->left) {
            tmp = target->parent->right;

            if(rb_isred(tmp)) {
                tmp->color = BLACK;
                target->parent->color = RED;
                rb_left_rotate(head, target->parent);
                tmp = target->parent->right;
            }

            if(rb_isblack(tmp->left) && rb_isblack(tmp->right)) {
                tmp->color = RED;
                target = target->parent;

            } else {
                if(rb_isblack(tmp->right)) {
                    tmp->left->color = BLACK;
                    tmp->color = RED;
                    rb_right_rotate(head, tmp);
                    tmp = target->parent->right;
                }

                tmp->color = target->parent->color;
                rb_setblack(target->parent);
                rb_setblack(tmp->right);
                rb_left_rotate(head, target->parent);
                target = head->root;
            }
        } else {
            tmp = target->parent->left;

            if(rb_isred(tmp)) {
                tmp->color = BLACK;
                target->parent->color = RED;
                rb_right_rotate(head, target->parent);
                tmp = target->parent->left;
            }

            if(rb_isblack(tmp->right) && rb_isblack(tmp->left)) {
                tmp->color = RED;
                target = target->parent;
            } else {
                if(rb_isblack(tmp->left)) {
                    tmp->right->color = BLACK;
                    tmp->color = RED;
                    rb_left_rotate(head, tmp);
                    tmp = target->parent->left;
                }

                tmp->color = target->parent->color;
                target->parent->color = BLACK;
                tmp->left->color = BLACK;
                rb_right_rotate(head, target->parent);
                target = head->root;
            }
        }
    }
    target->color = BLACK;
}


void rb_destroy(tree_head_t *head)
{
    rb_destroy_process(head->root);
    head->length = 0;
}


void rb_destroy_process(rb_node_t *root)
{
    if(root == NULL) {
        return ;
    }

    rb_destroy_process(root->left);
    rb_destroy_process(root->right);

    free(root);
    root = NULL;
}


void init_tree_header(tree_head_t *head)
{
    memset(head, 0, sizeof(*head));
}

