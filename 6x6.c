#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>

#define RED al_map_rgb(255,0,0)
#define GREEN al_map_rgb(0,255,0)
#define BLUE al_map_rgb(40,0,255)

// Declaring the fieldState matrix and the booleans to store player's move & stage of the game:
int M[6][6] = {0};
bool player = 0;
int END = 0;

// Create the Data Structure to store the rectangles:
typedef struct rect{
	int x0;
	int x1;
	int y0;
	int y1;
} rect;
rect Fields[36];

// Declare Headers of the functions:
void Start();
void Finish();
bool isInside(int x, int y, rect F);
int validMove(int x, int y);
int endGame();
void fillFields();
void BasicGrid(const ALLEGRO_FONT *font, ALLEGRO_BITMAP *grid);
void DrawBoard(const ALLEGRO_FONT *font);

int main(){
	ALLEGRO_DISPLAY *Window = NULL;
	ALLEGRO_FONT *edosz40, *edosz80;
	ALLEGRO_BITMAP *grid = NULL;
	ALLEGRO_MOUSE_STATE mouseState;

	fillFields();

	// Start the App:
	Start(Window);
	Window = al_create_display(600,700);

	// DONE!
	// Load the background image:
	grid = al_load_bitmap("6x6 grid.png");

	// Load the fonts:
	edosz40 = al_load_font("edo_sz/edosz.ttf",40,0);
	edosz80 = al_load_font("edo_sz/edosz.ttf",80,0);

	// Draw the structure:
	BasicGrid(edosz40,grid);

	al_flip_display();

	while(!END){
		// Exchange the atual player:
		player = !player;

		// Wait for the player to move:
		int click = 0;
		while(click == 0){
			al_get_mouse_state(&mouseState);
			if(mouseState.buttons == 1){
				al_rest(0.1);
				if((click = validMove(mouseState.x, mouseState.y))){
					break;
				}
				if(click) break;
			}
		}

		printf("%d\n", click);

		// Render the atual stage:

		BasicGrid(edosz40,grid);
		DrawBoard(edosz80);
		al_flip_display();

		// Check if the game ended:
		END = endGame();
	}

	BasicGrid(edosz40,grid);

	// Tell who won:
	if(END == 2){
		al_draw_textf(edosz40, BLUE, 300, 40, ALLEGRO_ALIGN_CENTER, "It's a draw!");
	}
	else{
		if(player){
			al_draw_textf(edosz40, GREEN, 300, 40, ALLEGRO_ALIGN_CENTER, "Player 1 wins!");
		}
		else{
			al_draw_textf(edosz40, RED, 300, 40, ALLEGRO_ALIGN_CENTER, "Player 2 wins!");
		}
	}

	// Keep the grid there:
	DrawBoard(edosz80);
	al_flip_display();

	// Uninstall resources:
	al_rest(2.5);
	Finish();
	al_destroy_font(edosz40);
	al_destroy_font(edosz80);
	al_destroy_bitmap(grid);
	al_destroy_display(Window);

	return 0;
}


// Initialize everything:
void Start(){
	al_init();
	al_install_keyboard();
	al_install_mouse();
	al_init_image_addon();
	al_init_font_addon();
	al_init_ttf_addon();
	al_init_primitives_addon();	
}

// Unitialize everything:
void Finish(){
	al_uninstall_keyboard();
	al_uninstall_mouse();
}

// Auxiliar function to check if a point is inside of a rectangle:
bool isInside(int x, int y, rect F){
	if(x < F.x0 || y < F.y0) return 0;
	if(y > F.y1 || x > F.x1) return 0;
	return 1;
}

// Function to check if a click is valid & in which field it lands:
int validMove(int x, int y){

	for(int i = 0; i < 36; i++){
		if(isInside(x,y,Fields[i])){
			if(M[i / 6][i % 6] == 0){
				M[i / 6][i % 6] = (player == 1 ? 1 : -1);
				return i+1;
			}
		}
	}

	return 0;
}

// Function to check is a game ended:
// 1 means someone has won | 2 means the game ended in a draw
int endGame(){
	for(int i = 0; i < 6; i++){
		for(int j = 0; j < 6; j++){
			int value = M[i][j];
			if(value == 0) continue;
			
			// Check LINE
			bool ok = 1;
			for(int x = 0; x < 6; x++){
				if(M[i][x] != value){
					ok = 0;
					break;
				}
			}
			if(ok) return 1;

			// Check COLUMN
			ok = 1;
			for(int x = 0; x < 6; x++){
				if(M[x][j] != value){
					ok = 0;
					break;
				}
			}
			if(ok) return 1;

			// Check LEFT DIAGONAL
			if(i != 0 && i != 5){
				ok = 1;
				for(int x = 0; x < 6; x++){
					for(int y = 0; y < 6; y++){
						if(i - j == x - y){
							if(M[x][y] != value){
								ok = 0;
								break;
							}
						}
					}
					if(!ok)
						break;
				}
				if(ok) return 1;
			}

			// Check RIGHT DIAGONAL
			if(j != 0 && j != 5){
				ok = 1;
				for(int x = 0; x < 6; x++){
					for(int y = 0; y < 6; y++){
						if(i + j == x + y){
							if(M[x][y] != value){
								ok = 0;
								break;
							}
						}
					}
					if(!ok)
						break;
				}
				if(ok) return 1;
			}
		}
	}

	// Check if the grid is filled:
	for(int i = 0; i < 6; i++)
		for(int j = 0; j < 6; j++)
			if(!M[i][j]) return 0;

	return 2;
}

// Store the Rectangles:
void fillFields(){
	Fields[0].x0 = 0; Fields[0].y0 = 105; Fields[0].x1 = 90; Fields[0].y1 = 190;
	Fields[1].x0 = 115; Fields[1].y0 = 105; Fields[1].x1 = 195; Fields[1].y1 = 190;
	Fields[2].x0 = 220; Fields[2].y0 = 105; Fields[2].x1 = 295; Fields[2].y1 = 190;
	Fields[3].x0 = 320; Fields[3].y0 = 105; Fields[3].x1 = 395; Fields[3].y1 = 190;
	Fields[4].x0 = 425; Fields[4].y0 = 105; Fields[4].x1 = 500; Fields[4].y1 = 190;
	Fields[5].x0 = 525; Fields[5].y0 = 105; Fields[5].x1 = 600; Fields[5].y1 = 190;
	Fields[6].x0 = 0; Fields[6].y0 = 218; Fields[6].x1 = 90; Fields[6].y1 = 290;
	Fields[7].x0 = 115; Fields[7].y0 = 218; Fields[7].x1 = 195; Fields[7].y1 = 290;
	Fields[8].x0 = 220; Fields[8].y0 = 218; Fields[8].x1 = 295; Fields[8].y1 = 290;
	Fields[9].x0 = 320; Fields[9].y0 = 218; Fields[9].x1 = 395; Fields[9].y1 = 290;
	Fields[10].x0 = 425; Fields[10].y0 = 218; Fields[10].x1 = 500; Fields[10].y1 = 290;
	Fields[11].x0 = 525; Fields[11].y0 = 218; Fields[11].x1 = 600; Fields[11].y1 = 290;
	Fields[12].x0 = 0; Fields[12].y0 = 318; Fields[12].x1 = 90; Fields[12].y1 = 390;
	Fields[13].x0 = 115; Fields[13].y0 = 318; Fields[13].x1 = 195; Fields[13].y1 = 390;
	Fields[14].x0 = 220; Fields[14].y0 = 318; Fields[14].x1 = 295; Fields[14].y1 = 390;
	Fields[15].x0 = 320; Fields[15].y0 = 318; Fields[15].x1 = 405; Fields[15].y1 = 390;
	Fields[16].x0 = 425; Fields[16].y0 = 318; Fields[16].x1 = 500; Fields[16].y1 = 390;
	Fields[17].x0 = 525; Fields[17].y0 = 318; Fields[17].x1 = 600; Fields[17].y1 = 390;
	Fields[18].x0 = 0; Fields[18].y0 = 415; Fields[18].x1 = 90; Fields[18].y1 = 490;
	Fields[19].x0 = 115; Fields[19].y0 = 415; Fields[19].x1 = 195; Fields[19].y1 = 490;
	Fields[20].x0 = 220; Fields[20].y0 = 415; Fields[20].x1 = 295; Fields[20].y1 = 490;
	Fields[21].x0 = 320; Fields[21].y0 = 415; Fields[21].x1 = 395; Fields[21].y1 = 490;
	Fields[22].x0 = 425; Fields[22].y0 = 415; Fields[22].x1 = 500; Fields[22].y1 = 490;
	Fields[23].x0 = 525; Fields[23].y0 = 415; Fields[23].x1 = 600; Fields[23].y1 = 490;
	Fields[24].x0 = 0; Fields[24].y0 = 515; Fields[24].x1 = 90; Fields[24].y1 = 590;
	Fields[25].x0 = 115; Fields[25].y0 = 515; Fields[25].x1 = 195; Fields[25].y1 = 590;
	Fields[26].x0 = 220; Fields[26].y0 = 515; Fields[26].x1 = 295; Fields[26].y1 = 590;
	Fields[27].x0 = 320; Fields[27].y0 = 515; Fields[27].x1 = 395; Fields[27].y1 = 590;
	Fields[28].x0 = 425; Fields[28].y0 = 515; Fields[28].x1 = 500; Fields[28].y1 = 590;
	Fields[29].x0 = 525; Fields[29].y0 = 515; Fields[29].x1 = 600; Fields[29].y1 = 590;
	Fields[30].x0 = 0; Fields[30].y0 = 615; Fields[30].x1 = 90; Fields[30].y1 = 700;
	Fields[31].x0 = 115; Fields[31].y0 = 615; Fields[31].x1 = 185; Fields[31].y1 = 700;
	Fields[32].x0 = 220; Fields[32].y0 = 615; Fields[32].x1 = 295; Fields[32].y1 = 700;
	Fields[33].x0 = 320; Fields[33].y0 = 615; Fields[33].x1 = 395; Fields[33].y1 = 700;
	Fields[34].x0 = 425; Fields[34].y0 = 615; Fields[34].x1 = 500; Fields[34].y1 = 700;
	Fields[35].x0 = 525; Fields[35].y0 = 615; Fields[35].x1 = 600; Fields[35].y1 = 700;
}

// Function to draw the basic structure of the game, needed once we have to use it many times:
void BasicGrid(const ALLEGRO_FONT *font, ALLEGRO_BITMAP *grid){

	// Draw Background:
	al_clear_to_color(al_map_rgb(255,255,255));
	al_draw_bitmap(grid,0,100,0);

	// Draw Title:
	al_draw_textf(font, BLUE, 300, 0, ALLEGRO_ALIGN_CENTER, "Tic Tac Toe 2.0 - Size: 6x6");

	// If the game is finished:
	// We use the boolean 'player' to know who won the game, this way we don't overcomplicate the code
	if(END)
		return;

	// Tell the first to play:
	al_draw_textf(font, BLUE, 240, 40, ALLEGRO_ALIGN_CENTER, "Vez de:       ");
	if(!player) al_draw_textf(font, GREEN, 280, 40, ALLEGRO_ALIGN_LEFT, "Jogador 1");
	else al_draw_textf(font, RED, 280, 40, ALLEGRO_ALIGN_LEFT, "Jogador 2");
}

void DrawBoard(const ALLEGRO_FONT *font){
	for(int i = 0; i < 6; i++){
		for(int j = 0; j < 6; j++){
			if(M[i][j]){
				if(M[i][j] == 1) 
					al_draw_textf(font, GREEN, (Fields[i*6 + j].x0 + Fields[i*6 + j].x1)/2, Fields[i*6 + j].y0 - 12, ALLEGRO_ALIGN_CENTER, "o");
					// al_draw_filled_rectangle(Fields[i*6 + j].x0, Fields[i*6 + j].y0, Fields[i*6 + j].x1, Fields[i*6 + j].y1, GREEN);
				else 
					al_draw_textf(font, RED, (Fields[i*6 + j].x0 + Fields[i*6 + j].x1)/2, Fields[i*6 + j].y0 - 12, ALLEGRO_ALIGN_CENTER, "x");
					// al_draw_filled_rectangle(Fields[i*6 + j].x0, Fields[i*6 + j].y0, Fields[i*6 + j].x1, Fields[i*6 + j].y1, RED);
			}
		}
	}
}