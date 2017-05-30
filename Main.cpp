#include <string>
#include <fstream>

#include <ncurses.h>
#include <panel.h>

bool Show_menu(void)
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

int main(int, char * *)
{
  initscr();

  cbreak();
  noecho();

  refresh();

  WINDOW * Window = stdscr;
  PANEL * Panel = new_panel(Window);

  std::string Buffer;

  while (true)
  {
    int Key = getch();
    if (Key == ';')
    {
      if (Show_menu())
        break;
    }

    wechochar(Window, Key);
    Buffer += Key;
  };

  del_panel(Panel);
  endwin();

  std::ofstream Stream("Output.txt");
  Stream << Buffer;

  return 0;
}

