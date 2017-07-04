#include "Menu.h"
#include "Shell_window.h"

#include <ncurses.h>
#include <panel.h>

bool menu::Show(void)
{
  Window_handle = newwin(5, 21, (Size.Y - 5) / 2, (Size.X - 21) / 2);
  PANEL * Panel = new_panel(Window_handle);

  wprintw(Window_handle, ";  Insert semicolon\n"
                         "s  Open shell\n"
                         "q  Quit");

  update_panels();
  doupdate();

  int Key;
  while (true)
  {
    Key = getch();
    if (Key == ';' || Key == 'q')
      break;
    if (Key == 's')
    {
      shell_window Shell(Size);
      Shell.Show();
    }
  };

  del_panel(Panel);
  delwin(Window_handle);

  return Key == 'q';
}

