#include <iostream>
#include "table.h"
#include <stdlib.h>  //rand()

Table::Table(){
    std::cout << "Construyendo una mesa" << std::endl;
}

Table::~Table(){
    std::cout << "Destruyendo una mesa" << std::endl;
}