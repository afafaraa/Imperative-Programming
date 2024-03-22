#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFF 100
#define SIZE 50

enum VehicleType{ecar, ebus, etruck};
enum DriveType{eelectric, ehybrid, combustion};

typedef struct Car{
    int max_passangers;
    float engine_power;
}Car;

typedef struct Bus{
    int seats;
    int standing_places;
}Bus;

typedef struct Truck{
    float engine_capacity;
    float axle_load;
}Truck;

typedef struct Vehicle{
    enum VehicleType v_type;
    enum DriveType d_type;

    char owner[BUFF];
    int last_inspection[3];

    union{
        Car car;
        Bus bus;
        Truck truck;
    };
}Vehicle;

void new_owner(Vehicle*pvehicle){
    //printf("gfds");
    char buff[BUFF];
    scanf("%s",buff);
    printf("%s",buff);
    strcpy((*pvehicle).owner, buff);
}

int delayed(Vehicle*vehicles,int size,int*base_date,char***pdelayed_owners){
    int n=0;
    char**delayed_owners=(char**)malloc(sizeof(char*)*size);
    //printf("%d.%d.%d\n",base_date[0],base_date[1],base_date[2]);
    for(int i=0; i<size; i++){
        if(date_cmp(vehicles[i].last_inspection,base_date)<0){
            delayed_owners[n]=(char*)malloc(sizeof(char)*strlen(vehicles[i].owner));
            strcpy(delayed_owners[n],vehicles[i].owner);
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
    if(last_insp[2]<base_date[2])
        return -1;
    else{
        if(last_insp[2]==base_date[2]){
            if(last_insp[1]<base_date[1]){
                return -1;
            }
            else{
                if(last_insp[1]==base_date[1]){
                    if(last_insp[0]<base_date[0]){
                        return -1;
                    }
                }
            }
        }
    }
    return 1;
}

void read_vehicle(Vehicle *vehicle){
    int v_t, d_t;
    scanf("%d %d",&v_t, &d_t);
    (*vehicle).v_type=v_t;
    (*vehicle).d_type=d_t;
    scanf("%d%d%d",&(*vehicle).last_inspection[0],&(*vehicle).last_inspection[1],&(*vehicle).last_inspection[2]);
    scanf("%s",&(*vehicle).owner);
    switch(v_t){
        case(0):
            scanf("%f",&(*vehicle).car.engine_power);
            scanf("%d",&(*vehicle).car.max_passangers);
        break;
        case(1):
            scanf("%d",&(*vehicle).bus.seats);
            scanf("%d",&(*vehicle).bus.standing_places);
        break;
        case(2):
            scanf("%f",&(*vehicle).truck.axle_load);
            scanf("%f",&(*vehicle).truck.engine_capacity);
        break;
    }
}

void add_to_vehicles(Vehicle vehicles[],Vehicle v,int *size){
    vehicles[*size]=v;
    *size+=1;
}

void dump_vehicle(Vehicle vehicle){
    printf("v_type: %d, d_type: %d, last inspection: %d.%d.%d\n",vehicle.v_type,vehicle.d_type,vehicle.last_inspection[0],vehicle.last_inspection[1],vehicle.last_inspection[2]);
    printf("owner: %s\n",&vehicle.owner);
    switch(vehicle.v_type){
        case(0):
            printf("%f, ",vehicle.car.engine_power);
            printf("%d\n",vehicle.car.max_passangers);
        break;
        case(1):
            printf("%d, ",vehicle.bus.seats);
            printf("%d\n",vehicle.bus.standing_places);
        break;
        case(2):
            printf("%f, ",vehicle.truck.axle_load);
            printf("%f\n",vehicle.truck.engine_capacity);
        break;
    }
}

void dump_char_array(char**t,int size){
    for(int i=0 ;i<size; i++){
        printf("%s\n",t[i]);
    }
}

void dump_vehicle_list(Vehicle *vehicles,int size){
    for(int i=0; i<size; i++){
        dump_vehicle(vehicles[i]);
    }
}

int main(){
    Vehicle vehicles[SIZE];
    int size=0,to_do,n,own_no=1,delayed_no=0;
    int base_date[3];
    char ***pdelayed_owners;
    scanf("%d",&n);
    Vehicle v;
    //char***pdelayed_owners;

    for(int i=0; i<n; i++){
        scanf("%d",&to_do);
        switch(to_do){
            case(1):
                read_vehicle(&v);
                add_to_vehicles(vehicles,v,&size);
            break;
            case(2):
                //scanf("%d",&own_no);
                new_owner(&vehicles[own_no]);
            break;
            case(3):
                pdelayed_owners=(char***)malloc(sizeof(char**));
                pdelayed_owners[0]=(char**)malloc(sizeof(char*)*(size+1));
                scanf("%d %d %d",&base_date[0],&base_date[1],&base_date[2]);
                //printf("%d %d %d\n",base_date[0],base_date[1],base_date[2]);
                delayed_no=delayed(vehicles,size,base_date,pdelayed_owners);
                printf("%d\n",delayed_no);
                dump_char_array(pdelayed_owners[0],delayed_no);
            break;
            case(4):
                dump_vehicle_list(vehicles,size);
            break;
        }

    }
    return 0;
}
