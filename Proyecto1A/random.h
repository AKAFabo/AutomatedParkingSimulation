#pragma once
using namespace std;
#include <random>

int generateRandom(int MIN, int MAX) {
    random_device rd;
    mt19937 gen(rd());    //Creates a randon number for a new code to take out a car or license plate
    uniform_int_distribution<> dis(MIN, MAX);
    int randomCode = dis(gen);
    return randomCode;
}