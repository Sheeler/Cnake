#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <GLUT/glut.h>
#include <pthread.h>

#define UP 		-1
#define DOWN 	-1
#define LEFT 	-1
#define RIGHT	-1



//Things todo

//function pointer syntax

//set up on github

//figure out the direction input. if it's janky, use wasd.
//draw a sqare that moves a unit per arrow press
//draw a square that moves an aligned box per arrow press
//draw a square that moves in head_direction until direction is changed via press.

//Apple placement: make it such that it appears where the snake is not

//synchronize access to states. brainstorm full synchro scheme

//Growth: collision with apple spawns new square behind head that follows
//Proper growth: collision with apple 
//Turns: ensure turning only happens at a "corner" i.e. when the head turns, the tail should be
//		unaffected until it advances through the corner. Make sure you don't add a "turn" entry
//		if new direction matches old
//Proper apple placement: spawn apple in spot not currently occupied. by any of snake
//proper snake drawing: fill in spaces between squares.

//Game over: collision with self or walls ends game/restarts game/goes to menu

//tweaking speed/framerate/sizes




//other general things
//colors
//messages
//levels/begin game/restart/pause/difficulty

//high scores? requires file state

typedef struct pos {
	unsigned int x;
	unsigned int y;

} pos;

typedef struct turn {
	pos *location;
	int from;
	int to; //may not want ints here
	struct turn *next_turn;

} turn;

typedef struct snake_state {

	pos *head;
	pos *tail;

	int head_direction;
	int tail_direction; //may not want ints here

	turn *oldest_turn;

} snake_state;


pthread_mutex_t *snake_state_lock = NULL;
snake_state *snake = NULL;



void init_snake(void) {

	snake = malloc(sizeof(*snake));
	snake->head = malloc(sizeof(*snake->head));
	snake->tail = malloc(sizeof(*snake->tail));
	snake->oldest_turn = NULL;

	int head_direction = 0; //TODO
	int tail_direction = 0; //TODO

	snake->head->x = 30;
	snake->head->y = 30;

}

void init_apple(void) {

}


void *input_loop(void *ctx) {

	(void) ctx;

	int input = 0;

	while(1) {
		printf("input loop\n");
		//get input
		input = getchar();

		//update snake params (direction)
		if (input == UP || input == (int) 'w') {
			snake->head->y -= 30; //bounds checking ugh
		}
		else if (input == DOWN || input == (int) 's') {
			snake->head->y += 30; //same bounds issue
		}
		else if (input == RIGHT || input == (int) 'd') {
			snake->head->x += 30; //bounds
		}
		else if (input == LEFT || input == (int) 'a') {
			snake->head->x -= 30; //bounds (if bad params collision?)
		}

	}
	return NULL;

}


void drawSnake(void) {

	//Advance snake one unit, if growing don't advance tail
	printf("draw snake %d, %d\n", snake->head->x, snake->head->y);
	//this is incorrect! look up glRecti! And clearing old rectangles
	glRecti(snake->head->x, snake->head->y, 50, 50);

}

void checkCollision(void) {
	//See if snake is in itself or wall
}

void drawApple(void) {
	//Choose a new apple location and draw it
}

void checkEating(void) {

	//if apple collision, set "growing" to now,
}

//the world is divided into 40x40 unit squares.
//the snake should always appear in the inner 20 in the direction parallel
//in the direction it advances, fill in the "empty space" between two occupied rectangles


void display(void) {

    glClearColor(0, 0, 0, 0);
    glColor3d(160, 80, 200);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //a more useful helper
   // glRecti(10, 10, 350, 470);

    drawSnake();
    checkCollision();
    checkEating();

    glutSwapBuffers();

}

void reshape(int width, int height) {

	printf("reshape");
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    //set the coordinate system, with the origin in the top left
    gluOrtho2D(0, width, height, 0);
    glMatrixMode(GL_MODELVIEW);

}

void idle(void) {

    glutPostRedisplay();
}

int main(int argc, char **argv) {

    //a basic set up...
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(360, 480);

    //create the window, the argument is the title
    glutCreateWindow("Snake");

    //set up snake and apples
    init_snake();
    init_apple();

    //Set up threading
    snake_state_lock = malloc(sizeof(*snake_state_lock));
    pthread_mutex_init(snake_state_lock, NULL);

    //input loop
    pthread_t input_thread;
    pthread_create(&input_thread, NULL, &input_loop, NULL);

    //pass the callbacks
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutIdleFunc(idle);

    glutMainLoop();

    //we never get here because glutMainLoop() is an infinite loop
    return 0;

}
