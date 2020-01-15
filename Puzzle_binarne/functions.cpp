#pragma warning(disable:4996)
#include "functions.h"
#include "conio2.h"
#include <cstdlib>
#include <stdio.h>
#include <string.h>
#include <time.h>

int power(int base, int index)
{
	int result = 1;
	for (int i = 0; i < index; i++)
	{
		result *= base;
	}
	return result;
}

field_board** create_board(int board_size, cursor *cursor_pos)
{
	field_board **board = new field_board *[board_size + BORDER];
	for (int i = 0; i < board_size + BORDER; i++)
	{
		board[i] = new field_board[board_size + BORDER];
		for (int j = 0; j < board_size + BORDER; j++)
		{
			board[i][j].value = '.';
			board[i][j].editable = true;
			board[i][j].cursor_over = false;
			if (i == 0 || i == board_size + 1)
			{
				board[i][j].value = '-';
			}
			else if (j == 0 || j == board_size + 1)
			{
				board[i][j].value = '|';
			}
		}
	}
	board[cursor_pos->x][cursor_pos->y].cursor_over = true;
	board[0][0].value = '+';
	board[board_size + 1][0].value = '+';
	board[0][board_size + 1].value = '+';
	board[board_size + 1][board_size + 1].value = '+';
	return board;
}

void remove_board(field_board** board, int board_size)
{
	for (int i = 0; i < board_size + BORDER; i++)
	{
		delete[] board[i];
	}
	delete[] board;
}

void show_board(field_board** board, int board_size, int cord_x, int cord_y)
{
	if (board != NULL)
	{
		for (int i = 0; i < board_size + BORDER; i++)
		{
			for (int j = 0; j < board_size + BORDER; j++)
			{
				gotoxy(cord_x + i, cord_y + j);
				if (board[i][j].cursor_over == true)
				{
					textbackground(LIGHTBLUE);
				}
				textcolor(WHITE);
				if (board[j][i].editable == false)
				{
					textcolor(LIGHTRED);
				}

				putch(board[j][i].value);
				textbackground(BLACK);
			}
		}
	}
}

int insert_board_size()
{
	int bufor[RULE_STRING_SIZE];
	int input_size = 0;
	for (int i = 0; i < 10; i++)
	{
		int a = getch();
		if (a == ENTER)//po wcisnieciu entera
		{
			for (int j = 0; j < i; j++)
			{
				input_size += (bufor[j] - 48) * power(10, i - j - 1);
			}
			break;
		}
		else if (a<'0' || a>'9')
		{
			i--;
			continue;
		}
		putch(a);
		bufor[i] = a;
	}
	return input_size;
}

void swap_default_board(char temp_board[][DEFAULT_TABLE_SIZE+1], field_board **board)
{
	for (int i = 0; i<DEFAULT_TABLE_SIZE; i++)
	{
		for (int j = 0; j<DEFAULT_TABLE_SIZE; j++)
		{
			board[i + 1][j + 1].value = temp_board[i][j];
			if (board[i + 1][j + 1].value == '0' || board[i + 1][j + 1].value == '1')
			{
				board[i + 1][j + 1].editable = false;
			}
		}
	}
}

bool check_rule_1(field_board **board, int board_size, int cord_x, int cord_y, char sign)
{
	int sum = 0;
	if (cord_x < board_size && board[cord_x + 1][cord_y].value == sign)//sprawdzenie dla 1 w dol
	{
		sum++;
		if (cord_x < board_size - 1 && board[cord_x + 2][cord_y].value == sign) sum++; //sprawdzenie dla 2 w dol
	}
	if (cord_x >= 1 && board[cord_x - 1][cord_y].value == sign)//sprawdzenie dla 1 w gore
	{
		sum++;
		if (cord_x >= 2 && board[cord_x - 2][cord_y].value == sign) sum++; //sprawdzenie dla 2 w gore
	}
	if (sum >= 2)  return false;
	sum = 0;
	if (cord_y < board_size && board[cord_x][cord_y + 1].value == sign)//sprawdzenie dla 1 w prawo
	{
		sum++;
		if (cord_y < board_size - 1 && board[cord_x][cord_y + 2].value == sign) sum++; //sprawdzenie dla 2 w prawo
	}
	if (cord_y >= 1 && board[cord_x][cord_y - 1].value == sign)//sprawdzenie dla 1 w lewo
	{
		sum++;
		if (cord_y >= 2 && board[cord_x][cord_y - 2].value == sign) sum++; //sprawdzenie dla 2 w lewo
	}
	if (sum >= 2)  return false;
	return true;
}

int row_column_counter(field_board **board, int board_size, int cord_x, int cord_y, char sign)
{
	int i;
	int row_sum = 0, column_sum = 0;
	for (i = 0; i < board_size; i++)
	{
		if (board[cord_x][i + 1].value == sign) row_sum++;
		if (board[i + 1][cord_y].value == sign) column_sum++;
	}
	if (row_sum + 1 <= (board_size / 2) && column_sum + 1 <= (board_size / 2)) return 0;
	if (row_sum + 1 >(board_size / 2)) return RETURN_ROW;
	if (column_sum + 1 > (board_size / 2)) return RETURN_COLUMN;
	return 1;
}

int identity_of_rows(field_board **board, int board_size, int cord_x)
{
	int identity = 0;
	for (int i = 1; i < board_size + 1; i++)
	{
		if (cord_x == i) continue;
		for (int j = 1; j < board_size + 1; j++)
		{
			if (board[cord_x][j].value != '.' && board[cord_x][j].value == board[i][j].value) identity++;
			if (identity == 0) continue;
			if (identity == board_size) return i;
		}
		identity = 0;
	}
	return 0;
}

int identity_of_columns(field_board **board, int board_size, int cord_y)
{
	int identity = 0;
	for (int i = 1; i < board_size; i++)
	{
		if (cord_y == i) continue;
		for (int j = 1; j < board_size + 1; j++)
		{
			if (board[j][cord_y].value != '.' && board[j][cord_y].value == board[j][i].value) identity++;
			if (identity == 0) continue;
			if (identity == board_size) return i;
		}
		identity = 0;
	}
	return 0;
}

int insert_data(field_board **board, int board_size, int cord_x, int cord_y, char sign)
{
	int rule1_good = check_rule_1(board, board_size, cord_x, cord_y, sign);
	int rule2_good = row_column_counter(board, board_size, cord_x, cord_y, sign);
	int rule3_row_good;
	int rule3_column_good;
	if (board[cord_x][cord_y].editable == true && (rule1_good != 0) && (rule2_good == 0))
	{
		char temp_board = board[cord_x][cord_y].value;
		board[cord_x][cord_y].value = sign;
		rule3_row_good = identity_of_rows(board, board_size, cord_x);
		rule3_column_good = identity_of_columns(board, board_size, cord_y);
		if (rule3_row_good != 0)
		{
			board[cord_x][cord_y].value = temp_board;
			return 0;
		}
		if (rule3_column_good != 0)
		{
			board[cord_x][cord_y].value = temp_board;
			return 0;
		}
		return 1;
	}
	else
	{
		return 0;
	}
}

int test_insert_data(field_board **board, int board_size, int cord_x, int cord_y, char sign, int output_offset = 0)
{
	int position_output = output_offset;
	char rule_string[RULE_STRING_SIZE];
	char bufor_position[BUFOR_SIZE];
	int rule1_good = check_rule_1(board, board_size, cord_x, cord_y, sign);
	int rule2_good = row_column_counter(board, board_size, cord_x, cord_y, sign);
	char temp_board = board[cord_x][cord_y].value;
	board[cord_x][cord_y].value = sign;
	int rule3_row_good = identity_of_rows(board, board_size, cord_x);
	int rule3_column_good = identity_of_columns(board, board_size, cord_y);
	if (board[cord_x][cord_y].editable == true && (rule1_good != 0) && (rule2_good == 0))
	{
		if (rule3_row_good != 0)
		{
			board[cord_x][cord_y].value = temp_board;
		}
		if (rule3_column_good != 0)
		{
			board[cord_x][cord_y].value = temp_board;
		}
	}
	else
	{
		board[cord_x][cord_y].value = temp_board;
	}
	if (!rule1_good)
	{
		strcpy(rule_string, &sign);
		strcat(rule_string, " nie moze wystepowac wiecej niz 2 razy obok siebie ");
		gotoxy(CORD_NAME_X, CORD_NAME_Y + SIZE_INF + 2 + position_output);
		cputs(rule_string);
		position_output++;
	}
	if (rule2_good == RETURN_ROW)//wykonuje sie, gdy regula 2 z wierszami nie jest spelniona
	{
		itoa(board_size / 2, bufor_position, 10);
		strcpy(rule_string, &sign);
		strcat(rule_string, " wystepuje ");
		strcat(rule_string, bufor_position);
		strcat(rule_string, " razy w wierszu");
		gotoxy(CORD_NAME_X, CORD_NAME_Y + SIZE_INF + 2 + position_output);
		cputs(rule_string);
		position_output++;
	}
	if (rule2_good == RETURN_COLUMN)//wykonuje sie, gdy regula 2 z kolumnami nie jest spelniona
	{
		itoa(board_size / 2, bufor_position, 10);
		strcpy(rule_string, &sign);
		strcat(rule_string, " wystepuje ");
		strcat(rule_string, bufor_position);
		strcat(rule_string, " razy w kolumnie");
		gotoxy(CORD_NAME_X, CORD_NAME_Y + SIZE_INF + 2 + position_output);
		cputs(rule_string);
		position_output++;
	}
	if (rule3_row_good != 0)//wykonuje sie, gdy wiersz jest duplikowany
	{
		itoa(rule3_row_good, bufor_position, 10);
		strcpy(rule_string, "Wpisanie cyfry ");
		strcat(rule_string, &sign);
		strcat(rule_string, " spowoduje, ze wiersz bedzie identyczny z wierszem numer ");
		strcat(rule_string, bufor_position);
		gotoxy(CORD_NAME_X, CORD_NAME_Y + SIZE_INF + 2 + position_output);
		cputs(rule_string);
		position_output++;
	}
	if (rule3_column_good != 0)//wykonuje sie, gdy kolumna jest duplikowana
	{
		itoa(rule3_column_good, bufor_position, 10);
		strcpy(rule_string, "Wpisanie cyfry ");
		strcat(rule_string, &sign);
		strcat(rule_string, " spowoduje, ze wiersz bedzie identyczny z kolumna numer ");
		strcat(rule_string, bufor_position);
		gotoxy(CORD_NAME_X, CORD_NAME_Y + SIZE_INF + 2 + position_output);
		cputs(rule_string);
		position_output++;
	}
	return position_output;
}

void random_fields_board(field_board **board, int board_size,int count_field=0)
{
	srand((unsigned int)time(NULL));
	int min_value = int(((MIN_VALUE / 100)*double(board_size*board_size)));
	int max_value = int(((MAX_VALUE / 100)*double(board_size*board_size)));
	int random_fields=0;
	if (count_field > 0)
	{
		random_fields = count_field;
	}
	else
	{
		random_fields = rand() % (max_value - min_value) + min_value;
	}
	
	int i = 0;
	int max_trials = board_size*board_size;
	while (i <= random_fields && max_trials > 0)
	{
		int random_x = rand() % board_size + 1;
		int random_y = rand() % board_size + 1;
		int random_value = rand() % board_size + 1;
		if (random_value % 2 == 0)
		{
			if (insert_data(board, board_size, random_x, random_y, '0'))
			{
				board[random_x][random_y].editable = false;
			}
			else
			{
				max_trials--;
			}
		}
		else
		{
			if (insert_data(board, board_size, random_x, random_y, '1'))
			{
				board[random_x][random_y].editable = false;
			}
			else
			{
				max_trials--;
			}
		}
		i++;
	}
}

void simple_hint(field_board **board, int board_size, int cord_x, int cord_y)
{
	if (board != NULL)
	{
		char temp_value = board[cord_x][cord_y].value;
		gotoxy(CORD_NAME_X, CORD_NAME_Y + SIZE_INF + 1);
		if (board[cord_x][cord_y].editable == false)
		{
			cputs("Pole nie jest modyfikowalne");
		}
		else
		{
			bool one_allowed = false;
			bool zero_allowed = false;
			int offset = 0;
			offset = test_insert_data(board, board_size, cord_x, cord_y, '1', offset);
			if (board[cord_x][cord_y].value == '1') one_allowed = true;
			board[cord_x][cord_y].value = temp_value;
			test_insert_data(board, board_size, cord_x, cord_y, '0', offset);
			if (board[cord_x][cord_y].value == '0') zero_allowed = true;
			board[cord_x][cord_y].value = temp_value;
			gotoxy(CORD_NAME_X, CORD_NAME_Y + SIZE_INF + 1);
			if (zero_allowed && !one_allowed)
			{
				cputs("W pole mozesz wpisac wartosc 0");
			}
			else if (!zero_allowed && one_allowed)
			{
				cputs("W pole mozesz wpisac wartosc 1");
			}
			else if (zero_allowed && one_allowed)
			{
				cputs("W pole mozesz wpisac wartosc 0 lub 1");
			}
			else if (!zero_allowed && !one_allowed)
			{
				cputs("Nie mozesz nic wpisac");
			}
		}
	}
}

int update_from_file(field_board **board, const int board_size)
{
	char string[SIGN_BOARD_SIZE];
	char bufor[BUFOR_SIZE];
	itoa(board_size, bufor, 10);
	strcpy(string, "plansza");
	strcat(string, bufor);
	strcat(string, ".txt");

	FILE *board_file;
	board_file = fopen(string, "r");
	char sign[SIGN_BOARD_SIZE][SIGN_BOARD_SIZE];
	if (board_file == NULL)
	{
		gotoxy(CORD_NAME_X, CORD_NAME_Y + SIZE_INF + 1);
		cputs("Otwarcie pliku nie powiodlo sie");
		return FILE_ERROR;
	}
	for (int i = 0; i < board_size; i++)
	{
		for (int j = 0; j < board_size; j++)
		{
			sign[i][j] = fgetc(board_file);
			if (sign[i][j] == '\n')
			{
				j--;
			}
			else
			{
				insert_data(board, board_size, i + 1, j + 1, sign[i][j]);
				if (sign[i][j] != '.')
				{
					board[i + 1][j + 1].editable = false;
				}
			}
		}
	}
	fclose(board_file);
	return 1;
}

void unsolved_field(field_board **board, int board_size)
{
	bool one_allowed = false;
	bool zero_allowed = false;
	for (int i = 1; i <= board_size; i++)
	{
		for (int j = 1; j <= board_size; j++)
		{
			char temp_value = board[i][j].value;
			insert_data(board, board_size, i, j, '1');
			if (board[i][j].value == '1') one_allowed = true;
			board[i][j].value = temp_value;
			insert_data(board, board_size, i, j, '0');
			if (board[i][j].value == '0') zero_allowed = true;
			board[i][j].value = temp_value;
			if (board[i][j].value == '.')
			{
				gotoxy(CORD_BOARD_X + j, CORD_BOARD_Y + i);
				textcolor(RED);
				putch(board[i][j].value);
			}
			if (!one_allowed && !zero_allowed && board[i][j].value == '.') {
				gotoxy(CORD_BOARD_X + j, CORD_BOARD_Y + i);
				textcolor(LIGHTGREEN);
				putch(board[j][i].value);
			}
			textcolor(WHITE);
			one_allowed = false;
			zero_allowed = false;
		}
	}
}

void display_counter_zero(field_board **board, int board_size, char sign)
{
	int i;
	char bufor_position_x[BUFOR_SIZE];
	char bufor_position_y[BUFOR_SIZE];
	int row_sum[BUFOR_POS_SIZE] = { 0 }, column_sum[BUFOR_POS_SIZE] = { 0 };
	for (i = 1; i <= board_size; i++)
	{
		for (int j = 1; j <= board_size; j++)
		{
			if (board[i][j].value == sign) row_sum[i]++;
			if (board[j][i].value == sign) column_sum[i]++;
		}
		itoa(row_sum[i], bufor_position_x, 10);
		itoa(column_sum[i], bufor_position_y, 10);
		gotoxy(CORD_BOARD_X - 3, CORD_BOARD_Y - 1);
		cputs("L.0");
		gotoxy(CORD_BOARD_X - 2, CORD_BOARD_Y + i);
		cputs(bufor_position_x);
		gotoxy(CORD_BOARD_X + i, CORD_BOARD_Y - 1);
		cputs(bufor_position_y);
	}
}

void display_counter_one(field_board **board, int board_size, char sign)
{
	int i;
	char bufor_position_x[BUFOR_SIZE];
	char bufor_position_y[BUFOR_SIZE];
	int row_sum[BUFOR_POS_SIZE] = { 0 }, column_sum[BUFOR_POS_SIZE] = { 0 };
	for (i = 1; i <= board_size; i++)
	{
		for (int j = 1; j <= board_size; j++)
		{
			if (board[i][j].value == sign) row_sum[i]++;
			if (board[j][i].value == sign) column_sum[i]++;
		}
		itoa(row_sum[i], bufor_position_x, 10);
		itoa(column_sum[i], bufor_position_y, 10);
		gotoxy(CORD_BOARD_X + board_size + 2, CORD_BOARD_Y + board_size + 2);
		cputs("L.1");
		gotoxy(CORD_BOARD_X + board_size + 2, CORD_BOARD_Y + i);
		cputs(bufor_position_x);
		gotoxy(CORD_BOARD_X + i, CORD_BOARD_Y + board_size + 2);
		cputs(bufor_position_y);
	}
}

void one_filled_field(field_board **board, int board_size)
{
	bool one_allowed = false;
	bool zero_allowed = false;
	for (int i = 1; i <= board_size; i++)
	{
		for (int j = 1; j <= board_size; j++)
		{
			char temp_value = board[i][j].value;
			insert_data(board, board_size, i, j, '1');
			if (board[i][j].value == '1') one_allowed = true;
			board[i][j].value = temp_value;
			insert_data(board, board_size, i, j, '0');
			if (board[i][j].value == '0') zero_allowed = true;
			board[i][j].value = temp_value;
			if ((one_allowed == true && zero_allowed == false && board[i][j].value == '.') ||
				(one_allowed == false && zero_allowed == true && board[i][j].value == '.')) 
			{
				gotoxy(CORD_BOARD_X + j, CORD_BOARD_Y + i);
				textcolor(YELLOW);
				putch(board[i][j].value);
			}
			if (board[i][j].editable == false)
			{
				gotoxy(CORD_BOARD_X + j, CORD_BOARD_Y + i);
				textcolor(LIGHTRED);
				putch(board[i][j].value);
			}
			textcolor(WHITE);
			one_allowed = false;
			zero_allowed = false;
		}
	}
}

void filled_field(field_board **board, int board_size)
{
	bool one_allowed = false;
	bool zero_allowed = false;
	for (int i = 1; i <= board_size; i++)
	{
		for (int j = 1; j <= board_size; j++)
		{
			char temp_value = board[i][j].value;
			insert_data(board, board_size, i, j, '1');
			if (board[i][j].value == '1') one_allowed = true;
			board[i][j].value = temp_value;
			insert_data(board, board_size, i, j, '0');
			if (board[i][j].value == '0') zero_allowed = true;
			board[i][j].value = temp_value;
			if (one_allowed == true && zero_allowed == false)
			{
				board[i][j].value = '1';
			}
			else if (one_allowed == false && zero_allowed == true)
			{
				board[i][j].value = '0';
			}
			textcolor(WHITE);
			one_allowed = false;
			zero_allowed = false;
		}
	}
}

void display_info(cursor* cursor_pos)
{
	char bufor_x[BUFOR_SIZE], bufor_y[BUFOR_SIZE];
	itoa(cursor_pos->x, bufor_x, 10);
	itoa(cursor_pos->y, bufor_y, 10);
	char tab[BOARD_CURSOR] = { 0 };
	strcpy(tab, "Pozycja kursora : ");
	strcat(tab, bufor_x);
	strcat(tab, " ");
	strcat(tab, bufor_y);
	char information[SIZE_INF][3 * SIZE_INF] = {
		"Dawid Wesolowski",
		"Nr indeksu: 169122",
		"strzalki - przemieszczanie sie kursorem po planszy",
		"esc  wyjscie z programu",
		"01   wpisanie cyfry na plansze",
		"n    nowa gra",
		"o    losowe wypelnianie planszy",
		"p    prosta odpowiedz",
		"r    zmiana rozmiaru planszy",
		"k    proste sprawdzenie mozliwosci ukonczenia gry",
		"d    wyswietlanie ilosc 0 i 1",
		"j    podswietlenie jednoznacznych pol",
		"w    wypelnienie podswietlonych jednoznacznych pol",
		"a    automatyczna detekcja konca gry"
	};
	for (int i = 0, k = CORD_NAME_Y; i < SIZE_INF; k++, i++)
	{
		gotoxy(CORD_NAME_X, k);
		cputs(information[i]);
	}
	gotoxy(CORD_NAME_X, CORD_NAME_Y + SIZE_INF);
	cputs(tab);
}

int handle_input(int board_size, field_board **board, cursor* cursor_pos)
{
	int zn = getch();
	if (zn == 0) //pierwszy znak w przypadku nacisniecia strzalek
	{
		int move_x = 0, move_y = 0;
		zn = getch();
		if (zn == UP_KEY && cursor_pos->y > 1) move_y--; //stzalka w gore
		else if (zn == DOWN_KEY && cursor_pos->y < board_size) move_y++; //stzalka w dol
		else if (zn == LEFT_KEY && cursor_pos->x > 1) move_x--; //stzalka w lewo
		else if (zn == RIGHT_KEY && cursor_pos->x < board_size) move_x++; //stzalka w prawo

		if (move_x != 0 || move_y != 0)
		{
			board[cursor_pos->x][cursor_pos->y].cursor_over = false;
			cursor_pos->x += move_x;
			cursor_pos->y += move_y;
			board[cursor_pos->x][cursor_pos->y].cursor_over = true;
		}
	}
	else if (zn == '1' || zn == '0')
	{
		if (board != NULL)
			insert_data(board, board_size, cursor_pos->y, cursor_pos->x, zn);
	}
	else if (zn == 'n' || zn == 'N')
		return MENU_DEFAULT_BOARD;
	else if (zn == EXIT)
		return MENU_EXIT;
	else if (zn == 'o' || zn == 'O')
		return MENU_RANDOM_BOARD;
	else if (zn == 'p' || zn == 'P')
		return MENU_HINT;
	else if (zn == 'r' || zn == 'R')
		return MENU_RESIZE_BOARD;
	else if (zn == 'k' || zn == 'K')
		return MENU_2HINT;
	else if (zn == 'd' || zn == 'D')
		return MENU_ADDIOTIONAL_HINT;
	else if (zn == 'j' || zn == 'J')
		return MENU_ONE_FILLED;
	else if (zn == 'w' || zn == 'W')
		return MENU_FILL_ONE_FIELD;
	else if (zn == 'a' || zn == 'A')
		return MENU_AUTO_DETECT;
	return 0;
}

void auto_detected(field_board **board, int board_size)
{
	bool one_allowed = false;
	bool zero_allowed = false;
	for (int i = 1; i <= board_size; i++)
	{
		for (int j = 1; j <= board_size; j++)
		{
			char temp_value = board[i][j].value;
			insert_data(board, board_size, i, j, '1');
			if (board[i][j].value == '1') one_allowed = true;
			board[i][j].value = temp_value;
			insert_data(board, board_size, i, j, '0');
			if (board[i][j].value == '0') zero_allowed = true;
			board[i][j].value = temp_value;
			if (one_allowed == false && zero_allowed == false)
			{
				gotoxy(CORD_NAME_X, CORD_NAME_Y + SIZE_INF + 5);
				cputs("Nie mozna ukonczyc gry");
			}
			one_allowed = false;
			zero_allowed = false;
		}
	}
}
