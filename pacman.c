// Pacman Game in C language 
#include <conio.h> 
#include <stdio.h> 
#include <stdlib.h> 

// All the elements to be used 
// Declared here 
#define WIDTH 40 
#define HEIGHT 20 
#define PACMAN 'C' 
#define WALL '#' 
#define FOOD '.' 
#define EMPTY ' ' 
#define DEMON 'X' 
#define moving_speed 0.1

// Global Variables are 
// Declared here 
int res = 0; 
int score = 0; 
char board[HEIGHT][WIDTH]; 
int food = 0; 
int curr = 0; 

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
	// Putting Walls as boundary in the Game 
	for (int i = 0; i < HEIGHT; i++) { 
		for (int j = 0; j < WIDTH; j++) { 
			if (i == 0 || j == WIDTH - 1 || j == 0 
				|| i == HEIGHT - 1) { 
				board[i][j] = WALL; 
			} 
			else
				board[i][j] = EMPTY; 
		} 
	} 

	// Putting Walls inside the Game 
	int count = 50; 
	while (count != 0) { 
		int i = (rand() % (HEIGHT + 1)); 
		int j = (rand() % (WIDTH + 1)); 

		if (board[i][j] != WALL && board[i][j] != PACMAN) { 
			board[i][j] = WALL; 
			count--; 
		} 
	} 

	int val = 5; 
	while (val--) { 
		int row = (rand() % (HEIGHT + 1)); 
		for (int j = 3; j < WIDTH - 3; j++) { 
			if (board[row][j] != WALL 
				&& board[row][j] != PACMAN) { 
				board[row][j] = WALL; 
			} 
		} 
	} 

	// Putting Demons in the Game 
	count = 10; 
	while (count != 0) { 
		int i = (rand() % (HEIGHT + 1)); 
		int j = (rand() % (WIDTH + 1)); 

		if (board[i][j] != WALL && board[i][j] != PACMAN) { 
			board[i][j] = DEMON; 
			count--; 
		} 
	} 

	// Cursor at Center 
	pacman_x = WIDTH / 2; 
	pacman_y = HEIGHT / 2; 
	board[(int)pacman_y][(int)pacman_x] = PACMAN; 

	// Points Placed 
	for (int i = 0; i < HEIGHT; i++) { 
		for (int j = 0; j < WIDTH; j++) { 
			if (i % 2 == 0 && j % 2 == 0 
				&& board[i][j] != WALL 
				&& board[i][j] != DEMON 
				&& board[i][j] != PACMAN) { 

				board[i][j] = FOOD; 
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
                printf("%c", toggle ? '*' : '.');
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

// Function of pacman moving direction
void smooth_move(float move_x, float move_y) {
	pacman_vx = move_x;
	pacman_vy = move_y;
}

// Update the position of pacman
void position_update() {
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
	}

	// Check if the new position is demon
	if (board[(int)pacman_y][(int)pacman_x] == DEMON) {
		res = 1;
	}

	// Update to the board
	board[(int)pacman_y][(int)pacman_x] = EMPTY;	// Clear the old position
	board[(int)pacman_y][(int)pacman_x] = PACMAN;	// Set the new position
}


// Main Function 
int main() 
{ 
	initialize(); 
	char ch; 
	food -= 35; 
	int totalFood = food; 
	// Instructions to Play 
	printf(" Use buttons for w(up), a(left) , d(right) and "
		"s(down)\nAlso, Press q for quit\n"); 

	printf("Enter Y to continue: \n"); 

	ch = getch(); 
	if (ch != 'Y' && ch != 'y') { 
		printf("Exit Game! "); 
		return 1; 
	} 

	while (1) { 
		draw(); 
		position_update();
		printf("Total Food count: %d\n", totalFood); 
		printf("Total Food eaten: %d\n", curr); 
		if (res == 1) { 
			// Clear screen 
			system("cls"); 
			printf("Game Over! Dead by Demon\n Your Score: "
				"%d\n", 
				score); 
			system("pause");
			return 1; 
		} 

		if (res == 2) { 
			// Clear screen 
			system("cls"); 
			printf("You Win! \n Your Score: %d\n", score); 
			return 1; 
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
				return 0; 
			} 
		}	
	} 

	return 0; 
}
