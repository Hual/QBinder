#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <io.h>
#include <string>
#include <vector>
#include <fstream>

#include "controls.h"

std::vector<std::string> split(const std::string, const char);
int random(const int, const int);
void GetSystemTime(char*, const bool);
void ToggleVisualStyles(const bool, ULONG_PTR&);