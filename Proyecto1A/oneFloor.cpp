#include "oneFloor.h"

PtrTParkingNode newSpace;
PtrTParkingNode Parking;

int generateRandom(int MIN, int MAX) {
    random_device rd;
    mt19937 gen(rd());    //Creates a randon number for a new code to take out a car
    uniform_int_distribution<> dis(MIN, MAX);
    int randomCode = dis(gen);
    return randomCode;
}

PtrTParkingNode createParkingNode(int slotNumber)
{
    PtrTParkingNode slot = new(ParkingNode);
    slot->parkingSpot = slotNumber;
    slot->isOccupied = false;
    slot->Next = NULL;
    int returnCode = 0;
    return slot;
}

void addParkingNode(PtrTParkingNode& Parking, PtrTParkingNode& newSpot)
{
    newSpot->Next = Parking;
    Parking = newSpot;
}

void carList(PtrTParkingNode& Parking)
{
    PtrTParkingNode Aux;
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

void addCarToParking(PtrTParkingNode& Parking, car* newCar) {

    newCar->licensePlate = generateRandom(100000, 999999);
    newCar->weight = generateRandom(1, 3);                    //Random attributes to each car
    newCar->size = generateRandom(1, 5);
    PtrTParkingNode Aux;
    Aux = Parking;
    bool addedFlag = false; // Cambiado a false

    while (Aux != NULL) {
        if (Aux->isOccupied == false) { // Cambiado a false
            Aux->carInSpot.licensePlate = newCar->licensePlate;
            Aux->carInSpot.size = newCar->size;
            Aux->carInSpot.weight = newCar->weight;
            Aux->isOccupied = true; // Cambiado a true
            int randomCode = generateRandom(100000, 999999);
            Aux->returnCode = randomCode;
            cout << "Se añadio un carro en el espacio " << Aux->parkingSpot << " con el codigo de retorno " << Aux->returnCode << endl;
            addedFlag = true; // Cambiado a true
            break;
        }
        Aux = Aux->Next;
    }
    if (addedFlag == false) { cout << "El parque esta lleno" << endl; } // Cambiado == false
}

void initializeParking(PtrTParkingNode& Parking) {
    static int MAX_SIZE = 25;
    PtrTParkingNode newSpace;
    for (int i = 0; i <= MAX_SIZE; i++)
    {
        newSpace = createParkingNode(i);
        addParkingNode(Parking, newSpace);
    }

}
void startSimulation() {

    while (true) {
        int randomChance = generateRandom(1, 1); //Chance treshold
        cout << randomChance << endl;
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

void oneFloor() {
    Parking = NULL;
    initializeParking(Parking);
    thread simThread(startSimulation);
    thread userInputThread(userInput);

    simThread.join();
    userInputThread.join();
}
