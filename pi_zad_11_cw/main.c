#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define BUFFER_SIZE 1024

struct List;
typedef void (*DataFp)(void*);
typedef void (*ConstDataFp)(const void*);
typedef  int (*CompareDataFp)(const void*, const void*);

typedef struct ListElement {
	struct ListElement *next;
	void *data;
} ListElement;

typedef struct {
	ListElement *head;
	ListElement *tail;
	ConstDataFp dump_data;
	DataFp free_data;
	CompareDataFp compare_data;
	DataFp modify_data;
} List;

void *safe_malloc(size_t size) {
	void *ptr = malloc(size);
	if(ptr) return ptr;
	printf("malloc error\n");
	exit(EXIT_FAILURE);
}

void *safe_strdup(const char *string) {
	void *ptr = strdup(string);
	if(ptr) return ptr;
	printf("strdup error\n");
	exit(EXIT_FAILURE);
}

// --- generic functions --- for any data type

void init_list(List *p_list, ConstDataFp dump_data, DataFp free_data, CompareDataFp compare_data, DataFp modify_data) {
    (*p_list).compare_data=compare_data;
    (*p_list).dump_data=dump_data;
    (*p_list).free_data=free_data;
    (*p_list).modify_data=modify_data;
    (*p_list).head=NULL;
    (*p_list).tail=NULL;
}

// Print elements of the list
void dump_list(const List* p_list) {
    ListElement*temp=(*p_list).head;
    while(temp){
        (*p_list).dump_data((*temp).data);
        temp=(*temp).next;
    }
}

// Print elements of the list if comparable to data
void dump_list_if(List *p_list, void *data) {

}

// Free all elements of the list
void free_list(List* p_list) {
    if((*p_list).head==NULL)
        return;
    if((*p_list).head==(*p_list).tail){
        (*p_list).free_data((*(*p_list).head).data);
        (*p_list).head=NULL;
        (*p_list).tail=NULL;
        return;
    }
    ListElement*curr=(*(*p_list).head).next;
    while(curr){
        ListElement*temp=curr;
        curr=(*curr).next;
        (*p_list).free_data((*temp).data);
        free(temp);
    }
    (*p_list).head=NULL;
    (*p_list).tail=NULL;
}


// Push element at the beginning of the list
void push_front(List *p_list, void *data){
    ListElement*to_push=malloc(sizeof(ListElement));
    (*to_push).data=data;
    if((*p_list).head==NULL){
        (*p_list).head=to_push;
        (*p_list).tail=to_push;
        (*(*p_list).tail).next=NULL;
        return;
    }
    (*to_push).next=(*p_list).head;
    (*p_list).head=to_push;
}

// Push element at the end of the list
void push_back(List *p_list, void *data) {
    ListElement*to_push=malloc(sizeof(ListElement));
    (*to_push).data=data;
    if((*p_list).head==NULL){
        (*p_list).head=to_push;
        (*p_list).tail=to_push;
        (*(*p_list).tail).next=NULL;
        return;
    }
    (*(*p_list).tail).next=to_push;
    (*p_list).tail=to_push;
    (*(*p_list).tail).next=NULL;
}

// Remove the first element
void pop_front(List *p_list) {
    if((*p_list).head==NULL){
        return;
    }
    if((*p_list).head==(*p_list).tail){
        (*p_list).free_data((*(*p_list).head).data);
        free((*p_list).head);
        (*p_list).head=NULL;
        (*p_list).tail=NULL;
        return;
    }
    ListElement*temp=(*p_list).head;
    (*p_list).head=(*(*p_list).head).next;
    (*p_list).free_data((*temp).data);
    free(temp);
}

// Reverse the list
void reverse(List *p_list) {
    if((*p_list).head==NULL){
        return;
    }
    if((*p_list).head==(*p_list).tail){
        return;
    }
    ListElement*curr=(*(*p_list).head).next;
    ListElement*prev=(*p_list).head;
    ListElement*last=(*p_list).tail;
    ListElement*first=(*p_list).head;
    (*prev).next=NULL;
    while((*curr).next){
        ListElement*temp=(*curr).next;
        (*curr).next=prev;
        prev=curr;
        curr=temp;
    }
    (*p_list).head=last;
    (*p_list).tail=first;
}

// find element in sorted list after which to insert given element
ListElement* find_insertion_point(const List *p_list, ListElement *p_element) {
}

// Insert element after 'previous'
void push_after(List *p_list, void *data, ListElement *previous) {
    ListElement*to_push=malloc(sizeof(ListElement));
    (*to_push).data=data;
    (*to_push).next=(*previous).next;
    (*previous).next=to_push;
}

// Insert element preserving order
void insert_in_order(List *p_list, void *p_data) {
    ListElement*curr=(*(*p_list).head).next;
    ListElement*prev=(*p_list).head;
    ListElement*to_insert=malloc(sizeof(ListElement));
    (*to_insert).data=p_data;
    while(curr){
        if((*p_list).compare_data((*curr).data,p_data)>0){  //(*curr).data>p_data
            (*prev).next=to_insert;
            (*to_insert).next=curr;
            return;
        }
        if((*p_list).compare_data((*curr).data,p_data)==0){
            if((*p_list).modify_data){
                (*p_list).modify_data((*curr).data);
            }
            return;
        }
        curr=(*curr).next;
        prev=(*prev).next;
    }
    push_back(p_list,p_data);
}

// -----------------------------------------------------------
// --- type-specific definitions

// int element

void dump_int(const void *d) {
    int*temp=(int*)d;
    printf("%d",*temp);
}

void free_int(void *d) {
    int*temp=(int*)temp;
    free(temp);
}

int cmp_int(const void *a, const void *b) {
    int temp1=*(int*)a;
    int temp2=*(int*)b;
    return(temp1-temp2);
}

int *create_data_int(int v) {
}

// Word element

typedef struct DataWord {
	char *word;
	int counter;
} DataWord;

void dump_word (const void *d) {
}

void free_word(void *d) {
    DataWord*temp=(DataWord*)d;
    free((*temp).word);
    free((*temp).counter);
}

int cmp_word_alphabet(const void *a, const void *b) {
}

int cmp_word_counter(const void *a, const void *b) {
}

void modify_word(void *p) {
}

void *create_data_word(const char *string, int counter) {
}

// read text, parse it to words, and insert those words to the list.
// Order of insertions is given by the last parameter of type CompareDataFp.
// (comparator function address). If this address is not NULL the element is
// inserted according to the comparator. Otherwise, read order is preserved.
void stream_to_list(List *p_list, FILE *stream, CompareDataFp cmp) {
}

// test integer list
void list_test(List *p_list, int n) {
	char op;
	int v;
	for (int i = 0; i < n; ++i) {
		scanf(" %c", &op);
		switch (op) {
			case 'f':
				scanf("%d", &v);
				push_front(p_list, create_data_int(v));
				break;
			case 'b':
				scanf("%d", &v);
				push_back(p_list, create_data_int(v));
				break;
			case 'd':
				pop_front(p_list);
				break;
			case 'r':
				reverse(p_list);
				break;
			case 'i':
				scanf("%d", &v);
				insert_in_order(p_list, create_data_int(v));
				break;
			default:
				printf("No such operation: %c\n", op);
				break;
		}
	}
}

int main(void) {
	int to_do, n;
	List list;

	scanf ("%d", &to_do);
	switch (to_do) {
		case 1: // test integer list
			scanf("%d",&n);
			init_list(&list, dump_int, free_int, cmp_int, NULL);
			list_test(&list, n);
			dump_list(&list);
			free_list(&list);
			break;
		case 2: // read words from text, insert into list, and print
			init_list(&list, dump_word, free_word, NULL, NULL);
			stream_to_list(&list, stdin, NULL);
			dump_list(&list);
			free_list(&list);
			break;
		case 3: // read words, insert into list alphabetically, print words encountered n times
			scanf("%d",&n);
			init_list(&list, dump_word, free_word, NULL, modify_word);
			stream_to_list(&list, stdin, cmp_word_alphabet);
			list.compare_data = cmp_word_counter;
			DataWord data = { NULL, n };
			dump_list_if(&list, &data);
			free_list(&list);
			break;
		default:
			printf("NOTHING TO DO FOR %d\n", to_do);
			break;
	}
	return 0;
}

