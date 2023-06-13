#include <iostream>
#include "sqlite3.h"

int main() {
    sqlite3* db;
    std::string dbPath = "./flights.db";

    int result = sqlite3_open(dbPath.c_str(), &db);
    if (result != SQLITE_OK) {
        std::cerr << "数据库打开失败: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_close(db);
        return false;
    }

    // 构建插入数据的 SQL 语句
    std::string insertQuery = "INSERT INTO Flights (FlightNumber, DepartureAirport, ArrivalAirport, DepartureTime, ArrivalTime, Airline, RemainingSeats) VALUES "
        "('111', 'Departure Airport', 'Arrival Airport', '2023-06-10 22:00', '2023-06-11 05:00', 'Airline', 100);";

    // 执行插入操作
    result = sqlite3_exec(db, insertQuery.c_str(), nullptr, nullptr, nullptr);
    if (result != SQLITE_OK) {
        std::cerr << "插入失败: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_close(db);
        return false;
    }

    std::cout << "插入成功" << std::endl;

    // 关闭数据库连接
    sqlite3_close(db);
    return 0;
}