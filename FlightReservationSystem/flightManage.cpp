#include <iostream>
#include <sstream>
#include <string>
#include <sqlite3.h>
#include "flightManage.h"
#include <iomanip>

void queryAndPrint(const std::string& dbPath, const std::string& query) {
    // �������ݿ�
    sqlite3* db;
    int result = sqlite3_open(dbPath.c_str(), &db);
    if (result != SQLITE_OK) {
        std::cerr << "���ݿ��ʧ��: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_close(db);
        return;
    }

    // ׼��SQL���
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << "SQL��乹��ʧ��: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return;
    }

    // ��ѯ�ɹ�
    int columnCount = sqlite3_data_count(stmt);
    std::cout << "�������(" << std::to_string(columnCount) << "��): " << std::endl;

    // �������������ӡ���
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        std::string flightNumber(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0)));
        std::string departureAirport(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)));
        std::string arrivalAirport(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2)));
        std::string departureTime(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3)));
        std::string arrivalTime(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4)));
        std::string airline(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 5)));
        std::string remainingSeats(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 6)));

        std::cout << "-----------------------------" << std::endl;
        std::cout << "�����: " << flightNumber << std::endl;
        std::cout << "������: " << departureAirport << std::endl;
        std::cout << "Ŀ�ĵ�: " << arrivalAirport << std::endl;
        std::cout << "����ʱ��: " << departureTime << std::endl;
        std::cout << "�ִ�ʱ��: " << arrivalTime << std::endl;
        std::cout << "���๫˾: " << airline << std::endl;
        std::cout << "ʣ��λ��: " << remainingSeats << std::endl;
    }

    // �ͷ���Դ
    sqlite3_finalize(stmt);

    // �ر����ݿ�����
    sqlite3_close(db);
}

void insertFlight(const std::string& dbPath) {
    // �������ݿ�
    sqlite3* db;
    int result = sqlite3_open(dbPath.c_str(), &db);
    if (result != SQLITE_OK) {
        std::cerr << "���ݿ��ʧ��: " << sqlite3_errmsg(db) << std::endl;
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

    //�������
    std::string block = "\n";
    std::getline(std::cin, block);

    // ���뺽����Ϣ
    std::cout << "�����: ";
    std::getline(std::cin, flightNumber);

    std::cout << "������: ";
    std::getline(std::cin, departureAirport);

    std::cout << "Ŀ�ĵ�: ";
    std::getline(std::cin, arrivalAirport);

    std::cout << "����ʱ�� (YYYY-MM-DD HH:MM): ";
    std::getline(std::cin, departureTime);

    std::cout << "�ִ�ʱ�� (YYYY-MM-DD HH:MM): ";
    std::getline(std::cin, arrivalTime);

    std::cout << "���๫˾: ";
    std::getline(std::cin, airline);

    std::cout << "��������: ";
    std::cin >> remainingSeats;

    // �ڴ˴������֤������Ϣ�Ĵ��룬ȷ����������ݺϷ�����

    // ��ʽ�����ں�ʱ��
    std::tm tmDepartureTime;
    std::istringstream departureTimeStream(departureTime);
    if (!(departureTimeStream >> std::get_time(&tmDepartureTime, "%Y-%m-%d %H:%M"))) {
        std::cerr << "����ʱ���ʽ����ȷ" << std::endl;
        sqlite3_close(db);
        return;
    }

    std::tm tmArrivalTime;
    std::istringstream arrivalTimeStream(arrivalTime);
    if (!(arrivalTimeStream >> std::get_time(&tmArrivalTime, "%Y-%m-%d %H:%M"))) {
        std::cerr << "�ִ�ʱ���ʽ����ȷ" << std::endl;
        sqlite3_close(db);
        return;
    }

    // ����ʽ��������ں�ʱ��ת��Ϊ�ַ���
    std::ostringstream formattedDepartureTime;
    formattedDepartureTime << std::put_time(&tmDepartureTime, "%Y-%m-%d %H:%M");

    std::ostringstream formattedArrivalTime;
    formattedArrivalTime << std::put_time(&tmArrivalTime, "%Y-%m-%d %H:%M");

    // �������뺽����Ϣ�� SQL ���
    std::string insertQuery = "INSERT INTO Flights (FlightNumber, DepartureAirport, ArrivalAirport, DepartureTime, ArrivalTime, Airline, RemainingSeats) VALUES ('" + flightNumber + "', '" + departureAirport + "', '" + arrivalAirport + "', '" + formattedDepartureTime.str() + "', '" + formattedArrivalTime.str() + "', '" + airline + "', " + remainingSeats + ");";

    // ִ�в������
    result = sqlite3_exec(db, insertQuery.c_str(), nullptr, nullptr, nullptr);

    if (result != SQLITE_OK) {
        std::cerr << "����ʧ��: " << sqlite3_errmsg(db) << std::endl << std::endl;
    }
    else {
        std::cout << "����ɹ�" << std::endl << std::endl;
    }

    // �ر����ݿ�����
    sqlite3_close(db);
    return;
}

void searchFlightViaDst(const std::string& dbPath) {

    std::string destination;
    std::cout << "����Ŀ�ĵ�: ";
    std::cin >> destination;

    // ִ�в�ѯ����
    std::string selectQuery = "SELECT * FROM Flights WHERE ArrivalAirport = '" + destination + "';";
    queryAndPrint(dbPath, selectQuery);

    std::cout << "-----------------------------" << std::endl;
    std::cout << "�����Ƿ���Ҫ��ĺ���" << std::endl << std::endl;

    return;
}

void searchFlightViaNumber(const std::string& dbPath) {

    std::string flightNumber;
    std::cout << "���뺽���: ";
    std::cin >> flightNumber;

    // ִ�в�ѯ����
    std::string selectQuery = "SELECT * FROM Flights WHERE FlightNumber = '" + flightNumber + "';";
    queryAndPrint(dbPath, selectQuery);

    std::cout << "-----------------------------" << std::endl;
    std::cout << "�����Ƿ���Ҫ��ĺ���" << std::endl << std::endl;

    return;
}

void browseFlights24(const std::string& dbPath) {
    // ��ȡ��ǰʱ���24Сʱ���ʱ��
    std::time_t now = std::time(nullptr);
    std::time_t dayLater = now + (24 * 60 * 60);  // ��ǰʱ����24Сʱ

    // ��ʱ��ת��Ϊ����ʱ��ṹ
    std::tm currentDate;
    localtime_s(&currentDate, &now);

    std::tm dayLaterDate;
    localtime_s(&dayLaterDate, &dayLater);

    // ��ʽ����ǰʱ���24Сʱ���ʱ��
    char currentDateTime[20];
    char dayLaterDateTime[20];
    std::strftime(currentDateTime, sizeof(currentDateTime), "%Y-%m-%d %H:%M:%S", &currentDate);
    std::strftime(dayLaterDateTime, sizeof(dayLaterDateTime), "%Y-%m-%d %H:%M:%S", &dayLaterDate);

    // ������ѯ��������ȡ��ǰʱ����24Сʱ�ڵĺ�����Ϣ
    std::string selectQuery = "SELECT * FROM Flights WHERE DepartureTime >= DATETIME('"
        + std::string(currentDateTime)
        + "') AND DepartureTime <= DATETIME('"
        + std::string(dayLaterDateTime)
        + "');";

    // ִ�в�ѯ����
    queryAndPrint(dbPath, selectQuery);
    std::cout << "-----------------------------" << std::endl;
    std::cout << "������24Сʱ�ڵĺ���" << std::endl << std::endl;
}

void browseFlights(const std::string& dbPath) {
    // ��ȡ��ǰʱ��
    std::time_t now = std::time(nullptr);
    std::tm currentDate;
    localtime_s(&currentDate, &now);

    // ��ʽ����ǰʱ��
    char currentDateTime[20];
    std::strftime(currentDateTime, sizeof(currentDateTime), "%Y-%m-%d %H:%M:%S", &currentDate);

    // ������ѯ��������ȡ��ǰʱ����ĺ�����Ϣ
    std::string selectQuery = "SELECT * FROM Flights WHERE DepartureTime >= DATETIME('"
        + std::string(currentDateTime)
        + "');";

    // ִ�в�ѯ����
    queryAndPrint(dbPath, selectQuery);
    std::cout << "-----------------------------" << std::endl;
    std::cout << "���������к���" << std::endl << std::endl;
}

void searchUserFlights(const std::string& dbPath, const std::string& userID) {
    sqlite3* db;
    int rc = sqlite3_open(dbPath.c_str(), &db);
    if (rc != SQLITE_OK) {
        std::cerr << "���ݿ��ʧ��: " << sqlite3_errmsg(db) << std::endl;
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
            std::cout << "�����: " << flightNumber << std::endl;
            std::cout << "��������: " << departureAirport << std::endl;
            std::cout << "�������: " << arrivalAirport << std::endl;
            std::cout << "����ʱ��: " << departureTime << std::endl;
            std::cout << "����ʱ��: " << arrivalTime << std::endl;
            std::cout << "���չ�˾: " << airline << std::endl;
        }
    }
    else {
        std::cerr << "��ѯԤ������ʧ��: " << sqlite3_errmsg(db) << std::endl;
    }

    std::cout << "-----------------------------" << std::endl;
    std::cout << "��ѯ���" << std::endl << std::endl;

    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return;
}

void bookFlight(const std::string& dbPath, const std::string& flightNumber, const std::string& userID) {
    sqlite3* db;
    int rc = sqlite3_open(dbPath.c_str(), &db);
    if (rc != SQLITE_OK) {
        std::cerr << "�����ݿ�ʧ��: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_close(db);
        return;
    }

    // ��ʼһ������
    rc = sqlite3_exec(db, "BEGIN TRANSACTION;", nullptr, nullptr, nullptr);
    if (rc != SQLITE_OK) {
        std::cerr << "��������ʧ��: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_close(db);
        return;
    }

    // ��麽���Ƿ����
    std::string checkFlightQuery = "SELECT FlightNumber, RemainingSeats FROM Flights WHERE FlightNumber = '" + flightNumber + "';";
    sqlite3_stmt* checkStmt;
    rc = sqlite3_prepare_v2(db, checkFlightQuery.c_str(), -1, &checkStmt, nullptr);
    if (rc == SQLITE_OK) {
        if (sqlite3_step(checkStmt) == SQLITE_ROW) {
            // ��ȡ�����ʣ����λ��
            int remainingSeats = sqlite3_column_int(checkStmt, 1);
            if (remainingSeats > 0) {
                // ���º�����ʣ����λ��
                std::string updateSeatsQuery = "UPDATE Flights SET RemainingSeats = " + std::to_string(remainingSeats - 1) + " WHERE FlightNumber = '" + flightNumber + "';";
                rc = sqlite3_exec(db, updateSeatsQuery.c_str(), nullptr, nullptr, nullptr);
                if (rc != SQLITE_OK) {
                    std::cerr << "���º���ʣ����λ��ʧ��: " << sqlite3_errmsg(db) << std::endl;
                    sqlite3_exec(db, "ROLLBACK;", nullptr, nullptr, nullptr); // �ع�����
                }
                else {
                    // �����û���Ķ��ĺ����
                    std::string updateSubscribedFlightsQuery = "UPDATE User SET SubscribedFlights = SubscribedFlights || '" + flightNumber + ", ' WHERE UserID = " + userID + ";";
                    rc = sqlite3_exec(db, updateSubscribedFlightsQuery.c_str(), nullptr, nullptr, nullptr);
                    if (rc != SQLITE_OK) {
                        std::cerr << "�����û����ĺ���ʧ��: " << sqlite3_errmsg(db) << std::endl;
                        sqlite3_exec(db, "ROLLBACK;", nullptr, nullptr, nullptr); // �ع�����
                    }
                    else {
                        // �ύ����
                        rc = sqlite3_exec(db, "COMMIT;", nullptr, nullptr, nullptr);
                        if (rc != SQLITE_OK) {
                            std::cerr << "�����ύʧ��: " << sqlite3_errmsg(db) << std::endl;
                        }
                        else {
                            std::cout << "���� " << flightNumber << " Ԥ���ɹ���" << std::endl;
                        }
                    }
                }
            }
            else {
                std::cout << "���� " << flightNumber << " û��ʣ����λ��" << std::endl;
                sqlite3_exec(db, "ROLLBACK;", nullptr, nullptr, nullptr); // �ع�����
            }
        }
        else {
            std::cerr << "�Ҳ�������: " << flightNumber << std::endl;
            sqlite3_exec(db, "ROLLBACK;", nullptr, nullptr, nullptr); // �ع�����
        }
    }
    else {
        std::cerr << "��ѯ����ʧ��: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_exec(db, "ROLLBACK;", nullptr, nullptr, nullptr); // �ع�����
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
        std::cerr << "�����ݿ�ʧ��: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_close(db);
        return;
    }

    // ��ʼһ������
    rc = sqlite3_exec(db, "BEGIN TRANSACTION;", nullptr, nullptr, nullptr);
    if (rc != SQLITE_OK) {
        std::cerr << "��������ʧ��: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_close(db);
        return;
    }

    // ��麽���Ƿ����
    std::string checkFlightQuery = "SELECT FlightNumber, RemainingSeats FROM Flights WHERE FlightNumber = '" + flightNumber + "';";
    sqlite3_stmt* checkStmt;
    rc = sqlite3_prepare_v2(db, checkFlightQuery.c_str(), -1, &checkStmt, nullptr);
    if (rc == SQLITE_OK) {
        if (sqlite3_step(checkStmt) == SQLITE_ROW) {
            // ��ȡ�����ʣ����λ��
            int remainingSeats = sqlite3_column_int(checkStmt, 1);

            // ���º�����ʣ����λ��
            std::string updateSeatsQuery = "UPDATE Flights SET RemainingSeats = " + std::to_string(remainingSeats + 1) + " WHERE FlightNumber = '" + flightNumber + "';";
            rc = sqlite3_exec(db, updateSeatsQuery.c_str(), nullptr, nullptr, nullptr);
            if (rc != SQLITE_OK) {
                std::cerr << "���º���ʣ����λ��ʧ��: " << sqlite3_errmsg(db) << std::endl;
                sqlite3_exec(db, "ROLLBACK;", nullptr, nullptr, nullptr); // �ع�����
                sqlite3_finalize(checkStmt);
                sqlite3_close(db);
                return;
            }

            // �����û���Ķ��ĺ����
            std::string updateSubscribedFlightsQuery = "UPDATE User SET SubscribedFlights = REPLACE(SubscribedFlights, '" + flightNumber + ", ', '') WHERE UserID = " + userID + ";";
            rc = sqlite3_exec(db, updateSubscribedFlightsQuery.c_str(), nullptr, nullptr, nullptr);
            if (rc != SQLITE_OK) {
                std::cerr << "�����û����ĺ���ʧ��: " << sqlite3_errmsg(db) << std::endl;
                sqlite3_exec(db, "ROLLBACK;", nullptr, nullptr, nullptr); // �ع�����
            }
            else {
                // �ύ����
                rc = sqlite3_exec(db, "COMMIT;", nullptr, nullptr, nullptr);
                if (rc != SQLITE_OK) {
                    std::cerr << "�ύ����ʧ��: " << sqlite3_errmsg(db) << std::endl;
                }
                else {
                    std::cout << "���� " << flightNumber << " �˶��ɹ���" << std::endl;
                }
            }
        }
        else {
            std::cerr << "�Ҳ�������: " << flightNumber << std::endl;
            sqlite3_exec(db, "ROLLBACK;", nullptr, nullptr, nullptr); // �ع�����
        }
    }
    else {
        std::cerr << "��ѯ����ʧ��: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_exec(db, "ROLLBACK;", nullptr, nullptr, nullptr); // �ع�����
    }

    sqlite3_finalize(checkStmt);
    sqlite3_close(db);

    std::cout << "-----------------------------" << std::endl;
    std::cout << std::endl << std::endl;

    return;
}
