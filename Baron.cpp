#include "Baron.h"
void Baron::invest() {
    removeCoins(3);
    addCoins(6);
}
void Baron::onSanctioned() {
    addCoins(1);
}
