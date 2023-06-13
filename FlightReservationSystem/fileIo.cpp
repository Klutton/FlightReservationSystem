#include <json/json.h>
#include <fstream>
#include <iostream>
#include "fileIo.h"
#include "sqlite3.h"

std::string readConfig() {
    std::string dbDirectory;

    // ��ȡ�����ļ�
    std::ifstream configFile("config.json");
    if (!configFile.is_open()) {
        // �����ļ������ڻ��޷���
        std::cerr << "�޷��������ļ�" << std::endl;
        return "";
    }

    // ���ļ����ݶ�ȡ���ַ�����
    std::string jsonString((std::istreambuf_iterator<char>(configFile)), std::istreambuf_iterator<char>());

    // ���� JSON
    Json::Value root;
    Json::Reader reader;
    if (!reader.parse(jsonString, root)) {
        // �޷����� JSON �ļ�
        std::cerr << "�޷����� JSON �ļ�" << std::endl;
        return "";
    }

    // ��ȡ���ݿ�Ŀ¼
    if (root.isMember("db_directory")) {
        dbDirectory = root["db_directory"].asString();
    }
    else {
        // �����ļ�ȱ�� "db_directory" ��
        std::cerr << "�����ļ�ȱ�� \"db_directory\" ��" << std::endl;
        return "";
    }

    // dbDirectory�ĸ�ʽ�Ϸ�����Ҫ�����������м��
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
        std::cout << "Ĭ�������ļ��Ѵ�����" << filename << std::endl;
        return true;
    }
    else {
        std::cerr << "�޷����������ļ���" << filename << std::endl;
        return false;
    }
}

bool checkTableStructure(sqlite3* db) {


    bool isStructureValid = true;
    // ��麽����Ƿ����
    std::string query = "SELECT name FROM sqlite_master WHERE type='table' AND name='Flights';";
    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr);
    if (rc == SQLITE_OK) {
        rc = sqlite3_step(stmt);
        if (rc == SQLITE_ROW) {
            // ����ڣ���������ṹ���ֶ�
            std::string structureQuery = "PRAGMA table_info(Flights);";
            rc = sqlite3_prepare_v2(db, structureQuery.c_str(), -1, &stmt, nullptr);
            if (rc == SQLITE_OK) {
                while (sqlite3_step(stmt) == SQLITE_ROW) {
                    std::string columnName(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)));

                    if (columnName == "FlightNumber" || columnName == "DepartureAirport" || columnName == "ArrivalAirport"
                        || columnName == "DepartureTime" || columnName == "ArrivalTime" || columnName == "Airline" || columnName == "RemainingSeats") {
                        // �ֶη���Ԥ�ڣ�������һ���ֶεļ��
                        continue;
                    }

                    // ����ֶβ�����Ԥ�ڣ����� isStructureValid Ϊ false ���˳�ѭ��
                    isStructureValid = false;
                    break;
                }


                sqlite3_finalize(stmt);

                if (!isStructureValid) {
                    std::cout << "���ݿ���ֶβ�����Ԥ��" << std::endl;
                }
                else {
                    std::cout << "���ݿ��ṹ���ֶξ�����Ԥ��" << std::endl;
                }
            }
        }
    }

    // ����û����Ƿ����
    query = "SELECT name FROM sqlite_master WHERE type='table' AND name='User';";
    rc = sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr);
    if (rc == SQLITE_OK) {
        rc = sqlite3_step(stmt);
        if (rc == SQLITE_ROW) {
            // �û�����ڣ���������ṹ���ֶ�
            std::string structureQuery = "PRAGMA table_info(User);";
            rc = sqlite3_prepare_v2(db, structureQuery.c_str(), -1, &stmt, nullptr);
            if (rc == SQLITE_OK) {
                while (sqlite3_step(stmt) == SQLITE_ROW) {
                    std::string columnName(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)));

                    if (columnName == "UserID" || columnName == "UserName" || columnName == "SubscribedFlights") {
                        // �ֶη���Ԥ�ڣ�������һ���ֶεļ��
                        continue;
                    }

                    // ����ֶβ�����Ԥ�ڣ����� isStructureValid Ϊ false ���˳�ѭ��
                    isStructureValid = false;
                    break;
                }

                sqlite3_finalize(stmt);

                if (!isStructureValid) {
                    std::cout << "�û����ֶβ�����Ԥ��" << std::endl;
                }
                else {
                    std::cout << "�û���ṹ���ֶξ�����Ԥ��" << std::endl;
                }
            }
        }
    }
    return isStructureValid;
}

bool checkTable(const std::string& db_dir) {
    // ������ݿ��ļ��Ƿ����
    std::ifstream f(db_dir.c_str());
    if (!f.good()) {
        std::cout << "���ݿ��ļ�������" << std::endl;
        return false;
    }

    // �����ݿ�����
    sqlite3* db;
    int rc = sqlite3_open(db_dir.c_str(), &db);
    if (rc != SQLITE_OK) {
        std::cout << "�޷������ݿ�����" << std::endl;
        return false;
    }

    // ������ݿ����Ӻͱ�ṹ
    bool isConnectionValid = checkTableStructure(db);

    // �ر����ݿ�����
    sqlite3_close(db);

    return isConnectionValid;
}

bool createDefaultDatabase(const std::string& dbPath) {
    sqlite3* db;
    int result = sqlite3_open(dbPath.c_str(), &db);

    if (result != SQLITE_OK) {
        std::cerr << "���ݿ��ʧ��: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_close(db);
        return false;
    }

    // ���� Flights ��
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
        std::cerr << "����ʧ��: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_close(db);
        return false;
    }

    // ���� User ��
    std::string createUserTableQuery = "CREATE TABLE IF NOT EXISTS User ("
        "UserID INTEGER PRIMARY KEY, "
        "UserName VARCHAR(50), "
        "SubscribedFlights VARCHAR(100)"
        ");";

    result = sqlite3_exec(db, createUserTableQuery.c_str(), nullptr, nullptr, nullptr);
    if (result != SQLITE_OK)
    {
        std::cerr << "����ʧ��: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_close(db);
        return false;
    }

    std::cout << "Ĭ�����ݿⴴ�����" << std::endl;

    sqlite3_close(db);
    return true;
}
