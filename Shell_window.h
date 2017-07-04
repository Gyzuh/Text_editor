#ifndef TEXT_EDITOR_SHELL_WINDOW
#define TEXT_EDITOR_SHELL_WINDOW

#include "Vector_2D.h"

#include <ncurses.h>

class shell_window
{
  public:
    shell_window(const vector_2d & iSize) { Size = iSize; };

    void Show(void);

  protected:
    vector_2d Size;
};

#endif

