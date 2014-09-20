#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"


struct list *alloc_cell(void)
/*!
	リストの要素を確保
*/
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
/*!
	リストの要素を削除
	@param cell 削除対象のリスト要素
*/
{
	cell->prev->next = cell->next;
	cell->next->prev = cell->prev;
	free(cell->data);
	free(cell);
}

struct list *init_list(void)
/*!
	リストを初期化
*/
{
	return alloc_cell();
}

void push_stack(struct list *head, void *data, size_t size_data)
/*!
	スタックに要素を挿入
	@param head リストの先頭
	@param data 挿入するデータ
	@param size_data 挿入するデータのサイズ
*/
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
/*!
	キューに要素を挿入
	@param head リストの先頭
	@param data 挿入するデータ
	@param size_data 挿入するデータのサイズ
*/
{
	push_stack(head, data, size_data); /* スタックへの挿入と同じ操作 */
}

void pop_stack(struct list *head, void *data, size_t size_data)
/*!
	スタックから要素を取り出し
	@param head リストの先頭
	@param data 取り出すデータを格納するバッファ
	@param size_data 取り出すデータのサイズ
*/
{
	struct list *target;

	if (head->next == head) {
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
/*!
	キューから要素を取り出し
	@param head リストの先頭
	@param data 取り出すデータを格納するバッファ
	@param size_data 取り出すデータのサイズ
*/
{
	struct list *target;

	if (head->next == head) {
		fprintf(stderr, "ERROR: No data in the queue.\n");
		return;
	}

	/* head の直前の要素を取り出す */
	target = head->prev;
	memcpy(data, target->data, size_data);
	delete_cell(target);

	(head->size)--;
}

void swap_cell(struct list *cell1, struct list *cell2)
/*!
	要素を交換
	@param cell1 交換する要素1
	@param cell2 交換する要素2
*/
{
	void *tmp_data;
	size_t tmp_size;

	if (cell1 == cell2)
		return;

	tmp_data = cell1->data;
	tmp_size = cell1->size;

	cell1->data = cell2->data;
	cell1->size = cell2->size;

	cell2->data = tmp_data;
	cell2->size = tmp_size;
}

size_t size_next_stack(struct list *head)
/*!
	スタックの最上部にあるデータのサイズを返す
	@param head リストの先頭
*/
{
	return head->next->size;
}

size_t size_next_queue(struct list *head)
/*!
	キューの出口にあるデータのサイズを返す
	@param head リストの先頭
*/
{
	return head->prev->size;
}

size_t size_list(struct list *head)
/*!
	リストの要素数を返す
	@param head リストの先頭
*/
{
	return head->size;
}


struct list *search_list(struct list *head, void *data, size_t size_data)
/*!
	リストから検索
	@param head リストの先頭
	@param data 検索したいデータ
	@param size_data データのサイズ
*/
{
	struct list *p;

	for (p = head->next; p != head; p = p->next)
		if (strncmp(p->data, data, size_data) == 0)
			return p;

	return NULL;
}

void delete_list(struct list *head)
/*!
	リスト全体を削除
	@param head リストの先頭
*/
{
	while (head->prev != head) {
		delete_cell(head->prev);
	}
	delete_cell(head);
}


char **tokenize_str(char *str, char *delim, int *num_token)
/*!
	文字列を指定のデリミタで区切り、トークンに分割する
	@param str 対象文字列
	@param delim デリミタ
	@param num_token 切り出されたトークンの数
*/
{
	char *b, *e;
	char *buf;
	char **token_ary;
	int i;
	size_t len_token;
	struct list *head;

	head = init_list();
	*num_token = 0;
	for (b = e = str + strspn(str, delim); *e != '\0'; ) {
		/* トークンの先頭・終端をセット */
		if (*num_token != 0)
			b += strcspn(b, delim);
		b += strspn(b, delim);
		e = b + strcspn(b, delim);
		(*num_token)++;
		/* トークンを処理 */
		len_token = e - b;
		buf = (char *) malloc(sizeof(char) * (len_token + 1));
		strncpy(buf, b, len_token);
		buf[len_token] = '\0';
		push_queue(head, buf, len_token + 1);
		free(buf);
		/* 文字列終端までのトークンの有無を検知 */
		e += strspn(e, delim);
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
/*!
	トークン格納用配列を解放
	@param token_ary トークン格納用配列
	@param num_token token_ary の要素数
*/
{
	int i;

	if (token_ary == NULL)
		return;

	for (i = 0; i < num_token; i++)
		free(token_ary[i]);
	free(token_ary);
}

void bubble_sort_list(struct list *head, int(*cmp)(const void *d0, const void *d1))
/*!
	リスト内の要素を比較関数に従ってソートする (バブルソート)
	@param head リストの先頭
	@param cmp 比較関数へのポインタ
*/
{
	struct list *p0, *p1;

	for (p0 = head->next; p0 != head; p0 = p0->next) {
		for (p1 = head->prev; p1 != p0; p1 = p1->prev) {
			if (cmp((p1->prev)->data, p1->data) > 0) {
				swap_cell(p1->prev, p1);
			}
		}
	}
}

