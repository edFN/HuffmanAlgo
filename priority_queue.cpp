#include "priority_queue.h"
#include <string.h>

void pq_init(priority_queue* q,vtype_tree_t key, vtype_tree_t value) {
	
	q->n = 0;
	q->type.key = key;
	q->type.value = value;
}

void pq_swap(priority_queue* q, int x, int y) {

	tree_node* temp = q->queue[x];
	q->queue[x] = q->queue[y];
	q->queue[y] = temp;
}



int pq_parent(int n) {
	if (n == 1) {
		return -1;
	}

	return (int)n / 2;
}
int pq_young_child(int n) {
	return n * 2;
}

void bubble_up(priority_queue* q, int p) {

	if (pq_parent(p) == -1) return;

	if (q->type.value == DECIMAL_ELEM || q->type.value == BYTE_ELEM) {
		int value_parent = q->queue[pq_parent(p)]->data.value.decimal;
		int p_value = q->queue[p]->data.value.decimal;

		if (value_parent > p_value) {
			pq_swap(q, p, pq_parent(p));
			bubble_up(q, pq_parent(p));
		}
	}
	else {
		char* value_parent = q->queue[pq_parent(p)]->data.value.string;
		char* p_value = q->queue[p]->data.value.string;

		if (strcmp((const char*)value_parent, (const char*)p_value) > 0) {
			pq_swap(q, p, pq_parent(p));
			bubble_up(q, pq_parent(p));
		}
	}

}

void pq_insert(priority_queue* q, tree_node* node) {
	if (q->n >= PQ_SIZE)
		return;
	else {
		q->n++;
		q->queue[q->n] = node;

		bubble_up(q, q->n);
	}
}


void bubble_down(priority_queue* q, int p) {
	int c;
	int i;
	int min_index;

	c = pq_young_child(p);
	min_index = p;

	for (int i = 0; i <= 1; i++) {
		if ((c + i) <= q->n) {

			if (q->type.value == DECIMAL_ELEM) {

				if (q->queue[min_index]->data.value.decimal > q->queue[c + i]->data.value.decimal) {
					min_index = c + i;
				}

			}
			else if (q->type.value = STRING_ELEM) {
				
				if (strcmp(q->queue[min_index]->data.value.string, (const char*)q->queue[c + i]->data.value.decimal)) {
					min_index = c + i;
				}

			}
		}
	}

	if (min_index != p) {
		pq_swap(q, p, min_index);
		bubble_down(q, min_index);
	}

}

//check memory leak

tree_node* pq_extract_min(priority_queue* q) {

	tree_node* min = NULL;

	if (q->n <= 0) return NULL;

	else {
		min = q->queue[1];
		

		q->queue[1] = q->queue[q->n];


		q->n--;

		bubble_down(q, 1);
	}
	return (min);
}


void output(priority_queue* queue) {
	for (int i = 1; i <= queue->n; i++) {
		printf("[%c] %d\n", queue->queue[i]->data.key.decimal, queue->queue[i]->data.value.decimal);
	}
}

