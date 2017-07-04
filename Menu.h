#ifndef TEXT_EDITOR_MENU
#define TEXT_EDITOR_MENU

#include "Vector_2D.h"

#include <ncurses.h>

class menu
{
  public:
    menu(const vector_2d & iSize) { Window_handle = NULL; Size = iSize; };

    bool Show(void);

  protected:
    WINDOW * Window_handle;
    vector_2d Size;

    void Open_shell(void);
};

#endif

