#ifndef TEXT_EDITOR_EDIT_WINDOW
#define TEXT_EDITOR_EDIT_WINDOW

#include "Buffer.h"
#include "Vector_2D.h"

#include <ncurses.h>

class edit_window
{
  public:
    edit_window() { Window_handle = NULL; Overwrite = false; }

    void Run(void);

  protected:
    static const int Tab_size = 8;

    WINDOW * Window_handle;
    buffer Buffer;
    vector_2d Cursor_position;
    vector_2d Window_position;
    vector_2d Window_size;
    bool Overwrite;

    void Move_up(void);
    void Move_down(void);
    void Move_left(void);
    void Move_right(void);

    void Process_newline(void);
    void Process_backspace(void);
    void Process_delete(void);
    void Process_home(void);
    void Process_end(void);
    void Process_page_up(void);
    void Process_page_down(void);
    void Process_insert(void);
    void Process_tab(void);
    void Process_character(int iKey);

    void Redraw_window(void);

    void Show_cursor_location(void);
};

#endif

