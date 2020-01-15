#pragma once
#ifndef FUNCTIONS_H
#define FUNTIONS_H
#define CORD_BOARD_X 70
#define CORD_BOARD_Y 3
#define CORD_NAME_X 5
#define CORD_NAME_Y 1
#define SIZE_INF 17
#define BORDER 2
#define EXIT 27
#define ENTER 13
#define DEFAULT_TABLE_SIZE 12
#define MIN_VALUE 10.0
#define MAX_VALUE 20.0
#define MENU_EXIT 1
#define MENU_DEFAULT_BOARD 2
#define MENU_RANDOM_BOARD 3
#define MENU_HINT 4
#define MENU_RESIZE_BOARD 5
#define MENU_2HINT 6
#define MENU_ADDIOTIONAL_HINT 7
#define MENU_ONE_FILLED 8
#define MENU_FILL_ONE_FIELD 9
#define MENU_AUTO_DETECT 10
#define RETURN_COLUMN 2
#define RETURN_ROW 1
#define RULE_STRING_SIZE 100
#define BUFOR_SIZE 5
#define BOARD_CURSOR 50
#define BUFOR_POS_SIZE 30
#define MIN_BOARD_SIZE 2
#define MAX_BOARD_SIZE 24
#define FILE_ERROR 0
#define SIGN_BOARD_SIZE 15
#define LEFT_KEY 0x4b
#define RIGHT_KEY 0x4d
#define UP_KEY 0x48
#define DOWN_KEY 0x50

struct field_board
{
	bool editable; 
	char value; 
	bool cursor_over;
};

struct cursor
{
	int x; 
	int y;
};

int power(int base, int index);

field_board** create_board(int board_size, cursor *cursor_pos);

void show_board(field_board** board, int board_size, int cord_x, int cord_y);

void remove_board(field_board** board, int board_size);

int insert_board_size();

void swap_default_board(char temp_board[][DEFAULT_TABLE_SIZE+1], field_board **board);

bool check_rule_1(field_board **board, int board_size, int cord_x, int cord_y, char sign);

int row_column_counter(field_board **board, int board_size, int cord_x, int cord_y, char sign);

int identity_of_rows(field_board **board, int board_size, int cord_x);

int identity_of_columns(field_board **board, int board_size, int cord_y);

int insert_data(field_board **board, int board_size, int cord_x, int cord_y, char sign);

int test_insert_data(field_board **board, int board_size, int cord_x, int cord_y, char sign, int offset);

void random_fields_board(field_board **board, int board_size, int count_field);

void simple_hint(field_board **board, int board_size, int cord_x, int cord_y);

int update_from_file(field_board **board, const int board_size);

void unsolved_field(field_board **board, int board_size);

void display_counter_zero(field_board **board, int board_size, char sign);

void display_counter_one(field_board **board, int board_size, char sign);

void one_filled_field(field_board **board, int board_size);

void filled_field(field_board **board, int board_size);

void display_info(cursor* cursor_pos);

int handle_input(int board_size, field_board **board, cursor* cursor_pos);

void auto_detected(field_board **board, int board_size);

#endif
