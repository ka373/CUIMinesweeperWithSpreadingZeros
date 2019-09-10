//*****************************************************************************************
// minesweeper_cui.c
// - Simple code Description: Minesweeper game
// - Date of production and release: Monday 9 September 2019
//
// - Code production: ka373
// - github: https://github.com/ka373
//
// - I have left some test code, so if you need to modify the code, please refer to it.
//*****************************************************************************************

#include<stdio.h>
#include<stdlib.h>
#include<time.h>

#define MINE -1

#define STEPPED -2
#define MINE_CHECK -3
#define DONT_KNOW -4

#define UNMARKED -5

#define NUM_MINE 7
#define NUM_OF_ROWS 8
#define NUM_OF_COLUMNS 8

int i = 0, j = 0; //for index
int num_of_stepped = 0; //Count the number of steps and use it to end the game

int board_visible[NUM_OF_ROWS][NUM_OF_COLUMNS];	//Similar to the board seen by the user
int board_kotae[NUM_OF_ROWS][NUM_OF_COLUMNS];	//Board with numbers and mines (correct answer board)

void print_board(int board_visible[NUM_OF_ROWS][NUM_OF_COLUMNS]);
void print_uppernum(int num_of_upper);

int obtained_randoms[NUM_MINE];	//Random numbers for Mines
int process_NUM_MINE_randoms();	//The process for obtaining random numbers

int process_game(int row_entered, int column_entered, int check_type);
int can_go(int i, int j);

clock_t start_time, end_time;
int elapsed_milli = 0;
void print_elapsed_milli();

/////////////////////////////////////////////////////////////////////////////
int main() {
	int checking_row = -1;
	int checking_column = -1;
	int temp_type = -1;
	char input_type_char = '?';

	int is_continue = 0;

	int num_of_mine = NUM_MINE;
	int temp_mine_count = 0;

	int mines_marked = 0; //The number of mines I marked on the screen

	srand((unsigned int)time(NULL));
	///////////////////////////////////
	//board initialize
	for (i = 0; i < NUM_OF_ROWS; i++) {
		for (j = 0; j < NUM_OF_COLUMNS; j++) {
			board_visible[i][j] = UNMARKED;
			board_kotae[i][j] = UNMARKED;
		}
	}
	//////////////////////////////////
	//mine planting
	if (process_NUM_MINE_randoms() == 1) {
		for (i = 0; i < NUM_MINE; i++) {
			int mine_row = obtained_randoms[i] / NUM_OF_ROWS;
			int mine_column = obtained_randoms[i] % NUM_OF_ROWS;

			board_kotae[mine_row][mine_column] = MINE;
			//printf("\nLocation of mines: row: %d column: %d\n", mine_row, mine_column);
		}
	}
	///////////////////////////////////

	////////////////////////////////////////////////////
	//Creating kotae array with numbers
	for (i = 0; i < NUM_OF_ROWS; i++) {
		for (j = 0; j < NUM_OF_COLUMNS; j++) {

			//Enter a number if it is not a land mine
			if (board_kotae[i][j] != MINE) {
				temp_mine_count = 0;
				/////////////////////////////////
				//You can also use the can_go(int i, int j) function.
				
				//upper
				if (i - 1 >= 0) {
					if (j - 1 >= 0) { if (board_kotae[i - 1][j - 1] == MINE) { temp_mine_count++; } }			//Determine whether the upper left is mine
					if (board_kotae[i - 1][j] == MINE) { temp_mine_count++; }						//upper
					if (j + 1 >= 0) { if (board_kotae[i - 1][j + 1] == MINE) { temp_mine_count++; } }			//upper right
				}
				
				//same line
				if (j - 1 >= 0) { if (board_kotae[i][j - 1] == MINE) { temp_mine_count++; } }					//left
				if (j + 1 < NUM_OF_COLUMNS) { if (board_kotae[i][j + 1] == MINE) { temp_mine_count++; } }			//right

				//lower
				if (i + 1 < NUM_OF_ROWS) {
					if (j - 1 >= 0) { if (board_kotae[i + 1][j - 1] == MINE) { temp_mine_count++; } }			//lower left
					if (board_kotae[i + 1][j] == MINE) { temp_mine_count++; }											//lower
					if (j + 1 < NUM_OF_COLUMNS) { if (board_kotae[i + 1][j + 1] == MINE) { temp_mine_count++; } }		//lower right
				}
				board_kotae[i][j] = temp_mine_count;
				//////////////////////////////
			}// end if != -1
		}//end of for j
	}//end of for i
	 ///////////////////////////////////

	 //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	 //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	 //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	 //Game play(Core game progress code)

	 //Game start
	print_board(board_visible);	//Print Board First
	start_time = clock(); //Start time measurement

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	do {
		printf("Total Mines: %d, Mines marked: %d\n\n", NUM_MINE, mines_marked);
		printf("Please enter the location(row, column) and type\n");
		printf("[type]     s: step     f: flag     ?: ?\n");
		printf("e.g. Row 3, column 5, step => 3 5 s \n");

		scanf_s("%d %d", &checking_row, &checking_column);
		scanf_s(" %c", &input_type_char, sizeof(input_type_char));

		switch (input_type_char) {
		case 's': case 'S':		temp_type = STEPPED;					break;
		case 'f': case 'F':		temp_type = MINE_CHECK;		mines_marked++;		break;
		case '?':  default:		temp_type = DONT_KNOW;					break;
		}
		is_continue = process_game(checking_row, checking_column, temp_type);
		print_board(board_visible);

		print_elapsed_milli();

	} while (is_continue == 1 && num_of_stepped < (NUM_OF_ROWS * NUM_OF_COLUMNS - num_of_mine));
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	//When you stepped on a landmine
	if (num_of_stepped < (NUM_OF_ROWS * NUM_OF_COLUMNS - num_of_mine)) {
		print_board(board_visible);
		print_board(board_kotae);
		printf("Game Over :)\n");
		print_elapsed_milli();
		system("pause");
	}

	//When you cleared the game
	else {
		printf("Game clear! Congratulations! \n");
		printf("THANK YOU FOR PLAYING :)\n");
		printf("- ka373 -\n");
		print_elapsed_milli();
		system("pause");
	}
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	return 0;
}//end of main

 ////////////////////////////////////////////////////////
int process_game(int row_entered, int column_entered, int check_type) {
	int is_live = 1;

	switch (board_visible[row_entered][column_entered]) {
	case 0: case 1: case 2: case 3: case 4: case 5: case 6: case 7: case 8:
		//printf("Row %d, column %d is where you've already gone.\n", row_entered, column_entered);
		return is_live;
		break;
	}

	if (can_go(row_entered, column_entered) == 0) { return is_live; }

	//printf("check_type: %d\n", check_type);
	switch (check_type) {
	case STEPPED:
		if (board_kotae[row_entered][column_entered] == MINE) {
			printf("You stepped on a land mine.\n");
			board_visible[row_entered][column_entered] = MINE;

			is_live = 0;	 return is_live;
		}
		else {
			num_of_stepped++;
			board_visible[row_entered][column_entered] = board_kotae[row_entered][column_entered];
		}

		/////////////////////////////////////////////////////////////////////////////
		//spreading zeros(0の連鎖爆弾！0의 연쇄폭탄!)
		if (board_visible[row_entered][column_entered] == 0) {

			if (can_go(row_entered - 1, column_entered - 1)) {
				process_game(row_entered - 1, column_entered - 1, STEPPED);	//Spread to left upper diagonal ↖
			}

			if (can_go(row_entered - 1, column_entered + 1)) {
				process_game(row_entered - 1, column_entered + 1, STEPPED);	//Spread across the upper right diagonal ↗
			}

			if (can_go(row_entered + 1, column_entered - 1)) {
				process_game(row_entered + 1, column_entered - 1, STEPPED);	//Spread to left lower diagonal ↙
			}

			if (can_go(row_entered + 1, column_entered + 1)) {
				process_game(row_entered + 1, column_entered + 1, STEPPED);	//Spread across the lower right diagonal ↘
			}

			if (can_go(row_entered - 1, column_entered)) { process_game(row_entered - 1, column_entered, STEPPED); }//Spread up ↑
			if (can_go(row_entered + 1, column_entered)) { process_game(row_entered + 1, column_entered, STEPPED); }//Spread down ↓
			if (can_go(row_entered, column_entered - 1)) { process_game(row_entered, column_entered - 1, STEPPED); }//Spread left ←
			if (can_go(row_entered, column_entered + 1)) { process_game(row_entered, column_entered + 1, STEPPED); }//Spread to the right →
		}// end of if연쇄폭탄
		 /////////////////////////////////////////////////////////////////////////////
		break;
	default:
		board_visible[row_entered][column_entered] = check_type;
		break;
	}
	//print_board(board_visible);
	return is_live;
}
////////////////////////////////////////////////////////
//Print the top numbers
void print_uppernum(int num_of_upper) {
	printf("\n\n ");
	printf("    ");
	for (i = 0; i < num_of_upper; i++) { printf(" %d ", i); }
	printf("\n");
}
////////////////////////////////////////////////////////
void print_board(int board_visible[NUM_OF_ROWS][NUM_OF_COLUMNS]) {
	system("cls");
	print_uppernum(NUM_OF_COLUMNS);
	printf(" _______________________________\n");

	for (i = 0; i < NUM_OF_ROWS; i++) {
		printf("  %d_ ", i);

		for (j = 0; j < NUM_OF_COLUMNS; j++) {
			switch (board_visible[i][j]) {

			case UNMARKED:			printf("□ ");		break;
			case MINE:			printf(" * ");		break;
			case MINE_CHECK:		printf(" F ");		break;	//Flag
			case DONT_KNOW:			printf(" ? ");		break;
			case 0:				printf("   ");		break;

			case 1:	case 2:	case 3:	case 4:	case 5:	case 6:	case 7:	case 8:
				printf(" %d ", board_visible[i][j]);
				break;

			default:			printf(" D ");		break;
			}
		}
		printf("\n");
		printf(" _______________________________\n");
	}
	//printf("\n");
}
////////////////////////////////////////////////////////
int can_go(int i, int j) {
	int flag = 1;
	if (i < 0 || j < 0) { flag = 0; }	//minus
	if (i >= NUM_OF_ROWS || j >= NUM_OF_COLUMNS) { flag = 0; } //over
	return flag;
}
////////////////////////////////////////////////////////
int process_NUM_MINE_randoms()
{
	int i = 0, j = 0; //for index
	int temp_random = 0;

	const int num_of_want_nums = NUM_MINE;			//for different number, NUM_OF_WANT_NUMS <= TO_NUM
	const int to_num = NUM_OF_ROWS * NUM_OF_COLUMNS;	//if you want to optaion integer of 0-9,set 10

	int is_duplicated = 0;

	do {
		is_duplicated = 0;
		temp_random = rand() % to_num;

		for (j = 0; j < i; j++) {
			if (temp_random == obtained_randoms[j]) {
				is_duplicated = 1;
				break;
			}
		}
		if (is_duplicated == 0) { obtained_randoms[i] = temp_random; i++; }
	} while (i < num_of_want_nums);

	//printf("%d different random numbers: ", num_of_want_nums);
	//for (i = 0; i < num_of_want_nums; i++) { printf(" %d", obtained_randoms[i]); }

	return 1;
}
////////////////////////////////////////////////////////
void print_elapsed_milli() {
	int s = 0;
	int left_elapsed_milli = 0;
	end_time = clock();
	elapsed_milli = (int)(end_time - start_time);
	s = elapsed_milli / 1000;
	left_elapsed_milli = elapsed_milli % 1000;

	printf("Elapsed time: %d.%03ds\n", s, left_elapsed_milli);
}
////////////////////////////////////////////////////////
