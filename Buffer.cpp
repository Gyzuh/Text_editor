#include "Buffer.h"

#include <cstdlib>
#include <cassert>

void buffer::Insert(int iRow, int iColumn, int iLetter)
{
  if (iLetter != '\n')
  {
    if (iRow < Data.size())
    {
      assert(iColumn <= Data[iRow].length());
      Data[iRow].insert(iColumn, std::string(1, iLetter));
    }
    else
    {
      assert(iColumn == 0);
      Data.push_back(std::string(1, iLetter));
    }
  }
  else
  {
    Data.push_back(Data[Data.size() - 1]);
    for (int Row = Data.size() - 1; Row > iRow + 1; Row--)
      Data[Row] = Data[Row - 1];
    Data[iRow + 1] = Data[iRow].substr(iColumn);
    Data[iRow] = Data[iRow].substr(0, iColumn);
  }
}

std::string buffer::Get_contents(void)
{
  std::string Text(Data[0]);
  if (Data.size() > 0)
    Text = Data[0];
  for (int Row = 1; Row < Data.size(); Row++)
    Text += '\n' + Data[Row];
  return Text;
}

