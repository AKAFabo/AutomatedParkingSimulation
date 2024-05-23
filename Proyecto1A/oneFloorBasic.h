#pragma once

#include <iostream>
#include <random>
#include <thread>
#include "car.h"

using namespace std;

typedef struct BasicParkingNode {
    int parkingSpot;
    int returnCode;
    bool isOccupied;
    car carInSpot;
    BasicParkingNode* Next;
}*PtrTBasicParkingNode;

PtrTBasicParkingNode createBasicParkingNode(int slotNumber);
void addParkingNode(PtrTBasicParkingNode& Parking, PtrTBasicParkingNode& newSpot);
int carList(PtrTBasicParkingNode& Parking);
void addCarToParking(PtrTBasicParkingNode& Parking, car* newCar);
void initializeParking(PtrTBasicParkingNode& Parking);
void oneFloorBasicSimulator();
void randomFillparking();

