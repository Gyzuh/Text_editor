#ifndef TEXT_EDITOR_EDIT_WINDOW
#define TEXT_EDITOR_EDIT_WINDOW

#include "Buffer.h"

#include <ncurses.h>

class edit_window
{
  public:
    void Run(void);

  protected:
    WINDOW * Window_handle;
    buffer Buffer;

    void Move_cursor(int iRow_delta, int iColumn_delta);
};

#endif

