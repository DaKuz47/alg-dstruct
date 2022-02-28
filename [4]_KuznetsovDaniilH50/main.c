#include <stdlib.h>
#include <stdio.h>
#include "labH.h"


int main(void) {
	bottomIni();
	node *root = bottom;
	char command;
	int k;
	while(scanf("%c %d%*c", &command, &k) >= 2){
		if(command == 'a'){
			root = add(root, k);
		}else if(command == 'r'){
			root = delete(root, k);
		}else if(command == 'f'){
			node *tmp = search(root, k);
			if(tmp == bottom){
				printf("no\n");
			}else{
				printf("yes\n");
			}
		}
	}
	return 0;
}


