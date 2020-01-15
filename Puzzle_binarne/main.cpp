#include "conio2.h"
#include "functions.h"
#include <cstdlib>

int main()
{
	cursor cursor_pos;
	cursor_pos.x = 1;
	cursor_pos.y = 1;
	int exit = 0;
	int input = 0;
	int board_size = 0;
	bool board_init = false;
	bool display_hint = false;
	bool display_hint2 = false;
	bool additional_hint = false;
	bool not_k = false;
	bool one_filled = false;
	bool fill_one = false;
	bool a_detect = false;
	bool issset_one_filled = false;
	bool field_rand = false;
	int field = 0;
	char temp_board[DEFAULT_TABLE_SIZE][DEFAULT_TABLE_SIZE + 1] = {
		"...1.......1",
		".0......1...",
		".....0......",
		".0.1.0.0.0.0",
		"1.1....0..1.",
		".....0..1...",
		"..11....1...",
		"0.....0....0",
		".1..1.....1.",
		"..0..0.0....",
		"..0..00....0",
		"...1..0..1.."
	};
	_setcursortype(_NOCURSOR);
	settitle("Dawid Wesolowski 169122");
	textcolor(WHITE);
	field_board **board = NULL;
	do
	{
		display_info(&cursor_pos);
		input = handle_input(board_size, board, &cursor_pos);
		switch (input)//inicjalizacja planszy/menu
		{
		case MENU_EXIT:
			exit = 1;
			break;
		case MENU_DEFAULT_BOARD://domyslna plansza
			if (not_k == false)
			{
				if (board_init == true)
					remove_board(board, board_size);
				board_init = true;
				board_size = DEFAULT_TABLE_SIZE;
				board = create_board(DEFAULT_TABLE_SIZE, &cursor_pos);
				swap_default_board(temp_board, board);
			}
			break;
		case MENU_RANDOM_BOARD://losowo wypelniana plansza
			if (board_size != 0)
			{
				if (not_k == false)
				{
					if (board_init == true)
						remove_board(board, board_size);
					board_init = true;
					board = create_board(board_size, &cursor_pos);
					random_fields_board(board, board_size,field);
				}
			}
			break;
		case MENU_HINT://prosta podpowiedz
			display_hint = true;
			break;
		case MENU_RESIZE_BOARD://zmiana rozmiaru planszy
			if (not_k == false)
			{
				if (board_init == true)
					remove_board(board, board_size);
				board_init = true;
				gotoxy(CORD_NAME_X, CORD_NAME_Y + SIZE_INF + 1);
				cputs("Podaj nowy rozmiar planszy z przedzialu 2-24 (liczby parzyste): ");
				board_size = insert_board_size();
				if (board_size >= MIN_BOARD_SIZE && board_size <= MAX_BOARD_SIZE)
				{
					if (board_size % 2 == 0)
					{
						board = create_board(board_size, &cursor_pos);
					}
					else
					{
						board_size -= 1;
						board = create_board(board_size, &cursor_pos);
					}
					if (!update_from_file(board, board_size))//gdy niepowiedzie sie otwarcie pliku
					{
						field_rand = true;
						gotoxy(CORD_NAME_X, CORD_NAME_Y + SIZE_INF + 2);
						cputs("Ile pol mam wypelnic?");
						field = insert_board_size();
					}
					else
					{
						update_from_file(board, board_size);
					}
				}
				if (field_rand == true)
				{
					random_fields_board(board, board_size,field);
					field_rand = false;
				}
			}
			break;
		case MENU_2HINT://druga podpowiedz
			display_hint2 = true;
			not_k = true;
			break;
		case MENU_ADDIOTIONAL_HINT://wyswietlenie ilosci zer i jedynek w wierszu
			additional_hint = true;
			break;
		case MENU_ONE_FILLED://wyswietlenie jednoznacznych pol
			one_filled = true;
			issset_one_filled = true;
			break;
		case MENU_FILL_ONE_FIELD://wpisanie wartosci w jednoznaczne pola
			fill_one = true;
			break;
		case MENU_AUTO_DETECT:
			if (a_detect == true)
				a_detect = false;
			else
				a_detect = true;
			break;
		default:
			if (board_init == false)//wykona sie, gdy nie jest zainicjalizowana plansza, a chcemy uzyc cos, co nie jest w menu
			{
				continue;
			}
		};
		clrscr();
		show_board(board, board_size, CORD_BOARD_X, CORD_BOARD_Y);
		if (display_hint)
		{
			simple_hint(board, board_size, cursor_pos.y, cursor_pos.x);
			display_hint = false;
		}
		if (display_hint2)
		{
			unsolved_field(board, board_size);
			display_hint2 = false;
		}
		if (additional_hint)
		{
			display_counter_zero(board, board_size, '0');
			display_counter_one(board, board_size, '1');
			additional_hint = false;
		}
		if (one_filled == true)
		{
			one_filled_field(board, board_size);
			one_filled = false;
		}
		if (fill_one == true && issset_one_filled==true)
		{
			filled_field(board, board_size);
			show_board(board, board_size, CORD_BOARD_X, CORD_BOARD_Y);
			fill_one = false;
		}
		if (a_detect == true)
		{
			gotoxy(CORD_NAME_X, CORD_NAME_Y + SIZE_INF + 6);
			cputs("Tryb automatyczny jest wlaczony");
			auto_detected(board, board_size);		
		}
	} while (exit != 1);
	if (board!=NULL)
		remove_board(board, board_size);

	return 0;
}

