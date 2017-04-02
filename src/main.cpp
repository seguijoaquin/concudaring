#include <iostream>
#include "concudaring.h"

int main(int argc, char** argv) {
    Concudaring concudaring(atoi(argv[1]));
    concudaring.start();
}