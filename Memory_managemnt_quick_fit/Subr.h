#ifndef SUBS
#define SUBS                // Make sure we don't redeclare the file if it already is

#include<stdio.h>
#define MEM_SIZE 1024
#define SIZE_INT 4
#define SIZE_CHAR 1
#define SIZE_STRUCT 12

char memory[MEM_SIZE];

struct memory_block{
 int size;
 int used;
 struct memory_block *next;
};

void mem_init(void);
void *my_malloc(size_t numBytes);
void my_merge_blocks();
void my_split_block(struct memory_block *fitting_slot,size_t size);
void my_free(void* ptr);
void show_memory_structure(void);

void mem_init(void){
	//initialize the memory to be used by setting the available memory
	//as there is only one block, next isn't set
	//the avaialble memory t=is the mem used minus the sized of the storage block.
	struct memory_block *freeList=(void*)memory;
	freeList->size=MEM_SIZE-SIZE_STRUCT;
	freeList->used=0;
	freeList->next=NULL;
}

void *my_malloc(size_t numBytes){
	//create the structs which hold the data
	struct memory_block *curr,*freeList=(void*)memory;
 	void *result;
 	//initalaize the memory if it hasn´t
 	if(!(freeList->size)){
  		mem_init();
 	}
 	//start the search at the beginning of memory
 	curr=freeList;

	while((((curr->size)<numBytes)||((curr->used)==1))&&(curr->next!=NULL)){
  		curr=curr->next;
 	}
	//if the block is exactly big enough
	if((curr->size)==numBytes){
  		curr->used=1;
  		result=(void*)(++curr);
  		return result;
 	}
	//if it is biger resize it
 	else if((curr->size)>(numBytes+SIZE_STRUCT)){
  		my_split_block(curr,numBytes);
  		result=(void*)(++curr);
  		return result;
 	}
	//else, there isn't a block big enough
 	else{
  		result=NULL;
  	return result;
 	}
}

void my_free(void* ptr){
	//if so, free the block that was in front of pnr, and merge the memory structure
	struct memory_block* curr=ptr;
	--curr;
	curr->used=0;
	my_merge_blocks();
}

void my_merge_blocks(){
 	struct memory_block *curr,*temp,*freeList=(void*)memory;
 	curr=freeList;
	//create pointers pointing to memory, and check is two cosecutive block are free
	//if so, add the size of the info block to the previous one and frop the 2nd item.
	while((curr->next)!=NULL){
	//save the next adress so it isn't lost as we drop a frame
    temp = curr->next;
  		if((!(curr->used)) && (!(curr->next->used))){
   			curr->size+=(curr->next->size)+SIZE_STRUCT;
   			curr->next=curr->next->next;
  		}
  		//relaod the next adress
  		curr=temp;
 	}
}

void my_split_block(struct memory_block *fitting_slot,size_t size){
	//take the existing given structure and split it in two, than use the date of the existing
	//struture to fill in the new one.
 	struct memory_block *newp=(void*)(fitting_slot+size+SIZE_STRUCT);
 	newp->size=(fitting_slot->size)-size-SIZE_STRUCT;
 	newp->used=0;
 	newp->next=fitting_slot->next;
 	fitting_slot->size=size;
 	fitting_slot->used=1;
 	fitting_slot->next=newp;
}

void show_memory_structure(void){
	struct memory_block *curr,*freeList=(void*)memory;
	curr = freeList;
	//loop troug the storage list and print the size and if they are used or not
	while(1)
	{
		if(curr->used){
			printf("Block of %*i bytes: in use\n", 7, curr->size);
		}
		else{
			printf("Block of %*i bytes: free\n", 7, curr->size);
		}
		//stop if you reach the end
		if(curr->next==NULL){
			break;
		}
		curr=curr->next;
	}
}

#endif //SUBS
