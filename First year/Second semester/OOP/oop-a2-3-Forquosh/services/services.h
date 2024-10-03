//
// Created by mihai on 11.03.2024.
//

#pragma once
#include "../repository/repository.h"

typedef struct {
    Repository* repository;
    Repository** history;
    int history_capacity;
    int history_length;
    int current_index;
}Services;

Services* createServices();
void destroyServices(Services* services);
void add_new_offer(Services* services, char* type, char* destination, int departure_date, int price);
int delete_existent_offer(Services* services, char* destination, int departure_date);
int update_existent_offer(Services* services, char* old_type, int old_departure_date, char* type, char* destination, int departure_date, int price);
Repository* get_offers_containing_given_word(Services* services, char* word);
Repository* get_offers_by_type_and_departure_date(Services* services, char* type, int departure_date);
void free_offers_list(Repository* offers_list);
void undo(Services* services);
void redo(Services* services);
