#include "Messenger.hh"

string RED = "\033[1;31m";
string GREEN = "\033[1;32m";
string YELLOW = "\033[1;33m";
string BLUE = "\033[1;34m";
string MAGENTA = "\033[1;35m";
string CYAN = "\033[1;36m";
string WHITE = "\033[1;37m";
string RESET = "\033[0m";

void F2RError(const string& message) {
    cout << RED << "<ERROR> " << message << RESET << endl;
}

void F2RError(const char *message) {
    cout << RED << "<ERROR> " << message << RESET << endl;
}

void F2RWarning(const string& message) {
    cout << YELLOW << "<WARNING> " << message << RESET << endl;
}

void F2RWarning(const char *message) {
    cout << YELLOW << "<WARNING> " << message << RESET << endl;
}

void F2RInfo(const string& message) {
    cout << BLUE << " <INFO>  " << message << RESET << endl;
}

void F2RInfo(const char *message) {
    cout << BLUE << " <INFO>  " << message << RESET << endl;
}

void F2RSuccess(const string& message) {
    cout << GREEN << "<SUCCESS> " << message << RESET << endl;
}

void F2RSuccess(const char *message) {
    cout << GREEN << "<SUCCESS> " << message << RESET << endl;
}