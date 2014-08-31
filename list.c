#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"


struct list *alloc_cell(void)
/* リストの要素を確保 */
{
	struct list *cell;

	cell = (struct list *) malloc(sizeof(struct list));
	if (cell == NULL) {
		fprintf(stderr, "ERROR: Unable to allocate memory.\n");
		exit(EXIT_FAILURE);
	}
	cell->prev = cell->next = cell;
	cell->data = NULL;
	cell->size = 0;

	return cell;
}

void delete_cell(struct list *cell)
/* リストの要素を削除 */
{
	cell->prev->next = cell->next;
	cell->next->prev = cell->prev;
	free(cell->data);
	free(cell);
}

struct list *init_list(void)
/* リストを初期化 */
{
	struct list *head;

	head = alloc_cell();

	return head;
}

void push_stack(struct list *head, void *data, size_t size_data)
/* スタックに要素を挿入 */
{
	struct list *new;

	new = alloc_cell();
	new->data = malloc(size_data);
	if (new->data == NULL) {
		fprintf(stderr, "ERROR: Unable to allocate memory.\n");
		exit(EXIT_FAILURE);
	}
	memcpy(new->data, data, size_data);
	new->size = size_data;
	/* head の直後に new を挿入 */
	new->prev = head;
	new->next = head->next;
	head->next->prev = new;
	head->next = new;

	(head->size)++;
}

void push_queue(struct list *head, void *data, size_t size_data)
/* キューに要素を挿入 */
{
	push_stack(head, data, size_data); /* スタックへの挿入と同じ操作 */
}

void pop_stack(struct list *head, void *data, size_t size_data)
/* スタックから要素を取り出し */
{
	struct list *target;

	if (head->next == head || head->prev == head) {
		fprintf(stderr, "ERROR: No data in the stack.\n");
		return;
	}

	/* head の直後の要素を取り出す */
	target = head->next;
	memcpy(data, target->data, size_data);
	delete_cell(target);

	(head->size)--;
}

void pop_queue(struct list *head, void *data, size_t size_data)
/* キューから要素を取り出し */
{
	struct list *target;

	if (head->next == head || head->prev == head) {
		fprintf(stderr, "ERROR: No data in the queue.\n");
		return;
	}
	/* head の直前の要素を取り出す */
	target = head->prev;
	memcpy(data, target->data, size_data);
	delete_cell(target);

	(head->size)--;
}

size_t size_next_stack(struct list *head)
/* スタックの最上部にあるデータのサイズを返す */
{
	return head->next->size;
}

size_t size_next_queue(struct list *head)
/* キューの出口にあるデータのサイズを返す */
{
	return head->prev->size;
}

size_t size_list(struct list *head)
/* リストの要素数を返す */
{
	return head->size;
}

struct list *search_list(struct list *head, void *data, size_t size_data)
/* リストから検索 */
{
	struct list *p;

	for (p = head->next; p != head; p = p->next)
		if (strncmp(p->data, data, size_data) == 0)
			return p;

	return NULL;
}

void delete_list(struct list *head)
/* リストの全要素を削除 */
{
	struct list *target;

	while (head->prev != head) {
		target = head->prev;
		delete_cell(target);
	}
	delete_cell(head);
}


char **tokenize_str(char *str, char *delim, int *num_token)
/* 文字列を指定のデリミタで区切り、トークンに分割する */
{
	char *p;
	char **token_ary;
	int i;
	size_t len_token;
	struct list *head;

	head = init_list();
	*num_token = 0;
	do {
		if (*num_token == 0)
			p = strtok(str, delim);
		else
			p = strtok(NULL, delim);
		if (p != NULL) {
			push_queue(head, p, strlen(p) + 1); /* +1 は \0 のため */
			(*num_token)++;
		}
	} while (p != NULL);

	if (*num_token == 0) {
		delete_list(head);
		return NULL;
	}

	token_ary = (char **) malloc(sizeof(char *) * (*num_token));
	if (token_ary == NULL) {
		fprintf(stderr, "ERROR: Unable to allocate memory.\n");
		exit(EXIT_FAILURE);
	}
	for (i = 0; i < *num_token; i++) {
		len_token = size_next_queue(head);
		token_ary[i] = (char *) malloc(len_token);
		if (token_ary[i] == NULL) {
			fprintf(stderr, "ERROR: Unable to allocate memory.\n");
			exit(EXIT_FAILURE);
		}
		pop_queue(head, token_ary[i], len_token);
	}
	delete_list(head);

	return token_ary;
}

void free_token_ary(char **token_ary, int num_token)
/* トークン格納用配列を解放 */
{
	int i;

	if (token_ary == NULL)
		return;

	for (i = 0; i < num_token; i++)
		free(token_ary[i]);
	free(token_ary);
}
