#include "ParkingTower.h"

PtrTParkingTowerNode newSpace;
PtrTParkingTowerNode Parking;

int generateRandomT(int MIN, int MAX) {
    random_device rd;
    mt19937 gen(rd());    //Creates a randon number for a new code to take out a car or license plate
    uniform_int_distribution<> dis(MIN, MAX);
    int randomCode = dis(gen);
    return randomCode;
}

PtrTParkingTowerNode createParkingTowerNode(int slotNumber)
{
    PtrTParkingTowerNode slot = new(ParkingTowerNode);
    slot->parkingSpot = slotNumber;
    slot->isRightOccupied = false;
    slot->isLeftOccupied = false;
    slot->Next = NULL;
    return slot;
}
void initializeParking(PtrTParkingTowerNode& Parking) {
    static int MAX_SIZE = 25;
    PtrTParkingTowerNode newSpace;
    for (int i = 0; i <= MAX_SIZE; i++)
    {
        newSpace = createParkingTowerNode(i);
        addParkingNode(Parking, newSpace);
    }
}

void addParkingNode(PtrTParkingTowerNode& Parking, PtrTParkingTowerNode& newSpot)
{
    newSpot->Next = Parking;
    Parking = newSpot;
}

void carList(PtrTParkingTowerNode& Parking) //only used for testing, TODO: FIX LOGIC FOR BOTH CARS IN SPACE
{
    PtrTParkingTowerNode Aux;
    Aux = Parking;
    while (Aux != NULL)
    {
        if (Aux->isRightOccupied == true) {
            cout << Aux->rightCar.licensePlate << ", ";
            cout << Aux->rightCar.size << ", ";
            cout << Aux->rightCar.weight << ". " << "CARRO DERECHO" << endl;                    
        }
        if (Aux->isLeftOccupied == true) {
            cout << Aux->leftCar.licensePlate << ", ";
            cout << Aux->leftCar.size << ", ";
            cout << Aux->leftCar.weight << ". " << "CARRO IZQUIERDO" << endl;

        }
        Aux = Aux->Next;
    }
    cout << endl;
}

void addCarToParking(PtrTParkingTowerNode& Parking, car* newCar) { 

    newCar->licensePlate = generateRandomT(100000, 999999);
    newCar->weight = generateRandomT(1, 3);                    //Random attributes to each car
    newCar->size = generateRandomT(1, 5);
    PtrTParkingTowerNode Aux;
    Aux = Parking;
    bool addedFlag = false; // Cambiado a false

    while (Aux != NULL) {
        if (Aux->isRightOccupied == false) { // Revision para planta baja
            Aux->rightCar.licensePlate = newCar->licensePlate;
            Aux->rightCar.size = newCar->size;
            Aux->rightCar.weight = newCar->weight;
            Aux->isRightOccupied = true; // Cambiado a true
            int randomCode = generateRandomT(100000, 999999);
            Aux->rightReturnCode = randomCode;
            cout << "Se añadio un carro en el espacio " << Aux->parkingSpot << " derecho, con el codigo de retorno " << randomCode << endl;
            addedFlag = true; // Cambiado a true
            break;
        }
        else if (Aux->isLeftOccupied == false) {   // Revision planta alta
            Aux->leftCar.licensePlate = newCar->licensePlate;
            Aux->leftCar.size = newCar->size;
            Aux->leftCar.weight = newCar->weight;
            Aux->isLeftOccupied = true; // Cambiado a true
            int randomCode = generateRandomT(100000, 999999);
            Aux->leftReturnCode = randomCode;
            cout << "Se añadio un carro en el espacio " << Aux->parkingSpot << " izquierdo, con el codigo de retorno " << randomCode << endl;
            addedFlag = true; // Cambiado a true
            break;
        }
        Aux = Aux->Next;
    }
    if (addedFlag == false) { cout << "El parque esta lleno" << endl; } // Cambiado == false
}

void startTowerSimulation() {

    while (true) {
        int randomChance = generateRandomT(1, 1); //Chance treshold
        if (randomChance == 1) {
            addCarToParking(Parking, new(car));

        }
        std::this_thread::sleep_for(std::chrono::seconds(5)); // Sleep treshold
    }
}

void userInputT() {
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

void parkingTower() {
    Parking = NULL;
    initializeParking(Parking);
    thread simThread(startTowerSimulation);
    thread userInputThread(userInputT);

    simThread.join();
    userInputThread.join();
}
