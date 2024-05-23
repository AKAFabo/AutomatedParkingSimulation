#pragma once

#include <iostream>
#include <random>
#include "car.h"
#include <thread>
using namespace std;

typedef struct ParkingTowerNode {
    int parkingSpot;
    int leftReturnCode;
    int rightReturnCode;
    bool isLeftOccupied;
    bool isRightOccupied;
    car leftCar;
    car rightCar;
    ParkingTowerNode* Next;
}*PtrTParkingTowerNode;


int generateRandom(int MIN, int MAX);
PtrTParkingTowerNode createParkingTowerNode(int slotNumber);
void addParkingNode(PtrTParkingTowerNode& Parking, PtrTParkingTowerNode& newSpot);
int carList(PtrTParkingTowerNode& Parking);
void addCarToParking(PtrTParkingTowerNode& Parking, car* newCar);
void initializeParking(PtrTParkingTowerNode& Parking);
void parkingTowerSimulator();
