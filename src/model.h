//
// Created by Cameron Olson on 5/13/26.
//

#pragma once

#include <string>

struct CoffeeEntry {
    // need way to track id for SQLite3
    int id = -1;
    std::string beanName;
    float doseGrams = 18.0f;
    int grindSetting = 0;
    float yieldGrams = 36.0f;
    float brewTimeSeconds = 30.0f;
    std::string notes;
    std::string createdAt;
};