#include "mask.h"

Mask::Mask( Settings settings) : settings_(settings)
{
  if (settings_.createDomainfromBitmap)
    createMaskFromPNGBitMap();
  else
  {
    assert(settings_.nCells[0] > 0 && settings_.nCells[1] > 0);

    size_ = {settings_.nCells[0] + 2, settings_.nCells[1] + 2};
    data_.resize(size_[0] * size_[1], FLUID);
  }

  if (settings_.createRectangularObject)
    makeRectangularObstacle();
  
  setFluidBC();
  setDomainBC();
  setObstacleBC();
}

int &Mask::operator()(int i, int j) {
  assert(i >= 0 && i < size_[0] && j >= 0 && j < size_[1]);
  return data_[i + j * size_[0]];
}

int Mask::operator()(int i, int j) const {
  assert(i >= 0 && i < size_[0] && j >= 0 && j < size_[1]);
  return data_[i + j * size_[0]];
}

std::array<int, 2> Mask::size() const {
  return size_;
}

void Mask::resetMask() {
  for (int i = 0; i < size_[0]; i++) {
    for (int j = 0; j < size_[1]; j++) {
      if (!isObstacle(i, j)) {
        data_[i + j * size_[0]] = AIR;
      }
    }
  }
}

// Primitive type
bool Mask::isFluid(int i, int j) const {
        return (data_[i + j * size_[0]] < FLUID_TYPE);
    }

bool Mask::isObstacle(int i, int j) const {
        return (data_[i + j * size_[0]] >= OBSTACLE);
    }

bool Mask::isAir(int i, int j) const {
        return (data_[i + j * size_[0]] == AIR);
    }

// Boundary type
bool Mask::isDomainBoundary(int i, int j) const{
  return (*this)(i,j)>Mask::DOMAIN_BOUNDARY;
}

bool Mask::isObstacleBoundary(int i, int j) const{
  return ((*this)(i,j)>Mask::OBSTACLE_INSIDE && 
          (*this)(i,j)<Mask::DOMAIN_BOUNDARY);
}

bool Mask::isFluidBoundary(int i, int j) const{
  return (*this)(i,j)<Mask::FLUID;
}

bool Mask::isInnerFluid(int i, int j) const{
  return (*this)(i,j)==Mask::FLUID;
}

int Mask::getNumberOfFluidCells() const {
  int numFluidCells = 0;
  for (int i = 0; i < size_[0]; i++) {
    for (int j = 0; j < size_[1]; j++) {
      if (isFluid(i, j)) {
        numFluidCells++;
      }
    }
  }
  return numFluidCells;
}


void Mask::makeRectangularObstacle()
{

  double scaleX = settings_.obstaclePosition[0] / settings_.physicalSize[0];
  double scaleY = settings_.obstaclePosition[1] / settings_.physicalSize[1];
  int nCellsX = size_[0] - 2;
  int nCellsY = size_[1] - 2;

  int i_beg = static_cast<int>(std::floor(scaleX * nCellsX)) + 1;
  int i_end = static_cast<int>(std::ceil((settings_.obstaclePosition[0] + settings_.obstacleDimension[0]) / settings_.physicalSize[0] * nCellsX)) + 1;
  int j_beg = static_cast<int>(std::floor(scaleY * nCellsY)) + 1;
  int j_end = static_cast<int>(std::ceil((settings_.obstaclePosition[1] + settings_.obstacleDimension[1]) / settings_.physicalSize[1] * nCellsY)) + 1;

  std::cout << i_beg << " from obstacle Position" << settings_.obstaclePosition[0] << " and size " << size_[0] - 2 << std::endl;
  std::cout << i_end << " from obstacle Size " << settings_.obstacleDimension[0] << " and size " << size_[0] - 2 << std::endl;

  for (int i = i_beg; i < i_end; i++)
  {
    for (int j = j_beg; j < j_end; j++)
    {
      data_[i + j * size_[0]] = OBSTACLE;
    }
  }
};

void Mask::createMaskFromPNGBitMap()
{
  std::cout << "Creating Domain from PNGBitMap\n";
  const char *file_path = settings_.pathToBitmap.c_str();
  FILE *fp = fopen(file_path, "rb");
  if (!fp)
    abort();

  png_structp png = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
  if (!png)
    abort();

  png_infop info = png_create_info_struct(png);
  if (!info)
    abort();

  if (setjmp(png_jmpbuf(png)))
    abort();

  png_init_io(png, fp);

  png_read_info(png, info);

  int width = png_get_image_width(png, info);
  int height = png_get_image_height(png, info);
  png_byte color_type = png_get_color_type(png, info);
  png_byte bit_depth = png_get_bit_depth(png, info);

  std::cout << "Bitmap width: " << width << ", Bitmap height: " << height << std::endl;

  size_ = {width + 2, height + 2};
  data_.resize(size_[0] * size_[1], FLUID);

  if (bit_depth == 16)
    png_set_strip_16(png);
  if (color_type == PNG_COLOR_TYPE_PALETTE)
    png_set_palette_to_rgb(png);
  if (color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8)
    png_set_expand_gray_1_2_4_to_8(png);
  if (png_get_valid(png, info, PNG_INFO_tRNS))
    png_set_tRNS_to_alpha(png);

  if (color_type == PNG_COLOR_TYPE_RGB || color_type == PNG_COLOR_TYPE_GRAY ||
      color_type == PNG_COLOR_TYPE_PALETTE)
    png_set_filler(png, 0xFF, PNG_FILLER_AFTER);

  if (color_type == PNG_COLOR_TYPE_GRAY || color_type == PNG_COLOR_TYPE_GRAY_ALPHA)
    png_set_gray_to_rgb(png);

  png_read_update_info(png, info);

  png_bytep *row_pointers = new png_bytep[height];
  for (int y = 0; y < height; y++)
    row_pointers[y] = new png_byte[png_get_rowbytes(png, info)];

  png_read_image(png, row_pointers);

  fclose(fp);

  // Iterate over each pixel
  for (int y = 0; y < height; y++)
  {
    for (int x = 0; x < width; x++)
    {
      png_byte *pixel = &row_pointers[y][x * 4]; // Assuming RGBA format

      int red = pixel[0];
      int green = pixel[1];
      int blue = pixel[2];

      // Black pixel
      if (red == 0 && green == 0 && blue == 0)
        (*this)(x+1,height-y) = OBSTACLE;
      //White pixel
      else if (red == 255 && green == 255 && blue == 255)
        (*this)(x+1,height-y) = AIR;
      // Blue pixel
      else
        (*this)(x+1,height-y) = FLUID;
    }
  }

  // Clean up
  for (int y = 0; y < height; y++)
    delete[] row_pointers[y];
  delete[] row_pointers;
}


void Mask::setFluidBC()
{
  for (int i = 0; i < size_[0]; i++) {
    for (int j = 0; j < size_[1]; j++) {
      int idx = i + j * size_[0];

      if (Mask::isFluid(i, j))
      {
        data_[idx] =  1 * !Mask::isAir(i - 1, j)
                    + 2 * !Mask::isAir(i, j + 1)
                    + 4 * !Mask::isAir(i + 1, j)
                    + 8 * !Mask::isAir(i, j - 1);
      }
    }
  }
}

void Mask::setDomainBC()
{
  // Set domain boundaries

  for (int i = 0; i < size_[0]; i++)
  {
    // Bottom Domain Boundary
    if (data_[i + size_[0]] > OBSTACLE)
      data_[i] = OBSTACLE_INSIDE;
    else if (settings_.BCBottom == "NoSlip")
      data_[i] = DOMAIN_BOTTOM_NOSLIP;
    else if (settings_.BCBottom == "Pressure")
      data_[i] = DOMAIN_BOTTOM_PRESSURE;

    // Top Domain Boundary
    if (data_[i + (size_[1] - 2) * size_[0]] > OBSTACLE)
      data_[i + (size_[1] - 1) * size_[0]] = OBSTACLE_INSIDE;
    else if (settings_.BCTop == "NoSlip")
      data_[i + (size_[1] - 1) * size_[0]] = DOMAIN_TOP_NOSLIP;
    else if (settings_.BCTop == "Pressure")
      data_[i + (size_[1] - 1) * size_[0]] = DOMAIN_TOP_PRESSURE;
  }

  for (int j = 0; j < size_[1]; j++)
  {
    // Left Domain Boundary
    if (data_[j * size_[0] + 1] > OBSTACLE)
      data_[j * size_[0]] = OBSTACLE_INSIDE;
    else if (settings_.BCLeft == "NoSlip")
      data_[j * size_[0]] = DOMAIN_LEFT_NOSLIP;
    else if (settings_.BCLeft == "Pressure")
      data_[j * size_[0]] = DOMAIN_LEFT_PRESSURE;

    // Right Domain Boundary
    if (data_[(size_[0] - 2) + j * size_[0]] > OBSTACLE)
      data_[(size_[0] - 1) + j * size_[0]] = OBSTACLE_INSIDE;
    else if (settings_.BCRight == "NoSlip")
      data_[(size_[0] - 1) + j * size_[0]] = DOMAIN_RIGHT_NOSLIP;
    else if (settings_.BCRight == "Pressure")
      data_[(size_[0] - 1) + j * size_[0]] = DOMAIN_RIGHT_PRESSURE;
  }

  // corners of domain
  data_[0] = OBSTACLE_INSIDE;
  data_[size_[0] - 1] = OBSTACLE_INSIDE;
  data_[0 + size_[0] * (size_[1] - 1)] = OBSTACLE_INSIDE;
  data_[size_[0] - 1 + size_[0] * (size_[1] - 1)] = OBSTACLE_INSIDE;
}

void Mask::setObstacleBC()
{
  for (int i = 0; i < size_[0]; i++)
  {
    for (int j = 0; j < size_[1]; j++)
    {
      if ((*this)(i, j) == OBSTACLE)
      {
        int BC = 1 * Mask::isFluid(i - 1, j) 
               + 2 * Mask::isFluid(i, j + 1) 
               + 4 * Mask::isFluid(i + 1, j) 
               + 8 * Mask::isFluid(i, j - 1) 
               + 100;
        
        bool BCinForbiddenCombinations = std::find(std::begin(forbiddenObstacleFluidCombinations), std::end(forbiddenObstacleFluidCombinations), BC) != std::end(forbiddenObstacleFluidCombinations);

        // Check that obstacle is not only one cell wide
        if (Mask::isAir(i - 1, j) && Mask::isAir(i + 1, j))
          throw std::runtime_error("Obstacel is only one cell wide at i: " + std::to_string(i) + " ,j: " + std::to_string(j));
        else if (Mask::isAir(i, j + 1) && Mask::isAir(i, j - 1))
          throw std::runtime_error("Obstacel is only one cell wide at i: " + std::to_string(i) + " ,j: " + std::to_string(j));
        else if (BCinForbiddenCombinations)
          throw std::runtime_error("Obstacel is only one cell wide at i: " + std::to_string(i) + " ,j: " + std::to_string(j));
        else
          (*this)(i, j) = BC;
      }
    }
  }
}


void Mask::printMask() const{
    for (int j = size_[1] - 1; j >= 0; j--){
        for (int i = 0; i < size_[0]; i++){
          if (data_[i + j * size_[0]] == AIR){
            std::cout << "    ";
          } else {
            std::cout << std::setw(3) << std::setfill('0') << data_[i + j * size_[0]] << " ";
          }
        }
        std::cout << std::endl;
    }
}


