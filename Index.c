#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>

void Start(){
	al_init();
	al_install_keyboard();
	al_install_mouse();
	al_init_image_addon();
	al_init_font_addon();
	al_init_ttf_addon();
	al_init_primitives_addon();
}

void Finish(){
	al_uninstall_keyboard();
	al_uninstall_mouse();
}

bool isInside(const ALLEGRO_MOUSE_STATE S, int x0, int y0, int x1, int y1){
	if(S.x < x0 || S.x > x1) return 0;
	if(S.y < y0 || S.y > y1) return 0;
	return 1;
}

int main(int argc, char *argv[]){
	ALLEGRO_DISPLAY *Window = NULL;
	// ALLEGRO_KEYBOARD_STATE *keyState;
	ALLEGRO_MOUSE_STATE mouseState;
	ALLEGRO_FONT *edosz84, *edosz46;
	// ALLEGRO_BITMAP *imgHeader = NULL;

	Start();
	Window = al_create_display(800,600);

	edosz84 = al_load_font("edo_sz/edosz.ttf",84,0);
	edosz46 = al_load_font("edo_sz/edosz.ttf",46,0);

	// Background:
	al_clear_to_color(al_map_rgb(255,255,255));

	// Title:
	al_draw_textf(edosz84, al_map_rgb(40,0,255), 400, 10, ALLEGRO_ALIGN_CENTER, "Tic Tac Toe 2.0");
	
	// Draw buttons:
	al_draw_rectangle(150,225,250,275,al_map_rgb(40,0,255),8);
	al_draw_rectangle(350,225,450,275,al_map_rgb(40,0,255),8);
	al_draw_rectangle(550,225,650,275,al_map_rgb(40,0,255),8);
	
	// Write on them:
	al_draw_textf(edosz46, al_map_rgb(40,0,255), 200, 225, ALLEGRO_ALIGN_CENTER, "4x4");
	al_draw_textf(edosz46, al_map_rgb(40,0,255), 400, 225, ALLEGRO_ALIGN_CENTER, "5x5");
	al_draw_textf(edosz46, al_map_rgb(40,0,255), 600, 225, ALLEGRO_ALIGN_CENTER, "6x6");

	al_flip_display();
	
	// Wait for the user to click on one mode:
	while(1){
		int found = 0;
		while(!found){
			al_get_mouse_state(&mouseState);
			if(mouseState.buttons == 1){
				if(isInside(mouseState,150,225,250,275))
					found = 1;
				if(isInside(mouseState,350,225,450,275))
					found = 2;
				if(isInside(mouseState,550,225,650,275))
					found = 3;
			}
		}

		if(found == 1){
			system("make 4x4 -B");		
		}
		else if(found == 2){
			system("make 5x5 -B");
		}
		else{
			system("make 6x6 -B");
		}
	}

	Finish();
	al_destroy_font(edosz46);
	al_destroy_font(edosz84);
	al_destroy_display(Window);



	return 0;
}