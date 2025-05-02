#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <string>
#include "Houseguest.h"
using namespace std;
class Houseguest;

void inputToLower(string&);

string validateYesNo(const string in, string n);

bool validateName(string, string, bool);

void displayCPUvector(vector<Houseguest> CPU);

#endif

