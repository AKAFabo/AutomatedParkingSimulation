#include "oneFloorBasic.h"
#include "oneFloorAdvanced.h"

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
            oneFloor();
            break;
        default:
            cout << "Invalid choice. Try again." << endl;
        }

    }
}

