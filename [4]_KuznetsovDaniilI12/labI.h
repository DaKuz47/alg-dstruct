#pragma once

#define tRIGHT 1
#define tLEFT 0

typedef struct list{ 		//Узел дерева
	_Bool leaf;
	int n;
	int* keys;
	struct list** childs;
	struct list* parent;
}node;

typedef struct tree{		//Само дерево
	int t;
	node* root;
}Btree;

Btree* tree_init(int x);
node* keyFinder(node* r, int k);
void Node_Split(node* s, int i, int t);
void add_node(node* s, int k, int t);
void add(Btree* Tr, int k);
void node_turn(node* r, int c, _Bool direction);
void merge(node* left, node* right, int c_ind, int t);
void treeCorrect(node* r, int t);
void delete(node* rt, int k, int t);
void tree_print(node* r);
