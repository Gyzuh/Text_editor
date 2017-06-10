#include "Edit_window.h"
#include "Menu.h"

#include <string>
#include <fstream>
#include <cassert>
#include <cstdlib>

#include <ncurses.h>
#include <panel.h>

// Better to create vector class with fields X and Y.
// Better to create vector Cursor_position for cursor position in buffer.
// Better to create vector Window_position for position of window.
// Location of cursor in window can then be computed.  No need for getyx anymore.

void edit_window::Run(void)
{
  initscr();

  cbreak();
  noecho();
  keypad(stdscr, true);

  refresh();

  Window_handle = stdscr;
  PANEL * Panel = new_panel(Window_handle);

  while (true)
  {
    int Key = getch();

    if (Key == ';')
    {
      menu Menu;
      if (Menu.Show())
        break;
    }

    switch(Key)
    {
      case KEY_UP:
        Move_up();
        break;

      case KEY_DOWN:
        Move_down();
        break;

      case KEY_LEFT:
        Move_left();
        break;

      case KEY_RIGHT:
        Move_right();
        break;

      case '\n':
        Process_newline();
        break;

      default:
        Process_character(Key);
        break;
    }

    Show_cursor_location();
  };

  del_panel(Panel);
  endwin();

  std::ofstream Stream("Output.txt");
  Stream << Buffer.Get_contents();
}

void edit_window::Move_up(void)
{
  assert(Window_handle != NULL);

  if (Row == 0)
    return;

  int Window_row, Window_column;
  getyx(Window_handle, Window_row, Window_column);

  Row--;
  Window_row--;

  wmove(Window_handle, Window_row, Window_column);
}

void edit_window::Move_down(void)
{
  assert(Window_handle != NULL);

  if (Row == Buffer.Get_rows())
    return;

  int Window_row, Window_column;
  getyx(Window_handle, Window_row, Window_column);

  int Window_rows, Window_columns;
  getmaxyx(Window_handle, Window_rows, Window_columns);

  Row++;
  Window_row++;

  int Length = Buffer.Get_row_length(Row);
  if (Column > Length)
  {
    Column = Length;
    Window_column = Length;
  }

  if (Window_row >= Window_rows)
  {
    scroll(Window_handle);
    Window_row = Window_rows - 1;
  }

  wmove(Window_handle, Window_row, Window_column);
}

void edit_window::Move_left(void)
{
  assert(Window_handle != NULL);

  if (Row == 0 && Column == 0)
    return;

  int Window_row, Window_column;
  getyx(Window_handle, Window_row, Window_column);

  int Window_rows, Window_columns;
  getmaxyx(Window_handle, Window_rows, Window_columns);

  Column--;
  Window_column--;

  if (Column < 0 && Row > 0)
  {
    int Length = Buffer.Get_row_length(Row);
    Row--;
    Column = Length;
    Window_row--;
    Window_column = Length;
  }

  wmove(Window_handle, Window_row, Window_column);
}

void edit_window::Move_right(void)
{
  assert(Window_handle != NULL);

  int Window_row, Window_column;
  getyx(Window_handle, Window_row, Window_column);

  int Window_rows, Window_columns;
  getmaxyx(Window_handle, Window_rows, Window_columns);

  Column++;
  Window_column++;

  int Length = Buffer.Get_row_length(Row);
  if (Column > Length)
  {
    if (Row < Buffer.Get_rows())
    {
      Row++;
      Column = 0;
      Window_row++;
      Window_column = 0;
    }
    else
    {
      Column = Length;
      Window_column = Length;
    }
  }

  if (Window_row >= Window_rows)
  {
    scroll(Window_handle);
    Window_row = Window_rows - 1;
  }

  wmove(Window_handle, Window_row, Window_column);
}

void edit_window::Process_newline(void)
{
  winsch(Window_handle, '\n');
  Buffer.Insert(Row, Column, '\n');

  int Window_row, Window_column;
  getyx(Window_handle, Window_row, Window_column);

  int Window_rows, Window_columns;
  getmaxyx(Window_handle, Window_rows, Window_columns);

  if (Column - Window_column > 0)
    Rewrite_window(Row - Window_row, 0);

  Column = 0;
  Row++;
  Window_column = 0;

  if (Window_row < Window_rows - 1)
  {
    Window_row++;

    wmove(Window_handle, Window_row, Window_column);

    winsertln(Window_handle);
  }
  else
  {
    wmove(Window_handle, Window_row, Window_column);

    scrollok(Window_handle, true);
    scroll(Window_handle);
    scrollok(Window_handle, false);
  };
}

void edit_window::Process_character(int iKey)
{
  winsch(Window_handle, iKey);
  Buffer.Insert(Row, Column, iKey);

  int Window_row, Window_column;
  getyx(Window_handle, Window_row, Window_column);

  int Window_rows, Window_columns;
  getmaxyx(Window_handle, Window_rows, Window_columns);

  if (Window_column < Window_columns - 1)
  {
    Column++;
    Window_column++;

    wmove(Window_handle, Window_row, Window_column);
  }
  else
  {
    Column++;

    Rewrite_window(Row - Window_row, Column - Window_column);

    wmove(Window_handle, Window_row, Window_column);
  }
}

void edit_window::Rewrite_window(int iRow, int iColumn)
{
  int Window_rows, Window_columns;
  getmaxyx(Window_handle, Window_rows, Window_columns);

  for (int R = 0; R < Window_rows; R++)
  {
    std::string Line = Buffer.Get_row(iRow + R, iColumn, Window_columns);
    Line += std::string(Window_columns - Line.length(), ' ');
    mvwaddstr(Window_handle, R, 0, Line.c_str());
  }
}

void edit_window::Show_cursor_location(void)
{
  int Window_row, Window_column;
  getyx(Window_handle, Window_row, Window_column);

  mvwprintw(Window_handle, 10, 40, "Win: (%i, %i) Buf: (%i, %i)", Window_row, Window_column, Row, Column);

  wmove(Window_handle, Window_row, Window_column);
}

