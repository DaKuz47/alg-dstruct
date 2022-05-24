#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <string.h>
#include <inttypes.h>
#include "lab3.h"

#define TRUE 1
#define FALSE 0

//Табличный алгоритм вычисления CRC32
uint32_t GetCRC32(void* data, int len){
	unsigned char* tmp = (unsigned char*) data;
	uint32_t CRC = 0xFFFFFFFF;
	CRC ^= *tmp;
	while(len--){
		CRC = (CRC << 8) ^ crctab[(CRC >> 24) ^ *tmp++];
	}
	return CRC;
}

//Инитсиализатсия начальными данными таблитсы
hash_table* tablInit(unsigned int size){
	hash_table* t = (hash_table*)malloc(sizeof(hash_table));
	cfc* tmp = (cfc*)malloc(sizeof(cfc)*size);
	if(t == NULL || tmp == NULL){
		printf("malloc error");
		return t;
	}
	t->size = size;
	t->items = tmp;
	for(unsigned int i = 0; i < size; i++){
		t->items[i].c = NULL;
		t->items[i].empty = TRUE;
	}
	return t;
}

//Поиск ячейки в тсепочке
cell* chainFind(cell* c, char* data){
	while(c != NULL){
		if(strcmp(c->value, data) == 0){
			return c;
		}
		c = c->next;
	}
	return c;
}

//Поиск ячейки в таблитсе
_Bool tablFind(hash_table* t, char* data){
	uint32_t CRC = GetCRC32(data, strlen(data));
	unsigned int k = CRC % t->size;
	if(!t->items[k].empty){
		if(chainFind(t->items[k].c, data) != NULL){
			return TRUE;
		}
	}
	return FALSE;
}

//Добавление ячейки в тсепочку
void chainAdd(cfc* ch, char* data){
	cell* c = malloc(sizeof(cell));
	if(c == NULL){
		printf("malloc error");
		return;
	}
	c->next = ch->c;
	ch->c = c;
	c->value = (char*)malloc(sizeof(char)*strlen(data));
	if(c->value == NULL){
		printf("malloc error");
		return;
	}
	strcpy(c->value, data);
	return;
}

//Добавление ячейки в таблитсу
void tablAdd(hash_table* t, char* data){
	if(!tablFind(t, data)){
		uint32_t CRC = GetCRC32(data, strlen(data));
		unsigned int k = CRC % t->size;
		chainAdd(&t->items[k], data);
		t->items[k].empty = FALSE;
	}
	return;
}

//Удаление ячейки из тсепочки
cell* chainRemoveItem(cell* c, char* data) {
    cell* cneeded = chainFind(c, data);

    if (strcmp(c->value, cneeded->value) == 0) {
        c = c->next;
    }
    else {
        cell* tmp = c;
        while (strcmp(tmp->next->value, cneeded->value) != 0){
            tmp = tmp->next;
        }
        tmp->next = cneeded->next;
    }
    free(cneeded->value);
    free(cneeded);
    return c;
}

//Удаление ячейки из таблитсы
void tablRemoveItem(hash_table* t, char* data){
	if(tablFind(t, data)){
		uint32_t CRC = GetCRC32(data, strlen(data));
		unsigned int k = CRC % t->size;
		t->items[k].c = chainRemoveItem(t->items[k].c, data);

		if(t->items[k].c == NULL){
			t->items[k].empty = TRUE;
		}
	}
	return;
}


int main(void) {//IгВы
	char command;
	hash_table* ht = tablInit(500000);
	char data[100] = "0";
	   while (scanf("%c", &command) >= 1) {
	    	getchar();
	        fgets(data, sizeof(data), stdin);
	        switch (command) {
	        case 'a':
	            tablAdd(ht, data);
	            break;
	        case 'r':
	            tablRemoveItem(ht, data);
	            break;

	        case 'f':
	            if (tablFind(ht, data))
	                printf("yes\n");
	            else
	                printf("no\n");

	            break;
	        }
	   }
	    return 0;
}
