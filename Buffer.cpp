#include "Buffer.h"

#include <cstdlib>
#include <cassert>

void buffer::Insert(const vector_2d & iPosition, int iLetter)
{
  if (iLetter != '\n')
  {
    if (iPosition.Y < Lines.size())
    {
      assert(iPosition.X <= Lines[iPosition.Y].length());
      Lines[iPosition.Y].insert(iPosition.X, std::string(1, iLetter));
    }
    else
    {
      assert(iPosition.X == 0);
      Lines.push_back(std::string(1, iLetter));
    }
  }
  else
  {
    Lines.push_back(Lines[Lines.size() - 1]);
    for (int Row = Lines.size() - 2; Row > iPosition.Y + 1; Row--)
      Lines[Row] = Lines[Row - 1];
    Lines[iPosition.Y + 1] = Lines[iPosition.Y].substr(iPosition.X);
    Lines[iPosition.Y] = Lines[iPosition.Y].substr(0, iPosition.X);
  }
}

std::string buffer::Get_lines(void)
{
  std::string Text(Lines[0]);
  if (Lines.size() > 0)
    Text = Lines[0];
  for (int Line = 1; Line < Lines.size(); Line++)
    Text += '\n' + Lines[Line];
  return Text;
}

