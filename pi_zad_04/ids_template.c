#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define IN_LINE_COMMENT 1
#define IN_BLOCK_COMMENT 2
#define IN_STRING 4
#define IN_ID 8

#define MAX_ID_LEN 64
#define MAX_IDS 1024

int index_cmp(const void*, const void*);
int cmp(const void*, const void*);
int idx_1=0;

char identifiers[MAX_IDS][MAX_ID_LEN];

const char *keywords[] = {
	"auto", "break", "case", "char",
	"const", "continue", "default", "do",
	"double", "else", "enum", "extern",
	"float", "for", "goto", "if",
	"int", "long", "register", "return",
	"short", "signed", "sizeof", "static",
	"struct", "switch", "typedef", "union",
	"unsigned", "void", "volatile", "while"
};

int condition_1(char x){
    if(x>='a' && x<='z') return 1;
    if(x>='A' && x<='Z') return 1;
    if(x=='_') return 1;
    return 0;
}

int condition_2(char x){
    if(x>='a' && x<='z') return 1;
    if(x>='A' && x<='Z') return 1;
    if(x>='0' && x<='9') return 1;
    if(x=='_') return 1;
    return 0;
}

int equal_str(char t1[],char t2[]){
    if(strlen(t1)!=strlen(t2)) return 0;
    for(int i=0; i<strlen(t1); i++){
        if(t1[i]!=t2[i]) return 0;
    }
    return 1;
}

int find_idents() {
    char text[50000], pom[MAX_ID_LEN]={0};
    int idx_2=0;
    int flag1=0, flag2=0, flag3=0, flag4=0;
    while(fgets(text,50000,stdin)!=NULL){
        //printf("%s ",text);
        for(int i=0; i<strlen(text); i++){
            if(text[i]==32 || text[i]==9 || text[i]==10 || text[i]==13) continue;
            char pom[MAX_ID_LEN]={0};
            idx_2=0;
            if(condition_1(text[i])==1){
                flag1=0;
                while(1){
                    if(condition_2(text[i])==1){
                        pom[idx_2]=text[i];
                        idx_2+=1;
                    }
                    else{
                        flag1=1;
                        break;
                    }
                    i+=1;
                }
            }
            //printf("%s ",pom);
            flag2=0;
            for(int j=0; j<idx_1; j++){
                if(strcmp(identifiers[j],pom)==0) flag2=1;
            }
            flag3=0;
            for(int j=0; j<idx_1; j++){
                for(int k=0; k<32; k++){
                    if(strcmp(identifiers[j], *keywords[k])==0) flag3=1;
                }
            }
            if(flag3==0 && flag2==0){
                strcpy(identifiers[idx_1],pom);
                idx_1++;
            }
            //printf(" %d ",idx_1);
        }
        char text[50000]={0};
    }

    for(int i=0; i<idx_1; i++){
        printf("%s ",identifiers[i]);
    }
}

int cmp(const void* first_arg, const void* second_arg) {
	char *a = *(char**)first_arg;
	char *b = *(char**)second_arg;
	return strcmp(a, b);
}

int index_cmp(const void* first_arg, const void* second_arg) {
	int a = *(int*)first_arg;
	int b = *(int*)second_arg;
	return strcmp(identifiers[a], identifiers[b]);
}

int main(void) {
	printf("%d\n", find_idents());
	return 0;
}

