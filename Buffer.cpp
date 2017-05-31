#include "Buffer.h"

#include <cstdlib>
#include <cassert>

void buffer::Insert(int iLetter)
{
  if (Row < Data.size())
    Data[Row].insert(Column, std::string(1, iLetter));
  else
    Data.push_back(std::string(1, iLetter));

  if (iLetter != '\n')
    Column++;
  else
  {
    Row++;
    Column = 0;
  }
}

void buffer::Move_position(int iRow_delta, int iColumn_delta)
{
  assert(abs(iRow_delta) <= 1 && abs(iColumn_delta) <= 1);

  int New_row = Row + iRow_delta;
  if (New_row >= 0 && New_row <= Data.size())
    Row = New_row;

  int New_column = Column + iColumn_delta;
  if (New_column >= 0 && New_column <= Data[Row].length())
    Column = New_column;
}

std::string buffer::Get_contents(void)
{
  std::string Text;
  for (int Row = 0; Row < Data.size(); Row++)
    Text += Data[Row];
  return Text;
}

