//
// Created by mihai on 11.03.2024.
//

#pragma once
#include "../domain/domain.h"
#include <stdlib.h>

typedef struct {
    Offer* elements;
    int capacity;
    int length;
}Repository;

Repository* createRepository(int capacity);
void resize(Repository* offers_list);
void add_defaultElements(Repository* offers_list);
void destroyRepository(Repository* offers_list);
Offer* find_offer(Repository* offers_list, char* destination, int departure_date);
Repository* get_list_by_destination(Repository* offers_list, char* word);
Repository* get_list_by_type_and_departure_date(Repository* offers_list, char* type, int departure_date);
void add_offer(Repository* offers_list, Offer new_offer);
int delete_offer(Repository* offers_list, char* destination, int departure_date);
void update_offer(Offer *offer, char *type, char *destination, int departure_date, int price);

