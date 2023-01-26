#include "adminui.h"
#include "pilot.h"
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>

bool_e quit_case = TRUE;

static void AdminUI_captureChoice();
static void AdminUI_askMVt(Direction dir);
static VelocityVector AdminUI_translate(Direction dir);
static void AdminUI_ask4Log();
static void AdminUI_askClearLog();
static void AdminUI_eraseLog();
static void AdminUI_quit();
static void AdminUI_run();
static void AdminUI_display();

void AdminUI_new()
{
	Pilot_new();
}

void AdminUI_start()
{
	Pilot_start();
	AdminUI_run();
}

static void AdminUI_run()
{
	while (quit_case)
	{
		AdminUI_display();
	}
}

void AdminUI_stop()
{

}

void AdminUI_free()
{
	Pilot_free();
}

static void AdminUI_captureChoice()
{
	struct termios oldt, newt;
	tcgetattr(STDIN_FILENO, &oldt);
	newt = oldt;
	newt.c_lflag &= ~(ICANON | ECHO);
	tcsetattr(STDIN_FILENO, TCSANOW, &newt);
	log_key_e command = getchar();
	tcsetattr(STDIN_FILENO,TCSANOW, &oldt);

	switch(command)
	{
		case LOG_LEFT:
			AdminUI_askMVt(LEFT);
			break;
		case LOG_RIGHT:
			AdminUI_askMVt(RIGHT);
			break;
		case LOG_BACKWARD:
			AdminUI_askMVt(BACKWARD);
			break;
		case LOG_FORWARD:
			AdminUI_askMVt(FORWARD);
			break;
		case LOG_CLEAR:
			AdminUI_askClearLog();
			break;
		case LOG_STOP:
			AdminUI_askMVt(STOP);
			break;
		case LOG_ROBOT_STATE:
			AdminUI_ask4Log();
			break;
		case LOG_QUIT:
			AdminUI_quit();
			break;
		default:
			break;
	}
}

static void AdminUI_askMVt(Direction dir)
{
	Pilot_setVelocity(AdminUI_translate(dir));
}

static VelocityVector AdminUI_translate(Direction dir)
{
	VelocityVector vel;
	vel.power = 100;
	vel.dir = dir;
	return vel;
}

static void AdminUI_ask4Log()
{
	Pilot_check();
	PilotState p_state = Pilot_getState();
	printf("\n Collision; %d", p_state.collision);
	printf("\n Luminosity: %f", p_state.luminosity);
	printf("\n Speed: %d", p_state.speed);
}

static void AdminUI_askClearLog()
{
	AdminUI_eraseLog();
}

static void AdminUI_eraseLog()
{
	printf("\033[2J");
}

static void AdminUI_quit()
{
	quit_case = FALSE;
	Pilot_stop();
}

static void AdminUI_display()
{
	printf("Robot V1\n");
	printf("Vous pouvez faire les actions suivantes :\n");
	printf("q:aller à gauche\n");
	printf("d:aller à droite\n");
	printf("z:avancer\n");
	printf("s:reculer\n");
	printf(" :stopper\n");
	printf("e:effacer les logs\n");
	printf("r:afficher l'état du robot\n");
	printf("a:quitter\n");
	AdminUI_captureChoice();
}
