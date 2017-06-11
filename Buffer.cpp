#include "Buffer.h"

#include <cstdlib>
#include <cassert>

#include <ncurses.h>

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
    Lines.insert(Lines.begin() + iPosition.Y + 1, Lines[iPosition.Y].substr(iPosition.X));
    Lines[iPosition.Y].erase(iPosition.X);
  }
}

void buffer::Overwrite(const vector_2d & iPosition, int iLetter)
{
  if (iPosition.X < Lines[iPosition.Y].length())
    Lines[iPosition.Y][iPosition.X] = iLetter;
  else
    Lines[iPosition.Y] += iLetter;
}

void buffer::Remove(const vector_2d & iPosition)
{
  if (iPosition.Y >= Lines.size())
    return;

  if (iPosition.X < Lines[iPosition.Y].length())
    Lines[iPosition.Y].erase(iPosition.X, 1);
  else if (iPosition.Y < Lines.size() - 1)
  {
    Lines[iPosition.Y] += Lines[iPosition.Y + 1];
    Lines.erase(Lines.begin() + iPosition.Y + 1);
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

