#pragma once
#include <string>
#include <chrono>
#include <iomanip>
#include <sstream>
#include <vector>
#include <limits>
#include <iostream>

enum class TransactionStatus { PENDING, PAID, COMPLETED, CANCELLED };

inline std::string statusToString(TransactionStatus status) {
    switch (status) {
        case TransactionStatus::PENDING: return "PENDING";
        case TransactionStatus::PAID: return "PAID";
        case TransactionStatus::COMPLETED: return "COMPLETED";
        case TransactionStatus::CANCELLED: return "CANCELLED";
        default: return "UNKNOWN";
    }
}

inline TransactionStatus stringToStatus(const std::string& s) {
    if (s == "PENDING") return TransactionStatus::PENDING;
    if (s == "PAID") return TransactionStatus::PAID;
    if (s == "COMPLETED") return TransactionStatus::COMPLETED;
    if (s == "CANCELLED") return TransactionStatus::CANCELLED;
    return TransactionStatus::PENDING;
}

inline std::string getCurrentDate() {
    auto now = std::chrono::system_clock::now();
    auto in_time_t = std::chrono::system_clock::to_time_t(now);
    std::tm buf;
    #ifdef _WIN32
        localtime_s(&buf, &in_time_t);
    #else
        localtime_r(&in_time_t, &buf);
    #endif
    std::stringstream ss;
    ss << std::put_time(&buf, "%Y-%m-%d");
    return ss.str();
}

inline long days_between(const std::string& date1_str, const std::string& date2_str) {
    std::tm tm1 = {}, tm2 = {};
    std::stringstream ss1(date1_str), ss2(date2_str);
    ss1 >> std::get_time(&tm1, "%Y-%m-%d");
    ss2 >> std::get_time(&tm2, "%Y-%m-%d");

    auto time1 = std::mktime(&tm1);
    auto time2 = std::mktime(&tm2);
    
    const double seconds_per_day = 60 * 60 * 24;
    return static_cast<long>(std::abs(time1 - time2) / seconds_per_day);
}

inline void clearInputBuffer() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}