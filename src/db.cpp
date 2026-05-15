//
// Created by Cameron Olson on 5/13/26.
//
#include "db.h"

#include "model.h"
#include <iostream>
#include <sqlite3.h>
#include "db.h"

#include <filesystem>

// Get db location on user computer
std::string getDatabasePath() {
    const char* home = std::getenv("HOME");

    if (!home) {
        throw std::runtime_error("HOME environment variable not set");
    }
    std::filesystem::path appSupportDir =
            std::filesystem::path(home) / "Library" / "Application Support" / "CoffeeDialer";

    std::filesystem::create_directories(appSupportDir);

    return (appSupportDir / "coffee_dialer.db").string();
}


// Note(Cam): Create a manual db lifecycle manager, simple no RAII
bool openDatabase(Database* db,  const std::string& path) {
    int rc = sqlite3_open(path.c_str(), &db->handle);
    if (rc != SQLITE_OK) {
        std::cerr << "Error opening database at " << path <<  ": " << sqlite3_errmsg(db->handle) << std::endl;
        sqlite3_close(db->handle);
        db->handle = nullptr;
        return false;
    }
    return true;
}
void closeDatabase(Database* db) {
    if (db->handle) {
        sqlite3_close(db->handle);
        db->handle = nullptr;
    }
}

void createTable(Database* db) {
    char *errorMessage = nullptr;

    const char *sql = "CREATE TABLE IF NOT EXISTS coffee_entries("
            "id INTEGER PRIMARY KEY, "
            "bean_name TEXT NOT NULL, "
            "dose_grams REAL NOT NULL, "
            "grind_setting INT NOT NULL,"
            "yield_grams REAL NOT NULL,"
            "time_seconds REAL NOT NULL,"
            "notes TEXT DEFAULT '',"
            "created_at TEXT NOT NULL DEFAULT CURRENT_TIMESTAMP);";

    int rc = sqlite3_exec(db->handle, sql, nullptr, 0, &errorMessage);
    if (rc != SQLITE_OK) {
        std::cerr << "Error creating table: " << errorMessage << std::endl;
        sqlite3_free(errorMessage);
        return;
    }
    std::cout << "Table created successfully" << std::endl;
}

bool bindText(Database* db, sqlite3_stmt *stmt, int index, const std::string& value, const char* name) {
    int rc = sqlite3_bind_text(stmt, index, value.c_str(), -1, SQLITE_TRANSIENT);
    if (rc != SQLITE_OK) {
        std::cerr << "Bind " << name << " failed: " << sqlite3_errmsg(db->handle) << std::endl;
        return false;
    }
    return true;
}

bool bindDouble(Database* db, sqlite3_stmt *stmt, int index, double value, const char* name) {
    int rc = sqlite3_bind_double(stmt, index, value);
    if (rc != SQLITE_OK) {
        std::cerr << "Bind " << name << " failed: " << sqlite3_errmsg(db->handle) << std::endl;
        return false;
    }
    return true;
}

bool bindInt(Database* db, sqlite3_stmt *stmt, int index, int value, const char* name) {
    int rc = sqlite3_bind_int(stmt, index, value);
    if (rc != SQLITE_OK) {
        std::cerr << "Bind " << name << " failed: " << sqlite3_errmsg(db->handle) << std::endl;
        return false;
    }
    return true;
}

bool insertRow(Database* db, const CoffeeEntry& entry) {
    // gonna need so much binding
    sqlite3_stmt *stmt = nullptr;

    const char* sql = "INSERT INTO coffee_entries ("
                      "bean_name, "
                      "dose_grams, "
                      "grind_setting,"
                      "yield_grams, "
                      "time_seconds, "
                      "notes"
                      ") VALUES (?, ?, ?, ?, ?, ?);";

    int rc = sqlite3_prepare_v2(db->handle, sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        std::cerr << "Prepare Failed: " << sqlite3_errmsg(db->handle) << std::endl;
        return false;
    }

    const bool ok =
            bindText(db, stmt, 1, entry.beanName, "bean_name") &&
            bindDouble(db, stmt, 2, entry.doseGrams, "dose_grams") &&
            bindInt(db, stmt, 3, entry.grindSetting, "grind_setting") &&
            bindDouble(db, stmt, 4, entry.yieldGrams, "yield_grams") &&
            bindDouble(db, stmt, 5, entry.brewTimeSeconds, "time_seconds") &&
            bindText(db, stmt, 6, entry.notes, "notes");
    if (!ok) {
        sqlite3_finalize(stmt);
        return false;
    }

    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        std::cerr << "Insert failed: " << sqlite3_errmsg(db->handle) << std::endl;
        sqlite3_finalize(stmt);
        return false;
    }
    sqlite3_finalize(stmt);
    return true;

}

bool deleteRow(Database* db, int rowId) {
    sqlite3_stmt *stmt = nullptr;
    const char* sql = "DELETE FROM coffee_entries WHERE id = ?;";
    int rc = sqlite3_prepare_v2(db->handle, sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        std::cerr << "Prepare failed: " << sqlite3_errmsg(db->handle) << std::endl;
        return false;
    }

    bool ok = bindInt(db, stmt, 1, rowId, "id");
    if (!ok) {
        sqlite3_finalize(stmt);
        return false;
    }
    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        std::cerr << "Delete failed: " << sqlite3_errmsg(db->handle) << std::endl;
        sqlite3_finalize(stmt);
        return false;
    }
    sqlite3_finalize(stmt);
    return true;
}
bool updateRow(Database* db, const CoffeeEntry& entry, int rowId) {
    sqlite3_stmt *stmt = nullptr;
    const char *sql = "UPDATE coffee_entries SET "
            "bean_name = ?, "
            "dose_grams = ?, "
            "grind_setting = ?, "
            "yield_grams = ?, "
            "time_seconds = ?, "
            "notes = ? WHERE id = ?;";
    int rc = sqlite3_prepare_v2(db->handle, sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        std::cerr << "Prepare failed: " << sqlite3_errmsg(db->handle) << std::endl;
        return false;
    }
    const bool ok =
            bindText(db, stmt, 1, entry.beanName, "bean_name") &&
            bindDouble(db, stmt, 2, entry.doseGrams, "dose_grams") &&
            bindInt(db, stmt, 3, entry.grindSetting, "grind_setting") &&
            bindDouble(db, stmt, 4, entry.yieldGrams, "yield_grams") &&
            bindDouble(db, stmt, 5, entry.brewTimeSeconds, "time_seconds") &&
            bindText(db, stmt, 6, entry.notes, "notes") &&
            bindInt(db, stmt, 7, rowId, "id");
    if (!ok) {
        sqlite3_finalize(stmt);
        return false;
    }
    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        std::cerr << "Update failed: " << sqlite3_errmsg(db->handle) << std::endl;
        sqlite3_finalize(stmt);
        return false;
    }
    sqlite3_finalize(stmt);
    return true;
}

std::vector<CoffeeEntry> loadCoffeeEntries(Database* db) {
    std::vector<CoffeeEntry> coffeeEntries;
    const char* sql =
            "SELECT "
            "id, "
            "bean_name, "
            "dose_grams, "
            "grind_setting, "
            "yield_grams, "
            "time_seconds, "
            "notes, "
            "created_at "
            "FROM coffee_entries "
            "ORDER BY created_at DESC;";

    sqlite3_stmt *stmt = nullptr;
    int rc = sqlite3_prepare_v2(db->handle, sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        std::cerr << "Prepare failed: " << sqlite3_errmsg(db->handle) << std::endl;
        return coffeeEntries;
    }

    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        CoffeeEntry coffeeEntry;
        coffeeEntry.id = sqlite3_column_int(stmt, 0);
        const unsigned char* beanNameText = sqlite3_column_text(stmt, 1);
        coffeeEntry.beanName = beanNameText ? reinterpret_cast<const char*>(beanNameText) : "";
        coffeeEntry.doseGrams = sqlite3_column_double(stmt, 2);
        coffeeEntry.grindSetting = sqlite3_column_int(stmt, 3);
        coffeeEntry.yieldGrams = sqlite3_column_double(stmt, 4);
        coffeeEntry.brewTimeSeconds = sqlite3_column_double(stmt, 5);
        const unsigned char* notesText = sqlite3_column_text(stmt, 6);
        coffeeEntry.notes = notesText ? reinterpret_cast<const char*>(notesText) : "";
        const unsigned char* createdAtText = sqlite3_column_text(stmt, 7);
        coffeeEntry.createdAt = createdAtText ? reinterpret_cast<const char*>(createdAtText) : "";
        coffeeEntries.push_back(coffeeEntry);
    }
    if (rc != SQLITE_DONE) {
        std::cerr << "Error loading coffees: " << sqlite3_errmsg(db->handle) << std::endl;
    }

    sqlite3_finalize(stmt);
    return coffeeEntries;
}