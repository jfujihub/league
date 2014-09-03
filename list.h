/*
 * 可変長のスタックおよびキュー
 */
#include <stdlib.h>


#ifndef FALSE
#define FALSE (0)
#endif

#ifndef TRUE
#define TRUE (!FALSE)
#endif


/*! 循環型双方向リスト */
struct list {
	void *data;
	size_t size;
	struct list *prev, *next;
};


/* 関数プロトタイプ宣言 */
struct list *alloc_cell(void);
void delete_cell(struct list *);
struct list *init_list(void);
void push_stack(struct list *, void *, size_t);
void push_queue(struct list *, void *, size_t);
void pop_stack(struct list *, void *, size_t);
void pop_queue(struct list *, void *, size_t);
void swap_cell(struct list *, struct list *);
size_t size_next_stack(struct list *);
size_t size_next_queue(struct list *);
size_t size_list(struct list *);
void lock_list(struct list *head);
void unlock_list(struct list *head);
int has_locked(struct list *head);
struct list *search_list(struct list *, void *, size_t);
void delete_list(struct list *);

char **tokenize_str(char *, char *, int *);
void free_token_ary(char **, int);
void bubble_sort_list(struct list *, int(*cmp)(const void *, const void *));

