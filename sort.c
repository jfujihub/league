#include <stdlib.h>
#include <string.h>
#include "sort.h"


void bubblesort(void *ary, size_t num_component, size_t size_component, int (*cmp)(const void *, const void *))
/**
	\blief バブルソート
	安定ソートの一つ
	\param[in,out] ary 整列する対象のデータを格納した配列
	\param[in] num_component ary の要素数
	\param[in] size_component ary の要素一つのサイズ (バイト)
	\param[in] cmp 二つの要素の大小を判定するコールバック関数
*/
{
	char *tmp = malloc(size_component);
	int i, j;

	for (i = 0; i < num_component - 1; i++) {
		for (j = 0; j < num_component - i - 1; j++) {
			if (cmp(ary + j * size_component, ary + (j + 1) * size_component) > 0) {
				memcpy(tmp, ary + j * size_component, size_component);
				memcpy(ary + j * size_component,
					ary + (j + 1) * size_component, size_component);
				memcpy(ary + (j + 1) * size_component, tmp, size_component);
			}
		}
	}

	free(tmp);
}

