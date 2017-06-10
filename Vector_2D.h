#ifndef TEXT_EDITOR_VECTOR_2D
#define TEXT_EDITOR_VECTOR_2D

class vector_2d
{
  public:
    int X, Y;

    vector_2d(void) { X = 0; Y = 0; }
    vector_2d(int iX, int iY) { X = iX; Y = iY; }

    const vector_2d operator +(const vector_2d & iVector) const
    {
      return vector_2d(X + iVector.X, Y + iVector.Y);
    };
};

#endif

