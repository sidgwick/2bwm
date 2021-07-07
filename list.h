#include <stdio.h>
struct item {
    void *data;
    struct item *prev;
    struct item *next;
};

/* Move element in item to the head of list mainlist. */
void movetohead(struct item **mainlist, struct item *item) {
    if (NULL == item || NULL == mainlist || NULL == *mainlist) return;

    /* item is NULL or we're already at head. Do nothing. */
    if (*mainlist == item) return;

    /* Braid together the list where we are now. */
    if (NULL != item->prev) item->prev->next = item->next;
    if (NULL != item->next) item->next->prev = item->prev;

    /* Now we'at head, so no one before us. */
    item->prev = NULL;
    /* Old head is our next. */
    item->next = *mainlist;
    /* Old head needs to know about us. */
    item->next->prev = item;
    /* Remember the new head. */
    *mainlist = item;
}

// Create space for a new item and add it to the head of mainlist.
// Returns item or NULL if out of memory.
struct item *additem(struct item **mainlist) {
    struct item *item;

    if (NULL == (item = (struct item *)malloc(sizeof(struct item)))) return NULL;

    /* First in the list. */
    if (NULL == *mainlist) {
        item->prev = item->next = NULL;
    } else {
        /* Add to beginning of list. */
        item->next = *mainlist;
        item->next->prev = item;
        item->prev = NULL;
    }

    *mainlist = item;
    return item;
}

/* 从链表中移除并释放节点 item */
void delitem(struct item **mainlist, struct item *item) {
    struct item *ml = *mainlist;

    if (NULL == mainlist || NULL == *mainlist || NULL == item) return;

    /* First entry was removed. Remember the next one instead. */
    if (item == *mainlist) {
        *mainlist = ml->next;
        if (item->next != NULL) {
            item->next->prev = NULL;
        }
    } else {
        item->prev->next = item->next;
        /* This is not the last item in the list. */
        if (NULL != item->next) {
            item->next->prev = item->prev;
        }
    }

    free(item);
}

/* 从链表里面删除元素 */
void freeitem(struct item **list, int *stored, struct item *item) {
    if (NULL == list || NULL == *list || NULL == item) return;

    /* 释放自定义数据 */
    if (NULL != item->data) {
        free(item->data);
        item->data = NULL;
    }

    /* 删除节点 */
    delitem(list, item);

    if (NULL != stored) {
        (*stored)--;
    }
}

/* Delete all elements in list and free memory resources. */
void delallitems(struct item **list, int *stored) {
    struct item *item;
    struct item *next;

    for (item = *list; item != NULL; item = next) {
        next = item->next;
        free(item->data);
        delitem(list, item);
    }

    if (NULL != stored) (*stored) = 0;
}

/* 遍历链表, 并打印调试信息 */
void listitems(struct item *mainlist) {
    struct item *item;
    int i;
    for (item = mainlist, i = 1; item != NULL; item = item->next, i++) {
        printf("item #%d (stored at %p).\n", i, (void *)item);
    }
}
