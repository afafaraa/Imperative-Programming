#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct {
	int day;
	int month;
	int year;
} Date;

// 1 bsearch2

#define FOOD_MAX  30   // max. number of goods
#define ART_MAX   15   // max. length of name's name + 1
#define RECORD_MAX 40  // max. line length

typedef struct {
	char name[20];
	float price;
	int amount;
	Date valid_date;
} Food;

typedef int (*ComparFp)(const void *, const void *);

// compare dates
int cmp_date(const void *d1, const void *d2){
    Date *temp_d1=(Date*)d1;
    Date *temp_d2=(Date*)d2;
    if((*temp_d1).year>(*temp_d1).year)
        return 1;
    else if((*temp_d1).year<(*temp_d2).year)
        return -1;
    else{
        if((*temp_d1).month>(*temp_d2).month)
            return 1;
        else if((*temp_d1).month<(*temp_d2).month)
            return -1;
        else{
            if((*temp_d1).day>(*temp_d2).day)
                return 1;
            else if((*temp_d1).day<(*temp_d2).day)
                return -1;
            return 0;
        }
    }
    return 0;
}

// compare foods
int cmp(const void *a, const void *b){
    Food *temp_a=(Food*)a;
    Food *temp_b=(Food*)b;
    if((*temp_a).price > (*temp_b).price)
        return 1;
    else if((*temp_a).price < (*temp_b).price)
        return -1;
    else
        return cmp_date(&(*temp_a).valid_date, &(*temp_b).valid_date);
    return 0;
}

// bsearch returning address where to insert new element
void *bsearch2(const void *key, const void *base, size_t nmemb, size_t size, ComparFp compar, char *result){
    int left=0, right=nmemb, mid;
    while(left<=right){
        mid=(left+right)/2;
        //printf("%d %d %d\n",mid,left,right);
        if(compar(base+mid*size,key)==0){
            //printf("%s\n","found");
            *result=1;
            return base+mid*size;
        }
        else if(compar(base+mid*size,key)>0){
            //printf("%s\n","right");
            right=mid-1;
        }
        else if(compar(base+mid*size,key)<0){
            //printf("%s\n","left");
            left=mid+1;
        }
    }
    if(compar(base+mid*size,key)<0)
        return base+(mid+1)*size;
    return base+mid*size;
}

// print goods of given name
void print_art(Food *p, int n, char *art) {
    //printf("%d",n);
    for(int i=0; i<n; i++){
        if(strcmp(p[i].name,art)==0){
            printf("%.2lf %d ", p[i].price, p[i].amount);
            if(p[i].valid_date.day<10){
                printf("%d%i.",0, p[i].valid_date.day);
            }
            else{
                printf("%i.", p[i].valid_date.day);
            }
            if(p[i].valid_date.month<10){
                printf("%d%i.",0, p[i].valid_date.month);
            }
            else{
                printf("%i.", p[i].valid_date.month);
            }
            printf("%i\n", p[i].valid_date.year);
        }
    }
}

// add record to table if absent
Food* add_record(Food *tab, int np, ComparFp compar, Food *new_art, int *n) {
    int result=0;
    int temp=((int)bsearch2(new_art,tab,np,sizeof(Food),cmp,&result)-(int)tab)/sizeof(Food);
    Food*pom=(Food*)bsearch2(new_art,tab,np,sizeof(Food),cmp,&result);
    if(result==0){
        *n+=1;
        for(int i=np; i>temp-1; i-=1){
            tab[i+1]=tab[i];
        }
        tab[temp]=*new_art;
    }
    else{
        //printf("%d %d\n",(*pom).amount,(*new_art).amount);
        (*pom).amount+=(*new_art).amount;
    }
    //printf("%d",*n);
}

// read no lines of data
// calls add_record if sorted = 1 and just adds element if sorted = 0
int read_goods(Food *tab, int no, FILE *stream, int sorted) {
    int result=0, n=0;
    Food art={"",0.0,0,0,0,0};
    char line[RECORD_MAX]={};
    fgets(line,RECORD_MAX,stream);
    sscanf(line,"%s %f %d %i.%i.%i", art.name, &art.price, &art.amount, &art.valid_date.day, &art.valid_date.month, &art.valid_date.year);
    tab[0]=art;
    for(int i=1; i<no; i++){
        result=0;
        Food art={"",0.0,0,0,0,0};
        char line[RECORD_MAX]={};
        fgets(line,RECORD_MAX,stream);
        sscanf(line,"%s %f %d %i.%i.%i", art.name, &art.price, &art.amount, &art.valid_date.day, &art.valid_date.month, &art.valid_date.year);
        add_record(tab,i-1,cmp,&art,&n);
    }
    return n;
}

int cmp_qs(const void *a, const void *b) {
	Food *fa = (Food*)a, *fb = (Food*)b;
	return cmp_date(&fa->valid_date, &fb->valid_date);
}

int cmp_bs(const void *a, const void *b) {
	Date *pd = (Date*)a;
	Food *fb = (Food*)b;
	return cmp_date(pd, &fb->valid_date);
}

// finds the value of goods due on 'curr_date'
float value2(Food *food_tab, size_t n, Date curr_date, int days) {

    time_t  my_time;
    struct tm mytm;
    float val=0;

    mytm.tm_year = curr_date.year - 1900; // 2021
    mytm.tm_mon = curr_date.month; // January
    mytm.tm_mday = curr_date.day+days; // Sunday

    my_time = mktime(&mytm);

    struct tm* ltm = localtime(&my_time);
    //printf("%d %d %d\n",1900 + ltm->tm_year,1 + ltm->tm_mon, ltm->tm_mday);
    curr_date.day=ltm->tm_mday;
    curr_date.month=ltm->tm_mon;
    curr_date.year=1900 + ltm->tm_year;
    //printf(ctime(&my_time));
    //printf("%d %d %d\n",curr_date.day,curr_date.month,curr_date.year);
    for(int i=0; i<n; i++){
        //printf("%d %d %d\n",curr_date.day,curr_date.month,curr_date.year);
        //printf("%d %d %d\n",food_tab[i].valid_date.day,food_tab[i].valid_date.month,food_tab[i].valid_date.year);
        if(food_tab[i].valid_date.day==curr_date.day && food_tab[i].valid_date.month==curr_date.month && food_tab[i].valid_date.year==curr_date.year){
            val+=food_tab[i].price*food_tab[i].amount;
        }
    }
    return val;
}

float value(Food *food_tab, size_t n, Date curr_date, int days) {
    time_t  my_time;
    struct tm mytm={ .tm_year=curr_date.year-1900, .tm_mon=curr_date.month-1, .tm_mday=curr_date.day };
    float val=0;

    mytm.tm_mday+=days;
    mktime(&mytm);
    curr_date.day=mytm.tm_mday;
    curr_date.month=mytm.tm_mon+1;
    curr_date.year=mytm.tm_year+1900;

    for(int i=0; i<n; i++){

        if(food_tab[i].valid_date.day==curr_date.day && food_tab[i].valid_date.month==curr_date.month && food_tab[i].valid_date.year==curr_date.year){
            val+=food_tab[i].price*food_tab[i].amount;
        }
    }
    return val;
}
/////////////////////////////////////////////////////////////////
// 3 Succession

#define CHILD_MAX 20

enum Sex {F, M};
enum BOOL {no, yes};

struct Bit_data {
	enum Sex sex:1;
	enum BOOL in_line:1;
};

typedef struct {
	char *name;
	struct Bit_data bits;
	Date born;
	char *parent;
} Person;

typedef struct {
	char *par_name;
	int index;
} Parent;

const Date primo_date = { 28, 10, 2011 }; // new succession act

int cmp_primo(Person *person1, Person *person2) {
	if (person1->bits.sex == person2->bits.sex) return 0;
	if (person1->bits.sex == F && cmp_date(&person1->born, &primo_date) > 0) return 0;
	if (person2->bits.sex == F && cmp_date(&person2->born, &primo_date) > 0) return 0;
	return person2->bits.sex - person1->bits.sex;
}

// compare persons
int cmp_person(const void *a, const void *b) {
}

int fill_indices_tab(Parent *idx_tab, Person *pers_tab, int size) {
}

void persons_shifts(Person *person_tab, int size, Parent *idx_tab, int no_parents) {
}

int cleaning(Person *person_tab, int n) {
}

void print_person(const Person *p) {
	printf("%s\n", p->name);
}

int create_list(Person *person_tab, int n) {
}

int main(void) {
	Person person_tab[] = {
		{"Charles III", {M, no}, {14, 11, 1948},"Elizabeth II"},
		{"Anne", {F,yes}, {15, 8, 1950},"Elizabeth II"},
		{"Andrew", {M,yes}, {19, 2, 1960},"Elizabeth II"},
		{"Edward", {M,yes}, {10, 3, 1964} ,"Elizabeth II"},
		{"David", {M,yes}, {3, 11, 1961} ,"Margaret"},
		{"Sarah", {F,yes}, {1, 5, 1964}, "Margaret"},
		{"William", {M,yes}, {21, 6, 1982}, "Charles III"},
		{"Henry", {M,yes}, {15, 9, 1984}, "Charles III"},
		{"Peter", {M,yes}, {15, 11, 1977}, "Anne"},
		{"Zara", {F,yes}, {15, 5, 1981}, "Anne"},
		{"Beatrice", {F,yes}, {8, 8, 1988}, "Andrew"},
		{"Eugenie", {F,yes}, {23, 3, 1990}, "Andrew"},
		{"James", {M,yes}, {17, 12, 2007}, "Edward"},
		{"Louise", {F,yes}, {8, 11, 2003}, "Edward"},
		{"Charles", {M,yes}, {1, 7, 1999}, "David"},
		{"Margarita", {F,yes}, {14, 5, 2002}, "David"},
		{"Samuel", {M,yes}, {28, 7, 1996}, "Sarah"},
		{"Arthur", {M,yes}, {6, 5, 2019}, "Sarah"},
		{"George", {M,yes}, {22, 7, 2013}, "William"},
		{"George VI", {M,no}, {14, 12, 1895}, NULL},
		{"Charlotte", {F,yes}, {2, 5, 2015}, "William"},
		{"Louis", {M,yes}, {23, 4, 2018}, "William"},
		{"Archie", {M,yes}, {6, 5, 2019}, "Henry"},
		{"Lilibet", {F,yes}, {4, 6, 2021}, "Henry"},
		{"Savannah", {F,yes}, {29, 12, 2010}, "Peter"},
		{"Isla", {F,yes}, {29, 3, 2012}, "Peter"},
		{"Mia", {F,yes}, {17, 1, 2014}, "Zara"},
		{"Lena", {F,yes}, {18, 6, 2018}, "Zara"},
		{"Elizabeth II", {F,no}, {21, 4, 1925}, "George VI"},
		{"Margaret", {F,no}, {21, 8, 1930}, "George VI"},
		{"Lucas", {M,yes}, {21, 3, 2021}, "Zara"},
		{"Sienna", {F,yes}, {18, 9, 2021}, "Beatrice"},
		{"August", {M,yes}, {9, 2, 2021}, "Eugenie"}
	};

	int to_do, no;
	int n;
	Food food_tab[FOOD_MAX];
	char buff[ART_MAX];
	fgets(buff, ART_MAX, stdin);
	sscanf(buff, "%d", &to_do);

	switch (to_do) {
		case 1:  // bsearch2
			fgets(buff, ART_MAX, stdin);
			sscanf(buff, "%d", &no);
			n = read_goods(food_tab, no, stdin, 1);
			scanf("%s",buff);
			print_art(food_tab, n, buff);
			break;
		case 2: // qsort
			fgets(buff, ART_MAX, stdin);
			sscanf(buff, "%d", &no);
			n = read_goods(food_tab, no, stdin, 0);
			Date curr_date;
			int days;
			scanf("%d %d %d", &curr_date.day, &curr_date.month, &curr_date.year);
			scanf("%d", &days);
			printf("%.2f\n", value(food_tab, (size_t)n, curr_date, days));
			break;
		case 3: // succession
			scanf("%d",&no);
			int no_persons = sizeof(person_tab) / sizeof(Person);
			create_list(person_tab,no_persons);
			print_person(person_tab + no - 1);
			break;
		default:
			printf("NOTHING TO DO FOR %d\n", to_do);
	}
	return 0;
}
