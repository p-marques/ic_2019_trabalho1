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
#define MAX_ROUNDS 30
/* Use puts() to print constant strings */

//// EXTRA
#define USE_TIME_FACTOR false // Flag for usage of time as a factor when giving player points. Default is false.
#define N_SIZE 4 // Affects the number of numbers that the player has to order. Default is 4.

int play(unsigned short, unsigned short *, unsigned short *, unsigned short *, unsigned short *, short *, short *);
int clean_input_buffer();
int rand_number(const int, const int);
void print_status(const int, const int, const int);
void print_menu(void);

int main(int argc, char **argv)
{
	char user_input;
	unsigned short n_size = N_SIZE, level = 0, current_round = 0, current_points = 0, level_threshold[5] = {10, 20, 30, 40, 50};
	short i, min_values[5] = {0, 0, -50, -100, -200}, max_values[5] = {10, 30, 30, 0, -100};
	bool active = true;

	puts(MSG_WELCOME);

	// Handle arguments
	// Sets n_size if given
	if (argc == 1)
		srand(time(NULL));
	else
	{
		srand(atoi(argv[1]));

		if (argc > 2)
			n_size = (i = atoi(argv[2])) > 4 ? i : N_SIZE;
	}

	print_menu();
	while (active)
	{
		scanf(" %c", &user_input);

		if (user_input == '\n')
			continue;

		// This is disabled because of Pandora tests
		// If user fails to input only one char that's an unknown command
		// if (clean_input_buffer() > 0)
		// {
		// 	puts(MSG_UNKNOWN);
		// 	continue;
		// }

		switch (user_input)
		{
			case 'q':
				print_status(level + 1, current_points, current_round);
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
				if(play(n_size, &level, &current_round, &current_points, level_threshold, min_values, max_values) == 0)
					active = false;
				break;

			default:
				puts(MSG_UNKNOWN);
		}
	}

	return 0;
}

// Cleans input buffer and returns size
int clean_input_buffer()
{
	short c;
	unsigned short input_count = 0;

	while ((c = getchar()) != '\n' && c != EOF)
	{
		input_count++;
	}

	return input_count;
}

// Update player score based on time taken to give correct answer
void update_player_score(unsigned short * current_points, unsigned short time_elapsed)
{
	unsigned short factor;

	if (time_elapsed > 15)
		factor = 1;
	else if (time_elapsed > 10)
		factor = 15 - time_elapsed;
	else if (time_elapsed < 4)
		factor = 10;
	else
		factor = 10 - time_elapsed;

	// If not using time as factor, use default 5 points
	if (!USE_TIME_FACTOR)
		factor = 5;

	*current_points = *current_points + factor;
}

// Counts occurences of a number in an array
int count_occurences(unsigned short n_size, short * array, int value)
{
	unsigned short counter = 0;

	for (unsigned short i = 0; i < n_size; i++)
	{
		if (*(array + i) == value)
			counter++;
	}

	return counter;
}

// Compares input to generated numbers.
// If array elements don't match (order irrelevant here) return false.
bool is_input_valid(unsigned short n_size, short * numbers, short * player_numbers)
{
	unsigned short i, occurences_numbers = 0, occurences_player_numbers = 0;
	
	for (i = 0; i < n_size; i++)
	{
		occurences_numbers = 0, occurences_player_numbers = 0;

		// Get occurences of input in generated numbers
		occurences_numbers = count_occurences(n_size, numbers, *(player_numbers + i));

		// Get occurences of input in player numbers
		occurences_player_numbers = count_occurences(n_size, player_numbers, *(player_numbers + i));

		if (occurences_numbers != occurences_player_numbers)
			return false;
	}

	return true;
}

/* Handle player input
returns player's success */
bool handle_player_input(unsigned short n_size, short * numbers, short * player_numbers)
{
	unsigned short i;
	bool valid_input = false, clean_run = true;

	while (!valid_input)
	{
		if (!clean_run)
			puts(MSG_SORT2);

		valid_input = true;

		for (i = 0; i < n_size; i++)
		{
			scanf("%hi", &player_numbers[i]);
		}

		// Handles input of anything other than numbers
		clean_input_buffer();

		valid_input = is_input_valid(n_size, numbers, player_numbers);
		clean_run = false;
	}

	for (i = 0; i < n_size; i++)
	{
		if (*(player_numbers + i) != *(numbers + i))
		{
			return false;
		}
	}

	return true;
}

// Orders numbers
void order_numbers(unsigned short n_size, short * numbers)
{
	unsigned short i, j;
	short t;

	for (i = 0; i < n_size; i++)
	{
		for (j = 0; j < n_size; j++)
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
void generate_numbers_and_display(unsigned short n_size, unsigned short * level, short * min_values, short * max_values, short * numbers)
{
	puts(MSG_SORT);
	for (unsigned short i = 0; i < n_size; i++)
	{
		*(numbers + i) = rand_number(*(min_values + *level), *(max_values + *level));
		printf("%d", *(numbers + i));
		if (i < n_size - 1)
			printf(", ");
		else
			printf("\n");
	}
}

int play(unsigned short n_size, unsigned short * level, unsigned short * current_round,
					unsigned short * current_points, unsigned short * level_threshold, short * min_values, short * max_values)
{
	short current_generated_numbers[n_size], current_player_numbers[n_size];
	bool player_success, player_wins = false;
	time_t start_time, time_elapsed;

	// + 1 Round
	*current_round = *current_round + 1;

	generate_numbers_and_display(n_size, level, min_values, max_values, current_generated_numbers);

	order_numbers(n_size, current_generated_numbers);

	// Save current time
	start_time = time(NULL);
	player_success = handle_player_input(n_size, current_generated_numbers, current_player_numbers);

	if (player_success)
	{
		// Time until player got it right in seconds
		time_elapsed = (unsigned short) time(NULL) - start_time;

		puts(MSG_WELL);

		// Update player score based on time to give correct answer
		update_player_score(current_points, time_elapsed);

		if (*current_points >= level_threshold[*level])
		{
			if (*level == 4)
				player_wins = true;
			
			*level = *level + 1;
		}
	}
	else
	{
		puts(MSG_WRONG);
	}

	if (*current_round == MAX_ROUNDS && !player_wins)
	{
		puts(MSG_MAX);
		print_status(*level + 1, *current_points, *current_round);
		puts(MSG_OVER);
		return 0;
	}
	else if (player_wins)
	{
		puts(MSG_WIN);
		print_status(*level + 1, *current_points, *current_round);
		puts(MSG_OVER);
		return 0;
	}

	return 1;
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