//
// Created by mihai on 18.03.2024.
//
#include "../domain/domain.h"
#include "assert.h"

void test_createOffer() {
    Offer offer = createOffer("city break", "Paris", 12, 100);
    assert(strcmp(offer.type, "city break") == 0);
    assert(strcmp(offer.destination, "Paris") == 0);
    assert(offer.departure_date == 12);
    assert(offer.price == 100);
}

void test_get_new_offer() {
    Offer new_offer = get_new_offer("seaside", "Tahiti", 20, 200);
    assert(strcmp(new_offer.type, "seaside") == 0);
    assert(strcmp(new_offer.destination, "Tahiti") == 0);
    assert(new_offer.departure_date == 20);
    assert(new_offer.price == 200);
}

void test_domain() {
    test_createOffer();
    test_get_new_offer();
}