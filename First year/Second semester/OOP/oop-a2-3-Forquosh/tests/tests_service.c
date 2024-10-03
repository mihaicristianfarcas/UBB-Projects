//
// Created by mihai on 18.03.2024.
//
#include "../services/services.h"
#include "assert.h"

void test_createServices() {
    Services* services = createServices();
    assert(services != NULL);
    assert(services->repository != NULL);
    assert(services->history != NULL);
}

void test_destroyServices() {
    Services* services = createServices();
    assert(services != NULL);
    destroyServices(services);
}

void test_services() {
    test_createServices();
    test_destroyServices();
}