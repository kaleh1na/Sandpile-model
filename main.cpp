#include "parse.cpp"
#include "sandpile.cpp"

int main(int argc, char** argv) {
    Arguments args;
    if (!Parse(argc, argv, args)) {
        return 1;
    }
    Collapse(args);
    return 0;
}