#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define MAX_STR_LEN 64

typedef struct Vector {
	void *data;
	size_t element_size;
	size_t size;
	size_t capacity;
} Vector;

typedef struct Person {
	int age;
	char first_name[MAX_STR_LEN];
	char last_name[MAX_STR_LEN];
} Person;

typedef int(*cmp_ptr)(const void*, const void*);
typedef int(*predicate_ptr)(void*);
typedef void(*read_ptr)(void*);
typedef void(*print_ptr)(const void*, const size_t);

// Allocate vector to initial capacity (block_size elements),
// Set element_size, size (to 0), capacity
void init_vector(Vector *vector, size_t block_size, size_t element_size) {
    (*vector).data=malloc(block_size*element_size);
    (*vector).size=0;
    (*vector).capacity=block_size;
    (*vector).element_size=element_size;
}

// If new_capacity is greater than the current capacity,
// new storage is allocated, otherwise the function does nothing.
void reserve(Vector *vector, size_t new_capacity) {
    if(new_capacity>(*vector).capacity){
        (*vector).capacity=new_capacity;
        (*vector).data=realloc((*vector).data, new_capacity*(*vector).element_size);
    }
}

// Resizes the vector to contain new_size elements.
// If the current size is greater than new_size, the container is
// reduced to its first new_size elements.

// If the current size is less than new_size,
// additional zero-initialized elements are appended
void resize2(Vector *vector, size_t new_size) {
    if(new_size>(*vector).size){
        char *temp=(char*)(*vector).data;
        temp+=(*vector).element_size*(*vector).size;
        for(size_t i=(*vector).size; i<new_size; i++){
            memcpy(temp, calloc(1,(*vector).element_size), (*vector).element_size);
            *temp=0;
            temp += (*vector).element_size;
        }
        (*vector).size=new_size;
        if((*vector).capacity<new_size){
            (*vector).capacity=new_size;
        }
    }

}
void resize(Vector *vector, size_t new_size) {
    if((*vector).size<new_size){
        reserve(vector, new_size*((*vector).element_size));
        memset(((*vector).data)+((*vector).size)*(*vector).element_size, 0, (new_size-(*vector).size)*((*vector).element_size));
    }
    (*vector).size=new_size;
}

// Add element to the end of the vector
void push_back(Vector *vector, void *value) {
    if((*vector).capacity==(*vector).size){
        reserve(vector,(*vector).capacity*2);
    }
    char*temp=(char*)(*vector).data;
    temp+=(*vector).size*(*vector).element_size;
    memcpy(temp,value,(*vector).element_size);
    (*vector).size+=1;
}

// Remove all elements from the vector
void clear(Vector *vector) {
    (*vector).size=0;
}

// Insert new element at index (0 <= index <= size) position
void insert(Vector *vector, size_t index, void *value) {
    if((*vector).capacity<=(*vector).size){
        reserve(vector,(*vector).capacity*2);
    }
    if(index==(*vector).size+1){
        push_back(vector,value);
    }
    char*first=(char*)(*vector).data;
    char*last=(char*)(*vector).data;
    char*prev=(char*)(*vector).data;
    last+=(*vector).size*(*vector).element_size;
    first+=index*(*vector).element_size;
    prev=prev+(*vector).size*(*vector).element_size-(*vector).element_size;
    for(int i=index; i<(*vector).size; i++){
        memcpy(last, prev, (*vector).element_size);
        last-=(*vector).element_size;
        prev-=(*vector).element_size;
    }
    memcpy(last, value, (*vector).element_size);
    (*vector).size+=1;
}

// Erase element at position index
void erase(Vector *vector, size_t index){
    char*prev=(char*)(*vector).data;
    char*curr=(char*)(*vector).data;
    prev+=(*vector).element_size*(index);
    curr+=(*vector).element_size*(index+1);
    for(int i=index; i<(*vector).size; i++){
        memcpy(prev,curr,(*vector).element_size);
        prev+=(*vector).element_size;
        curr+=(*vector).element_size;
    }
    (*vector).size-=1;
}

// Erase all elements that compare equal to value from the container
void erase_value(Vector *vector, void *value, cmp_ptr cmp){
    char*temp=(char*)(*vector).data;
    int i=0, curr_size=(*vector).size;
    while(i<curr_size){
        if(cmp(temp, value)==0){
            erase(vector,i);
            curr_size-=1;
        }
        else{
            i+=1;
            temp+=(*vector).element_size;
        }
    }
}

// Erase all elements that satisfy the predicate from the vector
void erase_if(Vector *vector, int (*predicate)(void *)){
    char*temp=(char*)(*vector).data;
    int i=0, curr_size=(*vector).size;
    while(i<curr_size){
        if(predicate(temp)==0){
            erase(vector,i);
            curr_size-=1;
        }
        else{
            i+=1;
            temp+=(*vector).element_size;
        }
    }
}

// Request the removal of unused capacity
void shrink_to_fit(Vector *vector){
    if((*vector).size!=(*vector).capacity){
        (*vector).data=realloc((*vector).data, (*vector).size*(*vector).element_size);
        (*vector).capacity=(*vector).size;
    }
}

// integer comparator
int int_cmp(const void *v1, const void *v2) {
    int*temp1=(int*)v1;
    int*temp2=(int*)v2;
    if(*temp1==*temp2){
        return 0;
    }
    if(*temp1<*temp2){
        return -1;
    }
    if(*temp1>*temp2){
        return 1;
    }
}

// char comparator
int char_cmp(const void *v1, const void *v2) {
    char*temp1=(char*)v1;
    char*temp2=(char*)v2;
    if(*temp1==*temp2){
        return 0;
    }
    if(*temp1<*temp2){
        return -1;
    }
    if(*temp1>*temp2){
        return 1;
    }
}

// Person comparator:
// Sort according to age (decreasing)
// When ages equal compare first name and then last name
int person_cmp(const void *p1, const void *p2) {
    Person*temp1=(Person*)p1;
    Person*temp2=(Person*)p2;
    if((*temp1).age>(*temp2).age)
        return -1;
    else if((*temp1).age<(*temp2).age)
        return 1;
    else{
        if(strcmp((*temp1).first_name,(*temp2).first_name)==0){
            return strcmp((*temp1).last_name, (*temp2).last_name);
        }
        return strcmp((*temp1).first_name,(*temp2).first_name);
    }
}

// predicate: check if number is even
int is_even(void *value) {
    int *val=(int*)value;
    if(*val%2==0){
        return 1;
    }
    return 0;
}

// predicate: check if char is a vowel
int is_vowel(void *value) {
    char *x = (char*)value;
    if(*x=='a' || *x== 'e' || *x=='i' || *x=='o' || *x=='u' || *x=='y' || *x=='A' || *x== 'E' || *x=='I' || *x=='O' || *x=='U' || *x=='Y'){
        return 0;
    }
    return 1;
}

// predicate: check if person is older than 25
int is_older_than_25(void *person) {
    Person*temp=(Person*)person;
    if((*temp).age>25)
        return 0;
    return 1;
}

// print integer value
void print_int(const void *v,const size_t size) {
    int *temp=(int*)v;
    for(size_t i=0; i<size; i++){
        printf("%d ", *temp);
        temp+=1;
    }
}

// print char value
void print_char(const void *v, const size_t size) {
    char*temp=(char*)v;
    for(size_t i=0; i<size; i++){
        printf("%c ",*temp);
        temp+=1;
    }
}

// print structure Person
void print_person(const void *v, const int size) {
    Person*temp=(Person*)v;
    for(size_t i=0; i<size; i++){
        printf("%d %s %s\n",(*temp).age, (*temp).first_name, (*temp).last_name);
        temp+=1;
    }
}

// print capacity of the vector and its elements
void print_vector(Vector *vector, print_ptr print){
    printf("%d\n",(*vector).capacity, (*vector).size);
    print((*vector).data, (*vector).size);
    printf("\n");
}

// read int value
void read_int(void* value) {
    scanf("%d", (int*)value);
}

// read char value
void read_char(void* value) {
    char c;
    scanf(" %c", &c);
    *(char*)value = c;
}

// read struct Person
void read_person(void* value) {
    Person*temp=(Person*)value;
    scanf("%d %s %s", &(*temp).age, &(*temp).first_name, &(*temp).last_name);
}

void *safe_malloc(size_t size) {
	void *ptr = malloc(size);
	if(ptr) return ptr;
	printf("malloc error\n");
	exit(EXIT_FAILURE);
}

void vector_test(Vector *vector, size_t block_size, size_t elem_size, int n, read_ptr read,
		 cmp_ptr cmp, predicate_ptr predicate, print_ptr print) {
	init_vector(vector, block_size, elem_size);
	void *v = safe_malloc(vector->element_size);
	size_t index, size;
	//printf("%c",'4');
	for (int i = 0; i < n; ++i) {
		char op;
		scanf(" %c", &op);
		//printf("%c",op);
		switch (op) {
			case 'p': // push_back
				read(v);
				push_back(vector, v);
				break;
			case 'i': // insert
				scanf("%zu", &index);
				read(v);
				insert(vector, index, v);
				break;
			case 'e': // erase
				scanf("%zu", &index);
				erase(vector, index);
				break;
			case 'v': // erase
				read(v);
				erase_value(vector, v, cmp);
				break;
			case 'd': // erase (predicate)
				erase_if(vector, predicate);
				break;
			case 'r': // resize
				scanf("%zu", &size);
				resize(vector, size);
				break;
			case 'c': // clear
				clear(vector);
				break;
			case 'f': // shrink
				shrink_to_fit(vector);
				break;
			case 's': // sort
				qsort(vector->data, vector->size,
				      vector->element_size, cmp);
				break;
			default:
				printf("No such operation: %c\n", op);
				break;
		}
		//printf("test");
		//print_vector(vector, print);
	}
	//printf("\n\n");
	print_vector(vector, print);
	free(vector->data);
	free(v);
}

int main(void) {
    //printf("jfgj");
	int to_do, n;
	Vector vector_int, vector_char, vector_person;

	scanf("%d%d", &to_do, &n);

    //printf("%d%d ",to_do,n);
	switch (to_do) {
		case 1:
			vector_test(&vector_int, 4, sizeof(int), n, read_int, int_cmp,
				is_even, print_int);
			break;
		case 2:
			vector_test(&vector_char, 2, sizeof(char), n, read_char, char_cmp,
				is_vowel, print_char);
			break;
		case 3:
			vector_test(&vector_person, 2, sizeof(Person), n, read_person,
				person_cmp, is_older_than_25, print_person);
			break;
		default:
			printf("Nothing to do for %d\n", to_do);
			break;
	}

	return 0;
}

