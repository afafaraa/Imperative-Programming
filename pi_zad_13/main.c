#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#define BUFFER_SIZE 1024
#define MAX_RATE 4
#define MEMORY_ALLOCATION_ERROR  (-1)

typedef union {
	int int_data;
	char char_data;
	// ... other primitive types used
	void *ptr_data;
} data_union;

typedef struct ht_element {
	struct ht_element *next;
	data_union data;
} ht_element;

typedef void (*DataFp)(data_union);
typedef void (*DataPFp)(data_union*);
typedef int (*CompareDataFp)(data_union, data_union);
typedef size_t (*HashFp)(data_union, size_t);
typedef data_union (*CreateDataFp)(void*);

typedef struct {
	size_t size;
	size_t no_elements;
	ht_element **ht;
	DataFp dump_data;
	CreateDataFp create_data;
	DataFp free_data;
	CompareDataFp compare_data;
	HashFp hash_function;
	DataPFp modify_data;
} hash_table;

// ---------------------- functions to implement

// initialize table fields
void init_ht(hash_table *p_table, size_t size, DataFp dump_data, CreateDataFp create_data, DataFp free_data, CompareDataFp compare_data, HashFp hash_function, DataPFp modify_data) {
    (*p_table).size=size;
    (*p_table).no_elements=0;
    (*p_table).ht=malloc(size*sizeof(ht_element*));
    for(int i=0; i<size; i++){
        (*p_table).ht[i]=malloc(sizeof(ht_element));
        (*(*p_table).ht[i]).next=NULL;
        (*(*p_table).ht[i]).data.int_data=-1;
    }
    (*p_table).compare_data=compare_data;
    (*p_table).dump_data=dump_data;
    (*p_table).create_data=create_data;
    (*p_table).free_data=free_data;
    (*p_table).hash_function=hash_function;
    (*p_table).modify_data=modify_data;
}

// print elements of the list with hash n
void dump_list(const hash_table* p_table, size_t n) {
    ht_element*temp=(*(*p_table).ht[n]).next;
    while(temp!=NULL){
        (*p_table).dump_data((*temp).data);
        temp=(*temp).next;
    }
}

// Free element pointed by data_union using free_data() function
void free_element(DataFp free_data, ht_element *to_delete) {
    if(free_data)
        free_data((*to_delete).data);
    free(to_delete);
}

// free all elements from the table (and the table itself)
void free_table(hash_table* p_table) {
    for(int i=0; i<(*p_table).size; i++){
        ht_element*temp=(*(*p_table).ht[i]).next;
        while(temp){
            ht_element*element=(*temp).next;
            free_element((*p_table).free_data,temp);
            temp=element;
        }
        free((*p_table).ht[i]);
    }
    free((*p_table).ht);
}

// calculate hash function for integer k
size_t hash_base(int k, size_t size) {
	static const double c = 0.618033988; // (sqrt(5.) – 1) / 2.;
	double tmp = k * c;
	return (size_t)floor((double)size * (tmp - floor(tmp)));
}

void rehash(hash_table *p_table) {
    ht_element**new_table=malloc((*p_table).size*2*sizeof(ht_element*));
    ht_element**prev_table=(*p_table).ht;

    for(int i=0; i<(*p_table).size*2; i++)
    {
        new_table[i]=malloc(sizeof(ht_element));
        (*new_table[i]).next=NULL;
    }

    for(int i=0; i<(*p_table).size; i++)
    {
        ht_element*element=(*prev_table[i]).next;
        while(element!=NULL){
            ht_element*temp=(*element).next;

            int idx=(*p_table).hash_function((*element).data,2*(*p_table).size);
            (*element).next=(*new_table[idx]).next;
            (*new_table[idx]).next=element;

            element=temp;
        }
    }
    (*p_table).ht=new_table;
    (*p_table).size=2*(*p_table).size;
}

// return pointer to element with given value
ht_element *get_element(hash_table *p_table, data_union *data){
    int idx=(*p_table).hash_function(*data,(*p_table).size);
    ht_element*prev=(*p_table).ht[idx];
    ht_element*curr=(*(*p_table).ht[idx]).next;
    while(curr!=NULL && (*p_table).compare_data((*curr).data, *data)!=0){
        prev=curr;
        curr=(*curr).next;
    }
    return curr;
}

// insert element
void insert_element(hash_table *p_table, data_union *data){
    if(((*p_table).no_elements/(*p_table).size)>MAX_RATE){
        rehash(p_table);
    }
    int idx=(*p_table).hash_function(*data,(*p_table).size);
    ht_element*temp=(*p_table).ht[idx];
    while((*temp).next){
        if((*p_table).compare_data(*data,(*(*temp).next).data)==0){
            if((*p_table).modify_data){
                (*p_table).modify_data(&temp->next->data);
            }
            return;
        }
        temp=(*temp).next;
    }
    ht_element*element=malloc(sizeof(ht_element));
    (*element).data=*data;
    (*p_table).no_elements+=1;
    (*element).next=(*(*p_table).ht[idx]).next;
    (*(*p_table).ht[idx]).next=element;

}

// remove element
void remove_element(hash_table *p_table, data_union data) {
    int idx=(*p_table).hash_function(data,(*p_table).size);
    ht_element*curr=(*(*p_table).ht[idx]).next;
    ht_element*prev=(*p_table).ht[idx];
    while(curr!=NULL){
        if((*p_table).compare_data(data,(*curr).data)==0){
            (*prev).next=(*curr).next;
            (*curr).next=NULL;
            (*p_table).no_elements-=1;
            if((*p_table).free_data){
                (*p_table).free_data((*curr).data);
                free(curr);
            }
            return;
        }
        prev=curr;
        curr=(*curr).next;
    }
}

// type-specific definitions

// int element

size_t hash_int(data_union data, size_t size) {
	return hash_base(data.int_data, size);
}

void dump_int(data_union data) {
    printf("%d ", data.int_data);
}

int cmp_int(data_union a, data_union b) {
    return a.int_data-b.int_data;
}

data_union create_int(void* value) {
    int temp;
    scanf("%d", &temp);
    data_union data;
    data.int_data=temp;
    return data;
}

// char element

size_t hash_char(data_union data, size_t size) {
	return hash_base((int)data.char_data, size);
}

void dump_char(data_union data) {
    printf("%c ", data.char_data);
}

int cmp_char(data_union a, data_union b) {
    char temp1 = a.char_data;
    char temp2 = b.char_data;

    if(temp1<temp2)
        return -1;
    if(temp1>temp2)
        return 1;
    return 0;
}

data_union create_char(void* value) {
    char temp;
    scanf(" %c", &temp);

    data_union data;
    data.char_data = temp;

    return data;
}

// Word element

typedef struct DataWord {
	char *word;
	int counter;
} DataWord;

void dump_word(data_union data) {
    DataWord*temp=(DataWord*)data.ptr_data;
    printf("%s %d", (*temp).word, (*temp).counter);
}

void free_word(data_union data) {
    DataWord*temp=(DataWord*)data.ptr_data;
    free((*temp).word);
    free(temp);
}

int cmp_word(data_union a, data_union b) {
    return strcmp(((DataWord*)a.ptr_data)->word,((DataWord*)b.ptr_data)->word);
}

size_t hash_word(data_union data, size_t size) {
	int s = 0;
	DataWord *dw =(DataWord*)data.ptr_data;
	char *p = dw->word;
	while (*p) {
		s += *p++;
	}
	return hash_base(s, size);
}

void modify_word(data_union *data) {
    DataWord*temp=(DataWord*)(*data).ptr_data;
    (*temp).counter+=1;
}

data_union create_data_word(void *value) {
    char*temp=strdup(value);
    DataWord*element=malloc(sizeof(DataWord));
    for(int i=0; i<strlen(temp); i++){
        temp[i]=tolower(temp[i]);
    }
    (*element).word=temp;
    (*element).counter=1;
    data_union x;
    x.ptr_data=element;
    //dump_word(x);
    //printf("\n");
    return x;
}

// read text, parse it to words, and insert these words to the hashtable
void stream_to_ht(hash_table *p_table, FILE *stream) {
    char buff[BUFFER_SIZE];
    char*buff_ptr=fgets(buff,BUFFER_SIZE,stream);
    char *sep = ".,?!:;-\n\t\r ";
    while(buff_ptr){
        char*word=strtok(buff,sep);
        while(word){
            fflush(stdout);
            data_union data = create_data_word(word);
            insert_element(p_table, &data);
            word=strtok(NULL,sep);
        }
        buff_ptr=fgets(buff,BUFFER_SIZE,stream);
    }
}

// test primitive type list
void test_ht(hash_table *p_table, int n) {
	char op;
	data_union data;
	for (int i = 0; i < n; ++i) {
		scanf(" %c", &op);
//		p_table->create_data(&data);
		data = p_table->create_data(NULL); // should give the same result as the line above
		switch (op) {
			case 'r':
				remove_element(p_table, data);
				break;
			case 'i':
				insert_element(p_table, &data);
				break;
			default:
				printf("No such operation: %c\n", op);
				break;
		}
	}
}

int main(void) {
	int to_do, n;
	size_t index;
	hash_table table;
	char buffer[BUFFER_SIZE];
	data_union data;

	scanf ("%d", &to_do);
	switch (to_do) {
		case 1: // test integer hash table
			scanf("%d %zu", &n, &index);
			init_ht(&table, 4, dump_int, create_int, NULL, cmp_int, hash_int, NULL);
			test_ht(&table, n);
			printf ("%zu\n", table.size);
			dump_list(&table, index);
			break;
		case 2: // test char hash table
			scanf("%d %zu", &n, &index);
			init_ht(&table, 4, dump_char, create_char, NULL, cmp_char, hash_char, NULL);
			test_ht(&table, n);
			printf ("%zu\n", table.size);
			dump_list(&table, index);
			break;
		case 3: // read words from text, insert into hash table, and print
			scanf("%s", buffer);
			init_ht(&table, 8, dump_word, create_data_word, free_word, cmp_word, hash_word, modify_word);
			stream_to_ht(&table, stdin);
			printf ("%zu\n", table.size);
			data = table.create_data(buffer);
			ht_element *e = get_element(&table, &data);
			if (e) table.dump_data(e->data);
			if (table.free_data) table.free_data(data);
			break;
		default:
			printf("NOTHING TO DO FOR %d\n", to_do);
			break;
	}
	free_table(&table);

	return 0;
}

