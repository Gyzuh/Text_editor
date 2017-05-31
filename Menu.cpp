#include "Menu.h"

#include <ncurses.h>
#include <panel.h>

bool menu::Show(void)
{
  WINDOW * Window = newwin(10, 10, 5, 5);
  PANEL * Panel = new_panel(Window);

  wprintw(Window, "MENU");

  update_panels();
  doupdate();

  int Key;
  while (true)
  {
    Key = getch();
    if (Key == ';' || Key == 'q')
      break;
  };

  del_panel(Panel);
  delwin(Window);

  return Key == 'q';
}

