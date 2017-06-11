#ifndef TEXT_EDITOR_BUFFER
#define TEXT_EDITOR_BUFFER

#include "Vector_2D.h"

#include <vector>
#include <string>

class buffer
{
  public:
    void Insert(const vector_2d & iVector, int iLetter);
    void Overwrite(const vector_2d & iVector, int iLetter);
    void Remove(const vector_2d & iVector);

    int Get_line_length(int iRow) { return iRow < Lines.size() ? Lines[iRow].length() : 0; }
    int Get_line_count(void) { return Lines.size(); }

    std::string Get_line(const vector_2d & iPosition, int iLength)
    {
      if (iPosition.Y < Lines.size() && iPosition.X <= Lines[iPosition.Y].length())
        return Lines[iPosition.Y].substr(iPosition.X, iLength);
      else
        return "";
    }

    std::string Get_lines(void);

  protected:
    std::vector<std::string> Lines;
};

#endif

