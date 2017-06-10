#include "Edit_window.h"
#include "Menu.h"

#include <string>
#include <fstream>
#include <cassert>
#include <cstdlib>

#include <ncurses.h>
#include <panel.h>

void edit_window::Run(void)
{
  initscr();

  cbreak();
  noecho();
  keypad(stdscr, true);

  refresh();

  Window_handle = stdscr;
  PANEL * Panel = new_panel(Window_handle);

  getmaxyx(Window_handle, Window_size.Y, Window_size.X);

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
  Stream << Buffer.Get_lines();
}

void edit_window::Move_up(void)
{
  assert(Window_handle != NULL);
  if (Cursor_position.Y > 0)
  {
    Cursor_position.Y--;
    Cursor_position.X = std::min(Cursor_position.X, Buffer.Get_line_length(Cursor_position.Y));
  }
  Redraw_window();
}

void edit_window::Move_down(void)
{
  assert(Window_handle != NULL);
  if (Cursor_position.Y < Buffer.Get_line_count() - 1)
  {
    Cursor_position.Y++;
    Cursor_position.X = std::min(Cursor_position.X, Buffer.Get_line_length(Cursor_position.Y));
  }
  Redraw_window();
}

void edit_window::Move_left(void)
{
  assert(Window_handle != NULL);
  if (Cursor_position.X > 0)
    Cursor_position.X--;
  else if (Cursor_position.Y > 0)
  {
    Cursor_position.Y--;
    Cursor_position.X = Buffer.Get_line_length(Cursor_position.Y);
  }
  Redraw_window();
}

void edit_window::Move_right(void)
{
  assert(Window_handle != NULL);
  if (Cursor_position.X < Buffer.Get_line_length(Cursor_position.Y))
    Cursor_position.X++;
  else if (Cursor_position.Y < Buffer.Get_line_count() - 1)
  {
    Cursor_position.Y++;
    Cursor_position.X = 0;
  }
  Redraw_window();
}

void edit_window::Process_newline(void)
{
  Buffer.Insert(Cursor_position, '\n');
  Cursor_position.X = 0;
  Cursor_position.Y++;
  Redraw_window();
}

void edit_window::Process_character(int iKey)
{
  Buffer.Insert(Cursor_position, iKey);
  Cursor_position.X++;
  Redraw_window();
}

void edit_window::Redraw_window(void)
{
  if (Cursor_position.X < Window_position.X)
    Window_position.X = Cursor_position.X;
  else if (Cursor_position.X >= Window_position.X + Window_size.X)
    Window_position.X = Cursor_position.X - (Window_size.X - 1);

  if (Cursor_position.Y < Window_position.Y)
    Window_position.Y = Cursor_position.Y;
  else if (Cursor_position.Y >= Window_position.Y + Window_size.Y)
    Window_position.Y = Cursor_position.Y - (Window_size.Y - 1);

  for (int Y = 0; Y < Window_size.Y; Y++)
  {
    std::string Line = Buffer.Get_line(Window_position + vector_2d(0, Y), Window_size.X);
    Line += std::string(Window_size.X - Line.length(), ' ');
    mvwaddstr(Window_handle, Y, 0, Line.c_str());
  }

  wmove(Window_handle, Cursor_position.Y - Window_position.Y, Cursor_position.X - Window_position.X);
}

void edit_window::Show_cursor_location(void)
{
  mvwprintw(Window_handle, 10, 40, "Win: (%i, %i) Cur: (%i, %i)",
    Window_position.X, Window_position.Y, Cursor_position.X, Cursor_position.Y);
  wmove(Window_handle, Cursor_position.Y - Window_position.Y, Cursor_position.X - Window_position.X);
}

