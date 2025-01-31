// CS_PET_SALON
//
// This program was written by Haiqa Qazi z5590411
// on 30/07/2024
//
// Works like a pet salon manager by creating rooms and salons to 
// take care of different type of pets. In the rooms, they can
// be taken care of. 


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

////////////////////////////////////////////////////////////////////////////////
///////////////////////////      Constants       ///////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// provided constants
#define MAX_NAME_LEN 20
#define INVALID_PET_TYPE -1
#define CAPACITY 10

// all different types of pets that can be found in rooms
enum pet_type {
    CAT,
    DOG,
    RABBIT,
    PARROT,
};

// added constants

#define CAT_COST 11.5
#define DOG_COST 15.5
#define PARROT_COST 7.6
#define RABBIT_COST 5.5

////////////////////////////////////////////////////////////////////////////////
//////////////////////////  USER DEFINED TYPES  ////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// finance of the salon
struct financial_summary {
    int total_cared;
    double total_profit;
};

// salon overview
struct salon {
    char salon_name[MAX_NAME_LEN];
    struct financial_summary summary;
    double base_cost;
    struct pet_room *rooms;
    struct salon *next;
};
 
// room that can be found in salon
struct pet_room {
    char room_name[MAX_NAME_LEN];
    enum pet_type pet_type;
    int num_pets; 
    struct pet_room *next;
};

// TODO: Put any additional structs here!


////////////////////////////////////////////////////////////////////////////////
////////////////////////// FUNCTION PROTOTYPES  ////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

struct salon *create_salon(char salon_name[MAX_NAME_LEN], double base_cost);
struct pet_room *create_room(char room_name[MAX_NAME_LEN], 
                                enum pet_type pet_type);
void print_one_room(int position, struct pet_room *room);
void print_pet_type(enum pet_type pet_type);
void print_salon_stats(struct salon *salon, int selected);
void print_total_stats(double profit, int pets);
void print_added_pets(int num_cats, int num_dogs, int num_rabbits, 
                        int num_parrots);

void scan_name(char string[MAX_NAME_LEN]);
enum pet_type scan_pet_type();


// You won't need to be calling the below functions. They are used to help
// you implement some of the provided helper functions!
int scan_token(char *buffer, int buffer_size);
enum pet_type string_to_pet_type(char *pet_type);

// TODO: Put your function prototypes here!

void do_c(struct salon *selected_position, char new_salon_name[20], double base_cost, struct salon *head);
void do_a(char name[20], char pet[10], struct salon *selected_position, struct salon *head);
void do_p(struct salon *selected_position);
void do_i(char name[20], char pet[10], struct salon *selected_position);
int check_room(struct salon *selected_position, struct pet_room *this_room);
void do_m(struct salon *selected_position, char pet[10], int amount);
int check_limit(struct pet_room *room);
void do_h(struct salon *selected_position);
void do_s(struct salon *selected_position, char name[20], char new_name[20]);
void check_odd(struct pet_room *selected_position, struct pet_room *new_room);
void do_f(struct salon *room, char name[20], int amount_cared);
double do_pet_cost (struct pet_room *current_room);
struct salon *do_back (struct salon *selected_position, struct salon *head);
int check_salon(struct salon *selected_position, struct pet_room *this_room, struct salon *head);
void do_d(struct salon *selected_position, struct salon *head);
void do_r(char name[20], struct salon *selected_salon);
struct salon *do_q(struct salon *selected_salon, struct salon *head);
void free_salon(struct salon *selected_salon);
void free_room(struct pet_room *room);
void do_o(struct salon *selected_salon);
int checking_name (struct salon *selected_salon, struct pet_room *head);
struct salon *do_x(struct salon *selected_salon, struct salon *head, char combined_salon[20], char name[20], char other_salon[20]);
void command(char input[100], struct salon *selected_position, char new_salon_name[20], double base_cost, struct salon *head, char name[20], char pet[10], int amount, char new_name[20], int amount_cared);
int is_Named_Salon_At_Head(struct salon *head, char name[20]);
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

int main(void)
{
    printf("Welcome to 1511 CS Pet Salon manager! =^.^=\n");

    char salon_name[MAX_NAME_LEN] = "cs_salon";
    struct salon *head = create_salon(salon_name, 10.2);
    struct salon *selected_position = head;
    char new_salon_name[20];
    char name[20];
    char pet[10];
    int amount = 0;
    char new_name[20];
    int amount_cared = 0;
    double base_cost = 0;
    char combined_salon[20];
    char other_salon[20];

    while(1 == 1 ){
        char input[100];
        input[0] = 'b';

        printf("Enter command: ");

        scan_name(input);

        if(input[0] == 'b') {
            break;
        }

        command(input, selected_position, new_salon_name, base_cost, head, name, pet, amount, new_name, amount_cared);

        if(input[0] == '<') {
            selected_position = do_back(selected_position, head);
        }

        if(input[0] == '>') {
            selected_position = selected_position -> next;
            printf("You have now selected salon: %s!\n", selected_position ->salon_name);
        }


        if(input[0] == 'q'){
            if(selected_position == head) {
                struct salon *temp = head -> next;
                selected_position = do_q(selected_position, head);

                if(temp != NULL){
                    head = temp;
                }
            }

            else{
                selected_position = do_q(selected_position, head);
            }
        }

        if(input[0] == 'x'){
            int reassignHead = 0;
            scan_name(combined_salon);
            scan_name(name);
            scan_name(other_salon);
            if((is_Named_Salon_At_Head(head, name) == 1)|| (is_Named_Salon_At_Head(head, other_salon) == 1)){
                reassignHead = 1;
            }
            selected_position = do_x(selected_position, head, combined_salon, name, other_salon);
            if(reassignHead == 1) {
                head = selected_position;
            }
            
        }
    }

    struct salon *current_salon = head;
    while (current_salon != NULL) {
        struct salon *next_salon = current_salon->next;
        free_salon(current_salon);
        current_salon = next_salon;
    }

    selected_position = NULL; 
    printf("\nAll pet salons closed! =^.^=\n");
    
    return 0;
}



////////////////////////////////////////////////////////////////////////////////
/////////////////////////////  FUNCTIONS  //////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

 void command(char input[100], struct salon *selected_position, char new_salon_name[20], double base_cost, struct salon *head, char name[20], char pet[10], int amount, char new_name[20], int amount_cared)
 {
        if (input[0] == 'c') {
            do_c(selected_position, new_salon_name, base_cost, head);
        }

        if (input[0] == 'a') {
            do_a(name, pet, selected_position, head);
        }

        if (input[0] == 'p') {
            do_p(selected_position);
        }

        if (input[0] == 'i') {
            do_i(name, pet, selected_position);
        }

        if (input[0] == 'm') {
            do_m(selected_position, pet, amount);
        }

        if (input[0] == 'h') {
            do_h(selected_position);
        }

        if (input[0] == 's') {
            do_s(selected_position, name, new_name);
        }

        if (input[0] == 'f') {
            do_f(selected_position, name, amount_cared);
        }

        if(input[0] == 'd'){
            do_d(selected_position, head);
        }

        if(input[0] == 'r'){
            do_r(name, selected_position);
        }

        if(input[0] == 'o'){
            do_o(selected_position);
        }
    }
// TODO: what does this function do?
// 
// Parameters:
//      TODO: explain what your parameters are here!
// Returns:
//      TODO: explain what your function returns here!
struct salon *create_salon(char salon_name[MAX_NAME_LEN], double base_cost) {

    // TODO: STAGE 1.1
    
    // TODO: malloc, initialise, and return a new salon.
    
    struct salon *salon_ptr = malloc(sizeof(struct salon));
    
    salon_ptr -> summary.total_cared = 0;
    salon_ptr -> summary.total_profit =0;

    strcpy(salon_ptr -> salon_name, salon_name);
    salon_ptr -> base_cost = base_cost; 
    
    salon_ptr -> next = NULL;
    salon_ptr ->rooms = NULL;

    return salon_ptr;
}

// TODO: what does this function do?
//
// Paramters:
//      TODO: explain what your parameters are here!
// Returns:
//      TODO: explain what your function returns here!
struct pet_room *create_room(char room_name[MAX_NAME_LEN], 
                                enum pet_type pet_type) {

    // STAGE 1.1
    // TODO: malloc, initialise, and return a new room.
    
    // hint: you will have to replace NULL in this return statement.
    
    struct pet_room *new_room = malloc(sizeof(struct pet_room));
    
    strcpy(new_room -> room_name, room_name);
    new_room ->pet_type = pet_type;

    new_room -> next = NULL;
    new_room -> num_pets = 0;

    return new_room;
}

// TODO: Put your additional functions here!
void do_c(struct salon *selected_position, char new_salon_name[20], double base_cost, struct salon *head ){
    scan_name(new_salon_name);
    scanf("%lf", &base_cost);

    struct salon *name_check = selected_position;

    while(name_check != NULL){
        if(strcmp(name_check ->salon_name, new_salon_name) == 0){
            printf("Error: Salon name already exists!\n");
            return;
        }
        name_check = name_check ->next;
    }

    if(base_cost <0 || base_cost > 40){
        printf("Error: Cost needs to within $0-$40!\n");
        return;
    }
    
    struct salon *this_salon = create_salon(new_salon_name, base_cost);
    
    struct salon *current = head;

    while((current -> next) != NULL && strcmp(current ->next->salon_name, new_salon_name)< 0){
        current = current -> next;
    }

    if(current -> next != NULL){
        struct salon *temp = current -> next;
        current -> next  = this_salon;
        this_salon ->next =  temp; 
    }

    else{
        current -> next  = this_salon;
    }


    printf("Success! The salon: %s has been created!\n", new_salon_name);
}

void do_a(char name[20], char pet[10], struct salon *selected_position, struct salon *head)
{
    scan_name(name);

    
    enum pet_type this_pet = scan_pet_type(pet);
    struct pet_room *this_room = create_room(name, this_pet);



    if (selected_position->rooms == NULL)
    {
        if(this_room ->pet_type == INVALID_PET_TYPE){
            printf("Error: Unfortunately, this salon does not cater for this pet type!\n");
            return;
        }
        
        if(check_salon(selected_position, this_room, head) == 0){
            return;
        }

        selected_position->rooms = this_room;
    }

    else{

        if(check_room(selected_position, this_room) == 0){
            return;
        }

        if(check_salon(selected_position, this_room, head) == 0){
            return;
        }

        struct pet_room *current = selected_position -> rooms;

        while((current -> next) != NULL){
            current = current -> next;
        }

        current -> next  = this_room;
    }


    printf("Room: '%s' added!", name);
    printf("\n");
}

int check_room(struct salon *selected_position, struct pet_room *this_room)
{
    struct pet_room *check = selected_position -> rooms; 


    if(check -> next == NULL){

        if(strcmp(this_room -> room_name, check->room_name) == 0){
            printf("Error: This room name already exists!\n");
    
            return 0;
        }

    }

    while((check -> next) != NULL){

        if(strcmp(this_room -> room_name, check->room_name) == 0){
            printf("Error: This room name already exists!\n");
    
            return 0;
        }

        
        check = check -> next;
    }
    
    if(this_room ->pet_type == INVALID_PET_TYPE){

        printf("Error: Unfortunately, this salon does not cater for this pet type!\n");
        return 0;
    }

    return 1;

}


int check_salon(struct salon *selected_position, struct pet_room *this_room, struct salon *head){
    struct salon *check = head; 

    if(check == selected_position){
        return 1;
    }

    while(check != NULL){
        struct pet_room *checking_room = head -> rooms;

        if(check == selected_position){
            check = check -> next;
            continue; 
        }
        while(checking_room != NULL){
            if(strcmp(this_room ->room_name,checking_room -> room_name) == 0){
                printf("Error: This room name already exists!\n");
                return 0;
            }

            checking_room = checking_room -> next;
        }

        check = check -> next; 
    }

    return 1;
}


void do_p(struct salon *selected_position){
    int counter = 1;

    if(selected_position-> rooms == NULL){
        printf("There are no pet rooms in this salon!\n");
        return;
    }

    struct pet_room *current = selected_position -> rooms; 

    while(current != NULL){
        print_one_room(counter, current);
        current = current -> next;
        counter ++;
    }


    printf("All the rooms listed above are in salon '%s'.\n", selected_position->salon_name);
}

void do_i(char name[20], char pet[10], struct salon *selected_position){

    scan_name(name);

    
    enum pet_type this_pet = scan_pet_type(pet);
    struct pet_room *this_room = create_room(name, this_pet);



    if (selected_position->rooms == NULL)
    {
        if(this_room ->pet_type == INVALID_PET_TYPE){
            printf("Error: Unfortunately, this salon does not cater for this pet type!\n");
            return;
        }
        selected_position->rooms = this_room;
    }

    else{

        if(check_room(selected_position, this_room) == 0){
            return;
        }

        struct pet_room *current = selected_position -> rooms;

        while((current -> next) != NULL && (current-> pet_type != this_room ->pet_type) ){
            current = current -> next;
        }

        struct pet_room *temp  = current -> next; 
        current -> next  = this_room;
        this_room -> next = temp;
    }


    printf("Room: '%s' inserted!", name);
    printf("\n");
}

void do_m(struct salon *selected_position, char pet[10], int amount){

    enum pet_type this_pet = scan_pet_type(pet);
    scanf("%d", &amount);

    if(this_pet == INVALID_PET_TYPE){
        printf("Error: Unfortunately, this salon does not cater for this pet type!\n");
        return;
    }
    
    if(amount <=0){
        printf("Error: Amount of pets must be greater than zero!\n");
        return;
    }

    struct pet_room *walk = selected_position -> rooms; 

    if(walk == NULL){
        printf("Error: There is not enough space to add that many pets!\n");
    }

    while(walk != NULL && walk->pet_type != this_pet){
        walk = walk ->next;
    }

    if(walk == NULL || walk -> pet_type != this_pet){
        printf("Error: There is not enough space to add that many pets!\n");
        return;
    }
    walk->num_pets = amount + walk->num_pets;

    while(check_limit(walk) == 0){
        
        int difference = 0;
        difference = walk -> num_pets - 10;
        walk -> num_pets = walk -> num_pets - difference; 
        walk = walk -> next;  

        while(walk != NULL && walk->pet_type != this_pet){
            walk = walk ->next ;
        }
          
        if(walk == NULL || walk -> pet_type != this_pet){
            printf("Error: There is not enough space to add that many pets!\n");
            return;
        }
        walk->num_pets = difference + walk->num_pets;
    }

    printf("Success! Pets have been added to room(s)!\n");
}

int check_limit(struct pet_room *room){

    if(room -> num_pets > 10){
        return 0;
    }

    return 1;
}

void do_h(struct salon *selected_position){

    int cat_difference = 0; 
    int dog_difference = 0; 
    int parrot_difference = 0; 
    int rabbit_difference = 0; 

    struct pet_room *current = selected_position -> rooms;

    while(current != NULL){
        
        if(current->pet_type == CAT){
            cat_difference = CAPACITY - current ->num_pets;
        }

        if(current->pet_type == DOG){
            dog_difference = CAPACITY - current ->num_pets;
        }

        if(current->pet_type == PARROT){
            parrot_difference = CAPACITY - current ->num_pets;
        }

        if(current->pet_type == RABBIT){
            rabbit_difference = CAPACITY - current ->num_pets;
        }

        current -> num_pets = CAPACITY;

        current = current -> next;
    }
    print_added_pets(cat_difference, dog_difference, rabbit_difference, parrot_difference);

}

void do_s(struct salon *selected_position, char name[20], char new_name[20]){
    scan_name(name); 
    scan_name(new_name);

    
    struct pet_room *room = selected_position -> rooms; 


    while(room -> next != NULL && strcmp(room->room_name,name) != 0){
        room = room -> next;
    }

    if(strcmp(name, room->room_name) != 0){
        printf("Error: This room does not exist!\n");
        return;
    }

    enum pet_type this_pet = room ->pet_type;
    struct pet_room *this_new_room = create_room(new_name, this_pet);

    if(strcmp(new_name, room->room_name) == 0){
        printf("Error: Cannot create room as room with given name already exists!\n");
        return;
    }

    if(room ->num_pets == 0){
        printf("Error: Not enough pets in room to split across two rooms!\n");
        return;
    }

    if(room -> next == NULL){
        room -> next = this_new_room;
        check_odd(room, this_new_room);
        printf("Success! Pet amount has been split across two rooms!\n");
        return; 
    }

    struct pet_room *temp = room -> next; 
    room -> next = this_new_room;
    this_new_room -> next = temp;
    check_odd(room, this_new_room);

}

void check_odd(struct pet_room *room, struct pet_room *new_room){

    if((room -> num_pets)%2 == 0){
        new_room ->num_pets = room -> num_pets/2;
        room -> num_pets = room -> num_pets/2;
    }

    else{
        new_room ->num_pets = room -> num_pets/2;
        room -> num_pets = room -> num_pets/2 + 1;
    }

}

void do_f(struct salon *room, char name[20], int amount_cared){
    scan_name(name);

    
    scanf("%d", &amount_cared);

    struct pet_room *checking = room -> rooms;

    while(checking != NULL && strcmp(checking ->room_name, name) != 0){
        checking = checking -> next;
    }

    if(checking == NULL){
        printf("Error: This room does not exist!\n");
        return;
    }

    if(amount_cared > checking -> num_pets || amount_cared <= 0){
        printf("Error: Invalid amount!\n");
        return;
    }

    if(amount_cared <= checking -> num_pets){
        checking ->num_pets = checking->num_pets - amount_cared;
    }

    room -> summary.total_cared = amount_cared;

    room -> summary.total_profit = amount_cared * (room -> base_cost + do_pet_cost(checking)); 
    
    printf("Success! %d pets have been cared for!\n", amount_cared);
}

double do_pet_cost (struct pet_room *current_room){

    if(current_room ->pet_type == CAT){
        return CAT_COST;
    }

    if(current_room -> pet_type == DOG){
        return DOG_COST;
    }

    if(current_room -> pet_type == PARROT){
        return PARROT_COST;
    }

    if(current_room -> pet_type == RABBIT){
        return RABBIT_COST;
    }

    else{
        return 0;
    }
}

struct salon *do_back (struct salon *selected_position, struct salon *head){
    struct salon *current_position = head;

    while(current_position -> next != NULL && current_position -> next != selected_position){
        current_position = current_position -> next;
    }

    if(current_position -> next == NULL){
        printf("You have now selected salon: %s!\n", current_position ->salon_name);
        return current_position;
    }

    printf("You have now selected salon: %s!\n", current_position ->salon_name);
    return current_position;

}

void do_d(struct salon *selected_position, struct salon *head){
    
    int selected = 1; 
    struct salon *running_through = selected_position; 
    double total_profit = 0;
    int total_cared_pets = 0;
    print_salon_stats(running_through, selected);
    
    if(running_through -> next == NULL){ 

        if(head -> next == NULL){
            total_cared_pets = total_cared_pets + (running_through ->summary.total_cared);
            total_profit = total_profit + (running_through ->summary.total_profit);
            print_total_stats(total_profit, total_cared_pets);
            return;
        }

        running_through = head;
        selected = 0; 
        print_salon_stats(running_through, selected);

    }

    while(running_through -> next != selected_position && running_through != NULL){
        selected = 0;

        if(running_through != head){
            print_salon_stats(running_through, selected);
        }

        running_through = running_through -> next; 

        if(running_through == NULL){
            break;
        }
    }


    struct salon *current = head;

    while(current != NULL){
        total_cared_pets = total_cared_pets + (current ->summary.total_cared);
        total_profit = total_profit + (current ->summary.total_profit);
        current = current -> next;
    }

    print_total_stats(total_profit, total_cared_pets);
}

void do_r(char name[20], struct salon *selected_salon){
    scan_name(name);

    struct pet_room *rooms = selected_salon ->rooms; 

    if(rooms == NULL){
        printf("Error: This room does not exist!\n");
        return;
    }

    if(rooms -> next == NULL && strcmp(rooms -> room_name, name) == 0){
        free(rooms); 
        printf("Success! Room has been removed!\n");
        selected_salon ->rooms = NULL;
        return;
    }

    while(rooms != NULL && strcmp(rooms->next->room_name, name) != 0){
        rooms = rooms -> next;
    }

    if(rooms ->next -> next == NULL){
        free(rooms -> next);
        rooms -> next = NULL; 
        printf("Success! Room has been removed!\n");
        return;
    }

    struct pet_room *temp = rooms -> next -> next; 
    free(rooms -> next);
    rooms ->next = temp;
    printf("Success! Room has been removed!\n");

}

struct salon *do_q(struct salon *selected_salon, struct salon *head){

    struct salon *current = head; 

    struct salon *end_of_list = head;
    while(end_of_list -> next != NULL){
        end_of_list = end_of_list ->next;
    }


    if(current -> next == NULL){
        printf("Error: Cannot remove when there is only one salon!\n");
        return selected_salon;
    }

    if(selected_salon == head){
        current = current -> next;
        free_salon(head);
        head = current;
        return end_of_list; 
    }
    while(current != NULL && current -> next != selected_salon){
        current = current -> next;
    }

    if(current ->next -> next == NULL){
        free_salon(current -> next);
        current -> next = NULL; 
        return current;
    }

    struct salon *temp = current -> next -> next; 
    free_salon(current -> next);
    current ->next = temp;

    return current;

}

void free_salon(struct salon *selected_salon) {
    if(selected_salon!= NULL){
        free_room(selected_salon->rooms);
        free(selected_salon);
    }

}

void free_room(struct pet_room *room){
    struct pet_room *next_room;
    while (room!= NULL){
        next_room = room -> next;
        free(room);
        room = next_room;
    }
}

void do_o(struct salon *selected_salon){
    struct pet_room *head = selected_salon ->rooms;
    struct pet_room *prev = NULL;
    int count =0; 

    struct pet_room *counting = selected_salon ->rooms;
    while(counting != NULL){
        counting = counting -> next; 
        count ++; 
    }

    for(int i =0; i < count -1; i++){
        head = selected_salon -> rooms;
        
        for(int j =0; j< count -i -1; j++){

            if (checking_name(selected_salon, head) > 0){
                struct pet_room *temp = head -> next -> next; 
                head ->next -> next = head; 

                if(selected_salon -> rooms == head){
                    selected_salon -> rooms = head -> next;
                }

                if(prev != NULL){
                    prev ->next = head->next;
                }

                head -> next = temp;

            }

            prev = head;
            head = head -> next;

        }
    }
 
}

int checking_name (struct salon *selected_salon, struct pet_room *head){
    
    return strcmp(head ->room_name, head ->next->room_name);

}

int is_Named_Salon_At_Head(struct salon *head, char name[20]){
    struct salon *name_check = head;
     while(name_check != NULL){
        if(strcmp(name_check ->salon_name, name) == 0 && name_check == head){
           
            return 1;
        }
        name_check = name_check -> next;
     }
        return 0;
}

struct salon *do_x (struct salon *selected_salon, struct salon *head, char combined_salon[20], char name[20], char other_salon[20]){
    int check = 0;
    int check_2 =0; 
    double base_cost =0;
    struct salon *name_check = head;
    struct salon *error_check = head;
    struct salon *error_check_1 = head;

    if(strcmp(combined_salon, other_salon) == 0 || strcmp(combined_salon, name) == 0){
        printf("Error: Cannot merge a salon with itself!\n");
        return selected_salon;
    }

    while(error_check != NULL && strcmp(name, error_check -> salon_name) != 0){
        error_check = error_check -> next;
    }

    if(error_check == NULL){
        printf("Error: Salon names(s) do not exist to be merged!\n");
        return selected_salon;
    }

    while(error_check_1!= NULL && strcmp(other_salon, error_check_1 -> salon_name) != 0){
        error_check_1 = error_check_1 -> next;
    }

    if(error_check_1 == NULL){
        printf("Error: Salon name(s) do not exist to be merged!\n");
        return selected_salon;
    }

    while(name_check != NULL){
        if(strcmp(name_check ->salon_name, combined_salon) == 0){
            printf("Error: Salon name already exists!\n");
            return selected_salon;
        }
        name_check = name_check ->next;
    }

    struct salon *salon_one = head;
    struct salon *salon_two = head; 

    if(strcmp(name,head ->salon_name) == 0){
        salon_one = head; 
        check = 1; 
    }

    if(strcmp(other_salon,head ->salon_name) == 0){
        salon_two = head; 
        check_2 = 1;
    }

    while(salon_one != NULL && strcmp(name, salon_one -> next ->salon_name) != 0 && check == 0){
        salon_one = salon_one -> next;
    }

    while(salon_two != NULL && strcmp(other_salon, salon_two -> salon_name) != 0 && check_2 == 0){
        salon_two = salon_two -> next;
    }

    base_cost = (salon_one->base_cost + salon_two->base_cost)/2;
    struct salon *this_salon = create_salon(combined_salon, base_cost);
    
    this_salon->summary.total_cared = salon_one->summary.total_cared + salon_two->summary.total_cared;
    this_salon->summary.total_profit = salon_one->summary.total_profit + salon_two->summary.total_profit;

    if(salon_one -> rooms != NULL){
        this_salon ->rooms = salon_one ->rooms;
        struct pet_room *end = this_salon -> rooms;

        while(end -> next != NULL){
            end = end -> next;
        }

        if(salon_two -> rooms != NULL){
            end -> next = salon_two -> rooms;
        }
    }

    if(salon_one -> rooms == NULL){
        if(salon_two -> rooms != NULL){
            this_salon -> rooms = salon_two -> rooms;
        } else{
            this_salon -> rooms = NULL;
        }
    }
    salon_one -> rooms = NULL;
    salon_two -> rooms = NULL;

    free_salon(salon_one);
    free_salon(salon_two);

    if(this_salon ->rooms != NULL){
        do_o(this_salon);
    }

    return this_salon;
}




////////////////////////////////////////////////////////////////////////////////
///////////////////////////  PROVIDED FUNCTIONS  ///////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// Prints a single room, displaying name, position, pet_type, number of pets and
// cost of care for that room
//
// Parameters:
//      position    - the position of the room in the salon (the first 
//                        room will be room 1, second will be 2, etc)
//      room        - a pointer to the struct pet_room
//
// Returns:   
//      None
void print_one_room(int position, struct pet_room *room) {
    printf("/-------------------------------\\\n");
    printf("Room name: %s\n", room->room_name);
    printf("    Room position: %d\n", position);
    printf("    Pet type: ");
    print_pet_type(room->pet_type);
    printf("\n    Num of pets in room: %d/10\n", room->num_pets);
    printf("\\-------------- | --------------/\n");
    printf("                V\n");
}

// Print the statistics of one salon
// 
// Parameters:
//      salon    - a pointer to the struct salon
//      selected - an integer indicating if the salon is 'selected' or not
// Returns:
//      None
//
void print_salon_stats(struct salon *salon, int selected) {
    printf("/-------------------------------\\\n");
    printf("Stats from salon: %s", salon->salon_name);
    if (selected == 1) {
        printf(" (selected)");
    }
    printf("\n    Salon's base cost: %.2lf\n", salon->base_cost);
    printf("    Total pets cared for: %d\n", salon->summary.total_cared);
    printf("    Total profit from cared pets: %.2lf\n", salon->summary.total_profit);
    printf("\\-------------- | --------------/\n");
    printf("                V\n");
}

// Print the statistics of all the salons combined
// 
// Parameters:
//      profit  - amount of money the salons has made
//      pets    - number of pets that have been cared for by the salons
// Returns:
//      None
//
void print_total_stats(double profit, int pets) {
    printf("/-------------------------------\\\n");
    printf("Combined stats from all salons!\n");
    printf("    Total pets cared for: %d\n", pets);
    printf("    Total profit from cared pets: %.2lf\n", profit);
    printf("\\-------------------------------/\n");
}

// Print type of pet as a string
// 
// Parameters:
//      pet_type    - the type of pet
// Returns:
//      None
//
void print_pet_type(enum pet_type pet_type) {
    if (pet_type == CAT) {
        printf("cat");
    } else if (pet_type == DOG) {
        printf("dog");
    } else if (pet_type == RABBIT) {
        printf("rabbit");
    } else if (pet_type == PARROT) {
        printf("parrot");
    }
}

// Print the amount of pets being added to the salon
// 
// Parameters:
//      num_cats    - number of additional cats added
//      num_dogs    - number of additional dogs added
//      num_rabbits - number of additional rabbits added
//      num_parrots - number of additional parrots added
// Returns:
//      None
//
void print_added_pets(int num_cats, int num_dogs, int num_rabbits, 
                        int num_parrots) {
    printf("/-------------------------------\\\n");
    printf("    Cats added: %d\n", num_cats);
    printf("    Dogs added: %d\n", num_dogs);
    printf("    Rabbits added: %d\n", num_rabbits);
    printf("    Parrots added: %d\n", num_parrots);
    printf("\\-------------------------------/\n");
}

// Scan for a name
// 
// Parameters:
//      string - the string to be scanned
// Returns:
//      None
//
void scan_name(char string[MAX_NAME_LEN]) {
    scan_token(string, MAX_NAME_LEN);
}

// Scan an enum pet_type
// 
// Parameters:
//      None
// Returns:
//      enum pet_type variable
//
enum pet_type scan_pet_type() {
    char pet_type[MAX_NAME_LEN];
    scan_token(pet_type, MAX_NAME_LEN);
    return string_to_pet_type(pet_type);
}

////////////////////////////////////////////////////////////////////////////////
////////////////////// ADDITIONAL PROVIDED FUNCTIONS ///////////////////////////
////////////////////////////////////////////////////////////////////////////////
// You don't need to use any of these, or understand how they work!
// We use them to implement some of the provided helper functions.

// Scan a token into the provided buffer of buffer_size
// 
// Parameters:
//      buffer      - the buffer being scanned into
//      buffer_size - the size of buffer
// Returns:
//      the number of chars scanned
//
int scan_token(char *buffer, int buffer_size) {
    if (buffer_size == 0) {
        return 0;
    }

    char c;
    int i = 0;
    int num_scanned = 0;

    // consume all leading whitespace
    scanf(" ");

    // scan in characters until whitespace
    while (i < buffer_size - 1
        && (num_scanned = scanf("%c", &c)) == 1 
        && !isspace(c)) {

        buffer[i++] = c;
    }

    if (i > 0) {
        buffer[i] = '\0';
    }

    return num_scanned;
}

// Convert a string into an enum pet_type
// 
// Parameters:
//      pet_type    - a string version of the pet type
// Returns:
//      enum pet_type
//
enum pet_type string_to_pet_type(char *pet_type) {
    int len = strlen(pet_type);

    if (strncasecmp(pet_type, "cat", len) == 0) {
        return CAT;
    } 
    if (strncasecmp(pet_type, "dog", len) == 0) {
        return DOG;
    } 
    if (strncasecmp(pet_type, "rabbit", len) == 0) {
        return RABBIT;
    }
    if (strncasecmp(pet_type, "parrot", len) == 0) {
        return PARROT;
    } 

    return INVALID_PET_TYPE;
}