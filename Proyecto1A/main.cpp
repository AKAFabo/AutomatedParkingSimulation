#include "oneFloorBasic.h"
#include "oneFloorAdvanced.h"
#include "ParkingTower.h"

#include <iostream>
#include <math.h>
#include <windows.h>
#include <stdlib.h>
#include <cstdlib>
#include <time.h>

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
#define FPS 60.0

int main() {

    // Si no se inicializa Allegro, se muestra un mensaje de error
    if (!al_init()) {
        al_show_native_message_box(NULL, "Ventana Emergente", "Error", "No se puede inicializar Allegro", NULL, NULL);
        return -1;
    }

    //Se instalan y se inician los componentes de Allegro
    al_init_font_addon();
    al_init_ttf_addon();
    al_init_image_addon();
    al_init_primitives_addon();
    al_install_keyboard();
    al_install_audio();
    al_init_acodec_addon();
    al_reserve_samples(7);
    al_install_mouse();

    //Se establecen las propiedades de la ventana

    const int RX = 1280;
    const int RY = 720;

    ALLEGRO_DISPLAY* display;
    al_set_new_display_flags(ALLEGRO_WINDOWED);
    display = al_create_display(RX, RY);
    al_set_window_position(display, 200, 200);
    al_set_window_title(display, "Smart Parking Simulator");

    if (!display) {
        al_show_native_message_box(NULL, "Ventana Emergente", "Error", "No se puede crear la pantalla", NULL, ALLEGRO_MESSAGEBOX_ERROR);
        return -1;
    }

    //Se crean los temporizadores, la cola de eventos y se cargan los recursos
    ALLEGRO_TIMER* timer = al_create_timer(1.0 / FPS);
    ALLEGRO_EVENT_QUEUE* eventQueue = al_create_event_queue();
    ALLEGRO_FONT* fontTitle = al_load_font("pixel.ttf", 60, NULL);
    ALLEGRO_FONT* fontSubtitle = al_load_font("pixel.ttf", 50, NULL);
    ALLEGRO_FONT* fonText = al_load_font("pixel.ttf", 40, NULL);
    ALLEGRO_BITMAP* background = al_load_bitmap("Recursos/MenuBackground.png");
    ALLEGRO_BITMAP* car = al_load_bitmap("Recursos/Car.png");
    ALLEGRO_BITMAP* building = al_load_bitmap("Recursos/Building.png");
    ALLEGRO_BITMAP* cloud = al_load_bitmap("Recursos/Clouds.png");

    // Se registran las fuentes de eventos en la cola de eventos
    al_register_event_source(eventQueue, al_get_timer_event_source(timer));
    al_register_event_source(eventQueue, al_get_display_event_source(display));
    al_register_event_source(eventQueue, al_get_keyboard_event_source());
    al_register_event_source(eventQueue, al_get_mouse_event_source());
    al_start_timer(timer);

    bool done = true;
    int mouseX = 0;
    int mouseY = 0;
    int pos = 1350;
    float cloudPos = 0;

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


                if (pos <= -180) {
                    pos = 1850;
                }
                pos -= 3;

                cloudPos += 0.2;
                if (cloudPos >= 1280) {
                    cloudPos = 0;
                }

                al_clear_to_color(al_map_rgb(0, 0, 0));
                al_draw_bitmap(background, 0, 0, NULL);
                al_draw_bitmap(car, pos, 620, NULL);
                al_draw_bitmap(cloud, cloudPos, 0, NULL);
                al_draw_bitmap(cloud, cloudPos - 1280, 0, NULL);
                al_draw_bitmap(building, 526, 0, NULL);
                al_draw_text(fontTitle, al_map_rgb(0, 0, 0), RX / 2, 20, ALLEGRO_ALIGN_CENTRE, "Bienvenido al Simulador de Parqueo Automatico");
                al_draw_text(fontSubtitle, al_map_rgb(0, 0, 0), RX / 2, 90, ALLEGRO_ALIGN_CENTRE, "Porfavor seleccione la opcion de simulador:");
                al_draw_text(fonText, al_map_rgb(0, 0, 0), RX / 2, 200, ALLEGRO_ALIGN_CENTRE, "1. Un piso en una sola planta");
                al_draw_text(fonText, al_map_rgb(0, 0, 0), RX / 2, 300, ALLEGRO_ALIGN_CENTRE, "2. Dos Pisos en una sola planta");
                al_draw_text(fonText, al_map_rgb(0, 0, 0), RX / 2, 400, ALLEGRO_ALIGN_CENTRE, "3. Torre de N Pisos con un ascensor central y dos estacionamientos");
                al_draw_text(fonText, al_map_rgb(0, 0, 0), RX / 2, 500, ALLEGRO_ALIGN_CENTRE, "4. Salir");
                al_draw_text(fontTitle, al_map_rgb(250, 150, 0), (RX / 2) + 2, 22, ALLEGRO_ALIGN_CENTRE, "Bienvenido al Simulador de Parqueo Automatico");
                al_draw_text(fontSubtitle, al_map_rgb(250, 150, 0), (RX / 2) + 2, 92, ALLEGRO_ALIGN_CENTRE, "Porfavor seleccione la opcion de simulador:");
                al_draw_text(fonText, al_map_rgb(250, 150, 0), (RX / 2) + 2, 202, ALLEGRO_ALIGN_CENTRE, "1. Un piso en una sola planta");
                al_draw_text(fonText, al_map_rgb(250, 150, 0), (RX / 2) + 2, 302, ALLEGRO_ALIGN_CENTRE, "2. Dos Pisos en una sola planta");
                al_draw_text(fonText, al_map_rgb(250, 150, 0), (RX / 2) + 2, 402, ALLEGRO_ALIGN_CENTRE, "3. Torre de N Pisos con un ascensor central y dos estacionamientos");
                al_draw_text(fonText, al_map_rgb(250, 150, 0), (RX / 2) + 2, 502, ALLEGRO_ALIGN_CENTRE, "4. Salir");

            }

        }

        //Simulador 1
        if ((mouseX >= 460 && mouseX <= 820) && (mouseY >= 200.0 && mouseY <= 235.0)) {
            al_draw_text(fonText, al_map_rgb(250, 250, 250), RX / 2, 200, ALLEGRO_ALIGN_CENTRE, "1. Un piso en una sola planta");

            if (events.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {

                if (events.mouse.button & 1) {
                    al_destroy_display(display);
                    system("cls");
                    cout << "=========================================" << endl;
                    cout << "Simulador de Un piso en una sola planta" << endl;
                    cout << "=========================================" << endl << endl;
                    oneFloorBasicSimulator();
                    main();
                    done = false;
                }
            }
        }

        //Simulador 2
        if ((mouseX >= 440 && mouseX <= 840) && (mouseY >= 300.0 && mouseY <= 335.0)) {
            al_draw_text(fonText, al_map_rgb(250, 250, 250), RX / 2, 300, ALLEGRO_ALIGN_CENTRE, "2. Dos Pisos en una sola planta");

            if (events.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {

                if (events.mouse.button & 1) {
                    al_destroy_display(display);
                    system("cls");
                    cout << "=========================================" << endl;
                    cout << "Simulador de Dos Pisos en una sola planta" << endl;
                    cout << "=========================================" << endl << endl;
                    parkingAdvancedSimulator();
                    main();
                    done = false;
                }
            }
        }

        //Simulador 3

        if ((mouseX >= 230 && mouseX <= 1050) && (mouseY >= 400.0 && mouseY <= 435.0)) {
            al_draw_text(fonText, al_map_rgb(250, 250, 250), RX / 2, 400, ALLEGRO_ALIGN_CENTRE, "3. Torre de N Pisos con un ascensor central y dos estacionamientos");

            if (events.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {

                if (events.mouse.button & 1) {
                    al_destroy_display(display);
                    system("cls");
                    cout << "============================================================================" << endl;
                    cout << "Simulador de Torre de N Pisos con un ascensor central y dos estacionamientos" << endl;
                    cout << "============================================================================" << endl << endl;
                    parkingTowerSimulator();
                    main();
                    done = false;
                }
            }
        }

        //Salir
        if ((mouseX >= 595 && mouseX <= 685) && (mouseY >= 500.0 && mouseY <= 535.0)) {
            al_draw_text(fonText, al_map_rgb(250, 250, 250), RX / 2, 500, ALLEGRO_ALIGN_CENTRE, "4. Salir");

            if (events.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {

                if (events.mouse.button & 1) {
                    al_destroy_display(display);
                    done = false;
                    break;
                }
            }
        }

        if (events.type == ALLEGRO_EVENT_KEY_DOWN) {

            switch (events.keyboard.keycode) {

            case ALLEGRO_KEY_ESCAPE:
                done = false;
            }
        }

        else if (events.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            done = false;
        }

        al_flip_display();
    }
}
