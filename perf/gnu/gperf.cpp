#include "command_options.h"
#include "perfecthash.hpp"
#include <cstring>
#include <iostream>
#include <string>
using namespace std;

int main(int argc, char* argv[])
{
    string cmdLineOption = argv[1]; // First command line argument
    cout << cmdLineOption <<std::endl;
    const CommandOption* option = Perfect_Hash::IsValidCommandLineOption(cmdLineOption.c_str(),
        cmdLineOption.length());
    if (option == NULL){
        cout << "invalid option" << endl;
        return -1;
    }
    cout << "switch" << endl;
    switch (option->OptionCode) {
    case CommandOptionCode::HELPVERBOSE:
        cout << "Application specific detailed help goes here" << endl;
        break;
    default:
        cout << "default" << endl;
        break;
    }
    return 0;
}
