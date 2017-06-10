#ifndef TEXT_EDITOR_EDIT_WINDOW
#define TEXT_EDITOR_EDIT_WINDOW

#include "Buffer.h"
#include "Vector_2D.h"

#include <ncurses.h>

class edit_window
{
  public:
    edit_window() { Window_handle = NULL; }

    void Run(void);

  protected:
    WINDOW * Window_handle;
    buffer Buffer;
    vector_2d Cursor_position;
    vector_2d Window_position;
    vector_2d Window_size;

    void Move_up(void);
    void Move_down(void);
    void Move_left(void);
    void Move_right(void);

    void Process_newline(void);
    void Process_character(int iKey);

    void Redraw_window(void);

    void Show_cursor_location(void);
};

#endif

