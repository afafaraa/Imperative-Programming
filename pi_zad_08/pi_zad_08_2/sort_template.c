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
int cmp_date(const void *d1, const void *d2) {
}

// compare foods
int cmp(const void *a, const void *b) {
}

int cmp_names(char str1[], char str2[]){
    int n_1=strlen(str1),n_2=strlen(str2);
    if(n_1==n_2){
        for(int i=0; i<n_1; i++){
            if(str1[i]>str2[i]){
                return 1;
            }
            else if(str1[i]<str2[i]){
                return 2;
            }
        }
        return 0;
    }
    if (n_1>n_2){
        for(int i=0; i<n_2; i++){
            if(str1[i]>str2[i]){
                return 1;
            }
            else if(str1[i]<str2[i]){
                return 2;
            }
        }
        return 1;
    }
    if (n_1<n_2){
        for(int i=0; i<n_1; i++){
            if(str1[i]>str2[i]){
                return 1;
            }
            else if(str1[i]<str2[i]){
                return 2;
            }
        }
        return 2;
    }
}
// bsearch returning address where to insert new element
int bsearch2 (Food key, Food base[], int n, int size, int *res) {
    int left=0, right=size-1, mid=0, temp;
    while(left<=right){
        printf("%d %d\n", left,right);
        mid=(int)(right+left)/2;
        temp=cmp_names(key.name, base[mid].name);
        if(temp==0){
            *res=1;
            return mid;
        }
        else if(temp==1){
            left=mid+1;
        }
        else{
            right=mid-1;
        }
    }
    return left;
}

// print goods of given name
void print_art(Food *p, int n, char *art) {
}

void add_record(Food tab[], int size, int n, Food art, int idx){
    for(int i=n; i>=idx; i--){
        tab[i+1]=tab[i];
    }
    tab[idx]=art;
}
// read no lines of data
// calls add_record if sorted = 1 and just adds element if sorted = 0
int read_goods(Food tab[], int no, FILE *stream) {
    int r=0, temp;
    int *p=&r;
    char name[20]={};
    int price=0, amount=0, day=0, month=0, year=0;
    for(int i=0; i<no; i++){
        r=0;
        Food art={"",0.0,0,0,0,0};
        char line[RECORD_MAX]={};
        fgets(line,RECORD_MAX,stream);
        sscanf(line,"%s %f %d %i.%i.%i", name, &price, &amount, &day, &month &year);
        temp=bsearch2(art,tab,FOOD_MAX,i,&r);
        if(r==1){
            tab[temp].amount+=art.amount;
        }
        else{
            add_record(tab,FOOD_MAX,i,art,temp);
        }
    }
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
float value(Food *food_tab, size_t n, Date curr_date, int days) {
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
			n = read_goods(food_tab, no, stdin);
			scanf("%s",buff);
			print_art(food_tab, n, buff);
			break;
		case 2: // qsort
			fgets(buff, ART_MAX, stdin);
			sscanf(buff, "%d", &no);
			n = read_goods(food_tab, no, stdin);
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
        {
            Food a;
            int r=0;
            int *p=&r;
            memcpy(a.name,"mid",3);
            a.amount=1;
		    Food tab[FOOD_MAX]={};
		    tab[0].amount=4;
		    memcpy(tab[0].name,"dfghj",5);
		    tab[1].amount=41;
		    memcpy(tab[1].name,"kefir",5);
		    tab[2].amount=6;
		    memcpy(tab[2].name,"mid",3);
		    printf("%d\n",bsearch2(a,tab,FOOD_MAX,3,&r));
		    printf("%d\n",r);
		    //printf("%d\n",cmp_names("yubdysuiof","fdsvkafh"));
			printf("NOTHING TO DO FOR %d\n", to_do);
        }
	}
	return 0;
}
