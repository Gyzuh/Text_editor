#ifndef TEXT_EDITOR_BUFFER
#define TEXT_EDITOR_BUFFER

#include <vector>
#include <string>

class buffer
{
  public:
    void Insert(int iRow, int iColumn, int iLetter);

    int Get_row_length(int iRow) { return iRow < Data.size() ? Data[iRow].length() : 0; }
    int Get_rows(void) { return Data.size(); }

    std::string Get_row(int iRow, int iStart, int iLength) { return iRow < Data.size() && iStart <= Data[iRow].length() ? Data[iRow].substr(iStart, iLength) : ""; }

    std::string Get_contents(void);

  protected:
    std::vector<std::string> Data;
};

#endif

