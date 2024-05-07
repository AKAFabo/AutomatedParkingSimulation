#include "oneFloorBasic.h"
#include "oneFloorAdvanced.h"
#include "ParkingTower.h"

//Only used to choose which mode will be simulated
int main() {
    /*
    cout << "Holaaaa";
    oneFloor();
    */
    char opt;
    while (true) {
        cin >> opt;
        switch (opt) {

        case '1': //One floor basic parking
            oneFloorBasic();
            break;
        case '2': //One floor advanced parking
            oneFloorAdvanced();
            break;
        case '3': //Parking Tower
            parkingTower();
            break;
        default:
            cout << "Invalid choice. Try again." << endl;
        }

    }
}

