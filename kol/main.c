typedef struct{
    char vehicle_ID[15];
    int places : 4;
    int driving_vehicles:2;
    int towbar : 1;
}Car_data;

union{
    Car_data cs;
    Truck_data ts;
}veh_union;

int delayed(const Vehicle*tv,int size,const Date*base_date,char***pdelayed_owners){
    int n=0;
    char**delayed_owners=(char**)malloc(sizeof(char*)*size);
    //printf("%d.%d.%d\n",base_date[0],base_date[1],base_date[2]);
    for(int i=0; i<size; i++){
        if(date_cmp(tv[i].valid_date,base_date)<0){
            delayed_owners[n]=(char*)malloc(sizeof(char)*strlen(vehicles[i].owner));
            strcpy(delayed_owners[n],tv[i].owner);
            n+=1;
        }
    }
    for(int i=0; i<n; i++){
        pdelayed_owners[0][i]=(char*)malloc(sizeof(char)*strlen(delayed_owners[i]));
        strcpy(pdelayed_owners[0][i],delayed_owners[i]);
    }
    return n;
}
int date_cmp(int *last_insp,int *base_date){
    if((*last_insp).y<(*base_date).y)
        return -1;
    else{
        if((*last_insp).y==(*base_date).y){
            if((*last_insp).m<(*base_date).m){
                return -1;
            }
            else{
                if((*last_insp).m==(*base_date).m){
                    if((*last_insp).d<(*base_date_.d){
                        return -1;
                    }
                }
            }
        }
    }
    return 1;
}

void print_owners(const char**t_owners,size_t size){
    for(int i=0; i<size; i++){
        printf("%s\n",t_owners[i]);
    }
}

Vehicle*create_vehicle(void){
    Vehicle*v=malloc(sizeof(Vehicle));
    (*v).owner=NULL;
    (*v).valid_date.d=1;
    (*v).valid_date.m=1;
    (*v).valid_date.y=1970;
    (*v).v_type=v_ndef;
}

void print_v_data(const Vehicle*v){
    if((*v).v_type==n_def){
        return;
    }
    printf("Car: ID: %s, miejsc: %d, naped na: %d kola, hack: %d",(*v).owner, (*v).cs.places, (*v).cs.driving_vehicle, (*v).tobar);
}

int main(){
    return 0;
}

typedef struct{
    char vehicle_ID[15];
    int places : 4;
    int driving_vehicles:2;
    int towbar : 1;
}Car_data;

union{
    Car_data cs;
    Truck_data ts;
}veh_union;

int date_cmp(int *vd,int *base_date){
    if((*vd).y<(*base_date).y)
        return -1;
    else{
        if((*v).y=d=(*base_date).y){
            if((*vd).m<(*base_date).m){
                return -1;
            }
            else{
                if((*vd).m==(*base_date).m){
                    if((*vd).d<(*base_date).d){
                        return -1;
                    }
                }
            }
        }
    }
    return 1;
}

int delayed(const Vehicle*tv,int size,const Date*base_date,char***pdelayed_owners){
    int n=0;
    char**delayed_owners=(char**)malloc(sizeof(char*)*size);
    for(int i=0; i<size; i++){
        if(date_cmp(tv[i].valid_date,base_date)<0){
            delayed_owners[n]=(char*)malloc(sizeof(char)*strlen(vehicles[i].owner));
            strcpy(delayed_owners[n],tv[i].owner);
            n+=1;
        }
    }
    for(int i=0; i<n; i++){
        pdelayed_owners[0][i]=(char*)malloc(sizeof(char)*strlen(delayed_owners[i]));
        strcpy(pdelayed_owners[0][i],delayed_owners[i]);
    }
    return n;
}

Vehicle*create_vehicle(void){
    Vehicle*v=malloc(sizeof(Vehicle));
    (*v).owner=NULL;
    (*v).valid_date.d=1;
    (*v).valid_date.m=1;
    (*v).valid_date.y=1970;
    (*v).v_type=v_ndef;
}
void print_owners(const char**t_owners,size_t size){
    for(int i=0; i<size; i++){
        printf("%s\n",t_owners[i]);
    }
}

void print_v_data(const Vehicle*v){
    if((*v).v_type==n_def){
        return;
    }
    printf("Car: ID: %s, miejsc: %d, naped na: %d kola, hack: %d",(*v).owner, (*v).cs.places, (*v).cs.driving_vehicle, (*v).tobar);
}

void dump_vehicle(const Vehicle *v){
    printf("v_type: %d, d_type: %d, last inspection: %d.%d.%d\n",(*v).owne,(*v).d_type,vehicle.valid_date[0])
    switch(v.v_type){
        case(0):
             printf("Car: ID: %s, miejsc: %d, naped na: %d kola, hack: %d",(*v).owner, (*v).cs.places, (*v).cs.driving_vehicle, (*v).tobar);
        break;
        case(1):

        break;
        case(2):

        break;
    }
}
