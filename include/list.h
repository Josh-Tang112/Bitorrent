/**
 * source : https://github.com/cwyang/bencode
 * new is replaced with new_one
*/

#ifndef SIMPLE_LIST_H
#define SIMPLE_LIST_H

typedef struct list_head {
    struct list_head *next, *prev;
} list_t;

#define LIST_HEAD_INIT(name) { &(name), &(name) }
#define LIST_HEAD(name) list_t name = LIST_HEAD_INIT(name)

static inline list_t* init_list_head(list_t *head) {
    head->next = head->prev = head;
    return head;
}

static inline void list_add(list_t *new_one, list_t *head) {
    new_one->prev = head;
    new_one->next = head->next;
    head->next->prev = new_one;
    head->next = new_one;
}

static inline void list_add_tail(list_t *new_one, list_t *head) {
    new_one->prev = head->prev;
    new_one->next = head;
    head->prev->next = new_one;
    head->prev = new_one;
}

static inline void list_del(list_t *entry) {
    entry->prev->next = entry->next;
    entry->next->prev = entry->prev;
}

static inline int list_empty(list_t *head) {
    return head->next == head;
}

#ifdef container_of
#define list_entry container_of
#else
#define list_entry(ptr, type, member)                               \
    ((type *)((char *)(ptr)-(unsigned long)(&((type *)0)->member)))
#endif

#define list_for_each(POS, HEAD)                                        \
    for ((POS) = (HEAD)->next; (POS) != (HEAD); (POS) = (POS)->next)
            
#define list_for_each_safe(POS, TEMP, HEAD)                    \
    for ((POS) = (HEAD)->next, (TEMP) = (POS)->next;           \
         (POS) != (HEAD);                                      \
         (POS) = (TEMP), (TEMP) = (POS)->next)

#endif // SIMPLE_LIST_H