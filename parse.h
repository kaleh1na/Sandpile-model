#pragma once
#include <iostream>
#include <fstream>
#include <limits>
#include <cstring>
#include <filesystem>

struct Arguments {
  char* input_file = nullptr;
  char* output_directory = nullptr;
  uint64_t max_iter = std::numeric_limits<uint64_t>::max();
  uint64_t freq = 0;
};

bool CheckFile(const char* arg);

uint64_t Conversion(const char* arg);

bool Parse(const int argc, char** argv, Arguments &args);

