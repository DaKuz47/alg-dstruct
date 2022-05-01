#include <stdio.h>
#include <stdlib.h>

#include "labI.h"

Btree* tree_init(int x){	//Инитсиализатсия дерева
	Btree* Tr = malloc(sizeof(Btree));
	node* r = malloc(sizeof(node));
	r->keys = malloc(sizeof(int)*(2*x-1));
	r->childs = malloc(sizeof(node*)*2*x);
	if(!Tr || !r || !r->keys || !r->childs){
		printf("Memory allocation error");
		return NULL;
	}
	Tr->t = x;
	r->n = 0;
	r->leaf = 1;
	r->parent = NULL;
	Tr->root = r;
	return Tr;
}

node* keyFinder(node* r, int k){		//Поиска ключа в узле и подузлах
	node* exist = NULL;
	int i = r->n;
	while((k <= r->keys[i-1]) && i > 0){
		if(k == r->keys[i-1]){
			return r;
		}
		i--;
	}
	if(!r->leaf){
		exist = keyFinder(r->childs[i], k);
	}
	return exist;
}

void Node_Split(node* s, int i, int t){			//Разделение заполненного узла на два
	node* a = malloc(sizeof(node));
	a->keys = malloc(sizeof(int)*(2*t-1));
	a->childs = malloc(sizeof(node*)*2*t);
	node* b = s->childs[i];
	if(!a || !a->keys || !a->childs){
		printf("memory allocation error");
		return;
	}
	a->parent = s;
	a->n = t-1;
	a->leaf = b->leaf;
	for(int j = 0; j <= t-2; j++){
		a->keys[j] = b->keys[j+t];
	}
	if(!b->leaf){
		for(int j=0; j <= t-1;j++){
			a->childs[j] = b->childs[j+t];
			a->childs[j]->parent = a;
		}
	}
	b->n = t-1;
	for(int j = s->n; j >= i+1; j--){
		s->childs[j+1] = s->childs[j];
	}
	s->childs[i+1] = a;
	for(int j = s->n-1; j >= i;j--){
		s->keys[j+1] = s->keys[j];
	}
	s->keys[i] = b->keys[t-1];
	s->n += 1;
	return;
}

void add_node(node* s, int k, int t){				//Добавление ключа в незаполненный узел
	int i = s->n;
	if(s->leaf){
		while((k < s->keys[i-1]) && (i > 0)){
			s->keys[i] = s->keys[i-1];
			i--;
		}
		s->keys[i] = k;
		s->n += 1;
	}else{
		while((k < s->keys[i-1]) && (i > 0)){
			i--;
		}
		if(s->childs[i]->n == 2*t-1){
			Node_Split(s, i, t);
			if(s->keys[i] < k){
				i++;
			}
		}
		add_node(s->childs[i], k, t);
	}
	return;
}

void add(Btree* Tr, int k){						//Добавление ключа в дерево, если такого ещё нет
	node* r = Tr->root;
	if(!keyFinder(r, k)){
		if (r->n == 2 * Tr->t - 1){
			node* s = malloc(sizeof(node));
			s->keys = malloc(sizeof(int)*(2*Tr->t-1));
			s->childs = malloc(sizeof(node*)*2*Tr->t);
			if(!s || !s->keys || !s->childs){
				printf("memory allocation error");
				return;
			}
			Tr->root = s;
			s->leaf = 0;
			s->n = 0;
			s->childs[0] = r;
			s->parent = NULL;
			r->parent = s;
			Node_Split(s, 0, Tr->t);
			add_node(s, k, Tr->t);
		}else{
			add_node(r,k,Tr->t);
		}
	}
	return;
}

void node_turn(node* r, int c, _Bool direction){
	node* p = r->parent;
	if(direction == tRIGHT){		//Поворот налево
		node* nbhd = p->childs[c+1];
		r->keys[r->n] = p->keys[c];
		p->keys[c] = nbhd->keys[0];
		for(int j = 0; j < nbhd->n-1;j++){
			nbhd->keys[j] = nbhd->keys[j+1];
		}
		if(!r->leaf){
			r->childs[r->n+1] = nbhd->childs[0];
			r->childs[r->n+1]->parent = r;
			for(int j = 0; j < nbhd->n;j++){
				nbhd->childs[j]=nbhd->childs[j+1];
			}
		}
		r->n += 1;
		nbhd->n -= 1;
	}else if(direction == tLEFT){	//Поворот направо
		node* nbhd = p->childs[c-1];
		for(int j = r->n; j >= 1; j--){
			r->keys[j] = r->keys[j-1];
		}
		r->keys[0] = p->keys[c-1];
		p->keys[c-1] = nbhd->keys[nbhd->n-1];
		if(!r->leaf){
			for(int j = r->n+1;j >= 1; j--){
				r->childs[j] = r->childs[j-1];
			}
			r->childs[0] = nbhd->childs[nbhd->n];
			r->childs[0]->parent = r;
		}
		r->n += 1;
		nbhd->n -= 1;
	}
	return;
}

void merge(node* left, node* right, int c_ind, int t){ //Слияние двух узлов

	node *p = left->parent;
	left->keys[left->n] = p->keys[c_ind];
	for(int j = 0; j < right->n; j++){
		left->keys[left->n+1+j] = right->keys[j];
	}
	if(!left->leaf){
		for(int j = 0; j <= right->n; j++){
			left->childs[left->n+1+j] = right->childs[j];
			right->childs[j]->parent = left;
		}
	}
	left->n = left->n + right->n + 1;
	for(int j = c_ind; j < p->n-1; j++){
		p->keys[j] = p->keys[j+1];
		p->childs[j+1] = p->childs[j+2];
	}
	p->n -= 1;
	return;
}

void treeCorrect(node* r, int t){ //Корректирует дерево, если в узле меньше t-1 ключей
	if(r->n == 0){			//Если абсолютно пустой, то корнем делаем единственного ребёнка
		r = r->childs[0];
	}else if(r->parent == NULL){ //Если это корень, то всё нормально
		return;
	}else{
		node* p = r->parent; //находим родителя и номер по счёту, какой это был ребёнок
		int c_ind = 0;
		while(p->childs[c_ind] != r){
			c_ind += 1;
		}
		_Bool left = 1;		//существование соседей
		_Bool right = 1;
		if(c_ind == 0){
			left = 0;
		}else if(c_ind == p->n){
			right = 0;
		}

		if((right) && (p->childs[c_ind+1]->n > t-1)){     //Выполняем либо поворот, либо слияние
			node_turn(r, c_ind, tRIGHT);
		}else if((left) && (p->childs[c_ind-1]->n > t-1)){
			node_turn(r, c_ind, tLEFT);
		}else{
			if(right){
				merge(r, p->childs[c_ind+1], c_ind, t);
			}else{
				merge(p->childs[c_ind-1], r, c_ind-1, t);
			}
			if(p->n < t-1){
				treeCorrect(p, t);
			}
		}
	}
	return;
}

void delete(node* rt, int k, int t){			//Удаление ключа из дерева
	node* r = keyFinder(rt, k);	//Находим нужный узел
	if(!r){
		return;
	}
	int key_num = r->n;			//Находим индекс элемента
	int key_ind = 0;
	while(r->keys[key_ind] != k){
		key_ind++;
	}

	if(r->leaf){	//Удаляем из листа элемент и выполняем корректировку дерева, если там
		for(int j = key_ind; j <= key_num-2; j++){	//оказалось меньше t-1 элемента.
			r->keys[j] = r->keys[j+1];
		}
		r->n -= 1;
		if(r->n < t-1){
			treeCorrect(r, t);
		}
	}else{ //Удаление из внутреннего узла
		node* child = r->childs[key_ind+1];	//Ищем самый левый лист ребёнка справа
		while(!child->leaf){
			child = child->childs[0];
		}
		int k1 = child->keys[0];
		r->keys[key_ind] = k1;
		delete(child, k1, t);
	}
}

void tree_print(node* r){					//Вывод структуры дерева на экран(от листьев к корню)
	if(!r->leaf){
		for(int i = 0; i <= r->n; i++){
			tree_print(r->childs[i]);
		}
	}
	for(int j = 0; j <= r->n-1; j++){
		printf("%d ", r->keys[j]);
	}

	printf("\n");
	return;
}
