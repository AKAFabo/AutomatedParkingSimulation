#include "ParkingTower.h"

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
#define FPS 12.0


PtrTParkingTowerNode newSpace;
PtrTParkingTowerNode Parking;

int carPosPT[2][2] = { {10, 20} ,{215, 20} };
int listPosPT[20] = { 0,0,1,1,2,2,3,3,4,4,5,5,6,6,7,7,8,8,9,9 };
vector<pair<int, int>> carsListPT;
int contCarsPT;

int exitCarPT = -1;
int addedCarPT = -1;
int randomCarPT = -1;
bool RunningThreadFlagPT = false;

// Codigo

int generateRandomT(int MIN, int MAX) {

    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(MIN, MAX);

    int randomCode = dis(gen);

    return randomCode;
}

PtrTParkingTowerNode createParkingTowerNode(int slotNumber) {

    PtrTParkingTowerNode slot = new(ParkingTowerNode);

    slot->parkingSpot = slotNumber;
    slot->isRightOccupied = false;
    slot->isLeftOccupied = false;
    slot->Next = NULL;

    return slot;
}

void initializeParking(PtrTParkingTowerNode& Parking) {

    static int MAX_SIZE = 10;
    PtrTParkingTowerNode newSpace;

    for (int i = MAX_SIZE; i >= 0; --i) {
        newSpace = createParkingTowerNode(i);
        addParkingNode(Parking, newSpace);
    }

}

void addParkingNode(PtrTParkingTowerNode& Parking, PtrTParkingTowerNode& newSpot) {

    newSpot->Next = Parking;
    Parking = newSpot;
}

int carList(PtrTParkingTowerNode& Parking) {

    PtrTParkingTowerNode Aux;
    Aux = Parking;

    int cont = 0;

    while (Aux != NULL) {

        if (Aux->isLeftOccupied == true) {
            cont++;
        }

        if (Aux->isRightOccupied == true) {
            cont++;
        }

        Aux = Aux->Next;
    }

    return cont;
}

void addCarToParking(PtrTParkingTowerNode& Parking, car* newCar) {

    newCar->licensePlate = generateRandomT(100000, 999999);
    newCar->weight = generateRandomT(1, 3);
    newCar->size = generateRandomT(1, 5);

    PtrTParkingTowerNode Aux;
    Aux = Parking;

    bool addedFlag = false;

    while (Aux != NULL) {

        if (Aux->isRightOccupied == false) {

            Aux->rightCar.licensePlate = newCar->licensePlate;
            Aux->rightCar.size = newCar->size;
            Aux->rightCar.weight = newCar->weight;
            Aux->isRightOccupied = true;

            int randomCode = generateRandomT(100000, 999999);

            Aux->rightReturnCode = randomCode;

            cout << "Se agrego un carro en el piso " << Aux->parkingSpot << " superior, con el codigo de retorno " << randomCode << endl;

            addedFlag = true;

            break;
        }

        else if (Aux->isLeftOccupied == false) {

            Aux->leftCar.licensePlate = newCar->licensePlate;
            Aux->leftCar.size = newCar->size;
            Aux->leftCar.weight = newCar->weight;
            Aux->isLeftOccupied = true;

            int randomCode = generateRandomT(100000, 999999);
            Aux->leftReturnCode = randomCode;

            cout << "Se agrego un carro en el piso " << Aux->parkingSpot << " inferior, con el codigo de retorno " << randomCode << endl;

            addedFlag = true;

            break;
        }
        Aux = Aux->Next;
    }
    if (addedFlag == false) { cout << "El parque esta lleno" << endl; }
}

void randomFillparking(PtrTParkingTowerNode Parking) {

    int randomChance = generateRandomT(7, 12);

    for (int i = 0; i <= randomChance; i++) {

        addCarToParking(Parking, new(car));
    }
}

void deleteParking(PtrTParkingTowerNode& Parking) {

    PtrTParkingTowerNode Aux;
    Aux = Parking;

    while (Aux != NULL) {

        Parking = Parking->Next;
        delete(Aux);
        Aux = Parking;
    }
}

void copyBitmapPT(ALLEGRO_BITMAP* dest[], ALLEGRO_BITMAP* src[], int len) {

    for (int i = 0; i < len; i++) {

        dest[i] = src[i];
    }
}

auto comparePT = [](const pair<int, int>& a, const pair<int, int>& b) {
    return a.first < b.first;
    };

pair<int, int> popElementPT() {

    int pos;
    pair<int, int> popedPair;
    int random = generateRandomT(0, 1);

    PtrTParkingTowerNode Aux;

    if (contCarsPT == 0) {
        contCarsPT = carList(Parking);
        return { 0,0 };
    }

    while (true) {

        pos = generateRandomT(1, carsListPT.size());
        Aux = Parking;

        for (int i = 0; i < carsListPT.size(); i++) {

            if (carsListPT[i].first == pos) {

                popedPair = carsListPT[i];
                carsListPT.erase(carsListPT.begin() + i);
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

            if (Aux->isLeftOccupied == true and pos % 2 == 0) {

                cout << "Se elimino un carro en el piso " << Aux->parkingSpot << " superior, con el codigo de retorno " << Aux->leftReturnCode << endl;

                Aux->isLeftOccupied = false;
                Aux->leftReturnCode = NULL;
                break;
            }

            if (Aux->isRightOccupied == true and pos % 2 == 1) {

                cout << "Se elimino un carro en el piso " << Aux->parkingSpot << " inferior, con el codigo de retorno " << Aux->rightReturnCode << endl;

                Aux->isRightOccupied = false;
                Aux->rightReturnCode = NULL;
                break;
            }

        }
    }

    contCarsPT = carList(Parking);
    return popedPair;
}

// Simulador

void* parkingExitThreadPT(ALLEGRO_THREAD* thr, void* arg) {

    if (contCarsPT <= 1) {
        al_show_native_message_box(NULL, "Ventana Emergente", "Error", "El parqueo esta vacio", NULL, ALLEGRO_MESSAGEBOX_ERROR);
        return NULL;
    }

    const int RX = 340;
    const int RY = 320;

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
    ALLEGRO_TIMER* timer = al_create_timer(1.0 / 20);
    ALLEGRO_EVENT_QUEUE* eventQueue = al_create_event_queue();
    ALLEGRO_FONT* fontTitle = al_load_font("pixel.ttf", 60, NULL);
    ALLEGRO_FONT* fontSubtitle = al_load_font("pixel.ttf", 50, NULL);
    ALLEGRO_BITMAP* ParkingSlot = al_load_bitmap("Recursos/ParkingSlot02.png");

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
    int carMovementXPos = 0;

    PtrTParkingTowerNode Aux = Parking;
    pair<int, int> popedPair = make_pair(-1, -1);

    while (done) { //Cola de eventos

        ALLEGRO_EVENT events;
        al_wait_for_event(eventQueue, &events);

        sort(carsListPT.begin(), carsListPT.end(), comparePT);

        if (events.timer.source == timer) {

            al_clear_to_color(al_map_rgb(0, 0, 0));
            al_draw_bitmap(ParkingSlot, 0, 0, NULL);
            al_draw_text(fontSubtitle, al_map_rgb(250, 150, 0), 115, 25, ALLEGRO_ALIGN_LEFT, "Piso numero:");

            if (popedPair.first == -1 && popedPair.second == -1) {
                popedPair = popElementPT();
            }

            if (popedPair.first == 0) {
                Aux = Parking;
            }

            else {

                Aux = Parking;
                int steps = popedPair.first / 2;

                for (int i = 0; i < steps; ++i) {

                    if (Aux != nullptr) {
                        Aux = Aux->Next;
                    }
                    else {
                        break;
                    }
                }
            }

            if (popedPair.first % 2 == 1) {

                for (int i = 0; i < carsListPT.size(); ++i) {

                    if (carsListPT[i].first == (popedPair.first - 1)) {
                        al_draw_bitmap(Cars[carsListPT[i].second], carPosPT[0][1], carPosPT[0][0], NULL);
                    }
                }
            }

            if (popedPair.first % 2 == 0) {

                for (int i = 0; i < carsListPT.size(); ++i) {

                    if (carsListPT[i].first == (popedPair.first + 1)) {
                        al_draw_bitmap(Cars[carsListPT[i].second], carPosPT[1][1], carPosPT[1][0], NULL);
                    }
                }
            }

            string str = to_string(Aux->parkingSpot);
            const char* cstr = str.c_str();

            al_draw_text(fontTitle, al_map_rgb(250, 150, 0), 115, 85, ALLEGRO_ALIGN_LEFT, cstr);

            if (popedPair.first % 2 == 0) {

                if (carMovementYPos == 0) {
                    carMovementYPos = carPosPT[(popedPair.first % 2)][0];
                }

                if (carMovementYPos >= (carPosPT[(popedPair.first % 2)][0]) + 105) {
                    carMovementYPos = carPosPT[(popedPair.first % 2)][0] + 105;

                    if (carMovementXPos == 0) {
                        carMovementXPos = carPosPT[(popedPair.first % 2)][1];
                    }

                    else {
                        carMovementXPos = carMovementXPos - 3;
                        al_draw_bitmap(Cars[popedPair.second], carMovementXPos, carMovementYPos, NULL);
                    }

                    if (carMovementXPos < -80) {
                        al_destroy_display(display);
                        done = false;
                        exitCarPT = popedPair.second;
                        return NULL;
                    }
                }

                else {
                    carMovementYPos = carMovementYPos + 3;
                    al_draw_bitmap(Cars[popedPair.second], carPosPT[(popedPair.first % 2)][1], carMovementYPos, NULL);
                }
            }

            if (popedPair.first % 2 == 1) {

                if (carMovementYPos == 0) {
                    carMovementYPos = carPosPT[(popedPair.first % 2)][0];
                }

                if (carMovementYPos <= (carPosPT[(popedPair.first % 2)][0]) - 100) {
                    carMovementYPos = carPosPT[(popedPair.first % 2)][0] - 100;

                    if (carMovementXPos == 0) {
                        carMovementXPos = carPosPT[(popedPair.first % 2)][1];
                    }

                    else {
                        carMovementXPos = carMovementXPos - 3;
                        al_draw_bitmap(Cars[popedPair.second], carMovementXPos, carMovementYPos, NULL);
                    }

                    if (carMovementXPos < -80) {

                        al_destroy_display(display);
                        done = false;
                        exitCarPT = popedPair.second;
                        return NULL;
                    }
                }

                else {
                    carMovementYPos = carMovementYPos - 3;
                    al_draw_bitmap(Cars[popedPair.second], carPosPT[(popedPair.first % 2)][1], carMovementYPos, NULL);
                }
            }

        }

        if (RunningThreadFlagPT == false) {
            al_destroy_display(display);
            done = false;
            return NULL;
        }

        al_flip_display();

    }

    return NULL;
}

void* parkingEnterThreadPT(ALLEGRO_THREAD* thr, void* arg) {

    if (contCarsPT >= 19) {
        al_show_native_message_box(NULL, "Ventana Emergente", "Error", "El parqueo esta lleno", NULL, ALLEGRO_MESSAGEBOX_ERROR);
        return NULL;
    }

    const int RX = 340;
    const int RY = 320;

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
    ALLEGRO_TIMER* timer = al_create_timer(1.0 / 20);
    ALLEGRO_EVENT_QUEUE* eventQueue = al_create_event_queue();
    ALLEGRO_FONT* fontTitle = al_load_font("pixel.ttf", 60, NULL);
    ALLEGRO_FONT* fontSubtitle = al_load_font("pixel.ttf", 50, NULL);
    ALLEGRO_BITMAP* ParkingSlot = al_load_bitmap("Recursos/ParkingSlot02.png");

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

    int carMovementYPos = 0;
    int carMovementXPos = 0;

    PtrTParkingTowerNode Aux = Parking;
    pair<int, int> enterPair = make_pair(-1, -1);

    while (done) { //Cola de eventos

        ALLEGRO_EVENT events;
        al_wait_for_event(eventQueue, &events);
        sort(carsListPT.begin(), carsListPT.end(), comparePT);

        if (events.timer.source == timer) {

            al_clear_to_color(al_map_rgb(0, 0, 0));
            al_draw_bitmap(ParkingSlot, 0, 0, NULL);
            al_draw_text(fontSubtitle, al_map_rgb(250, 150, 0), 115, 25, ALLEGRO_ALIGN_LEFT, "Piso numero:");

            if (enterPair.first == -1 and enterPair.second == -1) {

                for (int i = 0; i < carsListPT.size(); i++) {

                    if (carsListPT[i].first != i) {

                        addCarToParking(Parking, new(car));
                        enterPair = make_pair(i, randomCarPT);
                        added = true;
                        addedCarPT = 1;
                        break;

                    }
                }
            }

            if (added == false) {

                addCarToParking(Parking, new(car));
                enterPair = make_pair(carsListPT.size(), randomCarPT);
                added = true;
                addedCarPT = 1;

            }

            if (enterPair.first == 0) {
                Aux = Parking;
            }

            else {

                Aux = Parking;

                for (int i = 0; i < enterPair.first / 2; ++i) {

                    if (Aux != nullptr) {
                        Aux = Aux->Next;
                    }
                    else {
                        break;
                    }
                }
            }

            if (enterPair.first % 2 == 1) {

                for (int i = 0; i < carsListPT.size(); ++i) {

                    if (carsListPT[i].first == (enterPair.first - 1)) {
                        al_draw_bitmap(Cars[carsListPT[i].second], carPosPT[0][1], carPosPT[0][0], NULL);
                    }
                }
            }

            if (enterPair.first % 2 == 0) {

                for (int i = 0; i < carsListPT.size(); ++i) {

                    if (carsListPT[i].first == (enterPair.first + 1)) {
                        al_draw_bitmap(Cars[carsListPT[i].second], carPosPT[1][1], carPosPT[1][0], NULL);
                    }
                }
            }

            string str = to_string(Aux->parkingSpot);
            const char* cstr = str.c_str();

            al_draw_text(fontTitle, al_map_rgb(250, 150, 0), 115, 85, ALLEGRO_ALIGN_LEFT, cstr);

            if (enterPair.first % 2 == 0) {

                if (carMovementXPos == 0) {
                    carMovementXPos = -70;
                    carMovementYPos = 117;
                }

                if (carMovementXPos >= carPosPT[(enterPair.first % 2)][1]) {
                    carMovementXPos = carPosPT[(enterPair.first % 2)][1];

                    if (carMovementYPos <= carPosPT[(enterPair.first % 2)][0]) {

                        carsListPT.push_back(enterPair);
                        al_destroy_display(display);
                        done = false;
                        contCarsPT = carList(Parking);
                        return NULL;
                    }

                    else {
                        carMovementYPos = carMovementYPos - 3;
                        al_draw_bitmap(Cars[enterPair.second], carMovementXPos, carMovementYPos, NULL);
                    }
                }

                else {
                    carMovementXPos = carMovementXPos + 3;
                    al_draw_bitmap(Cars[enterPair.second], carMovementXPos, carMovementYPos, NULL);

                }
            }

            if (enterPair.first % 2 == 1) {


                if (carMovementXPos == 0) {
                    carMovementXPos = -70;
                    carMovementYPos = 117;
                }

                if (carMovementXPos >= carPosPT[(enterPair.first % 2)][1]) {
                    carMovementXPos = carPosPT[(enterPair.first % 2)][1];

                    if (carMovementYPos >= carPosPT[(enterPair.first % 2)][0]) {

                        carsListPT.push_back(enterPair);
                        al_destroy_display(display);
                        done = false;
                        contCarsPT = carList(Parking);
                        return NULL;
                    }

                    else {
                        carMovementYPos = carMovementYPos + 3;
                        al_draw_bitmap(Cars[enterPair.second], carMovementXPos, carMovementYPos, NULL);
                    }
                }

                else {
                    carMovementXPos = carMovementXPos + 3;
                    al_draw_bitmap(Cars[enterPair.second], carMovementXPos, carMovementYPos, NULL);

                }
            }

        }

        if (RunningThreadFlagPT == false) {

            al_destroy_display(display);
            done = false;
            break;
        }

        al_flip_display();

    }

    return NULL;

}

void parkingTowerSimulator() {

    initializeParking(Parking);
    randomFillparking(Parking);

    contCarsPT = carList(Parking);

    for (int i = 0; i < contCarsPT; i++) {

        int random = generateRandomT(0, 3);
        carsListPT.push_back(make_pair(i, random));
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

    int actionRandom = generateRandomT(0, 1);
    int randomCharacter = generateRandomT(0, 1);
    randomCarPT = generateRandomT(0, 3);

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
                al_draw_text(fontSubtitle, al_map_rgb(0, 0, 0), RX / 2, 90, ALLEGRO_ALIGN_CENTRE, "Torre de N Pisos con un ascensor central y dos estacionamientos");
                al_draw_text(fonText, al_map_rgb(0, 0, 0), 50, 660, ALLEGRO_ALIGN_CENTRE, "Salir");
                al_draw_text(fontTitle, al_map_rgb(250, 150, 0), (RX / 2) + 2, 22, ALLEGRO_ALIGN_CENTRE, "Simulador de Parqueo Automatico");
                al_draw_text(fontSubtitle, al_map_rgb(250, 150, 0), (RX / 2) + 2, 92, ALLEGRO_ALIGN_CENTRE, "Torre de N Pisos con un ascensor central y dos estacionamientos");
                al_draw_text(fonText, al_map_rgb(250, 150, 0), 52, 662, ALLEGRO_ALIGN_CENTRE, "Salir");

                if (actionRandom == 0) {

                    if (randomCharacter == 1) { copyBitmapPT(character, character1, 13); }
                    else { copyBitmapPT(character, character2, 13); }

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

                            ALLEGRO_THREAD* thread = al_create_thread(parkingExitThreadPT, NULL);

                            if (RunningThreadFlagPT == false) {
                                al_start_thread(thread);
                                RunningThreadFlagPT = true;
                            }
                        }

                        if (exitCarPT != -1 and RunningThreadFlagPT != false) {

                            if (carExitPos > 1400) {

                                this_thread::sleep_for(200ms);

                                cout << "Se extrajo el vehiculo con exito" << endl;

                                exitCarPT = -1;
                                RunningThreadFlagPT = false;
                                al_destroy_thread(thread);
                                characterPos = -180;
                                characterYPos = 495;
                                carExitPos = 620;
                                actionRandom = generateRandomT(0, 1);
                                randomCharacter = generateRandomT(0, 1);
                            }

                            else {
                                carExitPos = carExitPos + 20;
                                characterYPos = -100;
                                al_draw_bitmap(sideCars[exitCarPT], carExitPos, 560, NULL);
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

                    al_draw_bitmap(sideCars[randomCarPT], carPos, carYPos, NULL);

                    if (carPos == 622 and thread == NULL) {

                        ALLEGRO_THREAD* thread = al_create_thread(parkingEnterThreadPT, NULL);

                        if (RunningThreadFlagPT == false) {
                            al_start_thread(thread);
                            RunningThreadFlagPT = true;
                            randomCharacter = generateRandomT(0, 1);

                        }
                    }

                    if (addedCarPT != -1 and RunningThreadFlagPT != false) {

                        if (characterPos > 1400) {

                            this_thread::sleep_for(200ms);

                            cout << "Se guardo el vehiculo con exito" << endl;

                            addedCarPT = -1;
                            RunningThreadFlagPT = false;
                            al_destroy_thread(thread);
                            thread = NULL;
                            characterPos = -180;
                            carYPos = 560;
                            carPos = -180;
                            actionRandom = generateRandomT(0, 1);
                            randomCarPT = generateRandomT(0, 3);
                        }

                        else {

                            if (characterPos == -180) { characterPos = 790; }

                            if (randomCharacter == 1) { copyBitmapPT(character, character1, 13); }
                            else { copyBitmapPT(character, character2, 13); }

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
                    RunningThreadFlagPT = false;
                    carsListPT.clear();
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
                RunningThreadFlagPT = false;
                carsListPT.clear();
                deleteParking(Parking);
                done = false;
                break;
            }
        }

        else if (events.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            al_destroy_thread(thread);
            al_destroy_display(display);
            RunningThreadFlagPT = false;
            carsListPT.clear();
            deleteParking(Parking);
            done = false;
            break;
        }

        al_flip_display();
    }

}