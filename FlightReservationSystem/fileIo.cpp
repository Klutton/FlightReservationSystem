#include <json/json.h>
#include <fstream>
#include <iostream>
#include "fileIo.h"
#include "sqlite3.h"

std::string readConfig() {
    std::string dbDirectory;

    // 读取配置文件
    std::ifstream configFile("config.json");
    if (!configFile.is_open()) {
        // 配置文件不存在或无法打开
        std::cerr << "无法打开配置文件" << std::endl;
        return "";
    }

    // 将文件内容读取到字符串中
    std::string jsonString((std::istreambuf_iterator<char>(configFile)), std::istreambuf_iterator<char>());

    // 解析 JSON
    Json::Value root;
    Json::Reader reader;
    if (!reader.parse(jsonString, root)) {
        // 无法解析 JSON 文件
        std::cerr << "无法解析 JSON 文件" << std::endl;
        return "";
    }

    // 读取数据库目录
    if (root.isMember("db_directory")) {
        dbDirectory = root["db_directory"].asString();
    }
    else {
        // 配置文件缺少 "db_directory" 键
        std::cerr << "配置文件缺少 \"db_directory\" 键" << std::endl;
        return "";
    }

    // dbDirectory的格式合法性需要其他功能自行检查
    return dbDirectory;
}

bool createDefaultConfigFile() {
    std::string filename = "config.json";
    Json::Value config;
    config["db_directory"] = "./flights.db";

    std::ofstream configFile(filename);
    if (configFile.is_open()) {
        configFile << config;
        configFile.close();
        std::cout << "默认配置文件已创建：" << filename << std::endl;
        return true;
    }
    else {
        std::cerr << "无法创建配置文件：" << filename << std::endl;
        return false;
    }
}

bool checkTableStructure(sqlite3* db) {


    bool isStructureValid = true;
    // 检查航班表是否存在
    std::string query = "SELECT name FROM sqlite_master WHERE type='table' AND name='Flights';";
    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr);
    if (rc == SQLITE_OK) {
        rc = sqlite3_step(stmt);
        if (rc == SQLITE_ROW) {
            // 表存在，继续检查表结构和字段
            std::string structureQuery = "PRAGMA table_info(Flights);";
            rc = sqlite3_prepare_v2(db, structureQuery.c_str(), -1, &stmt, nullptr);
            if (rc == SQLITE_OK) {
                while (sqlite3_step(stmt) == SQLITE_ROW) {
                    std::string columnName(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)));

                    if (columnName == "FlightNumber" || columnName == "DepartureAirport" || columnName == "ArrivalAirport"
                        || columnName == "DepartureTime" || columnName == "ArrivalTime" || columnName == "Airline" || columnName == "RemainingSeats") {
                        // 字段符合预期，继续下一个字段的检查
                        continue;
                    }

                    // 如果字段不符合预期，设置 isStructureValid 为 false 并退出循环
                    isStructureValid = false;
                    break;
                }


                sqlite3_finalize(stmt);

                if (!isStructureValid) {
                    std::cout << "数据库表字段不符合预期" << std::endl;
                }
                else {
                    std::cout << "数据库表结构和字段均符合预期" << std::endl;
                }
            }
        }
    }

    // 检查用户表是否存在
    query = "SELECT name FROM sqlite_master WHERE type='table' AND name='User';";
    rc = sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr);
    if (rc == SQLITE_OK) {
        rc = sqlite3_step(stmt);
        if (rc == SQLITE_ROW) {
            // 用户表存在，继续检查表结构和字段
            std::string structureQuery = "PRAGMA table_info(User);";
            rc = sqlite3_prepare_v2(db, structureQuery.c_str(), -1, &stmt, nullptr);
            if (rc == SQLITE_OK) {
                while (sqlite3_step(stmt) == SQLITE_ROW) {
                    std::string columnName(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)));

                    if (columnName == "UserID" || columnName == "UserName" || columnName == "SubscribedFlights") {
                        // 字段符合预期，继续下一个字段的检查
                        continue;
                    }

                    // 如果字段不符合预期，设置 isStructureValid 为 false 并退出循环
                    isStructureValid = false;
                    break;
                }

                sqlite3_finalize(stmt);

                if (!isStructureValid) {
                    std::cout << "用户表字段不符合预期" << std::endl;
                }
                else {
                    std::cout << "用户表结构和字段均符合预期" << std::endl;
                }
            }
        }
    }
    return isStructureValid;
}

bool checkTable(const std::string& db_dir) {
    // 检查数据库文件是否存在
    std::ifstream f(db_dir.c_str());
    if (!f.good()) {
        std::cout << "数据库文件不存在" << std::endl;
        return false;
    }

    // 打开数据库连接
    sqlite3* db;
    int rc = sqlite3_open(db_dir.c_str(), &db);
    if (rc != SQLITE_OK) {
        std::cout << "无法打开数据库连接" << std::endl;
        return false;
    }

    // 检查数据库连接和表结构
    bool isConnectionValid = checkTableStructure(db);

    // 关闭数据库连接
    sqlite3_close(db);

    return isConnectionValid;
}

bool createDefaultDatabase(const std::string& dbPath) {
    sqlite3* db;
    int result = sqlite3_open(dbPath.c_str(), &db);

    if (result != SQLITE_OK) {
        std::cerr << "数据库打开失败: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_close(db);
        return false;
    }

    // 创建 Flights 表
    std::string createTableQuery = "CREATE TABLE IF NOT EXISTS Flights ("
        "FlightNumber VARCHAR(10) PRIMARY KEY, "
        "DepartureAirport VARCHAR(50), "
        "ArrivalAirport VARCHAR(50), "
        "DepartureTime DATETIME, "
        "ArrivalTime DATETIME, "
        "Airline VARCHAR(50), "
        "RemainingSeats INTEGER"
        ");";

    result = sqlite3_exec(db, createTableQuery.c_str(), nullptr, nullptr, nullptr);

    if (result != SQLITE_OK) {
        std::cerr << "表创建失败: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_close(db);
        return false;
    }

    // 创建 User 表
    std::string createUserTableQuery = "CREATE TABLE IF NOT EXISTS User ("
        "UserID INTEGER PRIMARY KEY, "
        "UserName VARCHAR(50), "
        "SubscribedFlights VARCHAR(100)"
        ");";

    result = sqlite3_exec(db, createUserTableQuery.c_str(), nullptr, nullptr, nullptr);
    if (result != SQLITE_OK)
    {
        std::cerr << "表创建失败: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_close(db);
        return false;
    }

    std::cout << "默认数据库创建完成" << std::endl;

    sqlite3_close(db);
    return true;
}
