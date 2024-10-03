//
// Created by mihai on 11.03.2024.
//

#include "repository.h"

/// creates the dynamic array Repository
Repository* createRepository(int capacity){
    Repository* offers_list = malloc(sizeof(Repository));
    if (offers_list == NULL)
        return NULL;

    offers_list->capacity = capacity;
    offers_list->length = 0;

    offers_list->elements = malloc(capacity * sizeof(Offer));
    if (offers_list->elements == NULL)
        return NULL;

    return offers_list;
}

/// adds the initial 10 elements to the Repository
void add_defaultElements(Repository* offers_list){
    add_offer(offers_list, createOffer("city break", "Prague", 12, 1200));
    add_offer(offers_list, createOffer("seaside", "Mamaia", 13, 1300));
    add_offer(offers_list, createOffer("mountain", "Busteni", 14, 1000));
    add_offer(offers_list, createOffer("city break", "London", 15, 3000));
    add_offer(offers_list, createOffer("seaside", "Tenerife", 16, 4000));
    add_offer(offers_list, createOffer("mountain", "Dolomiti", 17, 2700));
    add_offer(offers_list, createOffer("city break", "Paris", 18, 2300));
    add_offer(offers_list, createOffer("seaside", "Greece", 19, 1900));
    add_offer(offers_list, createOffer("mountain", "Sinaia", 20, 1400));
    add_offer(offers_list, createOffer("city break", "Alba Iulia", 21, 800));
}

/// destroys Repository
void destroyRepository(Repository* offers_list)
{
    if (offers_list == NULL)
        return;

    free(offers_list->elements);
    offers_list->elements = NULL;

    free(offers_list);
}

/// resizes the history array to fit more states
void resize(Repository* offers_list) {
    if (offers_list == NULL)
        return;

    offers_list->capacity *= 2;
    Offer* aux = realloc(offers_list->elements, offers_list->capacity * sizeof(Offer));
    if (aux == NULL)
        return;
    offers_list->elements = aux;
}

/// searches for an Offer by given destination and departure dates
Offer* find_offer(Repository* offers_list, char* destination, int departure_date) {
    for (int i = 0; i < offers_list->length; i++) {
        if (strcmp(offers_list->elements[i].destination, destination) == 0 &&
            offers_list->elements[i].departure_date == departure_date)
            return &offers_list->elements[i];
    }
    return NULL;
}

/// adds an Offer to the Repository
void add_offer(Repository* offers_list, Offer new_offer) {
    if (offers_list == NULL)
        return;
    if (offers_list->elements == NULL)
        return;

    if (offers_list->length == offers_list->capacity)
        resize(offers_list);
    offers_list->elements[offers_list->length] = new_offer;
    offers_list->length++;
}

/// deletes an Offer from the repository
int delete_offer(Repository* offers_list, char* destination, int departure_date){
    if (offers_list == NULL)
        return 0;
    if (offers_list->elements == NULL)
        return 0;

    int found = 0;

    for(int i=0; i < offers_list->length; i++) {
        if (strcmp(offers_list->elements[i].destination, destination) == 0 &&
            offers_list->elements[i].departure_date == departure_date) {
            found = 1;
            for (int j = i; j < offers_list->length - 1; j++) {
                offers_list->elements[j] = offers_list->elements[j + 1];
            }
            offers_list->length--;
        }
    }
    return found;
}

/// modifies the data of an Offer
void update_offer(Offer *offer, char *type, char *destination, int departure_date, int price) {
    strcpy(offer->type, type);
    strcpy(offer->destination, destination);
    offer->departure_date = departure_date;
    offer->price = price;
}

/// returns a list of Offers containing a given word in the destination field
Repository* get_list_by_destination(Repository* offers_list, char* word) {
    Repository* new_offers_list = createRepository(offers_list->length);

    if (strcmp(word, "") == 0) {
        for (int i = 0; i < offers_list->length; i++) {
            add_offer(new_offers_list, offers_list->elements[i]);
        }
    }
    else {
        for (int i = 0; i < offers_list->length; i++)
            if (strstr(offers_list->elements[i].destination, word) != NULL)
                add_offer(new_offers_list, offers_list->elements[i]);
    }

    for (int i = 0; i < new_offers_list->length - 1; i++)
        for (int j = i; j < new_offers_list->length; j++) {
            if (new_offers_list->elements[i].price < new_offers_list->elements[j].price) {
                Offer aux = new_offers_list->elements[i];
                new_offers_list->elements[i] = new_offers_list->elements[j];
                new_offers_list->elements[j] = aux;
            }
        }

    return new_offers_list;
}

/// returns a list of Offers of a given type after a given departure date
Repository* get_list_by_type_and_departure_date(Repository* offers_list, char* type, int departure_date) {

    Repository* new_offers_list = createRepository(offers_list->length);

    for (int i=0; i<offers_list->length; i++)
        if (strcmp(offers_list->elements[i].type, type) == 0 && offers_list->elements[i].departure_date > departure_date) {
            add_offer(new_offers_list, offers_list->elements[i]);
        }
    return new_offers_list;
}