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

//// EXTRA
#define N_SIZE 4 // Affects the number of numbers that the player has to order. Default is 4.

void play(unsigned short int *, unsigned short int *, unsigned short int *, unsigned short int *, short int *, short int *);
int rand_number(const int, const int);
void print_status(const int, const int, const int);
void print_menu(void);

int main(int argc, char **argv)
{
	char user_input;
	unsigned short int input_count, level = 0, current_round = 0, current_points = 0, level_threshold[5] = {10, 20, 30, 40, 50};
	short int min_values[5] = {0, 0, -50, -100, -200}, max_values[5] = {10, 30, 30, 0, -100};
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
				play(&level, &current_round, &current_points, level_threshold, min_values, max_values);
				break;

			default:
				puts(MSG_UNKNOWN);
		}
	}

	return 0;
}

// Orders numbers
void order_numbers(short int * numbers)
{
	unsigned short int i, j;
	short int t;

	for (i = 0; i < N_SIZE; i++)
	{
		for (j = 0; j < N_SIZE; j++)
		{
			if (*(numbers + i) < *(numbers + j))
			{
				t = *(numbers + j);
				*(numbers + j) = *(numbers + i);
				*(numbers + i) = t;
			}
		}
	}

}

// Generated the random numbers and displays them
void generate_numbers_and_display(unsigned short int * level, short int * min_values, short int * max_values, short int * numbers)
{
	puts(MSG_SORT);
	for (unsigned short i = 0; i < N_SIZE; i++)
	{
		*(numbers + i) = rand_number(*(min_values + *level), *(max_values + *level));
		printf("%d", *(numbers + i));
		if (i < N_SIZE - 1)
			printf(", ");
		else
			printf("\n");
	}
}

void play(unsigned short int * level, unsigned short int * current_round,
					unsigned short int * current_points, unsigned short int * level_threshold, short int * min_values, short int * max_values)
{
	short int current_generated_numbers[N_SIZE], current_player_numbers[N_SIZE];

	// + 1 Round
	*current_round = *current_round + 1;

	generate_numbers_and_display(level, min_values, max_values, current_generated_numbers);

	order_numbers(current_generated_numbers);
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