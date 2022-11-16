#pragma once

#include "tree.h"

#define PQ_SIZE 256


struct priority_queue
{
	tree_node* queue[PQ_SIZE + 1];
	
	int n;

	struct {
		vtype_tree_t key;
		vtype_tree_t value;
	}type;
};



void pq_init(priority_queue* q,vtype_tree_t key, vtype_tree_t value);
void pq_swap(priority_queue* q, int x, int y);
int pq_parent(int n);
int pq_young_child(int n);
void pq_insert(priority_queue* q, tree_node* node);
void bubble_up(priority_queue* q, int p);

tree_node* pq_extract_min(priority_queue* q);

void bubble_down(priority_queue* q, int p);
void output(priority_queue* queue);

