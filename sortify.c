#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <time.h>

#define MSG_WELCOME "WELCOME TO SORTIFY!"
#define MSG_SORT "Sort the following numbers:"
#define MSG_SORT2 "Please sort the numbers"
#define MSG_WELL "Well done!"
#define MSG_WIN "Congratulations, you win!"
#define MSG_OVER "Game Over."
#define MSG_WRONG "Wrong answer."
#define MSG_MAX "You have reached the maximum number of moves."
#define MSG_BYE "Bye."
#define MSG_UNKNOWN "Unknown option."
/* Use puts() to print constant strings */

int rand_number(const int, const int);
void print_status(const int, const int, const int);
void print_menu(void);

int main(int argc, char **argv)
{
	char user_input;
	unsigned short int input_count, level = 0, current_round = 0, current_points = 0;
	bool active = true;

	puts(MSG_WELCOME);

	// Handle Arguments
	if (argc == 1)
		srand(time(NULL));
	else
		srand(atoi(argv[1]));

	print_menu();
	while (active)
	{
		input_count = 0;

		scanf("%c", &user_input);

		if (user_input == '\n')
			continue;

		// Check for input buffer, count size
		int c;
		while ((c = getchar()) != '\n' && c != EOF)
		{
			input_count++;
		}

		// If user fails to input only one char that's an unknown command
		if (input_count > 0)
		{
			puts(MSG_UNKNOWN);
			continue;
		}

		switch (user_input)
		{
			case 'q':
				puts(MSG_BYE);
				active = false;
				break;

			case 'm':
				print_menu();
				break;

			case 's':
				print_status(level + 1, current_points, current_round);
				break;

			case 'p':
				break;

			default:
				puts(MSG_UNKNOWN);
		}
	}

	return 0;
}

/* generate a random integer between min and max */
int rand_number(const int min, const int max)
{
	if (max < min)
	{
		puts("Max must be larger than Min");
		exit(0);
	}
	int n = abs(max - min) + 1;
	return (rand() % n) + min;
}

/* print the game status */
void print_status(const int level, const int score, const int plays)
{
	puts("+-----------------------------+");
	printf("| level:  %02d                  |\n", level);
	printf("| points: %02d                  |\n", score);
	printf("| plays:  %02d                  |\n", plays);
	puts("+-----------------------------+");
}

/* print the option menu */
void print_menu(void)
{
	puts("+-----------------------------+");
	puts("| SORTIFY                     |");
	puts("| p - next chalenge           |");
	puts("| q - quit                    |");
	puts("| m - print this information  |");
	puts("| s - show your status        |");
	puts("+-----------------------------+");
}