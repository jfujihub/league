struct list {
	void *data;
	size_t size;
	struct list *prev, *next;
}; /* 循環型双方向リスト */


/* 関数プロトタイプ宣言 */
struct list *alloc_cell(void);
void delete_cell(struct list *);
struct list *init_list(void);
void push_stack(struct list *, void *, size_t);
void push_queue(struct list *, void *, size_t);
void pop_stack(struct list *, void *, size_t);
void pop_queue(struct list *, void *, size_t);
size_t size_next_stack(struct list *);
size_t size_next_queue(struct list *);
size_t size_list(struct list *);
struct list *search_list(struct list *, void *, size_t);
void delete_list(struct list *);

char **tokenize_str(char *, char *, int *);
void free_token_ary(char **, int);
