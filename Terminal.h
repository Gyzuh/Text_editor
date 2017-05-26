#ifndef TEXT_EDITOR_TERMINAL_H
#define TEXT_EDITOR_TERMINAL_H

class terminal
{
  public:
    terminal(void);
    ~terminal(void);

    int GetWidth(void);
    int GetHeight(void);

    void Clear();

    void SetCursor(int iColumn, int iRow);

    bool GetKey(int & oKey);

  protected:
    void SetupKeyboard();
    void CloseKeyboard();

    int Keyboard;
};

#endif

