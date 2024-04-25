#include <iostream>
#include <random>
using namespace std;

typedef struct car {
	int weight;
	int size;
	int licensePlate;
};

typedef struct ParkingNode {
	int parkingSpot;
	int returnCode;
	bool isOccupied;
	car carInSpot;
	ParkingNode* Next;
}*PtrTParkingNode;

int generateRandom(int MIN, int MAX) {
	random_device rd;
	mt19937 gen(rd());	//Creates a randon number for a new code to take out a car
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

void carList (PtrTParkingNode& Parking)
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
	}
	cout << endl;
}

void addCarToParking(PtrTParkingNode& Parking, car* newCar) {

	newCar->licensePlate = generateRandom(100000, 999999);
	newCar->weight = generateRandom(1, 3);					//Random attributes to each car
	newCar->size = generateRandom(1, 5);
	PtrTParkingNode Aux;
	Aux = Parking;
	bool addedFlag = 0;

	while (Aux != NULL) {
		if (Aux->isOccupied == 0) {
			Aux->carInSpot.licensePlate = newCar->licensePlate;
			Aux->carInSpot.size = newCar->size;
			Aux->carInSpot.weight = newCar->weight;
			Aux->isOccupied = 1;
			int randomCode = generateRandom(100000, 999999);
			Aux->returnCode = randomCode;
			cout << "Se añadio un carro en el espacio " << Aux->parkingSpot << " con el codigo de retorno " << Aux->returnCode << endl;
			addedFlag = 1;
			break;
		}
		Aux = Aux->Next;		
	}	
	if (addedFlag = 0) { cout << "El parque esta lleno" << endl; }
}