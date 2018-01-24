#include<stdio.h>
#include "Subr.h"

#define NUM_OF_MALLOCS 16

int main(){

	void *p[NUM_OF_MALLOCS];

	mem_init();
	show_memory_structure();
    printf("initial check\n\n");

    for(int i=0;i<(NUM_OF_MALLOCS-2);i++){
    	p[i] = my_malloc(i);
    }

	show_memory_structure();
    printf("Successfully malloced memory\n\n");

    for(int i=0;i<(NUM_OF_MALLOCS-2);i++){
    	my_free(p[i]);
    }

	show_memory_structure();
    printf("Successfully free memory\n\n");


	p[NUM_OF_MALLOCS-1] = my_malloc(NUM_OF_MALLOCS-1);

	show_memory_structure();
    printf("Successfully mallocced some more\n\n");


    my_free(p[NUM_OF_MALLOCS-1]);

    show_memory_structure();
    printf("freed last malloc\n\n");

    my_merge_blocks();
  	show_memory_structure();
}
