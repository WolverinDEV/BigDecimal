#include <iostream>
#include "include/BigDecimal.h"

using namespace std;
int main() {
    WolverinDEV::BigDecimal decimal(126);
    decimal += WolverinDEV::BigDecimal(4);

    cout << "Dec: " << decimal.toString(2) << endl;
    return 0;
}