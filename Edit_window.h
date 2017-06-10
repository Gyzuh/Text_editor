#ifndef TEXT_EDITOR_EDIT_WINDOW
#define TEXT_EDITOR_EDIT_WINDOW

#include "Buffer.h"

#include <ncurses.h>

class edit_window
{
  public:
    edit_window() { Row = 0; Column = 0; }

    void Run(void);

  protected:
    WINDOW * Window_handle;
    buffer Buffer;
    int Row, Column;

    void Move_up(void);
    void Move_down(void);
    void Move_left(void);
    void Move_right(void);

    void Process_newline(void);
    void Process_character(int iKey);

    void Rewrite_window(int iRow, int iColumn);

    void Show_cursor_location(void);
};

#endif

