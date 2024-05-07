#include "oneFloorBasic.h"

PtrTBasicParkingNode newSpace;
PtrTBasicParkingNode Parking;

int generateRandomB(int MIN, int MAX) {
    random_device rd;
    mt19937 gen(rd());    //Creates a randon number for a new code to take out a car
    uniform_int_distribution<> dis(MIN, MAX);
    int randomCode = dis(gen);
    return randomCode;
}

PtrTBasicParkingNode createBasicParkingNode(int slotNumber)
{
    PtrTBasicParkingNode slot = new(BasicParkingNode);
    slot->parkingSpot = slotNumber;
    slot->isOccupied = false;
    slot->Next = NULL;
    int returnCode = 0;
    return slot;
}
void initializeParking(PtrTBasicParkingNode& Parking) {
    static int MAX_SIZE = 25;
    PtrTBasicParkingNode newSpace;
    for (int i = 0; i <= MAX_SIZE; i++)
    {
        newSpace = createBasicParkingNode(i);
        addParkingNode(Parking, newSpace);
    }
}

void addParkingNode(PtrTBasicParkingNode& Parking, PtrTBasicParkingNode& newSpot)
{
    newSpot->Next = Parking;
    Parking = newSpot;
}

void carList(PtrTBasicParkingNode& Parking) //only used for testing
{
    PtrTBasicParkingNode Aux;
    Aux = Parking;
    while (Aux != NULL)
    {
        if (Aux->isOccupied == true) {
            cout << Aux->carInSpot.licensePlate << ", ";
            cout << Aux->carInSpot.size << ", ";
            cout << Aux->carInSpot.weight << endl;
        }
        Aux = Aux->Next;
    }
    cout << endl;
}

void addCarToParking(PtrTBasicParkingNode& Parking, car* newCar) {

    newCar->licensePlate = generateRandomB(100000, 999999);
    newCar->weight = generateRandomB(1, 3);                    //Random attributes to each car
    newCar->size = generateRandomB(1, 5);
    PtrTBasicParkingNode Aux;
    Aux = Parking;
    bool addedFlag = false; // Cambiado a false

    while (Aux != NULL) {
        if (Aux->isOccupied == false) { // Cambiado a false
            Aux->carInSpot.licensePlate = newCar->licensePlate;
            Aux->carInSpot.size = newCar->size;
            Aux->carInSpot.weight = newCar->weight;
            Aux->isOccupied = true; // Cambiado a true
            int randomCode = generateRandomB(100000, 999999);
            Aux->returnCode = randomCode;
            cout << "Se añadio un carro en el espacio " << Aux->parkingSpot << " con el codigo de retorno " << Aux->returnCode << endl;
            addedFlag = true; // Cambiado a true
            break;
        }
        Aux = Aux->Next;
    }
    if (addedFlag == false) { cout << "El parque esta lleno" << endl; } // Cambiado == false
}

void startBasicSimulation() {

    while (true) {
        int randomChance = generateRandomB(1, 1); //Chance treshold
        if (randomChance == 1) {
            addCarToParking(Parking, new(car));

        }
        std::this_thread::sleep_for(std::chrono::seconds(5)); // Sleep treshold
    }
}

void userInputB() {
    char choice;
    while (true) {
        cin >> choice;
        switch (choice) {

        case 'p': //Parking listing
            carList(Parking);
            break;
        default:
            cout << "Invalid choice. Try again." << endl;
        }
    }
}

void oneFloorBasic() {
    Parking = NULL;
    initializeParking(Parking);
    thread simThread(startBasicSimulation);
    thread userInputThread(userInputB);

    simThread.join();
    userInputThread.join();
}
