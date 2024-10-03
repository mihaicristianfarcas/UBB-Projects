//
// Created by mihai on 11.03.2024.
//

#include "domain.h"

Offer createOffer(char* type, char* destination, int departure_date, int price){
    Offer offer;
    strcpy(offer.type, type);
    strcpy(offer.destination, destination);
    offer.departure_date = departure_date;
    offer.price = price;
    return offer;
}

Offer get_new_offer(char* type, char* destination, int departure_date, int price){
    Offer new_offer = createOffer(type, destination, departure_date, price);
    return new_offer;
}