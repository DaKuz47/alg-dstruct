#include <stdio.h>
#include <stdlib.h>
#include "labI.h"


int main(void) {
	Btree* T = tree_init(3);

	char command;
	int k;
	while(scanf("%c %d%*c", &command, &k) >= 2){
		if(command == 'a'){
			add(T, k);
		}else if(command == 'r'){
			delete(T->root, k, T->t);
		}else if(command == 'f'){
			node *tmp = keyFinder(T->root, k);
			if(tmp == NULL){
				printf("no\n");
			}else{
				printf("yes\n");
			}
		}
	}
	return 0;
}
