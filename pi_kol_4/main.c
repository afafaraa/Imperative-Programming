#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define BUFF 100

typedef struct{
    char*name;
    char*surname;
    char pesel[12];
}Person;

typedef struct tagListElement{
    struct tagListElement*next;
    Person person;
}ListElement;

typedef struct tagPersonList{
    ListElement*head;
    ListElement*tail;
    int no_elements;
}PersonList;

void init_pl(PersonList*p_pl){
    (*p_pl).head=NULL;
    (*p_pl).tail=NULL;
    (*p_pl).no_elements=0;
}

void free_pl(PersonList*p_pl){
    ListElement*curr=(*p_pl).head;
    while(curr){
        ListElement*temp=curr;
        curr=(*curr).next;
        (*temp).next=NULL;
        free_person(temp);
    }
    (*p_pl).head=NULL;
    (*p_pl).tail=NULL;
    (*p_pl).no_elements=0;
}

void free_person(Person*p){
    free((*p).name);
    free((*p).surname);
    free((*p).pesel);
    free(p);
}

void fill_person_data2(Person *p_p,const char *pesel,const char *name, const char *surname){
    (*p_p).name=malloc(sizeof(char)*strlen(name));
    (*p_p).surname=malloc(sizeof(char)*strlen(surname));
    for(int i=0; i<strlen(name); i++){
        (*p_p).name[i]=name[i];
    }
    for(int i=0; i<strlen(surname); i++){
        (*p_p).surname[i]=surname[i];
    }
    for(int i=0; i<strlen(pesel); i++){
        (*p_p).pesel[i]=pesel[i];
    }
    (*p_p).name[strlen(name)]=NULL;
    (*p_p).surname[strlen(surname)]=NULL;
    dump_person(*p_p);
}

void fill_person_data(Person *p_p,const char *pesel,const char *name, const char *surname){
    (*p_p).name=malloc(sizeof(char)*(strlen(name)+1));
    (*p_p).surname=malloc(sizeof(char)*(strlen(surname)+1));
    strcpy((*p_p).name,name);
    strcpy((*p_p).surname,surname);
    strcpy((*p_p).pesel,pesel);
    //dump_person(*p_p);
}

int add(PersonList*p_pl,const char*pesel,const char*name,const char*surname){
    ListElement*to_add=malloc(sizeof(ListElement));
    fill_person_data(&(*to_add).person,pesel,name,surname);
    //dump_person((*to_add).person);
    if((*p_pl).head==NULL){
        (*p_pl).head=to_add;
        (*p_pl).tail=to_add;
        (*(*p_pl).tail).next=NULL;
        (*p_pl).no_elements=1;
        return 0;
    }
    ListElement*temp=(*p_pl).head;
    while(temp){
        if(strcmp((*to_add).person.pesel,(*temp).person.pesel)==0){
            strcpy((*temp).person.name,name);
            strcpy((*temp).person.surname,surname);
            return 1;
        }
        temp=(*temp).next;
    }
    (*(*p_pl).tail).next=to_add;
    (*p_pl).tail=to_add;
    (*(*p_pl).tail).next=NULL;
    return 0;
}

Person*find(PersonList*p_pl,const char*pesel){
    ListElement*temp=(*p_pl).head;
    while(temp){
        if(strcmp((*temp).person.pesel,pesel)==0){
            return &(*temp).person;
        }
        temp=(*temp).next;
    }
    return NULL;
}

void merge(PersonList*p_target,const PersonList*p_src){
    ListElement*temp=(*p_src).head;
    while(temp){
        add(p_target,(*temp).person.pesel,(*temp).person.name, (*temp).person.surname);
        temp=(*temp).next;
    }
}

void dump_list(PersonList*p_pl){
    ListElement*temp=(*p_pl).head;
    while(temp){
        dump_person((*temp).person);
        temp=(*temp).next;
    }
}

void dump_person(Person p){
    printf("%s %s %s\n",p.name,p.surname,p.pesel);
}

int main()
{
    PersonList person_list;
    int to_do,n;
    char name[BUFF],surname[BUFF],pesel[12];
    Person p;

    scanf("%d",&n);
    init_pl(&person_list);
    for(int i=0; i<n; i++){
        scanf("%d",&to_do);
        switch(to_do){
            case(1):
                scanf("%s%s%s",name,surname,pesel);
                add(&person_list,pesel,name,surname);
            break;
        }

    }
    dump_list(&person_list);
    printf("Hello world!\n");
    return 0;
}
