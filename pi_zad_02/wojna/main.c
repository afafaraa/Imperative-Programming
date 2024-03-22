#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

int player_1[52], player_2[52], cards[52];
int out_1=0, out_2=0, len_1=26, len_2=26;
int rund=0 ,rounds=0, ver=0, flag=0;

void swap (int t[], int i, int k ) {
    int n=8;
    int pom=t[i];
    t[i]=t[k];
    t[k]=pom;
}

void print_table(int t[],int out){
    for(int i=0; i<52; i++){
        if(t[(i+out)%52]!=-1)
            printf("%d ",t[(i+out)%52]);
    }
    printf("\n");
}

int rand_from_interval(int a, int b) {
    return rand()%(b-a+1)+a;
}

void shuffle(void){
    //utworzenie tablicy cards=[0,1,...,51], w tablicach player -1 oznacza puste miejsce
    for(int i=0; i<52; i++){
        cards[i]=i;
        player_1[i]=-1;
        player_2[i]=-1;
    }
    //tasowanie kart (mieszanie cards)
    int k=0;
    for(int i=0; i<51; i++){
        k=rand_from_interval(i,51);
        swap(cards,i,k);
    }
    //rozdanie kart graczom
    for(int i=0; i<26; i++){
        player_1[i]=cards[i];
        player_2[i]=cards[i+26];
    }
}

void add_card_1(int card) {
    player_1[(out_1+len_1)%52]=card;
    len_1++;
}

void add_card_2(int card) {
    player_2[(out_2+len_2)%52]=card;
    len_2++;
}

int del_card_1(){
    int pom=0;
    len_1--;
    pom=player_1[out_1];
    player_1[out_1]=-1;
    out_1=(out_1+1)%52;
    return pom;
}

int del_card_2(){
    int pom=0;
    len_2--;
    pom=player_2[out_2];
    player_2[out_2]=-1;
    out_2=(out_2+1)%52;
    return pom;
}

int war(){
    int idx=0, temp, i;
    while(player_1[(out_1+idx)%52]/4==player_2[(out_2+idx)%52]/4){
        idx+=2;
    }
    if(player_1[(out_1+idx)%52]==-1 || player_2[(out_2+idx)%52]==-1){
        printf("%d ",1);
        printf("%d ",len_1);
        printf("%d ",len_2);
        flag=1;
        return 0;
    }
    if(player_1[(out_1+idx)%52]/4>player_2[(out_2+idx)%52]/4){
        i=idx;
        while(i>=0){
            temp=del_card_1();
            add_card_1(temp);
            i--;
        }
        i=idx;
        while(i>=0){
            temp=del_card_2();
            add_card_1(temp);
            i--;
        }
    }
    else if(player_1[(out_1+idx)%52]/4<player_2[(out_2+idx)%52]/4){
        i=idx;
        while(i>=0){
            temp=del_card_2();
            add_card_2(temp);
            i--;
        }
        i=idx;
        while(i>=0){
            temp=del_card_1();
            add_card_2(temp);
            i--;
        }
    }

}

int game(){
    int temp=0;
    while(len_1!=52 && len_2!=52 && rund<rounds){
        if(player_1[(out_1)%52]/4>player_2[(out_2)%52]/4){
            temp=del_card_1();
            add_card_1(temp);
            temp=del_card_2();
            add_card_1(temp);
        }
        else if(player_1[(out_2)%52]/4<player_2[(out_2)%52]/4){
            temp=del_card_2();
            add_card_2(temp);
            temp=del_card_1();
            add_card_2(temp);
        }
        else if(player_1[(out_2)%52]/4==player_2[(out_2)%52]/4){
            if(ver==0){
                war();
                if(flag==1)
                    break;
            }
            else{
                temp=del_card_1();
                add_card_1(temp);
                temp=del_card_2();
                add_card_2(temp);
            }
        }
        rund+=1;
    }
    if(rund==rounds){
        return 0;
    }
}

int main()
{
    //zmienne
    int seed;
    scanf("%d %d %d", &seed, &ver, &rounds);
	srand((unsigned) seed);

    //rozdanie
    shuffle();
    print_table(player_1,out_1);
    print_table(player_2,out_2);
    //gra
    game();

    if(len_1==52){
        if(ver==1)
            rund--;
        printf("%d %d",2, rund+1);
    }
    else if(len_2==52){
        printf("%d\n",3);
        print_table(player_2, out_2);
    }
    else if(len_1!=52 && len_2!=52 && rund==rounds){
        printf("%d ",0);
        printf("%d ",len_1);
        printf("%d ",len_2);
    }
    print_table(player_1,out_1);
    print_table(player_2,out_2);
    return 0;
}
