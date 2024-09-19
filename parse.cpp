#include "parse.h"

bool CheckFile(const char* arg) {
  std::ifstream file;
  file.open(arg);
  if (file.is_open()) {
    file.close();
    return true;
  }
  return false;
}

uint64_t Conversion(const char* arg) {
  errno = 0;
  char* arg_end{};
  int64_t num = strtoll(arg, &arg_end, 10);
  if ((errno == ERANGE) || (arg_end != arg + strlen(arg)) || (num < 0)) {
    return -1;
  }
  return num;
}

bool Parse(const int argc, char** argv, Arguments& args) {
  for (int i = 1; i < argc; ++i) {
    char* arg = argv[i];
    uint64_t num;
    if (arg[0] == '-' && strlen(arg) == 2) {
      ++i;
      if (i == argc) {
        std::cerr << "Missing argument";
        return false;
      }
      char* value = argv[i];
      switch (arg[1]) {
        case 'i':
          if (!CheckFile(value)) {
            std::cerr << "File with this name not found";
            return false;
          }
          args.input_file = value;
          break;
        case 'o':
          if (!CheckFile(value)) {
            std::cerr << "Directory with this name not found";
            return false;
          }
          args.output_directory = value;
          break;
        case 'm':
          num = Conversion(value);
          if (num == -1) {
            std::cerr << "Wrong argument";
            return false;
          }
          args.max_iter = num;
          break;
        case 'f':
          num = Conversion(value);
          if (num == -1) {
            std::cerr << "Wrong argument";
            return false;
          }
          args.freq = num;
          break;
        default:
          std::cerr << "Unknown argument";
          return false;
      }
    } else if (arg[0] == '-' && arg[1] == '-') {
      uint8_t len;
      switch (arg[2]) {
        case 'i':
          len = 8;
          if (strncmp(arg, "--input=", len + 1) != 0) {
            std::cerr << "Wrong argument";
            return false;
          }
          arg = arg + len;
          if (!CheckFile(arg)) {
            std::cerr << "File with this name not found";
            return false;
          }
          args.input_file = arg;
          break;
        case 'o':
          len = 9;
          if (strncmp(arg, "--output=", len) != 0) {
            std::cerr << "Wrong argument";
            return false;
          }
          arg = arg + len;
          if (!CheckFile(arg)) {
            std::cerr << "Directory with this name not found";
            return false;
          }
          args.output_directory = arg;
          break;
        case 'm':
          len = 11;
          if (strncmp(arg, "--max-iter=", len) != 0) {
            std::cerr << "Wrong argument";
            return false;
          }
          arg = arg + len;
          num = Conversion(arg);
          if (num == -1) {
            std::cerr << "Wrong argument";
            return false;
          }
          args.max_iter = num;
          break;
        case 'f':
          len = 7;
          if (strncmp(arg, "--freq=", len) != 0) {
            std::cerr << "Wrong argument";
            return false;
          }
          arg = arg + len;
          num = Conversion(arg);
          if (num == -1) {
            std::cerr << "Wrong argument";
            return false;
          }
          args.freq = num;
          break;
        default:
          std::cerr << "Unknown argument";
          return false;
      }
    } else {
      std::cerr << "Wrong argument";
      return false;
    }
  }
  if (args.input_file == nullptr) {
    std::cerr << "Input file missing";
    return false;
  }
  if (args.output_directory == nullptr || (!std::filesystem::is_directory(args.output_directory))) {
    std::cerr << "Output directory missing";
    return false;
  }
  return true;
}
