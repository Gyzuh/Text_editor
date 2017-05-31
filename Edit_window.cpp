#include "Edit_window.h"
#include "Menu.h"

#include <string>
#include <fstream>
#include <cassert>

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

  scrollok(Window_handle, true);

  while (true)
  {
    int Key = getch();

    if (Key == ';')
    {
      menu Menu;
      if (Menu.Show())
        break;
    }

    bool Skip = false;
    switch(Key)
    {
      case KEY_UP:
      {
        Move_cursor(-1, 0);
        Buffer.Move_position(-1, 0);
        Skip = true;
        break;
      }

      case KEY_DOWN:
      {
        Move_cursor(1, 0);
        Buffer.Move_position(1, 0);
        Skip = true;
        break;
      }

      case KEY_LEFT:
      {
        Move_cursor(0, -1);
        Buffer.Move_position(0, -1);
        Skip = true;
        break;
      }

      case KEY_RIGHT:
      {
        Move_cursor(0, 1);
        Buffer.Move_position(0, 1);
        Skip = true;
        break;
      }
    }

    if (!Skip)
    {
      wechochar(Window_handle, Key);
      Buffer.Insert(Key);
    }
  };

  del_panel(Panel);
  endwin();

  std::ofstream Stream("Output.txt");
  Stream << Buffer.Get_contents();
}

void edit_window::Move_cursor(int iRow_delta, int iColumn_delta)
{
  assert(Window_handle != NULL);

  int Row, Column;
  getyx(Window_handle, Row, Column);

  if (Row == 0 && iRow_delta < 0 || Column == 0 && iColumn_delta < 0)
    return;

  Row += iRow_delta;
  Column += iColumn_delta;

  wmove(Window_handle, Row, Column);
}

