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

void init_list(List *p_list, ConstDataFp dump_data, DataFp free_data,
		CompareDataFp compare_data, DataFp modify_data) {
    (*p_list).dump_data=dump_data;
    (*p_list).free_data=free_data;
    (*p_list).compare_data=compare_data;
    (*p_list).modify_data=modify_data;
    (*p_list).head=NULL;
    (*p_list).tail=NULL;
}

// Print elements of the list
void dump_list(const List* p_list) {
    ListElement*temp=(*p_list).head;
    while(temp!=NULL){
        (*p_list).dump_data((*temp).data);
        temp=(*temp).next;
    }
    printf("\n");
}

// Print elements of the list if comparable to data
void dump_list_if(List *p_list, void *data) {
    ListElement*temp=(*p_list).head;
    while(temp!=NULL){
        if((*p_list).compare_data((*temp).data,data)==0){
            (*p_list).dump_data((*temp).data);
        }
        temp=(*temp).next;
    }
}

// Free all elements of the list
void free_list(List* p_list) {
	(*p_list).tail = NULL;
	ListElement*curr=(*p_list).head;
	while(curr!=NULL){
        (*p_list).free_data((*curr).data);
        curr=(*curr).next;
	}
	(*p_list).tail=NULL;
}

// Push element at the beginning of the list
void push_front(List *p_list, void *data){
    ListElement*temp=malloc(sizeof(ListElement));
    (*temp).data=data;
    (*temp).next=NULL;
    if((*p_list).head!=NULL){
        (*temp).next=(*p_list).head;
        (*p_list).head=temp;
    }
    else{
        (*p_list).head=temp;
        (*p_list).tail=temp;
    }
}

// Push element at the end of the list
void push_back(List *p_list, void *data) {
    ListElement*temp=malloc(sizeof(ListElement));
    (*temp).data=data;
    (*temp).next=NULL;
    if((*p_list).head!=NULL){
        (*(*p_list).tail).next=temp;
        (*p_list).tail=temp;
    }
    else{
        (*p_list).head=temp;
        (*p_list).tail=temp;
    }
}

// Remove the first element
void pop_front(List *p_list) {
    if((*p_list).head!=(*p_list).tail){
        ListElement*temp=(*p_list).head;
        (*p_list).head=(*(*p_list).head).next;
        (*p_list).free_data(temp);
    }
    else{
        if((*p_list).head!=NULL){
            ListElement*temp=(*p_list).head;
            (*p_list).free_data(temp);
            (*p_list).head=NULL;
            (*p_list).tail=NULL;
        }
    }

}

// Reverse the list
void reverse(List *p_list) {
    if((*p_list).head!=NULL){
        if((*p_list).head!=(*p_list).tail){
            ListElement*first=(*p_list).head;
            ListElement*last=(*p_list).tail;
            ListElement*curr=(*p_list).head;
            ListElement*prev=(*p_list).head;
            curr=(*curr).next;
            (*prev).next=NULL;
            while(curr!=NULL){
                ListElement*temp=(*curr).next;
                (*curr).next=prev;
                prev=curr;
                curr=temp;

            }
            (*p_list).head=last;
            (*p_list).tail=first;
        }
    }
}

// Insert element after 'previous'
void push_after(List *p_list, void *data, ListElement *previous) {
}

// Insert element preserving order
void insert_in_order(List *p_list, void *p_data) {
    if((*p_list).head==NULL){
        push_back(p_list,p_data);
    }
    else{
        //printf("%d\n",(*p_list).compare_data((*(*p_list).head).data,p_data));
        if((*p_list).compare_data((*(*p_list).head).data,p_data)>0){
            push_front(p_list,p_data);
            return;
        }
        else if((*p_list).compare_data((*(*p_list).head).data,p_data)==0){
            return;
        }
        if((*p_list).compare_data((*(*p_list).tail).data,p_data)<0){
            push_back(p_list,p_data);
            return;
        }
        else if((*p_list).compare_data((*(*p_list).tail).data,p_data)==0){
            return;
        }
        ListElement*temp=malloc(sizeof(ListElement));
        ListElement*curr=(*p_list).head;
        ListElement*prev=(*p_list).head;
        curr=(*curr).next;
        (*temp).data=p_data;
        while((*p_list).compare_data((*curr).data,p_data)<0){
            prev=(*prev).next;
            curr=(*curr).next;
        }
        if((*p_list).compare_data((*curr).data,p_data)!=0){
            (*prev).next=temp;
            (*temp).next=curr;
        }
        else if((*p_list).modify_data!=NULL){
            (*p_list).modify_data(curr);
        }
    }
}

// -----------------------------------------------------------
// --- type-specific definitions

// int element

void dump_int(const void *d) {
    printf("%d ",*(int*)d);
}

void free_int(void *d) {
    free(d);
}

int cmp_int(const void *a, const void *b) {
    int*temp1=(int*)a;
    int*temp2=(int*)b;
    if(*temp1>*temp2) return 1;
    if(*temp1<*temp2) return -1;
    return 0;
}

int *create_data_int(int v) {
    int *temp=malloc(sizeof(v));
    *temp=v;
    return temp;
}

// Word element

typedef struct DataWord {
	char *word;
	int counter;
} DataWord;

void dump_word (const void *d) {
    DataWord*temp=(DataWord*)d;
    printf("%s ",(*temp).word);
}

void free_word(void *d) {
    free((*(DataWord *)d).word);
    free(d);
}

int cmp_word_alphabet(const void *a, const void *b) {
    DataWord*temp1=(DataWord*)a;
    DataWord*temp2=(DataWord*)b;
    return strcasecmp((*temp1).word,(*temp2).word);
}

int cmp_word_counter(const void *a, const void *b) {
    DataWord*temp1=(DataWord*)a;
    DataWord*temp2=(DataWord*)b;
    return (*temp1).counter-(*temp2).counter;
}

void modify_word(void *p) {
    DataWord*temp=(DataWord*)p;
    (*temp).counter+=1;
}

void *create_data_word(const char *string, int counter) {
    DataWord*temp=malloc(sizeof(DataWord));
    (*temp).word=string;
    (*temp).counter=counter;
    return temp;
}

// read text, parse it to words, and insert those words to the list.
// Order of insertions is given by the last parameter of type CompareDataFp.
// (comparator function address). If this address is not NULL the element is
// inserted according to the comparator. Otherwise, read order is preserved.
void stream_to_list(List *p_list, FILE *stream, CompareDataFp cmp) {
    char line[BUFFER_SIZE];
    char *sep=".,?!:;-\n\t\r ";
    char *word;

    while(fgets(line,BUFFER_SIZE,stream)){
        //printf("%s ",line);
        word=strtok(line,sep);
        while(word!=NULL){
            if(word!=NULL){
                DataWord*temp=malloc(sizeof(DataWord));
                (*temp).word=safe_strdup(word);
                if(cmp!=NULL){
                    (*p_list).compare_data=cmp;
                    insert_in_order(p_list,temp);
                    dump_list(p_list);
                }
                else{
                    (*temp).counter=1;
                    push_back(p_list,temp);
                }
            }
            //printf("word: %s \n",word);
            word=strtok(NULL,sep);

        }
    }
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
		//dump_list(p_list);
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
			//dump_list(&list);
			dump_list_if(&list, &data);
			free_list(&list);
			break;
		default:
			printf("NOTHING TO DO FOR %d\n", to_do);
			break;
	}
	return 0;
}

