#include <iostream>
#include "sqlite3.h"

int main() {
    sqlite3* db;
    std::string dbPath = "./flights.db";

    int result = sqlite3_open(dbPath.c_str(), &db);
    if (result != SQLITE_OK) {
        std::cerr << "���ݿ��ʧ��: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_close(db);
        return false;
    }

    // �����������ݵ� SQL ���
    std::string insertQuery = "INSERT INTO Flights (FlightNumber, DepartureAirport, ArrivalAirport, DepartureTime, ArrivalTime, Airline, RemainingSeats) VALUES "
        "('111', 'Departure Airport', 'Arrival Airport', '2023-06-10 22:00', '2023-06-11 05:00', 'Airline', 100);";

    // ִ�в������
    result = sqlite3_exec(db, insertQuery.c_str(), nullptr, nullptr, nullptr);
    if (result != SQLITE_OK) {
        std::cerr << "����ʧ��: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_close(db);
        return false;
    }

    std::cout << "����ɹ�" << std::endl;

    // �ر����ݿ�����
    sqlite3_close(db);
    return 0;
}