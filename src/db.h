//
// Created by Cameron Olson on 5/14/26.
//

#pragma once
#include <sqlite3.h>
#include "model.h"
#include <string>
#include <vector>

struct Database {
    sqlite3* handle = nullptr;
};
bool openDatabase(Database* db, const std::string& name);
void closeDatabase(Database* db);

std::string getDatabasePath();

// Note(cam): don't need to expose these

// bool bindText(Database* db, sqlite3_stmt *stmt, int index, const std::string& value, const char* name);
// bool bindDouble(Database* db, sqlite3_stmt *stmt, int index, double value, const char* name);
// bool bindInt(Database* db, sqlite3_stmt *stmt, int index, int value, const char* name);

void createTable(Database* db);
bool insertRow(Database* db, const CoffeeEntry& entry);
bool deleteRow(Database* db, int rowId);
bool updateRow(Database* db, const CoffeeEntry& entry, int rowId);
std::vector<CoffeeEntry> loadCoffeeEntries(Database* db);
