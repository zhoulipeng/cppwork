#include "command_options.h"
#include "perfecthash.hpp"
#include <cstring>
#include <iostream>
#include <string>
using namespace std;

int main(int argc, char* argv[])
{
    for (int i = 1; i < argc; i++) {
        const CommandOption* opt = Perfect_Hash::IsValidCommandLineOption(argv[i], strlen(argv[i]));
        if (opt != NULL) {
            fprintf(stderr, "ValidOption: %s, %d\n", opt->Option, opt->OptionCode);
        } else {
            fprintf(stderr, "InvalidOption: %s\n", argv[i]);
        }
    }
    return 0;
}
