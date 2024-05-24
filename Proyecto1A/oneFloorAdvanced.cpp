#include "oneFloorAdvanced.h"

#include <iostream>
#include <math.h>
#include <windows.h>
#include <stdlib.h>
#include <cstdlib>
#include <time.h>
#include <vector> 
#include <thread> 
#include <string> 

#include <allegro5/allegro.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>

using namespace std;
#pragma warning(disable:4996);
#define FPS 10

//int carPosAP[20][2] = { {48, 20} ,{48, 116} ,{48, 212},{48, 308},{48, 404},{253, 20},{253, 116} ,{253, 212},{253, 308},{253, 404}, {20,45},{20,141},{20,237},{20,333},{20,429}, { 225,45 },{225,141},{225,237},{225,333},{225,429} };
int carPosAP[20][2] = { {48, 20}, {20,45}, {48, 116}, {20,141}, {48, 212}, {20,237}, {48, 308}, {20,333}, {48, 404}, {20,429}, {253, 20}, {225,45}, {253, 116}, {225,141}, {253, 212}, {225,237}, {253, 308}, {225,333}, {253, 404}, {225,429} };
vector<pair<int, int>> carsListAP;
int contCarsAP;

int exitCarAP = -1;
int addedCarAP = -1;
int randomCarAP = -1;
bool RunningThreadFlagAP = false;

PtrTAdvancedParkingNode newSpace;
PtrTAdvancedParkingNode Parking;

int generateRandomA(int MIN, int MAX) {

    random_device rd;
    mt19937 gen(rd());   
    uniform_int_distribution<> dis(MIN, MAX);

    int randomCode = dis(gen);

    return randomCode;
}

PtrTAdvancedParkingNode createAdvancedParkingNode(int slotNumber) {

    PtrTAdvancedParkingNode slot = new(AdvancedParkingNode);

    slot->parkingSpot = slotNumber;
    slot->isLowerOccupied = false;
    slot->isUpperOccupied = false;
    slot->Next = NULL;

    return slot;
}

void initializeParking(PtrTAdvancedParkingNode& Parking) {

    static int MAX_SIZE = 10;

    PtrTAdvancedParkingNode newSpace;

    for (int i = MAX_SIZE; i > 0; --i) {
        newSpace = createAdvancedParkingNode(i);
        addParkingNode(Parking, newSpace);
    }
}

void addParkingNode(PtrTAdvancedParkingNode& Parking, PtrTAdvancedParkingNode& newSpot) {
    newSpot->Next = Parking;
    Parking = newSpot;
}

void randomFillparking(PtrTAdvancedParkingNode Parking) {

    int randomChance = generateRandomA(7, 12);

    for (int i = 0; i <= randomChance; i++) {

        addCarToParking(Parking, new(car));
    }
}

int carList(PtrTAdvancedParkingNode& Parking) {

    PtrTAdvancedParkingNode Aux;
    Aux = Parking;

    int cont = 0;

    while (Aux != NULL) {

        if (Aux->isUpperOccupied == true) {
            cont++;
        }

        if (Aux->isLowerOccupied == true) {
            cont++;
        }

        Aux = Aux->Next;
    }

    return cont;
}

void addCarToParking(PtrTAdvancedParkingNode& Parking, car* newCar) { 

    newCar->licensePlate = generateRandomA(100000, 999999);
    newCar->weight = generateRandomA(1, 3);                    
    newCar->size = generateRandomA(1, 5);

    PtrTAdvancedParkingNode Aux;
    Aux = Parking;

    bool addedFlag = false; 

    while (Aux != NULL) {

        if (Aux->isLowerOccupied == false) { 

            Aux->lowerCar.licensePlate = newCar->licensePlate;
            Aux->lowerCar.size = newCar->size;
            Aux->lowerCar.weight = newCar->weight;
            Aux->isLowerOccupied = true; 

            int randomCode = generateRandomA(100000, 999999);
            Aux->lowerReturnCode = randomCode;

            cout << "Se agrego un carro en el espacio " << Aux->parkingSpot << " inferior, con el codigo de retorno " << randomCode << endl;

            addedFlag = true;
            break;
        }

        else if (Aux->isUpperOccupied == false) {  

            Aux->upperCar.licensePlate = newCar->licensePlate;
            Aux->upperCar.size = newCar->size;
            Aux->upperCar.weight = newCar->weight;
            Aux->isUpperOccupied = true; 

            int randomCode = generateRandomA(100000, 999999);
            Aux->upperReturnCode = randomCode;

            cout << "Se agrego un carro en el espacio " << Aux->parkingSpot << " superior, con el codigo de retorno " << randomCode << endl;

            addedFlag = true; 
            break;
        }
        Aux = Aux->Next;
    }
    if (addedFlag == false) { cout << "El parqueo esta lleno" << endl; } 
}

void deleteParking(PtrTAdvancedParkingNode& Parking) {

    PtrTAdvancedParkingNode Aux;
    Aux = Parking;

    while (Aux != NULL) {

        Parking = Parking->Next;
        delete(Aux);
        Aux = Parking;
    }
}

void copyBitmapAP(ALLEGRO_BITMAP* dest[], ALLEGRO_BITMAP* src[], int len) {

    for (int i = 0; i < len; i++) {

        dest[i] = src[i];
    }
}

auto compareAP = [](const pair<int, int>& a, const pair<int, int>& b) {
    return a.first < b.first;
    };

pair<int, int> popElementAP() {

    int pos;
    pair<int, int> popedPair;
    int random = generateRandomA(0, 1);

    PtrTAdvancedParkingNode Aux;

    if (contCarsAP == 0) {
        contCarsAP = carList(Parking);
        return { 0,0 };
    }

    while (true) {

        pos = generateRandomA(1, carsListAP.size());
        Aux = Parking;

        for (int i = 0; i < carsListAP.size(); i++) {

            if (carsListAP[i].first == pos) {

                popedPair = carsListAP[i];
                carsListAP.erase(carsListAP.begin() + i);
            }
        }

        for (int i = 0; i < pos / 2; ++i) {

            if (Aux != nullptr) {
                Aux = Aux->Next;
            }
            else {
                break;
            }
        }

        if (Aux != NULL) {

            if (Aux->isLowerOccupied == true and pos % 2 == 0) {

                cout << "Se elimino un carro en el espacio " << Aux->parkingSpot << " inferior, con el codigo de retorno " << Aux->upperReturnCode << endl;

                Aux->isLowerOccupied = false;
                Aux->upperReturnCode = NULL;
                break;
            }

            if (Aux->isUpperOccupied == true and pos % 2 == 1) {

                cout << "Se elimino un carro en el espacio " << Aux->parkingSpot << " superior, con el codigo de retorno " << Aux->isLowerOccupied << endl;

                Aux->isUpperOccupied = false;
                Aux->lowerReturnCode = NULL;
                break;
            }

        }
    }

    contCarsAP = carList(Parking);
    return popedPair;
}

void paintCars() {

    ALLEGRO_BITMAP* upperParking = al_load_bitmap("Recursos/UpperParking.png");
    ALLEGRO_BITMAP* lowerParking = al_load_bitmap("Recursos/UpperParking02.png");

    ALLEGRO_BITMAP* Cars[4];
    Cars[0] = al_load_bitmap("Recursos/Cars/Car01.png");
    Cars[1] = al_load_bitmap("Recursos/Cars/Car02.png");
    Cars[2] = al_load_bitmap("Recursos/Cars/Car03.png");
    Cars[3] = al_load_bitmap("Recursos/Cars/Car04.png");


    for (int i = 0; i < carsListAP.size(); ++i) {

        if (carsListAP[i].first % 2 == 0) {
            al_draw_bitmap(Cars[carsListAP[i].second], carPosAP[carsListAP[i].first][1], carPosAP[carsListAP[i].first][0], NULL);
        }
    }

    al_draw_bitmap(upperParking, -7, 0, NULL);
    al_draw_bitmap(lowerParking, -7, 208, NULL);

    for (int i = 0; i < carsListAP.size(); ++i) {

        if (carsListAP[i].first % 2 == 1) {
            al_draw_bitmap(Cars[carsListAP[i].second], carPosAP[carsListAP[i].first][1], carPosAP[carsListAP[i].first][0], NULL);
        }
    }

}

// Simulador

void* parkingExitThreadAP(ALLEGRO_THREAD* thr, void* arg) {

    if (contCarsAP <= 0) {
        al_show_native_message_box(NULL, "Ventana Emergente", "Error", "El parqueo esta vacio", NULL, ALLEGRO_MESSAGEBOX_ERROR);
        return NULL;
    }

    const int RX = 510;
    const int RY = 350;

    ALLEGRO_DISPLAY* display;
    al_set_new_display_flags(ALLEGRO_WINDOWED);
    display = al_create_display(RX, RY);
    al_set_window_position(display, 200, 200);
    al_set_window_title(display, "Smart Parking Simulator");

    if (!display) {
        al_show_native_message_box(NULL, "Ventana Emergente", "Error", "No se puede crear la pantalla", NULL, ALLEGRO_MESSAGEBOX_ERROR);
        return NULL;
    }

    //Se crean los temporizadores, la cola de eventos y se cargan los recursos
    ALLEGRO_TIMER* timer = al_create_timer(1.0 / 60);
    ALLEGRO_EVENT_QUEUE* eventQueue = al_create_event_queue();
    ALLEGRO_FONT* fontTitle = al_load_font("pixel.ttf", 60, NULL);
    ALLEGRO_FONT* fontSubtitle = al_load_font("pixel.ttf", 50, NULL);
    ALLEGRO_BITMAP* ParkingSlot = al_load_bitmap("Recursos/ParkingSlot01.png");
    ALLEGRO_BITMAP* upperParking = al_load_bitmap("Recursos/UpperParking.png");
    ALLEGRO_BITMAP* lowerParking = al_load_bitmap("Recursos/UpperParking02.png");

    ALLEGRO_BITMAP* Cars[4];
    Cars[0] = al_load_bitmap("Recursos/Cars/Car01.png");
    Cars[1] = al_load_bitmap("Recursos/Cars/Car02.png");
    Cars[2] = al_load_bitmap("Recursos/Cars/Car03.png");
    Cars[3] = al_load_bitmap("Recursos/Cars/Car04.png");

    // Se registran las fuentes de eventos en la cola de eventos
    al_register_event_source(eventQueue, al_get_timer_event_source(timer));
    al_register_event_source(eventQueue, al_get_display_event_source(display));
    al_start_timer(timer);

    bool done = true;

    int carMovementYPos = 0;
    int carMovementXPos = -200;

    pair<int, int> popedPair = make_pair(-1, -1);

    while (done) { //Cola de eventos

        ALLEGRO_EVENT events;
        al_wait_for_event(eventQueue, &events);

        if (events.timer.source == timer) {

            al_clear_to_color(al_map_rgb(0, 0, 0));
            al_draw_bitmap(ParkingSlot, 0, 0, NULL);

            paintCars();

            if (popedPair.first == -1 && popedPair.second == -1) {
                popedPair = popElementAP();
            }

            if (popedPair.first <= 9) {

                if (carMovementYPos == 0) {
                    carMovementYPos = carPosAP[popedPair.first][0];
                }

                if (carMovementYPos >= 145) {
                    carMovementYPos = 145;

                    if (carMovementXPos == -200) {
                        carMovementXPos = carPosAP[popedPair.first][1];
                    }

                    else {

                        carMovementXPos = carMovementXPos - 5;
                        al_draw_bitmap(Cars[popedPair.second], carMovementXPos, carMovementYPos, NULL);
                        paintCars();

                    }

                    if (carMovementXPos < -80) {
                        al_destroy_display(display);
                        done = false;
                        exitCarAP = popedPair.second;
                        return NULL;
                    }
                }

                else {

                    carMovementYPos = carMovementYPos + 5;

                    al_draw_bitmap(Cars[popedPair.second], carPosAP[popedPair.first][1], carMovementYPos, NULL);
                       
                    if (popedPair.first % 2 == 0) {
                        paintCars();
                    }

                }
            }

            if (popedPair.first >= 10) {

                if (carMovementYPos == 0) {
                    carMovementYPos = carPosAP[popedPair.first][0];
                }

                if (carMovementYPos <= 145) {
                    carMovementYPos = 145;

                    if (carMovementXPos == -200) {
                        carMovementXPos = carPosAP[popedPair.first][1];
                    }

                    else {
                        carMovementXPos = carMovementXPos - 5;
                        al_draw_bitmap(Cars[popedPair.second], carMovementXPos, carMovementYPos, NULL);
                        paintCars();
                    }

                    if (carMovementXPos < -80) {

                        al_destroy_display(display);
                        done = false;
                        exitCarAP = popedPair.second;
                        return NULL;
                    }
                }

                else {
                    carMovementYPos = carMovementYPos - 5;
                    al_draw_bitmap(Cars[popedPair.second], carPosAP[popedPair.first][1], carMovementYPos, NULL);
                    if (popedPair.first % 2 == 0) {
                        paintCars();
                    }
                }
            }
        }

        if (RunningThreadFlagAP == false) {
            al_destroy_display(display);
            done = false;
            return NULL;
        }

        al_flip_display();

    }

    return NULL;
}

void* parkingEnterThreadAP(ALLEGRO_THREAD* thr, void* arg) {

    if (contCarsAP >= 19) {
        al_show_native_message_box(NULL, "Ventana Emergente", "Error", "El parqueo esta lleno", NULL, ALLEGRO_MESSAGEBOX_ERROR);
        return NULL;
    }

    const int RX = 510;
    const int RY = 350;

    ALLEGRO_DISPLAY* display;
    al_set_new_display_flags(ALLEGRO_WINDOWED);
    display = al_create_display(RX, RY);
    al_set_window_position(display, 200, 200);
    al_set_window_title(display, "Smart Parking Simulator");

    if (!display) {
        al_show_native_message_box(NULL, "Ventana Emergente", "Error", "No se puede crear la pantalla", NULL, ALLEGRO_MESSAGEBOX_ERROR);
        return NULL;
    }

    //Se crean los temporizadores, la cola de eventos y se cargan los recursos
    ALLEGRO_TIMER* timer = al_create_timer(1.0 / 60);
    ALLEGRO_EVENT_QUEUE* eventQueue = al_create_event_queue();
    ALLEGRO_FONT* fontTitle = al_load_font("pixel.ttf", 60, NULL);
    ALLEGRO_FONT* fontSubtitle = al_load_font("pixel.ttf", 50, NULL);
    ALLEGRO_BITMAP* ParkingSlot = al_load_bitmap("Recursos/ParkingSlot01.png");
    ALLEGRO_BITMAP* upperParking = al_load_bitmap("Recursos/UpperParking.png");
    ALLEGRO_BITMAP* lowerParking = al_load_bitmap("Recursos/UpperParking02.png");

    ALLEGRO_BITMAP* Cars[4];
    Cars[0] = al_load_bitmap("Recursos/Cars/Car01.png");
    Cars[1] = al_load_bitmap("Recursos/Cars/Car02.png");
    Cars[2] = al_load_bitmap("Recursos/Cars/Car03.png");
    Cars[3] = al_load_bitmap("Recursos/Cars/Car04.png");

    // Se registran las fuentes de eventos en la cola de eventos
    al_register_event_source(eventQueue, al_get_timer_event_source(timer));
    al_register_event_source(eventQueue, al_get_display_event_source(display));
    al_start_timer(timer);

    //Varibles necesarias
    bool done = true;
    bool added = false;

    int carMovementYPos = 147;
    int carMovementXPos = -200;

    pair<int, int> enterPair = make_pair(-1, -1);

    while (done) { //Cola de eventos

        ALLEGRO_EVENT events;
        al_wait_for_event(eventQueue, &events);

        if (events.timer.source == timer) {

            al_clear_to_color(al_map_rgb(0, 0, 0));
            al_draw_bitmap(ParkingSlot, 0, 0, NULL);

            paintCars();
            sort(carsListAP.begin(), carsListAP.end(), compareAP);

            if (enterPair.first == -1 and enterPair.second == -1) {

                for (int i = 0; i < carsListAP.size(); i++) {

                    if (carsListAP[i].first != i) {

                        addCarToParking(Parking, new(car));
                        enterPair = make_pair(i, randomCarAP);
                        added = true;
                        addedCarAP = 1;
                        break;

                    }
                }
            }

            if (added == false) {

                addCarToParking(Parking, new(car));
                enterPair = make_pair(carsListAP.size(), randomCarAP);
                added = true;
                addedCarAP = 1;

            }

            if (enterPair.first <= 9) {

                if (carMovementXPos == -200) {
                    carMovementXPos = -40;
                }

                if (carMovementXPos >= carPosAP[enterPair.first][1]) {

                    carMovementXPos = carPosAP[enterPair.first][1];

                    if (carMovementYPos <= carPosAP[enterPair.first][0]) {

                        carsListAP.push_back(enterPair);
                        al_destroy_display(display);
                        done = false;
                        contCarsAP = carList(Parking);
                        return NULL;
                    }

                    else {
                        carMovementYPos = carMovementYPos - 7;
                        al_draw_bitmap(Cars[enterPair.second], carMovementXPos, carMovementYPos, NULL);

                        if (enterPair.first % 2 == 0) {
                            paintCars();
                        }
                    }
                }

                else {
                    carMovementXPos = carMovementXPos + 7;
                    al_draw_bitmap(Cars[enterPair.second], carMovementXPos, carMovementYPos, NULL);
                    paintCars();

                } 
            }

            if (enterPair.first >= 10) {

                if (carMovementXPos == -200) {
                    carMovementXPos = -40;
                }

                if (carMovementXPos >= carPosAP[enterPair.first][1]) {
                    carMovementXPos = carPosAP[enterPair.first][1];

                    if (carMovementYPos >= carPosAP[enterPair.first][0]) {

                        carsListAP.push_back(enterPair);
                        al_destroy_display(display);
                        done = false;
                        contCarsAP = carList(Parking);
                        return NULL;
                    }

                    else {
                        carMovementYPos = carMovementYPos + 7;
                        al_draw_bitmap(Cars[enterPair.second], carMovementXPos, carMovementYPos, NULL);

                        if (enterPair.first % 2 == 0) {
                            paintCars();
                        }
                    }
                }

                else {
                    carMovementXPos = carMovementXPos + 7;
                    al_draw_bitmap(Cars[enterPair.second], carMovementXPos, carMovementYPos, NULL);
                    paintCars();

                }
            }
        }
        
        if (RunningThreadFlagAP == false) {

            al_destroy_display(display);
            done = false;
            break;
        }

        al_flip_display();

    }

    return NULL;

}

void parkingAdvancedSimulator() {

    initializeParking(Parking);
    randomFillparking(Parking);

    contCarsAP = carList(Parking);

    for (int i = 0; i < contCarsAP; ++i) {

        int random = generateRandomA(0, 3);
        carsListAP.push_back(make_pair(i, random));
    }

    this_thread::sleep_for(200ms);

    const int RX = 1280;
    const int RY = 720;

    ALLEGRO_DISPLAY* display;
    al_set_new_display_flags(ALLEGRO_WINDOWED);
    display = al_create_display(RX, RY);
    al_set_window_position(display, 200, 200);
    al_set_window_title(display, "Smart Parking Simulator");

    if (!display) {
        al_show_native_message_box(NULL, "Ventana Emergente", "Error", "No se puede crear la pantalla", NULL, ALLEGRO_MESSAGEBOX_ERROR);
        return;
    }

    //Se crean los temporizadores, la cola de eventos y se cargan los recursos
    ALLEGRO_TIMER* timer = al_create_timer(1.0 / FPS);
    ALLEGRO_EVENT_QUEUE* eventQueue = al_create_event_queue();
    ALLEGRO_FONT* fontTitle = al_load_font("pixel.ttf", 60, NULL);
    ALLEGRO_FONT* fontSubtitle = al_load_font("pixel.ttf", 50, NULL);
    ALLEGRO_FONT* fonText = al_load_font("pixel.ttf", 40, NULL);
    ALLEGRO_BITMAP* background = al_load_bitmap("Recursos/MenuBackground.png");
    ALLEGRO_BITMAP* building = al_load_bitmap("Recursos/Building.png");
    ALLEGRO_BITMAP* cloud = al_load_bitmap("Recursos/Clouds.png");

    ALLEGRO_BITMAP* character1[13];
    character1[0] = al_load_bitmap("Recursos/Characters/Character1_walk0.png");
    character1[1] = al_load_bitmap("Recursos/Characters/Character1_walk1.png");
    character1[2] = al_load_bitmap("Recursos/Characters/Character1_walk2.png");
    character1[3] = al_load_bitmap("Recursos/Characters/Character1_walk3.png");
    character1[4] = al_load_bitmap("Recursos/Characters/Character1_walk4.png");
    character1[5] = al_load_bitmap("Recursos/Characters/Character1_walk5.png");
    character1[6] = al_load_bitmap("Recursos/Characters/Character1_walk6.png");
    character1[7] = al_load_bitmap("Recursos/Characters/Character1_walk7.png");
    character1[8] = al_load_bitmap("Recursos/Characters/Character1_wait_0.png");
    character1[9] = al_load_bitmap("Recursos/Characters/Character1_wait_1.png");
    character1[10] = al_load_bitmap("Recursos/Characters/Character1_wait_2.png");
    character1[11] = al_load_bitmap("Recursos/Characters/Character1_wait_3.png");
    character1[12] = al_load_bitmap("Recursos/Characters/Character1_wait_4.png");

    ALLEGRO_BITMAP* character2[13];
    character2[0] = al_load_bitmap("Recursos/Characters/Character2_walk0.png");
    character2[1] = al_load_bitmap("Recursos/Characters/Character2_walk1.png");
    character2[2] = al_load_bitmap("Recursos/Characters/Character2_walk2.png");
    character2[3] = al_load_bitmap("Recursos/Characters/Character2_walk3.png");
    character2[4] = al_load_bitmap("Recursos/Characters/Character2_walk4.png");
    character2[5] = al_load_bitmap("Recursos/Characters/Character2_walk5.png");
    character2[6] = al_load_bitmap("Recursos/Characters/Character2_walk6.png");
    character2[7] = al_load_bitmap("Recursos/Characters/Character2_walk7.png");
    character2[8] = al_load_bitmap("Recursos/Characters/Character2_wait_0.png");
    character2[9] = al_load_bitmap("Recursos/Characters/Character2_wait_1.png");
    character2[10] = al_load_bitmap("Recursos/Characters/Character2_wait_2.png");
    character2[11] = al_load_bitmap("Recursos/Characters/Character2_wait_3.png");
    character2[12] = al_load_bitmap("Recursos/Characters/Character2_wait_4.png");

    ALLEGRO_BITMAP* sideCars[4];
    sideCars[0] = al_load_bitmap("Recursos/Cars/CarSide01.png");
    sideCars[1] = al_load_bitmap("Recursos/Cars/CarSide02.png");
    sideCars[2] = al_load_bitmap("Recursos/Cars/CarSide03.png");
    sideCars[3] = al_load_bitmap("Recursos/Cars/CarSide04.png");

    // Se registran las fuentes de eventos en la cola de eventos
    al_register_event_source(eventQueue, al_get_timer_event_source(timer));
    al_register_event_source(eventQueue, al_get_timer_event_source(timer));
    al_register_event_source(eventQueue, al_get_display_event_source(display));
    al_register_event_source(eventQueue, al_get_keyboard_event_source());
    al_register_event_source(eventQueue, al_get_mouse_event_source());
    al_start_timer(timer);

    bool done = true;

    int mouseX = 0;
    int mouseY = 0;

    int actionRandom = generateRandomA(0, 1);
    int randomCharacter = generateRandomA(0, 1);
    randomCarAP = generateRandomA(0, 3);

    int characterFrameWalk = 0;
    int characterFrameWait = 8;

    int characterPos = -180;
    int characterYPos = 495;

    int carPos = -180;
    int carYPos = 560;
    int carExitPos = 620;
    float cloudPos = 0;

    ALLEGRO_THREAD* thread = NULL;
    ALLEGRO_BITMAP* character[13];

    while (done) { //Cola de eventos

        ALLEGRO_EVENT events;
        al_wait_for_event(eventQueue, &events);

        if (events.type == ALLEGRO_EVENT_MOUSE_AXES) { //Posicion del mouse

            mouseX = events.mouse.x;
            mouseY = events.mouse.y;
            //cout << mouseX << " , " << mouseY << endl;
        }

        if (events.type == ALLEGRO_EVENT_TIMER) {

            if (events.timer.source == timer) {


                cloudPos += 0.2;
                if (cloudPos >= 1280) {
                    cloudPos = 0;
                }

                al_clear_to_color(al_map_rgb(0, 0, 0));
                al_draw_bitmap(background, 0, 0, NULL);
                al_draw_bitmap(cloud, cloudPos, 0, NULL);
                al_draw_bitmap(cloud, cloudPos - 1280, 0, NULL);
                al_draw_bitmap(building, 526, 0, NULL);
                al_draw_text(fontTitle, al_map_rgb(0, 0, 0), RX / 2, 20, ALLEGRO_ALIGN_CENTRE, "Simulador de Parqueo Automatico");
                al_draw_text(fontSubtitle, al_map_rgb(0, 0, 0), RX / 2, 90, ALLEGRO_ALIGN_CENTRE, "Un piso en una sola planta");
                al_draw_text(fonText, al_map_rgb(0, 0, 0), 50, 660, ALLEGRO_ALIGN_CENTRE, "Salir");
                al_draw_text(fontTitle, al_map_rgb(250, 150, 0), (RX / 2) + 2, 22, ALLEGRO_ALIGN_CENTRE, "Simulador de Parqueo Automatico");
                al_draw_text(fontSubtitle, al_map_rgb(250, 150, 0), (RX / 2) + 2, 92, ALLEGRO_ALIGN_CENTRE, "Un piso en una sola planta");
                al_draw_text(fonText, al_map_rgb(250, 150, 0), 52, 662, ALLEGRO_ALIGN_CENTRE, "Salir");

                if (actionRandom == 0) {

                    if (randomCharacter == 1) { copyBitmapAP(character, character1, 13); }
                    else { copyBitmapAP(character, character2, 13); }

                    characterPos = characterPos + 10;

                    if (characterPos >= 570) {
                        characterPos = 570;
                    }

                    if (characterPos < 570) {
                        if (characterFrameWalk >= 7) { characterFrameWalk = 0; }
                        al_draw_bitmap(character[characterFrameWalk], characterPos, characterYPos, NULL);
                        characterFrameWalk++;
                    }

                    else {

                        if (characterFrameWait == 12) { characterFrameWait = 8; }
                        al_draw_bitmap(character[characterFrameWait], characterPos, characterYPos, NULL);
                        characterFrameWait++;

                        if (thread == NULL) {

                            ALLEGRO_THREAD* thread = al_create_thread(parkingExitThreadAP, NULL);

                            if (RunningThreadFlagAP == false) {
                                al_start_thread(thread);
                                RunningThreadFlagAP = true;
                            }
                        }

                        if (exitCarAP != -1 and RunningThreadFlagAP != false) {

                            if (carExitPos > 1400) {

                                this_thread::sleep_for(200ms);

                                cout << "Se extrajo el vehiculo con exito" << endl;

                                exitCarAP = -1;
                                RunningThreadFlagAP = false;
                                al_destroy_thread(thread);
                                characterPos = -180;
                                characterYPos = 495;
                                carExitPos = 620;
                                actionRandom = generateRandomA(0, 1);
                                randomCharacter = generateRandomA(0, 1);
                            }

                            else {
                                carExitPos = carExitPos + 20;
                                characterYPos = -100;
                                al_draw_bitmap(sideCars[exitCarAP], carExitPos, 560, NULL);
                            }
                        }
                    }
                }

                if (actionRandom == 1) {

                    if (carPos >= 622) {
                        carPos = 622;
                    }

                    if (carPos != 622) {
                        carPos = carPos + 20;
                    }

                    al_draw_bitmap(sideCars[randomCarAP], carPos, carYPos, NULL);

                    if (carPos == 622 and thread == NULL) {

                        ALLEGRO_THREAD* thread = al_create_thread(parkingEnterThreadAP, NULL);

                        if (RunningThreadFlagAP == false) {
                            al_start_thread(thread);
                            RunningThreadFlagAP = true;
                            randomCharacter = generateRandomA(0, 1);

                        }
                    }

                    if (addedCarAP != -1 and RunningThreadFlagAP != false) {

                        if (characterPos > 1400) {

                            this_thread::sleep_for(200ms);

                            cout << "Se guardo el vehiculo con exito" << endl;

                            addedCarAP = -1;
                            RunningThreadFlagAP = false;
                            al_destroy_thread(thread);
                            thread = NULL;
                            characterPos = -180;
                            carYPos = 560;
                            carPos = -180;
                            actionRandom = generateRandomA(0, 1);
                            randomCarAP = generateRandomA(0, 3);
                        }

                        else {

                            if (characterPos == -180) { characterPos = 790; }

                            if (randomCharacter == 1) { copyBitmapAP(character, character1, 13); }
                            else { copyBitmapAP(character, character2, 13); }

                            characterPos = characterPos + 10;

                            if (characterFrameWalk >= 7) { characterFrameWalk = 0; }
                            al_draw_bitmap(character[characterFrameWalk], characterPos, characterYPos, NULL);
                            characterFrameWalk++;
                            carYPos = -100;

                        }
                    }
                }
            }
        }

        //Salir
        if ((mouseX >= 20 && mouseX <= 80) && (mouseY >= 670.0 && mouseY <= 695.0)) {
            al_draw_text(fonText, al_map_rgb(250, 250, 250), 50, 660, ALLEGRO_ALIGN_CENTRE, "Salir");

            if (events.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {

                if (events.mouse.button & 1) {
                    al_destroy_thread(thread);
                    al_destroy_display(display);
                    RunningThreadFlagAP = false;
                    carsListAP.clear();
                    deleteParking(Parking);
                    done = false;
                    break;
                }
            }
        }

        if (events.type == ALLEGRO_EVENT_KEY_DOWN) {

            switch (events.keyboard.keycode) {

            case ALLEGRO_KEY_ESCAPE:
                al_destroy_thread(thread);
                al_destroy_display(display);
                RunningThreadFlagAP = false;
                carsListAP.clear();
                deleteParking(Parking);
                done = false;
                break;
            }
        }

        else if (events.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            al_destroy_thread(thread);
            al_destroy_display(display);
            RunningThreadFlagAP = false;
            carsListAP.clear();
            deleteParking(Parking);
            done = false;
            break;
        }

        al_flip_display();
    }

}
