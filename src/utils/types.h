#ifndef CONCUDARING_TYPES_H
#define CONCUDARING_TYPES_H

#include <iostream>

struct Card {
  unsigned int number ;
  char letter;
};

typedef struct MSG {
    int32_t id;
    std::string message;
}MSG_t;

#endif //CONCUDARING_TYPES_H
