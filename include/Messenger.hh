//////// MESSENGER FOR TERMINAL ///////
#ifndef MESSENGER_HH
#define MESSENGER_HH

#include <iostream>
#include <string>

using namespace std;

extern string RED;
extern string GREEN;
extern string YELLOW;
extern string BLUE;
extern string MAGENTA;
extern string CYAN;
extern string WHITE;
extern string RESET;

void F2RError(const string& message);
void F2RError(const char *message);
void F2RWarning(const string& message);
void F2RWarning(const char *message);
void F2RInfo(const string& message);
void F2RInfo(const char *message);
void F2RSuccess(const string& message);
void F2RSuccess(const char *message);

#endif