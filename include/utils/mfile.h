#ifndef M_FILE_H
#define M_FILE_H

#include <iostream>
#include <fstream>
#include <sstream>


void removeFile(std::string filePath);
void copyFile(std::string src, std::string dest);
std::string getHomePath();

#endif // M_FILE_H
