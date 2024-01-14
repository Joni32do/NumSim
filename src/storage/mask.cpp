#include "mask.h"

Mask::Mask(std::array<int, 2> size) : size_(size)
{
  assert(size[0] > 0 && size[1] > 0);
  data_.resize(size_[0] * size_[1], 0);
}