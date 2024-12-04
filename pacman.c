// Pacman Game in C language 
#include <conio.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <time.h>
#include <string.h>

// All the elements to be used 
// Declared here 
#define WIDTH 60 
#define HEIGHT 11
#define PACMAN 'C' 
#define WALL '#' 
#define FOOD '.' 
#define EMPTY ' ' 
#define DEMON 'X' 
#define moving_speed 0.3

// Global Variables are 
// Declared here 
int alive = 1; 	// 1 for alive, 0 for dead
int score = 0; 
char board[HEIGHT][WIDTH]; 
int food = 0; 	// Food count
int curr = 0;
int stage = 0; 
char conti;

// Define pacman position and velocity
float pacman_x = WIDTH / 2.0;
float pacman_y = HEIGHT / 2.0;
float pacman_vx = 0;
float pacman_vy = 0;

// to make the food blink
int toggle = 0;  // 0 stands for visible, 1 stands for invisible
int frame_count = 0;



void initialize() 
{ 
	// add a random seed
	srand(time(0));
	
	// Map Load Done Editing//

	FILE *file = fopen("D:/c codes/project/pacman/PACMAN-on-c/map.txt", "r");
    if (file == NULL) {
        printf("Error: Unable to open map file.\n");
		system("pause");
        exit(1);
    }
	printf("success loading map.\n");
	system("cls");
    
    for (int i = 0; i < HEIGHT; i++) {
        if (fgets(board[i], WIDTH + 3, file)) {  // Adjusted to read the newline character
            // Remove newline if present
			int len = strlen(board[i]);
			if (len > 0 && board[i][len - 1] == '\n') {
				board[i][len - 1] = '\0';
			}
            // Fill in any extra space to WIDTH with EMPTY
            for (int j = strlen(board[i]); j < WIDTH; j++) {
                board[i][j] = EMPTY;
            }
        } 
		else {
            // Fill the rest of the board in case of missing lines
            for (int j = 0; j < WIDTH; j++) {
                board[i][j] = EMPTY;
            }
        }
    }

    fclose(file);

    // 設定初始位置
    board[(int)pacman_y][(int)pacman_x] = PACMAN;


    // 放置惡魔（隨機生成）
    int demon_count = 5; 
    while (demon_count > 0) {
        int rand_x = rand() % WIDTH;
        int rand_y = rand() % HEIGHT;

        // make sure demons doesn't generate in the wall
        if (board[rand_y][rand_x] != '#' && !(rand_y == (int)pacman_y && rand_x == (int)pacman_x)) {
            board[rand_y][rand_x] = DEMON;
            demon_count--;
        }
    }

	// 計算食物的數量
    food = 0;
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            if (board[i][j] == FOOD) {
                food++;
            }
        }
    }
}

void draw() 
{ 
	// Clear screen 
	system("cls"); 

    // Make the state change of the foods
    if (frame_count % 10 == 0) {
        toggle = !toggle;
    }

	// Drawing All the elements in the screen 
	for (int i = 0; i < HEIGHT; i++) { 
		for (int j = 0; j < WIDTH; j++) { 
            if (board[i][j] == FOOD) {
                // print different food based on the toggle state
                printf("%c", toggle ? ' ' : '.');
            }
            else {
                printf("%c", board[i][j]);
            }
		} 
		printf("\n"); 
	} 
	printf("Score: %d\n", score); 
    frame_count++;
} 

/// Character Moving Done Editing ///

// Function of pacman moving direction
void smooth_move(float move_x, float move_y) {
	pacman_vx = move_x;
	pacman_vy = move_y;
}

/// Position Update Done Editing ///

// Update the position of pacman 
void position_update() {
	board[(int)pacman_y][(int)pacman_x] = EMPTY;	// Clear the old position
	float new_x = pacman_x + pacman_vx;
	float new_y = pacman_y + pacman_vy;

	// Check if the new position is valid
	if (board[(int)new_y][(int)new_x] != WALL) {
		// if true, update the position
		pacman_x = new_x;
		pacman_y = new_y;
	}

	// Check if the new position is food
	if (board[(int)pacman_y][(int)pacman_x] == FOOD) {
		score += 1;
		curr++;
		food--;
	}

	// Check if the new position is demon
	if (board[(int)pacman_y][(int)pacman_x] == DEMON) {
		alive = 0;
	}

	// Update to the board
	board[(int)pacman_y][(int)pacman_x] = PACMAN;	// Set the new position
}


// Main Function 
int main() 
{ 
	int flag = 1;
	initialize(); 
	char ch; 
	int totalFood = food; 
	// Instructions to Play 
	printf("Use buttons for w(up), a(left) , d(right) and "
		"s(down)\nAlso, Press q for quit\n"); 

	printf("Enter Y to continue: \n"); 

	ch = getch(); 
	if (ch != 'Y' && ch != 'y') { 
		printf("Exit Game! "); 
		flag = 0;
	} 

	while (flag && alive && food > 0) { 
		draw(); 
		position_update();
		printf("Total Food count: %d\n", totalFood); 
		printf("Total Food eaten: %d\n", curr); 
		if (alive == 0) { 
			// Clear screen 
			system("cls"); 
			printf("Game Over! Eaten by Demon\n Your Score: "
				"%d\n", 
				score); 
			flag = 0; 
			printf("Do you want to continue? (Y/N)\n");
			ch = getch();
			if (ch == 'Y' || ch == 'y') {
				// Reset game state
				alive = 1;
				score = 0;
				curr = 0;
				frame_count = 0;
				flag = 1;
				initialize();
			} else {
				flag = 0;
			}
		} 
		//printf("breakpoint\n");
		if (food == 0) { 
			// Clear screen 
			system("cls"); 
			printf("You Win! \n Your Score: %d\n", score); 
			flag = 0; 
			ch = getch();
			if (ch == 'Y' || ch == 'y') { 
				// Reset game state
				alive = 1;
				score = 0;
				curr = 0;
				frame_count = 0;
				stage++; 
				flag = 1;
				initialize(); 
			}
		} 
		
		// check if any key is pressed
		if (_kbhit()) {
			// Taking the Input from the user 
			ch = getch(); 

			// Moving According to the 
			// input character 
			switch (ch) { 
			case 'w': 
				smooth_move(0, -moving_speed);
				break; 
			case 's': 
				smooth_move(0, moving_speed); 
				break; 
			case 'a': 
				smooth_move(-moving_speed, 0); 
				break; 
			case 'd': 
				smooth_move(moving_speed, 0); 
				break; 
			case 'q': 
				printf("Game Over! Your Score: %d\n", score); 
				flag = 0; 
			} 
			
		// determine to continue or not
		
		}	
	} 

	system("pause");
	return 0; 
}
