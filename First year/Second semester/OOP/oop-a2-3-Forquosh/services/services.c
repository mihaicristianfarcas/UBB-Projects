//
// Created by mihai on 11.03.2024.
//

#include "services.h"

/// resizes the history array to fit more states
void resize_history(Services* services) {

    services->history_capacity *= 2;
    Repository** aux = (Repository**)realloc(services->history, services->history_capacity * sizeof(Repository*));
    if (aux == NULL)
        return;
    services->history = aux;
}

/// makes a copy of a given repository
Repository* clone_repository(Repository* repository) {
    Repository* clone = createRepository(repository->capacity);
    if (clone != NULL) {
        clone->length = repository->length;
        for (int i=0;i<clone->length;i++) {
            clone->elements[i] = repository->elements[i];
        }
    }
    return clone;
}

/// saves the current state of the repository in the history array
void save_history(Services* services) {
    if (services->history_length >= services->history_capacity) {
        resize_history(services);
    }

    Repository* cloned_repository = clone_repository(services->repository);
    if (cloned_repository == NULL) {
        return;
    }

    services->history_length++;
    services->current_index = services->history_length - 1;
    services->history[services->current_index] = cloned_repository;
}

/// creates Services
Services* createServices() {
    Services* services = (Services*)malloc(sizeof(Services));
    if (services == NULL) {
        return NULL;
    }
    services->repository = createRepository(20);
    add_defaultElements(services->repository);
    services->history_capacity = 10;
    services->history_length = 0;
    services->current_index = -1;
    services->history = (Repository**)malloc(services->history_capacity * sizeof(Repository*));
    if (services->history == NULL)
        return NULL;

    save_history(services);
    return services;
}

/// destroys Services
void destroyServices(Services* services){
    if (services != NULL) {
        destroyRepository(services->repository);
        free(services);
    }
}

/// creates an Offer and sends it to the repository to be added
void add_new_offer(Services* services, char* type, char* destination, int departure_date, int price){
    add_offer(services->repository, get_new_offer(type, destination, departure_date, price));
    save_history(services);
}

/// returns 1 if the Offer was deleted successfully, 0 otherwise
int delete_existent_offer(Services* services, char* destination, int departure_date){
    int is_successful =  delete_offer(services->repository, destination, departure_date);
    if (is_successful)
        save_history(services);

    return is_successful;
}

/// updates an Offer from the repository
/// returns 1 if the Offer was updated successfully, 0 otherwise
int update_existent_offer(Services* services, char* old_type, int old_departure_date, char* type, char* destination, int departure_date, int price) {
    Offer *existing_offer = find_offer(services->repository, old_type, old_departure_date);
    if (existing_offer == NULL)
        return 0;
    update_offer(existing_offer, type, destination, departure_date, price);
    save_history(services);
    return 1;
}

/// returns a list of Offers containing a given word in the destination field
Repository* get_offers_containing_given_word(Services* services, char* word){
    return get_list_by_destination(services->repository, word);
}

/// returns a list of Offers of a given type after a given departure date
Repository* get_offers_by_type_and_departure_date(Services* services, char* type, int departure_date){
    return get_list_by_type_and_departure_date(services->repository, type, departure_date);
}

/// frees a given Offers list
void free_offers_list(Repository* offers_list){
    if (offers_list != NULL) {
        free(offers_list->elements);
        offers_list->elements = NULL;
        free(offers_list);
    }
}

/// undos the last operation
void undo(Services* services) {
    if (services->current_index > 0) {
        services->current_index--;
        services->repository = services->history[services->current_index];
    }
}

/// redos the last operation
void redo(Services* services) {
    if (services->current_index < services->history_length - 1) {
        services->current_index++;
        services->repository = services->history[services->current_index];
    }
}