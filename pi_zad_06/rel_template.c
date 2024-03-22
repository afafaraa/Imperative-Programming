#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_REL_SIZE 100
#define MAX_RANGE 100

typedef struct {
	int first;
	int second;
} pair;

// Case 1:

// The relation R is reflexive if xRx for every x in X
int is_reflexive(pair *t, int n){
    int maks=0,min=101;
    for(int i=0; i<n; i++){
        if(t[i].first>maks)
            maks=t[i].first;
        if(t[i].second>maks)
            maks=t[i].second;
        if(t[i].first<min)
            min=t[i].first;
        if(t[i].second<min)
            min=t[i].second;
    }

    int check[maks];
    for(int i=0; i<maks; i++) check[i]=0;
    for(int i=0; i<n; i++){
        if(t[i].first==t[i].second){
            check[t[i].first]=1;
        }
    }
    for(int i=min; i<maks; i++)
        if(check[i]==0) return 0;
    return 1;
}
// The relation R on the set X is called irreflexive
// if xRx is false for every x in X
int is_irreflexive(pair *t, int n){
    int maks=0,min=101;
    for(int i=0; i<n; i++){
        if(t[i].first>maks)
            maks=t[i].first;
        if(t[i].second>maks)
            maks=t[i].second;
        if(t[i].first<min)
            min=t[i].first;
        if(t[i].second<min)
            min=t[i].second;
    }
    int check[maks];
    for(int i=0; i<maks; i++) check[i]=0;
    for(int i=0; i<n; i++){
        if(t[i].first==t[i].second){
            check[t[i].first]=1;
        }
    }
    for(int i=min; i<=maks; i++)
        if(check[i]==1) return 0;
    return 1;
}

// A binary relation R over a set X is symmetric if:
// for all x, y in X xRy <=> yRx
int is_symmetric(pair *t, int n){
    int x=0,y=0,flag=0;
    for(int i=0; i<n; i++){
        x=t[i].first;
        y=t[i].second;
        flag=0;
        for(int j=0; j<n; j++){
            if(t[j].first==y && t[j].second==x)
                flag=1;
        }
        if(flag==0)
            return 0;
    }
    return 1;
}

// A binary relation R over a set X is antisymmetric if:
// for all x,y in X if xRy and yRx then x=y
int is_antisymmetric(pair *t, int n){
    int x=0,y=0,flag=0;
    for(int i=0; i<n; i++){
        x=t[i].first;
        y=t[i].second;
        flag=0;
        for(int j=0; j<n; j++){
            if(t[j].first==y && t[j].second==x)
                flag=1;
        }
        if(flag==1 && t[i].first!=t[i].second)
            return 0;
    }
    return 1;
}

// A binary relation R over a set X is asymmetric if:
// for all x,y in X if at least one of xRy and yRx is false
int is_asymmetric(pair *t, int n){
    int x=0,y=0;
    for(int i=0; i<n; i++){
        x=t[i].first;
        y=t[i].second;
        for(int j=0; j<n; j++){
            if(t[j].first==y && t[j].second==x)
                return 0;
        }
    }
    return 1;
}

// A homogeneous relation R on the set X is a transitive relation if:
// for all x, y, z in X, if xRy and yRz, then xRz
int is_transitive(pair *t, int n){
    int x=0,y=0,z=0,flag=0;
    for(int i=0; i<n; i++){
        x=t[i].first;
        y=t[i].second;
        for(int j=0; j<n; j++){
            if(t[j].first==y){
                z=t[j].second;
                flag=0;
                for(int k=0; k<n; k++){
                    if(t[k].first==x && t[k].second==z)
                        flag=1;
                }
                if(flag==0) return 0;
            }
        }
    }
    return 1;
}

// Case 2:

// A partial order relation is a homogeneous relation that is
// reflexive, transitive, and antisymmetric
int is_partial_order(pair *t, int n){
    if(is_reflexive(t,n)==1 && is_antisymmetric(t,n)==1 && is_transitive(t,n)==1)
        return 1;
    return 0;
}

int is_connected(pair *t, int n){
    int maks=0,flag=0;
    for(int i=0; i<n; i++){
        if(t[i].first>maks)
            maks=t[i].first;
        if(t[i].second>maks)
            maks=t[i].second;
    }
    for(int i=0; i<maks; i++){
        for(int j=0; j<maks; j++){
            if(j!=i){
                flag=0;
                for(int k=0; k<n; k++){
                    if((t[k].first==i && t[k].second==j) || (t[k].first==j && t[k].second==i))
                        flag=1;
                }
                if(flag==0)
                    return 0;
            }
        }
    }
    return 1;
}

// A total order relation is a partial order relation that is connected
int is_total_order(pair *t, int n){
    if(is_reflexive(t,n)==1 && is_antisymmetric(t,n)==1 && is_transitive(t,n)==1 && is_connected(t,n)==1)
        return 1;
    return 0;
}

// Relation R is connected if for each x, y in X:
// xRy or yRx (or both)


int find_max_elements(pair *t, int n, int *max_el){
    int domain[MAX_RANGE];
    for(int i=0; i<MAX_RANGE; i++) {domain[i]=0; max_el[i]=0;}
    int g=0,x=0,idx=0,flag=0,maks=get_domain(t,n,domain);
    for(int i=0; i<MAX_RANGE; i++){
        if(domain[i]==1){
            g=i;
            flag=0;
            for(int j=0; j<n; j++){
                if(t[j].first==g && t[j].second!=g){
                    flag=1;
                }
            }
            if(flag==0){
                max_el[g]=1;
                idx+=1;
            }
        }
    }
    return idx;
}

int find_min_elements(pair *t, int n, int *min_el){
    int domain[MAX_RANGE];
    for(int i=0; i<MAX_RANGE; i++) {domain[i]=0; min_el[i]=0;}
    int g=0,x=0,idx=0,flag=0,maks=get_domain(t,n,domain);
    for(int i=0; i<MAX_RANGE; i++){
        if(domain[i]==1){
            g=i;
            flag=0;
            for(int j=0; j<n; j++){
                if(t[j].second==g && t[j].first!=g){
                    flag=1;
                }
            }
            if(flag==0){
                min_el[g]=1;
                idx+=1;
            }
        }
    }
    return idx;
}

int get_domain(pair *t,int n, int *domain){
    int cnt=0;
    for(int i=0; i<MAX_REL_SIZE; i++) domain[i]=0;
    for(int i=0; i<n; i++){
        domain[t[i].first]=1;
        domain[t[i].second]=1;
    }
    for(int i=0; i<MAX_RANGE; i++)
        if(domain[i]==1) cnt+=1;
    return cnt;
}

// Case 3:

int composition (pair *t1, int n1, pair *t2, int n2, pair *ans){
    int x1=0,x2=0, y1=0,y2=0, idx=0, flag=0;
    for(int i=0; i<n1; i++){
        x1=t1[i].first;
        y1=t1[i].second;
        for(int j=0; j<n2; j++){
            x2=t2[j].first;
            y2=t2[j].second;
            if(y1==x2){
                flag=0;
                for(int k=0; k<idx; k++){
                    if(ans[k].first==x1 && ans[k].second==y2)
                        flag=1;
                }
                if(flag==0){
                    ans[idx].first=x1;
                    ans[idx].second=y2;
                    idx+=1;
                }
            }
        }
    }
    return idx;
}

// Comparator for pair
int cmp_pair (const void *a, const void *b) {
}

int insert_int (int *tab, int n, int new_element) {
}

// Add pair to existing relation if not already there

// Read number of pairs, n, and then n pairs of ints
int read_relation(pair *relation) {
    int n=0, x=0, y=0, flag=0, idx=0;
    scanf("%d",&n);
    for(int i=0; i<n; i++){
        scanf("%d",&x);
        scanf("%d",&y);
        flag=0;
        for(int j=0; j<idx; j++){
            if(relation[i].first==x && relation[i].second==y){
                flag=1;
            }
        }
        if(flag==0){
            relation[idx].first=x;
            relation[idx].second=y;
            idx+=1;
        }
    }
    return n;
}

void print_int_array(int *array, int n) {
    printf("%d\n",n);
    for(int i=0; i<MAX_RANGE; i++){
        if(array[i]==1){
            printf("%d ",i);
        }
    }
    printf("\n");
}

void print_array(pair *t,int n){
    for(int i=0; i<n; i++){
        printf("%d %d\n",t[i].first, t[i].second);
    }
}

int main(void) {
	int to_do;
	pair relation[MAX_REL_SIZE];
	pair relation_2[MAX_REL_SIZE];
	pair comp_relation[MAX_REL_SIZE];
	int domain[MAX_REL_SIZE];
	int max_elements[MAX_REL_SIZE];
	int min_elements[MAX_REL_SIZE];

	scanf("%d",&to_do);
	int size = read_relation(relation);
	int ordered, size_2, n_domain;


	switch (to_do) {
		case 1:
			printf("%d ", is_reflexive(relation, size));
			printf("%d ", is_irreflexive(relation, size));
			printf("%d ", is_symmetric(relation, size));
			printf("%d ", is_antisymmetric(relation, size));
			printf("%d ", is_asymmetric(relation, size));
			printf("%d\n", is_transitive(relation, size));
			break;
		case 2:
			ordered = is_partial_order(relation, size);
			n_domain = get_domain(relation, size, domain);
			printf("%d %d\n", ordered, is_total_order(relation, size));
			print_int_array(domain, n_domain);
			if (!ordered) break;
			int no_max_elements = find_max_elements(relation, size, max_elements);
			int no_min_elements = find_min_elements(relation, size, min_elements);
			print_int_array(max_elements, no_max_elements);
			print_int_array(min_elements, no_min_elements);
			break;
		case 3:
			size_2 = read_relation(relation_2);
			printf("%d\n", composition(relation, size, relation_2, size_2, comp_relation));
			break;
		default:
			printf("NOTHING TO DO FOR %d\n", to_do);
			break;
	}
	return 0;
}

