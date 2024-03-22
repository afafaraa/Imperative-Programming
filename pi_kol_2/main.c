#include <stdio.h>
#include <stdlib.h>

typedef struct{
    double *results;
    int len;
}Data;

typedef struct tagQueueElement{
    struct tagQueueElement *next;
    Data data;
}QueueElement;

typedef struct tagQueue{
    QueueElement *head;
    QueueElement *tail;
    int no_elements;
}Queue;

void innit_queue(Queue*pqueue){
    (*pqueue).head=NULL;
    (*pqueue).tail=NULL;
    (*pqueue).no_elements=0;
}

void free_queue(Queue *pqueue){
    if((*pqueue).no_elements==0){
        return;
    }
    (*pqueue).tail=NULL;
    QueueElement*temp=(*pqueue).head;
    while(temp){
        QueueElement *to_free=temp;
        temp=(*temp).next;
        free(to_free);
    }
    (*pqueue).head=NULL;
    (*pqueue).no_elements=0;
}

void push_copy(Queue *pqueue, const Data*pdata){
    QueueElement*to_push=malloc(sizeof(QueueElement));
    (*to_push).data=*pdata;
    (*to_push).next=NULL;
    if((*pqueue).no_elements==0){
        (*pqueue).head=to_push;
        (*pqueue).tail=to_push;
        (*pqueue).no_elements=1;
        return;
    }
    QueueElement*last=(*pqueue).tail;
    (*last).next=to_push;
    (*pqueue).tail=to_push;
    (*pqueue).no_elements+=1;
}

int peek(Queue*pqueue, Data*pdata){
    if((*pqueue).no_elements==0){
        return 0;
    }
    pdata=malloc(sizeof(Data));
    QueueElement*temp=(*pqueue).head;
    (*pdata).len=(*temp).data.len;
    for(int i=0; i<(*pdata).len; i++){
        (*pdata).results=(*temp).data.results;
    }
    //dump_data(pdata);
    return 1;
}

int pop(Queue*pqueue, Data*pdata){
    if((*pqueue).no_elements==0){
        return -1;
    }
    QueueElement*to_pop=(*pqueue).head;
    QueueElement*temp=(*(*pqueue).head).next;

    (*pdata).len=(*to_pop).data.len;
    (*pdata).results=(*to_pop).data.results;

    (*to_pop).next=NULL;
    free(to_pop);
    (*pqueue).head=temp;
    (*pqueue).no_elements-=1;
    if((*pqueue).no_elements==0){
        return 0;
    }
    return 1;
}

int get_total_count(Queue*pqueue){
    QueueElement*temp=(*pqueue).head;
    int no=0;
    while(temp){
        no+=(*temp).data.len;
        temp=(*temp).next;
    }
    return no;
}

void dump_data(Data *data){
    for(int i=0; i<(*data).len; i++){
        printf("%lf ",(*data).results[i]);
    }
    printf("\n%d\n",(*data).len);
}

void dump_queue(Queue*pqueue){
    if((*pqueue).no_elements==0)
        printf("Nothing here -\(o.o)/-");
    QueueElement*temp=(*pqueue).head;
    while(temp){
        dump_data(&(*temp).data);
        temp=(*temp).next;
    }
}

double*read_data(double*res,int n){
    res=malloc(n*sizeof(double));
    for(int i=0; i<n; i++){
        scanf("%lf",res+i);
    }
    return res;
}

//1-push_copy, 2-peek, 3-pop
int main()
{
    Queue pqueue;
    int n,to_do,len;
    double *res;
    Data pdata,to_peek,to_pop;
    scanf("%d",&n);
    innit_queue(&pqueue);
    for(int i=0; i<n; i++){
        scanf("%d",&to_do);
        switch(to_do){
            case(1):
                scanf("%d",&len);
                res=read_data(res,len);
                pdata.results=res;
                pdata.len=len;
                push_copy(&pqueue,&pdata);
            break;
            case(2):
                peek(&pqueue,&pdata);
            break;
            case(3):
                pop(&pqueue,&to_pop);
            break;
            case(4):
                printf("total element count: %d\n",get_total_count(&pqueue));
            break;
            default:
                printf("Missclick\n");
        }
    }
    //free_queue(&pqueue);
    printf("to_pop:\n");
    dump_data(&to_pop);
    printf("queue:\n");
    dump_queue(&pqueue);
    return 0;
}
