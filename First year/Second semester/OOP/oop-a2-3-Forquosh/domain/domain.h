//
// Created by mihai on 11.03.2024.
//

#pragma once
#include <string.h>

typedef struct{
    int departure_date, price;
    char type[50], destination[50];
}Offer;

Offer createOffer(char* type, char* destination, int departure_date, int price);
Offer get_new_offer(char* type, char* destination, int departure_date, int price);
