#include <cstdint>
#include <iostream>

extern "C" void kek(int16_t* start, int32_t size);

int main() {
    int16_t a[] = {-5, -3, 0, 1, 2, -6};
    int32_t size = 6;
    kek(&a[0], size);
    return 0;
}
