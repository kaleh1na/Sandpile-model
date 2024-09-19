#include "sandpile.h"

void Sandpile::Output() {
  for (int i = 0; i < height; ++i) {
    for (int j = 0; j < width; ++j) {
      std::cout << field[i][j] << " ";
    }
    std::cout << "\n";
  }
}

void Sandpile::Add(const uint16_t x, const uint16_t y, const uint64_t num) {
  field[y][x] = num;
}

bool Sandpile::IsUnstable(uint64_t num) { return num > 3; }

bool Sandpile::IsStable() {
  for (int i = 0; i < height; ++i) {
    for (int j = 0; j < width; ++j) {
      if (IsUnstable(field[i][j])) return false;
    }
  }

  return true;
}

void Sandpile::ExpandRight() {
  new_field = new uint64_t *[height];
  for (int i = 0; i < height; ++i) {
    new_field[i] = new uint64_t[width + 1];
  }
  for (int i = 0; i < height; ++i) {
    for (int j = 0; j < width; ++j) {
      new_field[i][j] = field[i][j];
    }
  }
  for (int i = 0; i < height; ++i) {
    new_field[i][width] = 0;
  }
  for (int i = 0; i < height; ++i) {
    delete[] field[i];
  }
  delete[] field;
  ++width;
  field = new_field;
}

void Sandpile::ExpandLeft() {
  new_field = new uint64_t *[height];
  for (int i = 0; i < height; ++i) {
    new_field[i] = new uint64_t[width + 1];
  }
  for (int i = 0; i < height; ++i) {
    for (int j = 0; j < width; ++j) {
      new_field[i][j + 1] = field[i][j];
    }
  }
  for (int i = 0; i < height; ++i) {
    new_field[i][0] = 0;
  }
  for (int i = 0; i < height; ++i) {
    delete[] field[i];
  }
  delete[] field;
  ++width;
  field = new_field;
}

void Sandpile::ExpandDown() {
  new_field = new uint64_t *[height + 1];
  for (int i = 0; i < height + 1; ++i) {
    new_field[i] = new uint64_t[width];
  }
  for (int i = 0; i < height; ++i) {
    for (int j = 0; j < width; ++j) {
      new_field[i][j] = field[i][j];
    }
  }
  for (int i = 0; i < width; ++i) {
    new_field[height][i] = 0;
  }
  for (int i = 0; i < height; ++i) {
    delete[] field[i];
  }
  delete[] field;
  ++height;
  field = new_field;
}

void Sandpile::ExpandUp() {
  new_field = new uint64_t *[height + 1];
  for (int i = 0; i < height + 1; ++i) {
    new_field[i] = new uint64_t[width];
  }
  for (int i = 0; i < height; ++i) {
    for (int j = 0; j < width; ++j) {
      new_field[i + 1][j] = field[i][j];
    }
  }
  for (int i = 0; i < width; ++i) {
    new_field[0][i] = 0;
  }
  for (int i = 0; i < height; ++i) {
    delete[] field[i];
  }
  delete[] field;
  ++height;
  field = new_field;
}

bool Sandpile::NeedExpansionRight() {
  for (int i = 0; i < height; ++i) {
    if (IsUnstable(field[i][width - 1])) return true;
  }

  return false;
}

bool Sandpile::NeedExpansionLeft() {
  for (int i = 0; i < height; ++i) {
    if (IsUnstable(field[i][0])) return true;
  }

  return false;
}

bool Sandpile::NeedExpansionDown() {
  for (int i = 0; i < width; ++i) {
    if (IsUnstable(field[height - 1][i])) return true;
  }

  return false;
}

bool Sandpile::NeedExpansionUp() {
  for (int i = 0; i < width; ++i) {
    if (IsUnstable(field[0][i])) return true;
  }

  return false;
}

void Sandpile::Iteration() {
  if (NeedExpansionDown()) ExpandDown();
  if (NeedExpansionLeft()) ExpandLeft();
  if (NeedExpansionUp()) ExpandUp();
  if (NeedExpansionRight()) ExpandRight();

  new_field = new uint64_t *[height];
  for (int i = 0; i < height; ++i) {
    new_field[i] = new uint64_t[width];
  }

  for (int i = 0; i < height; ++i) {
    for (int j = 0; j < width; ++j) {
      new_field[i][j] = field[i][j];
    }
  }

  for (int i = 1; i < height - 1; ++i) {
    for (int j = 1; j < width - 1; ++j) {
      if (IsUnstable(field[i][j])) {
        new_field[i][j] -= 4;
        new_field[i - 1][j]++;
        new_field[i + 1][j]++;
        new_field[i][j - 1]++;
        new_field[i][j + 1]++;
      }
    }
  }

  for (int i = 0; i < height; ++i) {
    delete[] field[i];
  }
  delete[] field;
  field = new_field;
}

void Bitmap::FillHeaderBMP(const Sandpile &sandpile) {
  // bfType
  header_bmp[0] = 66;
  header_bmp[1] = 77;

  // bfSize
  int bfSize = kHeaderBmpSize + kHeaderRastSize + kPalleteSize + size;
  header_bmp[2] = bfSize % kBase;
  header_bmp[3] = (bfSize / kBase) % kBase;
  header_bmp[4] = ((bfSize / kBase) / kBase) % kBase;
  header_bmp[5] = (((bfSize / kBase) / kBase) / kBase) % kBase;

  // bfReserved1
  header_bmp[6] = 0;
  header_bmp[7] = 0;

  // bfReserved2
  header_bmp[8] = 0;
  header_bmp[9] = 0;

  // bfOffBits
  header_bmp[10] = 74;
  header_bmp[11] = 0;
  header_bmp[12] = 0;
  header_bmp[13] = 0;
}

void Bitmap::FillHeaderRast(const Sandpile &sandpile) {
  // biSize
  header_rast[0] = 40;
  header_rast[1] = 0;
  header_rast[2] = 0;
  header_rast[3] = 0;

  // biWidth
  header_rast[4] = width % kBase;
  header_rast[5] = (width / kBase) % kBase;
  header_rast[6] = ((width / kBase) / kBase) % kBase;
  header_rast[7] = (((width / kBase) / kBase) / kBase) % kBase;

  // biHeight
  header_rast[8] = height % kBase;
  header_rast[9] = (height / kBase) % kBase;
  header_rast[10] = ((height / kBase) / kBase) % kBase;
  header_rast[11] = (((height / kBase) / kBase) / kBase) % kBase;

  // biPlanes
  header_rast[12] = 1;
  header_rast[13] = 0;

  // biBitCount
  header_rast[14] = 4;
  header_rast[15] = 0;

  // biCompression
  header_rast[16] = 0;
  header_rast[17] = 0;
  header_rast[18] = 0;
  header_rast[19] = 0;

  // biSizeImage
  header_rast[20] = 0;
  header_rast[21] = 0;
  header_rast[22] = 0;
  header_rast[23] = 0;

  // biXPelsPerMeter
  header_rast[24] = 0;
  header_rast[25] = 0;
  header_rast[26] = 0;
  header_rast[27] = 0;

  // biYPelsPerMeter
  header_rast[28] = 0;
  header_rast[29] = 0;
  header_rast[30] = 0;
  header_rast[31] = 5;

  // biCirUsed
  header_rast[32] = 0;
  header_rast[33] = 0;
  header_rast[34] = 0;
  header_rast[35] = 0;

  // biCirImportant
  header_rast[36] = 0;
  header_rast[37] = 0;
  header_rast[38] = 0;
  header_rast[39] = 0;
}

void Bitmap::FillPalette() {
  // white
  pallete[0] = 255;
  pallete[1] = 255;
  pallete[2] = 255;
  pallete[3] = 0;

  // green
  pallete[4] = 74;
  pallete[5] = 148;
  pallete[6] = 68;
  pallete[7] = 0;

  // yellow
  pallete[8] = 0;
  pallete[9] = 169;
  pallete[10] = 244;
  pallete[11] = 0;

  // purple
  pallete[12] = 77;
  pallete[13] = 16;
  pallete[14] = 48;
  pallete[15] = 0;

  // black
  pallete[16] = 0;
  pallete[17] = 0;
  pallete[18] = 0;
  pallete[19] = 0;
}

void Bitmap::FillTable(const Sandpile &sandpile) {
  int ind = 0;
  table = new uint8_t[size];

  for (int i = height - 1; i >= 0; --i) {
    for (int j = 0; j < width - 1; j += 2) {
      uint8_t a = sandpile.field[i][j];
      uint8_t b = sandpile.field[i][j + 1];
      if (a > 4) a = 4;
      if (b > 4) b = 4;
      table[ind] = a * 16 + b;
      ++ind;
    }

    if (width % 2 == 1) {
      uint8_t a = sandpile.field[i][width - 1];
      if (a > 4) a = 4;
      table[ind] = a * 16;
      ++ind;
    }

    for (int j = ind; j < bytes * (height - i); ++j) {
      table[ind] = 0;
      ++ind;
    }
  }
}

void Bitmap::WriteFile(const char *directory, int ind) {
  std::filesystem::path dir(directory);
  dir += "/";
  int count = 0;
  int ind_copy = ind;

  while (ind_copy > 0) {
    ind_copy /= 10;
    ++count;
  }

  int num[count];
  for (int i = count - 1; i >= 0; --i) {
    num[i] = ind % 10;
    ind /= 10;
  }

  for (int i = 0; i < count; ++i) {
    dir += num[i] + '0';
  }

  dir += ".bmp";

  std::ofstream fout;
  fout.open(dir, std::ios::binary);
  for (int i = 0; i < 14; ++i) {
    fout << header_bmp[i];
  }
  for (int i = 0; i < 40; ++i) {
    fout << header_rast[i];
  }
  for (int i = 0; i < 20; ++i) {
    fout << pallete[i];
  }
  for (int i = 0; i < size; ++i) {
    fout << table[i];
  }
  fout.close();
}

void Collapse(const Arguments &args) {
  std::ifstream fin;
  fin.open(args.input_file);
  int16_t x;
  int16_t y;
  uint64_t num;

  int16_t max_x = Constants::kINT16TMin;
  int16_t min_x = Constants::kINT16TMax;
  int16_t max_y = Constants::kINT16TMin;
  int16_t min_y = Constants::kINT16TMax;

  while (!fin.eof()) {
    fin >> x >> y >> num;
    if (x > max_x) max_x = x;
    if (x < min_x) min_x = x;
    if (y > max_y) max_y = y;
    if (y < min_y) min_y = y;
  }

  uint16_t width = max_x - min_x + 1;
  uint16_t length = max_y - min_y + 1;
  Sandpile sandpile(width, length);

  fin.clear();
  fin.seekg(0, std::fstream::beg);

  uint16_t n;
  uint16_t m;

  while (!fin.eof()) {
    fin >> x >> y >> num;
    n = x - min_x;
    m = y - min_y;
    sandpile.Add(n, m, num);
  }

  if (args.freq == 0) {
    for (uint64_t i = 1; i <= args.max_iter; ++i) {
      if (sandpile.IsStable()) break;
      sandpile.Iteration();
    }
    Bitmap bmp(sandpile);
    bmp.WriteFile(args.output_directory, 1);
  } else {
    for (uint64_t i = 1; i <= args.max_iter; ++i) {
      if (sandpile.IsStable()) break;
      sandpile.Iteration();
      if (i % args.freq == 0) {
        Bitmap bmp(sandpile);
        bmp.WriteFile(args.output_directory, i / args.freq);
      }
    }
  }
}
