#ifndef FILEIO_H
#define FILEIO_H

#include <string>

std::string readConfig();
bool createDefaultConfigFile();
bool checkTable(const std::string& db_dir);
bool createDefaultDatabase(const std::string& dbPath);

#endif // FILEIO_H
