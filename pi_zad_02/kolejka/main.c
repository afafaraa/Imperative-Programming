#include <stdio.h>

#define STACK_SIZE 10

enum state { OK = 0, UNDERFLOW = -1, OVERFLOW = -2 };

int stack[STACK_SIZE];
int top = 0;

int stack_push(int x) {
    if(top==STACK_SIZE){
        return OVERFLOW;
    }
    else{
        stack[top]=x;
        top+=1;
        return OK;
    }
}

int stack_pop(void) {
    int pom=0;
    if(top==0)
        return UNDERFLOW;
    else{
         pom=stack[top-1];
         stack[top-1]=0;
         top-=1;
         return pom;
    }
}

int stack_state(void) {
    int l=0;
    for(int i=0; i<STACK_SIZE; i++){
        if(stack[i]==0)
            break;
        l+=1;
    }
    return l;
}

// FIFO queue with shifts

#define QUEUE_SIZE 10

int queue[QUEUE_SIZE];
int in = 0, curr_nr = 0;

int queue_push(int in_nr) { // in_nr clients try to enter the queue
    int i=0;
    while(i<QUEUE_SIZE && in_nr>0){
        if(queue[i]==0){
            in_nr--;
            curr_nr++;
            queue[i]=curr_nr;
            in++;
        }
        i++;
    }
    curr_nr+=in_nr;
    if(in_nr>0)
        return OVERFLOW;
    return OK;
}

int queue_pop(int out_nr) {
    int i=out_nr;
    if(i>=in){
        in=0;
        for(int j=0; j<QUEUE_SIZE; j++)
            queue[j]=0;
        return UNDERFLOW;
    }
    while(i<QUEUE_SIZE && queue[i]!=0){
        queue[i-out_nr]=queue[i];
        i++;
    }
    while(i-out_nr<QUEUE_SIZE){
        queue[i-out_nr]=0;
        i++;
    }
    in-=out_nr;
    return in;
}

int queue_state(void) {
    int l=0, i=0;
    while(i<QUEUE_SIZE && queue[i]!=0){
        l+=1;
        i++;
    }
    return l;
}

void queue_print(void) {
    int l=0, i=0;
    while(i<QUEUE_SIZE && queue[i]!=0){
        printf("%d ",queue[i]);
        i++;
    }
}

// Queue with cyclic buffer

#define CBUFF_SIZE 10

int cbuff[CBUFF_SIZE];
int out = 0, len = 0;


int cbuff_push(int cli_nr) {
    if(len<CBUFF_SIZE){
        cbuff[(out+len)%CBUFF_SIZE]=cli_nr;
        len++;
        return OK;
    }
    else
        return OVERFLOW;
}

int cbuff_pop(void) {
    int pom=0;
    if(cbuff[out]==0)
        return UNDERFLOW;
    else{
        len--;
        pom=cbuff[out];
        cbuff[out]=0;
        out=(out+1)%CBUFF_SIZE;
        return pom;
    }
}

int cbuff_state(void) {
    return len;
}

void cbuff_print(void) {
    int i=out, j=0;
    while(j<len){
        printf("%d ",cbuff[i%CBUFF_SIZE]);
        i++;
        j++;
    }
}

int main(void) {
	int to_do, n, client_no, answer;
	scanf("%d", &to_do);
	switch(to_do) {
		case 1: // stack
			do {
				scanf("%d", &n);
				if (n > 0) {
					if ((answer = stack_push(n)) < 0) printf("%d ", answer);
				} else if (n < 0) {
					printf("%d ", stack_pop());
				} else printf("\n%d\n", stack_state());
			} while(n != 0);
			break;
		case 2: // FIFO queue with shifts
			do {
				scanf("%d", &n);
				if (n > 0) {
					if ((answer = queue_push(n)) < 0) printf("%d ", answer);
				} else if (n < 0) {
					if ((answer = queue_pop(-n)) < 0) printf("%d ", answer);
				} else {
					printf("\n%d\n", queue_state());
					queue_print();
				}
			} while(n != 0);
			break;
		case 3: // queue with cyclic buffer
			client_no = 0;
			do {
				scanf("%d", &n);
				if (n > 0) {
					if ((answer = cbuff_push(++client_no)) < 0) printf("%d ", answer);
				} else if (n < 0) {
					printf("%d ", cbuff_pop());
				} else {
					printf("\n%d\n", cbuff_state());
					cbuff_print();
				}
			} while(n != 0);
			break;
		default:
			printf("NOTHING TO DO!\n");
	}
	return 0;
}
