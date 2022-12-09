#include <unistd.h>
#include <stdio.h>
#include <wait.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdatomic.h>


bool activate_service(size_t user_id, void* service);
bool place_order(_Atomic unsigned accounts[], size_t user_id, unsigned price, void* service) {

    if (accounts[user_id] < price) {
        return false;
    }

    _Atomic unsigned *oldval = &accounts[user_id];
    while (!atomic_compare_exchange_weak(&accounts[user_id], oldval, oldval - price));
    if (!activate_service(user_id, service)) {
        _Atomic unsigned *newval = &accounts[user_id];
        while(!atomic_compare_exchange_weak(&accounts[user_id], newval, newval + price));
        return false;
    }
    return true;
}
