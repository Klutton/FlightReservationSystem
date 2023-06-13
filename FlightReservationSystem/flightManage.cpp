#include <iostream>
#include <sstream>
#include <string>
#include <sqlite3.h>
#include "flightManage.h"
#include <iomanip>

void queryAndPrint(const std::string& dbPath, const std::string& query) {
    // 连接数据库
    sqlite3* db;
    int result = sqlite3_open(dbPath.c_str(), &db);
    if (result != SQLITE_OK) {
        std::cerr << "数据库打开失败: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_close(db);
        return;
    }

    // 准备SQL语句
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << "SQL语句构建失败: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return;
    }

    // 查询成功
    int columnCount = sqlite3_data_count(stmt);
    std::cout << "结果如下(" << std::to_string(columnCount) << "条): " << std::endl;

    // 遍历结果集并打印结果
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        std::string flightNumber(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0)));
        std::string departureAirport(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)));
        std::string arrivalAirport(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2)));
        std::string departureTime(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3)));
        std::string arrivalTime(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4)));
        std::string airline(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 5)));
        std::string remainingSeats(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 6)));

        std::cout << "-----------------------------" << std::endl;
        std::cout << "航班号: " << flightNumber << std::endl;
        std::cout << "出发地: " << departureAirport << std::endl;
        std::cout << "目的地: " << arrivalAirport << std::endl;
        std::cout << "出发时间: " << departureTime << std::endl;
        std::cout << "抵达时间: " << arrivalTime << std::endl;
        std::cout << "航班公司: " << airline << std::endl;
        std::cout << "剩余位置: " << remainingSeats << std::endl;
    }

    // 释放资源
    sqlite3_finalize(stmt);

    // 关闭数据库连接
    sqlite3_close(db);
}

void insertFlight(const std::string& dbPath) {
    // 连接数据库
    sqlite3* db;
    int result = sqlite3_open(dbPath.c_str(), &db);
    if (result != SQLITE_OK) {
        std::cerr << "数据库打开失败: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_close(db);
        return;
    }

    std::string flightNumber;
    std::string departureAirport;
    std::string arrivalAirport;
    std::string departureTime;
    std::string arrivalTime;
    std::string airline;
    std::string remainingSeats;

    //清除缓存
    std::string block = "\n";
    std::getline(std::cin, block);

    // 输入航班信息
    std::cout << "航班号: ";
    std::getline(std::cin, flightNumber);

    std::cout << "出发地: ";
    std::getline(std::cin, departureAirport);

    std::cout << "目的地: ";
    std::getline(std::cin, arrivalAirport);

    std::cout << "出发时间 (YYYY-MM-DD HH:MM): ";
    std::getline(std::cin, departureTime);

    std::cout << "抵达时间 (YYYY-MM-DD HH:MM): ";
    std::getline(std::cin, arrivalTime);

    std::cout << "航班公司: ";
    std::getline(std::cin, airline);

    std::cout << "乘坐人数: ";
    std::cin >> remainingSeats;

    // 在此处添加验证航班信息的代码，确保输入的内容合法，略

    // 格式化日期和时间
    std::tm tmDepartureTime;
    std::istringstream departureTimeStream(departureTime);
    if (!(departureTimeStream >> std::get_time(&tmDepartureTime, "%Y-%m-%d %H:%M"))) {
        std::cerr << "出发时间格式不正确" << std::endl;
        sqlite3_close(db);
        return;
    }

    std::tm tmArrivalTime;
    std::istringstream arrivalTimeStream(arrivalTime);
    if (!(arrivalTimeStream >> std::get_time(&tmArrivalTime, "%Y-%m-%d %H:%M"))) {
        std::cerr << "抵达时间格式不正确" << std::endl;
        sqlite3_close(db);
        return;
    }

    // 将格式化后的日期和时间转换为字符串
    std::ostringstream formattedDepartureTime;
    formattedDepartureTime << std::put_time(&tmDepartureTime, "%Y-%m-%d %H:%M");

    std::ostringstream formattedArrivalTime;
    formattedArrivalTime << std::put_time(&tmArrivalTime, "%Y-%m-%d %H:%M");

    // 构建插入航班信息的 SQL 语句
    std::string insertQuery = "INSERT INTO Flights (FlightNumber, DepartureAirport, ArrivalAirport, DepartureTime, ArrivalTime, Airline, RemainingSeats) VALUES ('" + flightNumber + "', '" + departureAirport + "', '" + arrivalAirport + "', '" + formattedDepartureTime.str() + "', '" + formattedArrivalTime.str() + "', '" + airline + "', " + remainingSeats + ");";

    // 执行插入操作
    result = sqlite3_exec(db, insertQuery.c_str(), nullptr, nullptr, nullptr);

    if (result != SQLITE_OK) {
        std::cerr << "插入失败: " << sqlite3_errmsg(db) << std::endl << std::endl;
    }
    else {
        std::cout << "插入成功" << std::endl << std::endl;
    }

    // 关闭数据库连接
    sqlite3_close(db);
    return;
}

void searchFlightViaDst(const std::string& dbPath) {

    std::string destination;
    std::cout << "输入目的地: ";
    std::cin >> destination;

    // 执行查询操作
    std::string selectQuery = "SELECT * FROM Flights WHERE ArrivalAirport = '" + destination + "';";
    queryAndPrint(dbPath, selectQuery);

    std::cout << "-----------------------------" << std::endl;
    std::cout << "以上是符合要求的航班" << std::endl << std::endl;

    return;
}

void searchFlightViaNumber(const std::string& dbPath) {

    std::string flightNumber;
    std::cout << "输入航班号: ";
    std::cin >> flightNumber;

    // 执行查询操作
    std::string selectQuery = "SELECT * FROM Flights WHERE FlightNumber = '" + flightNumber + "';";
    queryAndPrint(dbPath, selectQuery);

    std::cout << "-----------------------------" << std::endl;
    std::cout << "以上是符合要求的航班" << std::endl << std::endl;

    return;
}

void browseFlights24(const std::string& dbPath) {
    // 获取当前时间和24小时后的时间
    std::time_t now = std::time(nullptr);
    std::time_t dayLater = now + (24 * 60 * 60);  // 当前时间点后24小时

    // 将时间转换为本地时间结构
    std::tm currentDate;
    localtime_s(&currentDate, &now);

    std::tm dayLaterDate;
    localtime_s(&dayLaterDate, &dayLater);

    // 格式化当前时间和24小时后的时间
    char currentDateTime[20];
    char dayLaterDateTime[20];
    std::strftime(currentDateTime, sizeof(currentDateTime), "%Y-%m-%d %H:%M:%S", &currentDate);
    std::strftime(dayLaterDateTime, sizeof(dayLaterDateTime), "%Y-%m-%d %H:%M:%S", &dayLaterDate);

    // 构建查询条件，获取当前时间点后24小时内的航班信息
    std::string selectQuery = "SELECT * FROM Flights WHERE DepartureTime >= DATETIME('"
        + std::string(currentDateTime)
        + "') AND DepartureTime <= DATETIME('"
        + std::string(dayLaterDateTime)
        + "');";

    // 执行查询操作
    queryAndPrint(dbPath, selectQuery);
    std::cout << "-----------------------------" << std::endl;
    std::cout << "以上是24小时内的航班" << std::endl << std::endl;
}

void browseFlights(const std::string& dbPath) {
    // 获取当前时间
    std::time_t now = std::time(nullptr);
    std::tm currentDate;
    localtime_s(&currentDate, &now);

    // 格式化当前时间
    char currentDateTime[20];
    std::strftime(currentDateTime, sizeof(currentDateTime), "%Y-%m-%d %H:%M:%S", &currentDate);

    // 构建查询条件，获取当前时间点后的航班信息
    std::string selectQuery = "SELECT * FROM Flights WHERE DepartureTime >= DATETIME('"
        + std::string(currentDateTime)
        + "');";

    // 执行查询操作
    queryAndPrint(dbPath, selectQuery);
    std::cout << "-----------------------------" << std::endl;
    std::cout << "以上是所有航班" << std::endl << std::endl;
}

void searchUserFlights(const std::string& dbPath, const std::string& userID) {
    sqlite3* db;
    int rc = sqlite3_open(dbPath.c_str(), &db);
    if (rc != SQLITE_OK) {
        std::cerr << "数据库打开失败: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_close(db);
        return;
    }

    std::string query = "SELECT * FROM Flights WHERE FlightNumber IN (SELECT SubscribedFlights FROM User WHERE UserID = " + userID + ");";
    sqlite3_stmt* stmt;
    rc = sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr);
    if (rc == SQLITE_OK) {
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            std::string flightNumber(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0)));
            std::string departureAirport(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)));
            std::string arrivalAirport(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2)));
            std::string departureTime(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3)));
            std::string arrivalTime(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4)));
            std::string airline(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 5)));

            std::cout << "-----------------------------" << std::endl;
            std::cout << "航班号: " << flightNumber << std::endl;
            std::cout << "出发机场: " << departureAirport << std::endl;
            std::cout << "到达机场: " << arrivalAirport << std::endl;
            std::cout << "出发时间: " << departureTime << std::endl;
            std::cout << "到达时间: " << arrivalTime << std::endl;
            std::cout << "航空公司: " << airline << std::endl;
        }
    }
    else {
        std::cerr << "查询预订航班失败: " << sqlite3_errmsg(db) << std::endl;
    }

    std::cout << "-----------------------------" << std::endl;
    std::cout << "查询完毕" << std::endl << std::endl;

    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return;
}

void bookFlight(const std::string& dbPath, const std::string& flightNumber, const std::string& userID) {
    sqlite3* db;
    int rc = sqlite3_open(dbPath.c_str(), &db);
    if (rc != SQLITE_OK) {
        std::cerr << "打开数据库失败: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_close(db);
        return;
    }

    // 开始一个事务
    rc = sqlite3_exec(db, "BEGIN TRANSACTION;", nullptr, nullptr, nullptr);
    if (rc != SQLITE_OK) {
        std::cerr << "事务启动失败: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_close(db);
        return;
    }

    // 检查航班是否存在
    std::string checkFlightQuery = "SELECT FlightNumber, RemainingSeats FROM Flights WHERE FlightNumber = '" + flightNumber + "';";
    sqlite3_stmt* checkStmt;
    rc = sqlite3_prepare_v2(db, checkFlightQuery.c_str(), -1, &checkStmt, nullptr);
    if (rc == SQLITE_OK) {
        if (sqlite3_step(checkStmt) == SQLITE_ROW) {
            // 获取航班的剩余座位数
            int remainingSeats = sqlite3_column_int(checkStmt, 1);
            if (remainingSeats > 0) {
                // 更新航班表的剩余座位数
                std::string updateSeatsQuery = "UPDATE Flights SET RemainingSeats = " + std::to_string(remainingSeats - 1) + " WHERE FlightNumber = '" + flightNumber + "';";
                rc = sqlite3_exec(db, updateSeatsQuery.c_str(), nullptr, nullptr, nullptr);
                if (rc != SQLITE_OK) {
                    std::cerr << "更新航班剩余座位数失败: " << sqlite3_errmsg(db) << std::endl;
                    sqlite3_exec(db, "ROLLBACK;", nullptr, nullptr, nullptr); // 回滚事务
                }
                else {
                    // 更新用户表的订阅航班号
                    std::string updateSubscribedFlightsQuery = "UPDATE User SET SubscribedFlights = SubscribedFlights || '" + flightNumber + ", ' WHERE UserID = " + userID + ";";
                    rc = sqlite3_exec(db, updateSubscribedFlightsQuery.c_str(), nullptr, nullptr, nullptr);
                    if (rc != SQLITE_OK) {
                        std::cerr << "更新用户订阅航班失败: " << sqlite3_errmsg(db) << std::endl;
                        sqlite3_exec(db, "ROLLBACK;", nullptr, nullptr, nullptr); // 回滚事务
                    }
                    else {
                        // 提交事务
                        rc = sqlite3_exec(db, "COMMIT;", nullptr, nullptr, nullptr);
                        if (rc != SQLITE_OK) {
                            std::cerr << "事务提交失败: " << sqlite3_errmsg(db) << std::endl;
                        }
                        else {
                            std::cout << "航班 " << flightNumber << " 预订成功！" << std::endl;
                        }
                    }
                }
            }
            else {
                std::cout << "航班 " << flightNumber << " 没有剩余座位！" << std::endl;
                sqlite3_exec(db, "ROLLBACK;", nullptr, nullptr, nullptr); // 回滚事务
            }
        }
        else {
            std::cerr << "找不到航班: " << flightNumber << std::endl;
            sqlite3_exec(db, "ROLLBACK;", nullptr, nullptr, nullptr); // 回滚事务
        }
    }
    else {
        std::cerr << "查询航班失败: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_exec(db, "ROLLBACK;", nullptr, nullptr, nullptr); // 回滚事务
    }

    sqlite3_finalize(checkStmt);
    sqlite3_close(db);

    std::cout << "-----------------------------" << std::endl;
    std::cout << std::endl << std::endl;

    return;
}

void cancelBooking(const std::string& dbPath, const std::string& flightNumber, const std::string& userID) {
    sqlite3* db;
    int rc = sqlite3_open(dbPath.c_str(), &db);
    if (rc != SQLITE_OK) {
        std::cerr << "打开数据库失败: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_close(db);
        return;
    }

    // 开始一个事务
    rc = sqlite3_exec(db, "BEGIN TRANSACTION;", nullptr, nullptr, nullptr);
    if (rc != SQLITE_OK) {
        std::cerr << "事务启动失败: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_close(db);
        return;
    }

    // 检查航班是否存在
    std::string checkFlightQuery = "SELECT FlightNumber, RemainingSeats FROM Flights WHERE FlightNumber = '" + flightNumber + "';";
    sqlite3_stmt* checkStmt;
    rc = sqlite3_prepare_v2(db, checkFlightQuery.c_str(), -1, &checkStmt, nullptr);
    if (rc == SQLITE_OK) {
        if (sqlite3_step(checkStmt) == SQLITE_ROW) {
            // 获取航班的剩余座位数
            int remainingSeats = sqlite3_column_int(checkStmt, 1);

            // 更新航班表的剩余座位数
            std::string updateSeatsQuery = "UPDATE Flights SET RemainingSeats = " + std::to_string(remainingSeats + 1) + " WHERE FlightNumber = '" + flightNumber + "';";
            rc = sqlite3_exec(db, updateSeatsQuery.c_str(), nullptr, nullptr, nullptr);
            if (rc != SQLITE_OK) {
                std::cerr << "更新航班剩余座位数失败: " << sqlite3_errmsg(db) << std::endl;
                sqlite3_exec(db, "ROLLBACK;", nullptr, nullptr, nullptr); // 回滚事务
                sqlite3_finalize(checkStmt);
                sqlite3_close(db);
                return;
            }

            // 更新用户表的订阅航班号
            std::string updateSubscribedFlightsQuery = "UPDATE User SET SubscribedFlights = REPLACE(SubscribedFlights, '" + flightNumber + ", ', '') WHERE UserID = " + userID + ";";
            rc = sqlite3_exec(db, updateSubscribedFlightsQuery.c_str(), nullptr, nullptr, nullptr);
            if (rc != SQLITE_OK) {
                std::cerr << "更新用户订阅航班失败: " << sqlite3_errmsg(db) << std::endl;
                sqlite3_exec(db, "ROLLBACK;", nullptr, nullptr, nullptr); // 回滚事务
            }
            else {
                // 提交事务
                rc = sqlite3_exec(db, "COMMIT;", nullptr, nullptr, nullptr);
                if (rc != SQLITE_OK) {
                    std::cerr << "提交事务失败: " << sqlite3_errmsg(db) << std::endl;
                }
                else {
                    std::cout << "航班 " << flightNumber << " 退订成功！" << std::endl;
                }
            }
        }
        else {
            std::cerr << "找不到航班: " << flightNumber << std::endl;
            sqlite3_exec(db, "ROLLBACK;", nullptr, nullptr, nullptr); // 回滚事务
        }
    }
    else {
        std::cerr << "查询航班失败: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_exec(db, "ROLLBACK;", nullptr, nullptr, nullptr); // 回滚事务
    }

    sqlite3_finalize(checkStmt);
    sqlite3_close(db);

    std::cout << "-----------------------------" << std::endl;
    std::cout << std::endl << std::endl;

    return;
}
