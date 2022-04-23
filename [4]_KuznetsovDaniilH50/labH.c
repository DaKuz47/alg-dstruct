#include <stdio.h>
#include <stdlib.h>
#include "labH.h"

void bottomIni(void){
	bottom = malloc(sizeof(node));
	if(!bottom){
		printf("Memory allocation error");
		return;
	}
	bottom->level = 0;
	bottom->right = bottom;
	bottom->left = bottom;
	deleted = bottom;
	return;
}

node *create(node *root, int k){
	node *tmp = malloc(sizeof(node));
	if(!tmp){
		printf("Memory allocation error");
		return NULL;
	}
	tmp->key = k;
	tmp->level = 1;
	tmp->left = bottom;
	tmp->right = bottom;
	root = tmp;
	return root;
}

node *Skew(node *root){
	if(root->left->level == root->level){
		node *tmp = root;
		root = root->left;
		tmp->left = root->right;
		root->right = tmp;
	}
	return root;
}

node *Split(node *root){
	if(root->right->right->level == root->level){
		node *tmp = root;
		root = root->right;
		tmp->right = root->left;
		root->left = tmp;
		root->level++;
	}
	return root;
}

node *search(node *root, int k){
	if(root == bottom){
		return bottom;
	}
	node *tmp = root;
	if(tmp->key < k){
		tmp = search(tmp->right, k);
	}else if(tmp->key > k){
		tmp = search(tmp->left, k);
	}
	return tmp;
}

node *add(node *root, int k){
	if(search(root, k) == bottom){
		if(root == bottom){
			node *tmp = malloc(sizeof(node));
			if(!tmp){
				printf("Memory allocation error");
				return NULL;
			}
			tmp->key = k;
			tmp->level = 1;
			tmp->left = bottom;
			tmp->right = bottom;
			root = tmp;
		}else{
			if(k < root->key){
				root->left = add(root->left, k);
			}else{
				root->right = add(root->right, k);
			}
		}
		root = Skew(root);
		root = Split(root);
	}
	return root;
}

node *delete(node *root, int k){
    if(root != bottom){
    	last = root;
    	if(k < root->key){
    		root->left = delete(root->left, k);
    	}else{
    		deleted = root;
    		root->right = delete(root->right, k);
    	}

    	if((root == last) && (deleted != bottom) && (k == deleted->key)){
    		deleted->key = root->key;
    		deleted = bottom;
    		root = root->right;
    		free(last);
    	}else if(root->left->level < root->level - 1 || root->right->level < root->level - 1){
    		root->level--;
    		if(root->right->level > root->level){
    			root->right->level = root->level;
    		}
    		root = Skew(root);
    		root->right = Skew(root->right);
    		root->right->right = Skew(root->right->right);
    		root = Split(root);
    		root->right = Split(root->right);
    	}
    }


    return root;
}

void printtree(node *root){
	if(root == bottom){
		return;
	}
	printf("%d ", root->key);
	printtree(root->left);
	printtree(root->right);
	return;
}
