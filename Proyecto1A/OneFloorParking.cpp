#include "Structures.h"
#include <thread> // For std::this_thread::sleep_for

int MAX_SIZE = 25;
PtrTParkingNode newSpace;
PtrTParkingNode Parking;

//Parqueo de un solo piso
void initializeParking(PtrTParkingNode& Parking) {
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
			// Add your switch cases here
		case 'p': //Parking listing
			carList(Parking);
			break;
		default:
			cout << "Invalid choice. Try again." << endl;
		}
	}
}


void main() {
	Parking = NULL;
	initializeParking(Parking);
	thread simThread(startSimulation);
	thread userInputThread(userInput);

	simThread.join();
	userInputThread.join();

}
