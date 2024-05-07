#pragma once

#include <iostream>
#include <random>
#include "car.h"
#include <thread>
using namespace std;

typedef struct AdvancedParkingNode {
    int parkingSpot;
    int upperReturnCode;
    int lowerReturnCode;
    bool isLowerOccupied;
    bool isUpperOccupied;
    car upperCar;
    car lowerCar;
    AdvancedParkingNode* Next;
}*PtrTAdvancedParkingNode;


PtrTAdvancedParkingNode createAdvancedParkingNode(int slotNumber);
void addParkingNode(PtrTAdvancedParkingNode& Parking, PtrTAdvancedParkingNode& newSpot);
void carList(PtrTAdvancedParkingNode& Parking);
void addCarToParking(PtrTAdvancedParkingNode& Parking, car* newCar);
void initializeParking(PtrTAdvancedParkingNode& Parking);
void oneFloorAdvanced();