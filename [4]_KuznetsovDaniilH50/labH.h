#pragma once

typedef struct tree{
	int key;
	int level;
	struct tree* left;
	struct tree* right;
}node;


extern node *bottom, *last, *deleted;

void bottomIni(void);

node *create(node *root, int k);
node *Skew(node *root);
node *Split(node *root);
node *add(node *root, int k);
node *delete(node *root, int k);
node *search(node *root, int k);
void printtree(node *root);
