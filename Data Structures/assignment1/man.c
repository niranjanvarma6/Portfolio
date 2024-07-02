#include "list.h"
#include "dynarray.h"
#include <time.h>
#include <stdio.h>
#include <stdlib.h>

int cmp(void* a, void* b){
	if(*(int*)(a) == *(int*)(b)){
		return 0;
	}
	else{
		return -1;
	}
}

int main(){
	
	printf("Test\n");	
	int z1 = 10;
	int z2 = 20;
	int z3 = 30;
	int z4 = 40;
	int z5 = 50;
	int z6 = 60;
	void* v1 = &z1;
	void* v2 = &z2;
	void* v3 = &z3;
	void* v4 = &z4;
	void* v5 = &z5;
	void* v6 = &z6;

	
	//Dynarray
		
	struct dynarray* harray = dynarray_create();
	dynarray_insert(harray, v1);
	dynarray_print(harray);
	dynarray_insert(harray, v2);
	dynarray_print(harray);
	dynarray_insert(harray, v3);	
	dynarray_print(harray);
	dynarray_set(harray, 2, v5);
	dynarray_print(harray);
	dynarray_insert(harray, v4);
	dynarray_print(harray);
	dynarray_insert(harray, v6);
	dynarray_print(harray);
	dynarray_remove(harray, 2);
	dynarray_print(harray);
	dynarray_insert(harray, v5);
	dynarray_print(harray);
	printf("harray[2] = %d\n", *(int*)(dynarray_get(harray, 2)));
	printf("harray[10] = %d\n", *(int*)(dynarray_get(harray, 10)));
	dynarray_remove(harray, 8);


	dynarray_free(harray);
	/*
	struct list* Linked = list_create();
	
	list_insert(Linked, v1);
	list_print(Linked);
	list_insert(Linked, v2);
	list_print(Linked);
	list_insert(Linked, v3);
	list_print(Linked);
	list_insert(Linked, v4);
	list_print(Linked);
	list_insert(Linked, v5);
	list_print(Linked);
	list_insert(Linked, v6);
	list_print(Linked);
	list_reverse(Linked);
	printf("Pos 20 %d\n", list_position(Linked, v2, cmp));
	list_remove(Linked, v5, cmp);
	list_print(Linked);

	list_free(Linked);

	*/	
	return 0;	


}
