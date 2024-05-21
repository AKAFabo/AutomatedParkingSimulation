#include "oneFloorBasic.h"

#include <iostream>
#include <math.h>
#include <windows.h>
#include <stdlib.h>
#include <cstdlib>
#include <time.h>
#include <vector> 
#include <thread> 

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

PtrTBasicParkingNode newSpace;
PtrTBasicParkingNode Parking;

int carPos[20][2] = { {10, 20} ,{10, 116} ,{10, 212},{10, 308},{10, 404},{215, 20},{215, 116} ,{215, 212},{215, 308},{215, 404},{335, 20},{335, 116} ,{335, 212},{335, 308},{335, 404},{535, 20},{535, 116} ,{535, 212},{535, 308},{535, 404} };
vector<pair<int, int>> carsList;
int contCars;

int exitCar = -1;
int enterCar = -1;
int randomCar = -1;
bool RunningThreadFlag = false;

// Codigo

int generateRandomB(int MIN, int MAX) {

    random_device rd;
    mt19937 gen(rd());    
    uniform_int_distribution<> dis(MIN, MAX);

    int randomCode = dis(gen);

    return randomCode;
}

PtrTBasicParkingNode createBasicParkingNode(int slotNumber) {
    PtrTBasicParkingNode slot = new(BasicParkingNode);

    slot->parkingSpot = slotNumber;
    slot->isOccupied = false;
    slot->Next = NULL;

    int returnCode = 0;

    return slot;
}

void initializeParking(PtrTBasicParkingNode& Parking) {

    static int MAX_SIZE = 20;
    PtrTBasicParkingNode newSpace;

    for (int i = 0; i <= MAX_SIZE; i++) {

        newSpace = createBasicParkingNode(i);
        addParkingNode(Parking, newSpace);
    }
}

void addParkingNode(PtrTBasicParkingNode& Parking, PtrTBasicParkingNode& newSpot) {
    newSpot->Next = Parking;
    Parking = newSpot;
}

int carList(PtrTBasicParkingNode& Parking) {

    PtrTBasicParkingNode Aux;
    Aux = Parking;

    int cont = 0;

    while (Aux != NULL) {

        if (Aux->isOccupied == true) {
            cont++;
        }

        Aux = Aux->Next;
    }

    return cont;
}

void addCarToParking(PtrTBasicParkingNode& Parking, car* newCar) {

    newCar->licensePlate = generateRandomB(100000, 999999);
    newCar->weight = generateRandomB(1, 3);                    
    newCar->size = generateRandomB(1, 5);

    PtrTBasicParkingNode Aux;
    Aux = Parking;

    int cont = 0;
    bool addedFlag = false; 

    while (Aux != NULL) {

        if (Aux->isOccupied == false) {

            Aux->carInSpot.licensePlate = newCar->licensePlate;
            Aux->carInSpot.size = newCar->size;
            Aux->carInSpot.weight = newCar->weight;
            Aux->isOccupied = true; 

            int randomCode = generateRandomB(100000, 999999);
            Aux->returnCode = randomCode;

            cout << "Se agrego un carro en el espacio " << Aux->parkingSpot << " con el codigo de retorno " << Aux->returnCode << endl;

            addedFlag = true; 
            break;
        }

        Aux = Aux->Next;
        cont++;
    }

    if (addedFlag == false) { cout << "El parque esta lleno" << endl; } // Cambiado == false

}

void randomFillparking() {

    int randomChance = generateRandomB(7, 13);
    for (int i = 0; i <= randomChance; i++) {
           addCarToParking(Parking, new(car));
    }
}

pair<int, int> popElement() {

    int pos;
    pair<int, int> popedPair;

    if (contCars == 0) {
        contCars = carList(Parking);
        return { 0, 0 };
    }

    while (true) {

        pos = generateRandomB(0, 20);
        
        PtrTBasicParkingNode Aux = Parking; 
        int cont = 0;

        for (int i = 0; i < carsList.size(); i++) {

            if (carsList[i].first == pos) {

                popedPair = carsList[i];
                carsList.erase(carsList.begin() + i);
            }
        }    

        while (Aux != NULL && cont < pos) {

            Aux = Aux->Next;
            cont++;
        }

        if (Aux != NULL && cont == pos) {
      
            if (Aux->isOccupied == true) {

                cout << "Se elimino un carro en el espacio " << Aux->parkingSpot << " con el codigo de retorno " << Aux->returnCode << endl;
                
                Aux->isOccupied = false;
                Aux->returnCode = NULL;
                break;
            }
        }
    }

    contCars = carList(Parking);
    return popedPair;    
}

void copyBitmap(ALLEGRO_BITMAP* dest[], ALLEGRO_BITMAP* src[], int len) {

    for (int i = 0; i < len; i++) {

        dest[i] = src[i];
    }
}

auto compare = [](const pair<int, int>& a, const pair<int, int>& b) {
    return a.first < b.first;
    };

void deleteParking(PtrTBasicParkingNode& Parking) {

    PtrTBasicParkingNode Aux;
    Aux = Parking;

    while (Aux != NULL) {

        Parking = Parking->Next;
        delete(Aux);
        Aux = Parking;
    }
}

// Simulador

void* parkingExitThread(ALLEGRO_THREAD* thr, void* arg) {

    if (contCars <= 0) {
        al_show_native_message_box(NULL, "Ventana Emergente", "Error", "El parqueo esta vacio", NULL, ALLEGRO_MESSAGEBOX_ERROR);
        return NULL;
    }

    const int RX = 580;
    const int RY = 640;

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
    ALLEGRO_TIMER* timer = al_create_timer(1.0 / 45);
    ALLEGRO_EVENT_QUEUE* eventQueue = al_create_event_queue();
    ALLEGRO_FONT* fontTitle = al_load_font("pixel.ttf", 60, NULL);
    ALLEGRO_FONT* fontSubtitle = al_load_font("pixel.ttf", 50, NULL);
    ALLEGRO_BITMAP* ParkingSlot = al_load_bitmap("Recursos/ParkingSlot.png");

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

    int random = 0;

    int carMovementYPos = 0;
    int carMovementXPos = 0;

    pair<int, int> popedPair = make_pair(-1, -1);

    while (done) { //Cola de eventos

        ALLEGRO_EVENT events;
        al_wait_for_event(eventQueue, &events);

        if (events.timer.source == timer) {

            al_clear_to_color(al_map_rgb(0, 0, 0));
            al_draw_bitmap(ParkingSlot, 0, 0, NULL);

            for (auto it = carsList.begin(); it != carsList.end(); it++){

                al_draw_bitmap(Cars[it->second], carPos[it->first][1], carPos[it->first][0], NULL);
            }

            if (popedPair.first == -1 && popedPair.second == -1) {
                popedPair = popElement();
            }

            if (popedPair.first <= 4 or popedPair.first >= 10 && popedPair.first <= 14) {

                if (carMovementYPos == 0) {
                    carMovementYPos = carPos[popedPair.first][0];
                }

                if (carMovementYPos >= (carPos[popedPair.first][0]) + 105) {
                    carMovementYPos = carPos[popedPair.first][0] + 105;

                    if (carMovementXPos == 0) {
                        carMovementXPos = carPos[popedPair.first][1];
                    }

                    else {
                        carMovementXPos = carMovementXPos - 3;
                        al_draw_bitmap(Cars[popedPair.second], carMovementXPos, carMovementYPos, NULL);
                    }

                    if (carMovementXPos < -80 ) {
                        al_destroy_display(display);
                        done = false;
                        exitCar = popedPair.second;
                        return NULL;
                    }
                }

                else {
                    carMovementYPos = carMovementYPos + 3;
                    al_draw_bitmap(Cars[popedPair.second], carPos[popedPair.first][1], carMovementYPos, NULL);
                }
            }

            if (popedPair.first >= 5 and popedPair.first <= 9 or popedPair.first >= 15) {

                if (carMovementYPos == 0) {
                    carMovementYPos = carPos[popedPair.first][0];
                }

                if (carMovementYPos <= (carPos[popedPair.first][0]) - 100) {
                    carMovementYPos = carPos[popedPair.first][0] - 100;

                    if (carMovementXPos == 0) {
                        carMovementXPos = carPos[popedPair.first][1];
                    }

                    else {
                        carMovementXPos = carMovementXPos - 3;
                        al_draw_bitmap(Cars[popedPair.second], carMovementXPos, carMovementYPos, NULL);
                    }

                    if (carMovementXPos < -80) {

                        al_destroy_display(display);
                        done = false;
                        exitCar = popedPair.second;
                        return NULL;
                    }
                }

                else {
                    carMovementYPos = carMovementYPos - 3;
                    al_draw_bitmap(Cars[popedPair.second], carPos[popedPair.first][1], carMovementYPos, NULL);
                }
            }
        }

        if (RunningThreadFlag == false) {
            al_destroy_display(display);
            done = false;
            return NULL;
        }

        al_flip_display();

    }

    return NULL;
}

void* parkingEnterThread(ALLEGRO_THREAD* thr, void* arg) {

    if (contCars >= 19) {
        al_show_native_message_box(NULL, "Ventana Emergente", "Error", "El parqueo esta lleno", NULL, ALLEGRO_MESSAGEBOX_ERROR);
        return NULL;
    }

    const int RX = 580;
    const int RY = 640;

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
    ALLEGRO_TIMER* timer = al_create_timer(1.0 / 45);
    ALLEGRO_EVENT_QUEUE* eventQueue = al_create_event_queue();
    ALLEGRO_FONT* fontTitle = al_load_font("pixel.ttf", 60, NULL);
    ALLEGRO_FONT* fontSubtitle = al_load_font("pixel.ttf", 50, NULL);
    ALLEGRO_BITMAP* ParkingSlot = al_load_bitmap("Recursos/ParkingSlot.png");

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
    bool agregado = false;
    int agregago = 0;

    int random = 0;

    int carMovementYPos = 0;
    int carMovementXPos = 0;

    pair<int, int> enterPair = make_pair(-1, -1);

    while (done) { //Cola de eventos

        ALLEGRO_EVENT events;
        al_wait_for_event(eventQueue, &events);

        if (events.timer.source == timer) {

            al_clear_to_color(al_map_rgb(0, 0, 0));
            al_draw_bitmap(ParkingSlot, 0, 0, NULL);

            for (auto it = carsList.begin(); it != carsList.end(); it++) {

                al_draw_bitmap(Cars[it->second], carPos[it->first][1], carPos[it->first][0], NULL);
            }

            sort(carsList.begin(), carsList.end(), compare);

            if (enterPair.first == -1 and enterPair.second == -1) {

                for (int i = 0; i < carsList.size(); i++) {

                    if (carsList[i].first != i) {

                        addCarToParking(Parking, new(car));
                        enterPair = make_pair(i, randomCar);
                        agregado = true;
                        agregado = i;
                        break;

                    }
                }
            }

            if (agregado == false) {

                addCarToParking(Parking, new(car));
                enterPair = make_pair(carsList.size(), randomCar);
                agregado = true;
                agregado = carsList.size();

            }

            if (enterPair.first >= 0 and enterPair.first <= 9) {

                if (enterPair.first >= 0 and enterPair.first <= 4) {

                    if (carMovementXPos == 0) {
                        carMovementXPos = -70;
                        carMovementYPos = 117;
                    }

                    if (carMovementXPos >= carPos[enterPair.first][1]) {
                        carMovementXPos = carPos[enterPair.first][1];

                        if (carMovementYPos <= carPos[enterPair.first][0]) {

                            carPos[enterPair.first][0];
                            carsList.push_back(enterPair);
                            al_destroy_display(display);
                            done = false;
                            contCars = carList(Parking);
                            enterCar = 1;
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
                
                else {

                    if (carMovementXPos == 0) {
                        carMovementXPos = -70;
                        carMovementYPos = 117;
                    }

                    if (carMovementXPos >= carPos[enterPair.first][1]) {
                        carMovementXPos = carPos[enterPair.first][1];

                        if (carMovementYPos >= carPos[enterPair.first][0]) {

                            carPos[enterPair.first][0];
                            carsList.push_back(enterPair);
                            al_destroy_display(display);
                            done = false;
                            contCars = carList(Parking);
                            enterCar = 1;
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

            if (enterPair.first >= 10 and enterPair.first <= 19) {

                if (enterPair.first >= 10 and enterPair.first <= 14) {

                    if (carMovementXPos == 0) {
                        carMovementXPos = -70;
                        carMovementYPos = 440;
                    }

                    if (carMovementXPos >= carPos[enterPair.first][1]) {
                        carMovementXPos = carPos[enterPair.first][1];

                        if (carMovementYPos <= carPos[enterPair.first][0]) {
                            carPos[enterPair.first][0];

                            carsList.push_back(enterPair);
                            al_destroy_display(display);
                            done = false;
                            contCars = carList(Parking);
                            enterCar = 1;
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
               
                else {

                    if (carMovementXPos == 0) {
                        carMovementXPos = -70;
                        carMovementYPos = 440;
                    }

                    if (carMovementXPos >= carPos[enterPair.first][1]) {
                        carMovementXPos = carPos[enterPair.first][1];

                        if (carMovementYPos >= carPos[enterPair.first][0]) {

                            carPos[enterPair.first][0];
                            carsList.push_back(enterPair);
                            al_destroy_display(display);
                            done = false;
                            contCars = carList(Parking);

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
        }

        if (RunningThreadFlag == false) {

            al_destroy_display(display);
            done = false;
            break;
        }

        al_flip_display();

    }

    return NULL;

}

void oneFloorBasicSimulator() {

    initializeParking(Parking);
    randomFillparking();

    contCars = carList(Parking);

    for (int i = 0; i < contCars; i++) {
        int random = generateRandomB(0, 3);
        carsList.push_back(make_pair(i, random));
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
    ALLEGRO_BITMAP* background = al_load_bitmap("Recursos/MenuBackground.png");;
    
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

    int actionRandom = generateRandomB(0, 1);
    int randomCharacter = generateRandomB(0, 1);
    randomCar = generateRandomB(0, 3);

    int characterFrameWalk = 0;
    int characterFrameWait = 8;

    int characterPos = -180; //Cambiar
    //int characterPos = 580;
    int characterYPos = 495;

    int carPos = -180;
    int carYPos = 560;
    int carExitPos = 620;

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

                al_clear_to_color(al_map_rgb(0, 0, 0));
                al_draw_bitmap(background, 0, 0, NULL);
                al_draw_text(fontTitle, al_map_rgb(0, 0, 0), RX / 2, 20, ALLEGRO_ALIGN_CENTRE, "Simulador de Parqueo Automatico");
                al_draw_text(fontSubtitle, al_map_rgb(0, 0, 0), RX / 2, 90, ALLEGRO_ALIGN_CENTRE, "Un piso en una sola planta");
                al_draw_text(fonText, al_map_rgb(0, 0, 0), 50, 660, ALLEGRO_ALIGN_CENTRE, "Salir");
                al_draw_text(fontTitle, al_map_rgb(250, 150, 0), (RX / 2) + 2, 22, ALLEGRO_ALIGN_CENTRE, "Simulador de Parqueo Automatico");
                al_draw_text(fontSubtitle, al_map_rgb(250, 150, 0), (RX / 2) + 2, 92, ALLEGRO_ALIGN_CENTRE, "Un piso en una sola planta");
                al_draw_text(fonText, al_map_rgb(250, 150, 0), 52, 662, ALLEGRO_ALIGN_CENTRE, "Salir");

                if (actionRandom == 0) {

                    if (randomCharacter == 1) { copyBitmap(character, character1, 13); }
                    else { copyBitmap(character, character2, 13); }

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

                            ALLEGRO_THREAD* thread = al_create_thread(parkingExitThread, NULL);

                            if (RunningThreadFlag == false) {
                                al_start_thread(thread);
                                RunningThreadFlag = true;
                            }
                        }

                        if (exitCar != -1 and RunningThreadFlag != false) {

                            if (carExitPos > 1400) {

                                this_thread::sleep_for(200ms);

                                cout << "Se extrajo el vehiculo con exito" << endl;

                                exitCar = -1;
                                RunningThreadFlag = false;
                                al_destroy_thread(thread);
                                characterPos = -180;
                                characterYPos = 495;
                                carExitPos = 620;
                                actionRandom = generateRandomB(0, 1);
                                randomCharacter = generateRandomB(0, 1);
                            }

                            else {
                                carExitPos = carExitPos + 20;
                                characterYPos = -100;
                                al_draw_bitmap(sideCars[exitCar], carExitPos, 560, NULL);
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

                    al_draw_bitmap(sideCars[randomCar], carPos, carYPos, NULL);

                    if (carPos == 622 and thread == NULL) {

                        ALLEGRO_THREAD* thread = al_create_thread(parkingEnterThread, NULL);

                        if (RunningThreadFlag == false) {
                            al_start_thread(thread);
                            RunningThreadFlag = true;
                            randomCharacter = generateRandomB(0, 1);

                        }
                    }

                    if (enterCar != -1 and RunningThreadFlag != false) {

                        if (characterPos > 1400) {

                            this_thread::sleep_for(200ms);

                            cout << "Se guardo el vehiculo con exito" << endl;

                            enterCar = -1;
                            RunningThreadFlag = false;
                            al_destroy_thread(thread);
                            thread = NULL;
                            characterPos = -180;
                            carYPos = 560;
                            carPos = -180;
                            actionRandom = generateRandomB(0, 1);
                            randomCar = generateRandomB(0, 3);
                        }

                        else {

                            if (characterPos == -180) { characterPos = 660; }

                            if (randomCharacter == 1) { copyBitmap(character, character1, 13); }
                            else { copyBitmap(character, character2, 13); }

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
                    RunningThreadFlag = false;
                    carsList.clear();
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
                RunningThreadFlag = false;
                carsList.clear();
                deleteParking(Parking);
                done = false;
                break;
            }
        }

        else if (events.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            al_destroy_thread(thread);
            al_destroy_display(display);
            RunningThreadFlag = false;
            carsList.clear();
            deleteParking(Parking);
            done = false;
            break;
        }

        al_flip_display();
    }
 
}

