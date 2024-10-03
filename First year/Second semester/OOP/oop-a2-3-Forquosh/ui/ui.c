//
// Created by mihai on 11.03.2024.
//
#include <stdio.h>
#include "../services/services.h"
#include "../tests/tests.h"
#include <crtdbg.h>

const int ADD_COMMAND = 1, DELETE_COMMAND = 2, UPDATE_COMMAND = 3, DISPLAY_BY_DESTINATION_COMMAND = 4;
const int DISPLAY_BY_TYPE_COMMAND = 5, UNDO_COMMAND = 6, REDO_COMMAND = 7, STOP_COMMAND = 0;

void print_menu() {
    printf("Hello! Please choose an action:\n");
    printf("1. Add an offer!\n");
    printf("2. Delete an offer!\n");
    printf("3. Update an offer!\n");
    printf("4. Display offer containing a given string!\n");
    printf("5. Display offer of a given type after a given departure date!\n");
    printf("6. Undo last action!\n");
    printf("7. Redo last action!\n");
    printf("0. Stop the program.\n");
}

void print_offers(Repository* offers_list){
    if (offers_list->length != 0) {
        printf("Offers:\n");
        for (int i = 0; i < offers_list->length; i++) {
            printf("Type: %s ; Destination: %s ; Departure date: %d ; Price: %d\n",
                   offers_list->elements[i].type, offers_list->elements[i].destination,
                   offers_list->elements[i].departure_date, offers_list->elements[i].price);
        }
    }
    else
        printf("No offers match your search! Please try again...\n");
}

int process_command(int command) {
    if (command < 0 || command > 7) {
        printf("Invalid input command! Please try again...\n");
        return 0;
    }
    return 1;
}

void get_type(char* type){
    printf("Please give the type:\n");
    getchar();
    scanf("%[^\n]s", type);
}

void get_destination(char* destination){
    printf("Please give the destination:\n");
    getchar();
    scanf("%[^\n]s", destination);
}

void get_departure_date(int* departure_date){
    printf("Please give the departure date:\n");
    scanf("%d", departure_date);
}

void get_price(int* price){
    printf("Please give the price:\n");
    scanf("%d", price);
}

void get_word(char* word){
    getchar();
    printf("Please give the word:\n");
    fgets(word,50, stdin);
    word[strlen(word) - 1] = '\0';
}

void ui()
{
    int user_choice;
    Services* services = createServices();

    test_domain();
    test_services();
    test_repository();

    char type[50], destination[50], old_destination[50], word[50];
    int departure_date, old_departure_date, price;
    while(1){

        user_choice = 9;
        print_menu();

        scanf("%d", &user_choice);
        int is_command_valid = process_command(user_choice);
        if (is_command_valid){

            if (user_choice == ADD_COMMAND) {
                get_type(type);
                get_destination(destination);
                get_departure_date(&departure_date);
                get_price(&price);

                add_new_offer(services, type, destination, departure_date, price);
                printf("Offer added!\n");
            }

            if (user_choice == DELETE_COMMAND || user_choice == UPDATE_COMMAND){
                get_destination(destination);
                get_departure_date(&departure_date);

                if (user_choice == UPDATE_COMMAND) {
                    strcpy(old_destination, destination);
                    old_departure_date = departure_date;
                    printf("Enter new data for the offer:\n");
                    get_type(type);
                    get_destination(destination);
                    get_departure_date(&departure_date);
                    get_price(&price);
                    if (update_existent_offer(services, old_destination, old_departure_date, type, destination, departure_date, price))
                        printf("Offer updated successfully!\n");
                    else
                        printf("Offer not found. Please try again...\n");
                }
                else
                    if (delete_existent_offer(services, destination, departure_date))
                        printf("Offer deleted successfully!\n");
                    else
                        printf("Offer couldn't be found. Please try again...\n");
            }

            if (user_choice == DISPLAY_BY_DESTINATION_COMMAND){
                get_word(word);
                Repository* offers_list = get_offers_containing_given_word(services, word);
                print_offers(offers_list);
                free_offers_list(offers_list);
            }

            if (user_choice == DISPLAY_BY_TYPE_COMMAND){
                get_type(type);
                get_departure_date(&departure_date);
                Repository* offers_list = get_offers_by_type_and_departure_date(services, type, departure_date);
                print_offers(offers_list);
                free_offers_list(offers_list);
            }

            if (user_choice == UNDO_COMMAND){
                undo(services);
                printf("Undo complete!\n");
            }

            if (user_choice == REDO_COMMAND){
                redo(services);
                printf("Redo complete!\n");
            }

            if (user_choice == STOP_COMMAND) {
                destroyServices(services);
                printf("Memory leaks: %d\nBye!\n", _CrtDumpMemoryLeaks());
                break;
            }
        }
    }
}