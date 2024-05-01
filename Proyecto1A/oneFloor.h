#pragma once

#include <iostream>
#include <random>
#include "car.h"
#include <thread>
using namespace std;

typedef struct ParkingNode {
    int parkingSpot;
    int returnCode;
    bool isOccupied;
    car carInSpot;
    ParkingNode* Next;
}*PtrTParkingNode;


int generateRandom(int MIN, int MAX);
PtrTParkingNode createParkingNode(int slotNumber);
void addParkingNode(PtrTParkingNode& Parking, PtrTParkingNode& newSpot);
void carList(PtrTParkingNode& Parking);
void addCarToParking(PtrTParkingNode& Parking, car* newCar);
void initializeParking(PtrTParkingNode& Parking);
void oneFloor();

