#ifndef TEXT_EDITOR_BUFFER
#define TEXT_EDITOR_BUFFER

#include <vector>
#include <string>

class buffer
{
  public:
    buffer(void) { Row = 0; Column = 0; };

    void Insert(int iLetter);
    void Move_position(int iRow_delta, int iColumn_delta);

    std::string Get_contents(void);

  protected:
    std::vector<std::string> Data;
    int Row, Column;
};

#endif

