#include "oneFloorAdvanced.h"

PtrTAdvancedParkingNode newSpace;
PtrTAdvancedParkingNode Parking;

int generateRandomA(int MIN, int MAX) {
    random_device rd;
    mt19937 gen(rd());    //Creates a randon number for a new code to take out a car or license plate
    uniform_int_distribution<> dis(MIN, MAX);
    int randomCode = dis(gen);
    return randomCode;
}

PtrTAdvancedParkingNode createAdvancedParkingNode(int slotNumber)
{
    PtrTAdvancedParkingNode slot = new(AdvancedParkingNode);
    slot->parkingSpot = slotNumber;
    slot->isLowerOccupied = false;
    slot->isUpperOccupied = false;
    slot->Next = NULL;
    return slot;
}
void initializeParking(PtrTAdvancedParkingNode& Parking) {
    static int MAX_SIZE = 25;
    PtrTAdvancedParkingNode newSpace;
    for (int i = 0; i <= MAX_SIZE; i++)
    {
        newSpace = createAdvancedParkingNode(i);
        addParkingNode(Parking, newSpace);
    }
}

void addParkingNode(PtrTAdvancedParkingNode& Parking, PtrTAdvancedParkingNode& newSpot)
{
    newSpot->Next = Parking;
    Parking = newSpot;
}

void carList(PtrTAdvancedParkingNode& Parking) //only used for testing, 
{
    PtrTAdvancedParkingNode Aux;
    Aux = Parking;
    while (Aux != NULL)
    {
        if (Aux->isLowerOccupied == true) {
            cout << Aux->lowerCar.licensePlate << ", ";
            cout << Aux->lowerCar.size << ", ";
            cout << Aux->lowerCar.weight <<". " << "LOWER CAR" << endl;
        }
        if (Aux->isUpperOccupied == true) {
            cout << Aux->upperCar.licensePlate << ", ";
            cout << Aux->upperCar.size << ", ";
            cout << Aux->upperCar.weight << ". " << "UPPER CAR" << endl;

        }
        Aux = Aux->Next;
    }
    cout << endl;
}

void addCarToParking(PtrTAdvancedParkingNode& Parking, car* newCar) { 

    newCar->licensePlate = generateRandomA(100000, 999999);
    newCar->weight = generateRandomA(1, 3);                    //Random attributes to each car
    newCar->size = generateRandomA(1, 5);
    PtrTAdvancedParkingNode Aux;
    Aux = Parking;
    bool addedFlag = false; // Cambiado a false

    while (Aux != NULL) {
        if (Aux->isLowerOccupied == false) { // Revision para planta baja
            Aux->lowerCar.licensePlate = newCar->licensePlate;
            Aux->lowerCar.size = newCar->size;
            Aux->lowerCar.weight = newCar->weight;
            Aux->isLowerOccupied = true; // Cambiado a true
            int randomCode = generateRandomA(100000, 999999);
            Aux->lowerReturnCode = randomCode;
            cout << "Se añadio un carro en el espacio " << Aux->parkingSpot << " con el codigo de retorno " << randomCode << endl;
            addedFlag = true; // Cambiado a true
            break;
        }
        else if (Aux->isUpperOccupied == false) {   // Revision planta alta
            Aux->upperCar.licensePlate = newCar->licensePlate;
            Aux->upperCar.size = newCar->size;
            Aux->upperCar.weight = newCar->weight;
            Aux->isUpperOccupied = true; // Cambiado a true
            int randomCode = generateRandomA(100000, 999999);
            Aux->upperReturnCode = randomCode;
            cout << "Se añadio un carro en el espacio " << Aux->parkingSpot << " con el codigo de retorno " << randomCode << endl;
            addedFlag = true; // Cambiado a true
            break;
        }
        Aux = Aux->Next;
    }
    if (addedFlag == false) { cout << "El parque esta lleno" << endl; } // Cambiado == false
}

void startAdvancedSimulation() {

    while (true) {
        int randomChance = generateRandomA(1, 1); //Chance treshold
        if (randomChance == 1) {
            addCarToParking(Parking, new(car));

        }
        std::this_thread::sleep_for(std::chrono::seconds(5)); // Sleep treshold
    }
}

void userInput() {
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

void oneFloorAdvanced() {
    Parking = NULL;
    initializeParking(Parking);
    thread simThread(startAdvancedSimulation);
    thread userInputThread(userInput);

    simThread.join();
    userInputThread.join();
}


