#include "Terminal.h"

#include <iostream>

void ShowMenu(void)
{
  std::cout << "e  Exit";
}

int main(int iArgCount, char * iArgs[])
{
  terminal Terminal;
  Terminal.Clear();

  Terminal.SetCursor(1, 1);

  ShowMenu();

  int Key;
  bool Valid;
  do
  {
    Valid = Terminal.GetKey(Key);
  } while (!Valid || Key != 'e');

  return 0;
}
