#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

// consider chars from [FIRST_CHAR, LAST_CHAR)
#define FIRST_CHAR 33
#define LAST_CHAR 127
#define MAX_CHARS (LAST_CHAR - FIRST_CHAR)
#define MAX_BIGRAMS ((LAST_CHAR - FIRST_CHAR) * (LAST_CHAR - FIRST_CHAR))

#define NEWLINE '\n'
#define IN_WORD 1

#define IN_LINE_COMMENT 1
#define IN_BLOCK_COMMENT 2
#define MAX_LINE 128


int count[MAX_BIGRAMS] = { 0 };

// sort indices according to their respective counts.
// sort alphabetically if counts equal
int cmp (const void *a, const void *b) {
	int va = *(int*)a;
	int vb = *(int*)b;
	if (count[va] == count[vb]) return va - vb;	return count[vb] - count[va];
}

// sort indices according to their respective counts.
// sort alphabetically if counts equal
int cmp_di (const void *a, const void *b) {
	int va = *(int*)a;
	int vb = *(int*)b;
	// sort according to second char if counts and the first char equal
	if (count[va] == count[vb] && va / MAX_CHARS == vb / MAX_CHARS) return va % MAX_CHARS - vb % MAX_CHARS;
	// sort according to first char if counts equal
	if (count[va] == count[vb]) return va / MAX_CHARS - vb / MAX_CHARS;
	return count[vb] - count[va];
}

// count lines, words & chars in a given text file
void wc(int *nl, int *nw, int *nc) {
    char text[MAX_LINE];
    *nl=*nw=*nc=0;
    while(fgets(text,MAX_LINE,stdin)!=NULL){
        *nl+=1;
        if(text[0]!=' ' && text[0]!='   ' && text[0]!='\n') *nw+=1;
        for(int i=0; i<strlen(text); i++){
            if((text[i]==' ' || text[i]=='   ') && text[i+1]!=' ' && text[i+1]!='\n' && text[i+1]!='    ') *nw+=1;
            *nc+=1;
        }
    }
}

void char_count(int char_no, int *n_char, int *cnt) {
    char text[MAX_LINE], ans[95];
    int t[95]={0}, maks=0, temp[95]={0};
    //t-do count_sorta, temp=posortowana kopia,
    while(fgets(text,MAX_LINE,stdin)!=NULL){
        for(int i=0; i<strlen(text); i++){
            t[text[i]-33]+=1;
        }
    }
    for(int i=0; i<95; i++) temp[i]=t[i];
    qsort(temp,95,sizeof(int),cmp);
    *cnt=temp[95-char_no];
    *n_char=LAST_CHAR;
    for(int i=0; i<95; i++){
        if(t[i]==*cnt){
            if(i<*n_char) *n_char=i;
        }
    }
    *n_char+=33;
}

void bigram_count(int bigram_no, int bigram[]) {
    char text[MAX_LINE]={};
    int t[95*95]={0}, i_pom, maks=0,pom1,pom2,letter_1=LAST_CHAR+1, letter_2=LAST_CHAR+1;
    while(fgets(text,MAX_LINE,stdin)!=NULL){
        for(int i=0; i<strlen(text)-1; i++){
            if ((text[i]>=FIRST_CHAR && text[i]<=LAST_CHAR) && (text[i+1]>=FIRST_CHAR && text[i+1]<=LAST_CHAR))
            {
                t[(text[i]-FIRST_CHAR)*(LAST_CHAR-FIRST_CHAR+1)+(text[i+1]-FIRST_CHAR)]++;
            }
        }
    }
    while(bigram_no>0){
        maks=0;
        for(int i=0; i<95*95; i++){
            //if(t[i]!=0) printf("%c%c ", pom2,pom1,t[i]);
            pom1=(int)(i%95)+FIRST_CHAR;
            pom2=(i-(int)(i%95))/95+FIRST_CHAR;
            //printf("%c%c %d,", pom2,pom1,t[i]);
            if(t[i]>maks){
                i_pom=i;
                maks=t[i];
                letter_1=pom2;
                letter_2=pom1;
            }
        }
        if(bigram_no==1){
            bigram[0]=letter_1;
            bigram[1]=letter_2;
            bigram[2]=maks;
        }
        t[i_pom]=0;
        bigram_no-=1;
    }
}

void find_comments(int *line_comment_counter, int *block_comment_counter) {
    char text[MAX_LINE];
    int l1=0, l2=0,flag=0;

    while(fgets(text,MAX_LINE,stdin)!=NULL){
        for(int i=0; i<strlen(text); i++){
            if(flag==0 && text[i]=='/' && text[i+1]=='/') {
                l1+=1;
                break;
            }
            if(flag==0 && text[i]=='/' && text[i+1]=='*'){
                i+=2;
                flag=1;
            }
            if(flag==1 && text[i]=='*' && text[i+1]=='/'){
                i++;
                flag=0;
                l2+=1;
            }
        }
    }
    *line_comment_counter=l1;
    *block_comment_counter=l2;
}


int read_int() {
	char line[MAX_LINE];
	fgets(line, MAX_LINE, stdin); // to get the whole line
	return (int)strtol(line, NULL, 10);
}

int main(void) {
	int to_do;
	int nl, nw, nc, char_no, n_char, cnt;
	int line_comment_counter, block_comment_counter;
	int bigram[3];

	to_do = read_int();
	switch (to_do) {
		case 1: // wc()
			wc (&nl, &nw, &nc);
			printf("%d %d %d\n", nl, nw, nc);
			break;
		case 2: // char_count()
			char_no = read_int();
			char_count(char_no, &n_char, &cnt);
			printf("%c %d\n", n_char, cnt);
			break;
		case 3: // bigram_count()
			char_no = read_int();
			bigram_count(char_no, bigram);
			printf("%c%c %d\n", bigram[0], bigram[1], bigram[2]);
			break;
		case 4:
			find_comments(&line_comment_counter, &block_comment_counter);
			printf("%d %d\n", block_comment_counter, line_comment_counter);
			break;
		default:
			printf("NOTHING TO DO FOR %d\n", to_do);
			break;
	}
	return 0;
}

