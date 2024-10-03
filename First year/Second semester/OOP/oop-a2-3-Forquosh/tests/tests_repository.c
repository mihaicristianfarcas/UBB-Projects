//
// Created by mihai on 18.03.2024.
//
#include "../repository/repository.h"
#include "assert.h"

void test_createRepository() {
    Repository *offers_list = createRepository(10);
    assert(offers_list != NULL);
    assert(offers_list->capacity == 10);
    assert(offers_list->length == 0);
    destroyRepository(offers_list);
}
void test_add_offer() {
    Repository* offers_list = createRepository(2);
    Offer new_offer = {12, 1200, "city break", "Prague"};
    add_offer(offers_list, new_offer);
    assert(offers_list->length == 1);
    assert(strcmp(offers_list->elements[0].destination, "Prague") == 0);
    destroyRepository(offers_list);
}

void test_destroyRepository() {
    Repository* offers_list = createRepository(10);
    assert(offers_list != NULL);
    destroyRepository(offers_list);
}

void test_resize() {
    Repository* offers_list = createRepository(2);
    assert(offers_list->capacity == 2);
    resize(offers_list);
    assert(offers_list->capacity == 4);
    destroyRepository(offers_list);
}

void test_find_offer() {
    Repository* offers_list = createRepository(2);
    Offer offer1 = {12, 1200, "city break", "Prague"};
    add_offer(offers_list, offer1);
    Offer* found_offer = find_offer(offers_list, "Prague", 12);
    assert(found_offer != NULL);
    assert(strcmp(found_offer->destination, "Prague") == 0);
    destroyRepository(offers_list);
}

void test_delete_offer() {
    Repository* offers_list = createRepository(2);
    Offer offer1 = {12, 1200, "city break", "Prague"};
    add_offer(offers_list, offer1);
    assert(offers_list->length == 1);
    int deleted = delete_offer(offers_list, "Prague", 12);
    assert(deleted == 1);
    assert(offers_list->length == 0);
    deleted = delete_offer(offers_list, "NotExisting", 100);
    assert(deleted == 0);
    destroyRepository(offers_list);
}

void test_update_offer() {
    Repository* offers_list = createRepository(2);
    Offer offer1 = {12, 1200, "city break", "Prague"};
    add_offer(offers_list, offer1);
    Offer* found_offer = find_offer(offers_list, "Prague", 12);
    assert(found_offer != NULL);
    assert(strcmp(found_offer->type, "city break") == 0);
    assert(found_offer->price == 1200);
    update_offer(found_offer, "UpdatedType", "UpdatedDestination", 100, 200);
    assert(strcmp(found_offer->type, "UpdatedType") == 0);
    assert(strcmp(found_offer->destination, "UpdatedDestination") == 0);
    assert(found_offer->departure_date == 100);
    assert(found_offer->price == 200);
    destroyRepository(offers_list);
}

void test_get_list_by_destination() {
    Repository* offers_list = createRepository(2);
    Offer offer1 = {12, 1200, "city break", "Prague"};
    Offer offer2 = {21, 2100, "city break", "Alba Iulia"};
    add_offer(offers_list, offer1);
    add_offer(offers_list, offer2);
    Repository* filtered_list = get_list_by_destination(offers_list, "Prague");
    assert(filtered_list->length == 1);
    assert(strcmp(filtered_list->elements[0].destination, "Prague") == 0);
    destroyRepository(filtered_list);
    filtered_list = get_list_by_destination(offers_list, "NonExisting");
    assert(filtered_list->length == 0);
    destroyRepository(filtered_list);
    destroyRepository(offers_list);
}

void test_get_list_by_type_and_departure_date() {
    Repository *offers_list = createRepository(2);
    Offer offer1 = {12, 1200, "city break", "Prague"};
    Offer offer2 = {21, 2100, "city break", "Alba Iulia"};
    add_offer(offers_list, offer1);
    add_offer(offers_list, offer2);
    Repository* filtered_list = get_list_by_type_and_departure_date(offers_list, "type", 10);
    assert(filtered_list->length == 0);
    destroyRepository(filtered_list);
    filtered_list = get_list_by_type_and_departure_date(offers_list, "city break", 10);
    assert(filtered_list->length == 2);
    destroyRepository(filtered_list);
    destroyRepository(offers_list);
}

void test_repository() {
    test_createRepository();
    test_destroyRepository();
    test_resize();
    test_find_offer();
    test_add_offer();
    test_delete_offer();
    test_update_offer();
    test_get_list_by_destination();
    test_get_list_by_type_and_departure_date();
}