#pragma once

#include "parse.h"

struct Constants {
  static const uint16_t kINT16TMax = std::numeric_limits<int16_t>::max();
  static const uint16_t kINT16TMin = std::numeric_limits<int16_t>::min();
};

struct Sandpile {
  size_t width;
  size_t height;
  uint64_t **field;
  uint64_t **new_field;

  Sandpile(const uint16_t x, const uint16_t y) {
    width = x;
    height = y;
    field = new uint64_t *[height];
    for (int i = 0; i < height; ++i) {
      field[i] = new uint64_t[width];
    }
    for (int i = 0; i < height; ++i) {
      for (int j = 0; j < width; ++j) {
        field[i][j] = 0;
      }
    }
  }

  ~Sandpile() {
    for (int i = 0; i < height; ++i) {
      delete[] field[i];
    }
    delete[] field;
  }

  void Output();

  void Add(const uint16_t x, const uint16_t y, const uint64_t num);

  bool IsUnstable(uint64_t num);

  bool IsStable();

  void ExpandRight();

  void ExpandLeft();

  void ExpandDown();

  void ExpandUp();

  bool NeedExpansionRight();

  bool NeedExpansionLeft();

  bool NeedExpansionDown();

  bool NeedExpansionUp();

  void Iteration();
};

struct Bitmap {
  static const size_t kBase = 256;
  static const size_t kHeaderBmpSize = 14;
  static const size_t kHeaderRastSize = 40;
  static const size_t kPalleteSize = 20;

  uint8_t header_bmp[kHeaderBmpSize];
  uint8_t header_rast[kHeaderRastSize];
  uint8_t pallete[kPalleteSize];
  uint8_t *table;

  size_t width;
  size_t height;
  size_t bytes;
  size_t size;

  Bitmap(const Sandpile &sandpile) {
    width = sandpile.width;
    height = sandpile.height;
    bytes = ((((width + 1) / 2) + 3) / 4) * 4;
    size = bytes * height;
    FillHeaderBMP(sandpile);
    FillHeaderRast(sandpile);
    FillPalette();
    FillTable(sandpile);
  }

  ~Bitmap() { delete[] table; }

  void FillHeaderBMP(const Sandpile &sandpile);

  void FillHeaderRast(const Sandpile &sandpile);

  void FillPalette();

  void FillTable(const Sandpile &sandpile);

  void WriteFile(const char* directory, int j);
};

void Collapse(const Arguments &args);