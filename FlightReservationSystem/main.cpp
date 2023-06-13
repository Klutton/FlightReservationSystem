#include <iostream>
#include <fstream>
#include "sqlite3.h"
#include "dashBoard.h"
#include "fileIo.h"
#include "flightManage.h"

// ȫ�ֱ���
 // ���ݿ��ַ
std::string g_dbDirectory;

void clearScreen()
{
    std::cout << "\033[2J\033[1;1H"; // ANSI ת�����У������Ļ��������ƶ�����һ�е�һ��
}

bool startup() {

	// ��ӭ
	Dashboard::printWelcomeMessage();

	// ��ȡ�����ļ������û�У�ѯ���Ƿ񴴽�Ĭ�������ļ�
	std::string db_directory = FILEIO_H::readConfig();

	// Ϊ�մ����ȡʧ��
	if (db_directory.empty()) {
		Dashboard::printConfigFileError();

        // ����ѡ��
		char s;
        bool cs;
        bool isValidOption = false;
        while (!isValidOption) {
            std::cin >> s;
            switch (s) {
            case '1':
                // �����½�һ��Ĭ�������ļ�
                cs = FILEIO_H::createDefaultConfigFile();
                 // ����ʧ��
                if (!cs) return false;
                // Ĭ�ϵ����ݿ��ļ���ַ
                g_dbDirectory = "./flights.db";
                isValidOption = true;
                break;
            case '2':
                // �˳�
                return false;
                break;
            default:
                // ��Чѡ���ʾ������Ϣ��������ѭ��
                std::cout << "��Чѡ����������롣\n";
                break;
            }
        }
    }
    else {
        // ��ֵ
        g_dbDirectory = db_directory;
    }


	// ��ʼ�����ݿ⣬���û�У�ѯ���Ƿ��½�
    bool s = checkTable(g_dbDirectory);
    if (!s) {
        Dashboard::printDatabaseConnectionFailed();

        // ����ѡ��
        char s;
        bool cs;
        bool isValidOption = false;
        while (!isValidOption) {
            std::cin >> s;
            switch (s) {
            case '1':
                // �����½�һ�������ݿ�
                cs = FILEIO_H::createDefaultDatabase(g_dbDirectory);
                // ����ʧ��
                if (!cs) return false;
                isValidOption = true;
                break;
            case '2':
                // �˳�
                return false;
                break;
            default:
                // ��Чѡ���ʾ������Ϣ��������ѭ��
                std::cout << "��Чѡ����������롣\n";
                break;
            }
        }
    }
    return true;
}

int main() {
    bool startUpStat = startup();
    if (!startUpStat) return 0;

    clearScreen();

    char s, ss;
    bool isValidOption;
    std::string userID;
    std::string number;
    while (true) {

        // ��ӡ�û����
        Dashboard::printMenu();
        std::cin >> s;
        switch (s) {


        case '0':
            // �˳�
            return 0;
            break;


        case '1':
            // ¼�뺽����Ϣ
            F_MANAGE_H::insertFlight(g_dbDirectory);
            break;


        case '2':
            // ���������Ϣ
            Dashboard::printBrowseSelection();
            isValidOption = false;
            while (!isValidOption) {
                std::cin >> ss;
                switch (ss) {
                case '1':
                    // 24Сʱ�ں���
                    F_MANAGE_H::browseFlights24(g_dbDirectory);
                    isValidOption = true;
                    break;
                case '2':
                    // ���к���
                    F_MANAGE_H::browseFlights(g_dbDirectory);
                    isValidOption = true;
                    break;
                case '0':
                    // Ŀ�ĵز�ѯ
                    isValidOption = true;
                    break;
                default:
                    // ��Чѡ���ʾ������Ϣ��������ѭ��
                    std::cout << "��Чѡ����������롣\n";
                    break;
                }
            }

            break;


        case '3':
            // ���߲�ѯ
            Dashboard::printSearchSelection();
            isValidOption = false;
            while (!isValidOption) {
                std::cin >> ss;
                switch (ss) {
                case '1':
                    // ����Ų�ѯ
                    F_MANAGE_H::searchFlightViaNumber(g_dbDirectory);
                    isValidOption = true;
                    break;
                case '2':
                    // Ŀ�ĵز�ѯ
                    F_MANAGE_H::searchFlightViaDst(g_dbDirectory);
                    isValidOption = true;
                    break;
                case '0':
                    // ȡ��
                    isValidOption = true;
                    break;
                default:
                    // ��Чѡ���ʾ������Ϣ��������ѭ��
                    std::cout << "��Чѡ����������롣\n";
                    break;
                }
            }

            break;


        case '4':
            // ��Ʊ��Ʊҵ��
            std::cout << "��������ID" << std::endl;
            std::cin >> userID;
            isValidOption = false;
            while (!isValidOption) {
                Dashboard::printReserveSelection();
                std::cin >> ss;
                switch (ss) {
                case '1':
                    // ��ѯ����
                    F_MANAGE_H::searchUserFlights(g_dbDirectory, userID);
                    isValidOption = true;
                    break;
                case '2':
                    // Ԥ��
                    std::cout << "���뺽���" << std::endl;
                    std::cin >> number;
                    F_MANAGE_H::bookFlight(g_dbDirectory, number, userID);
                    isValidOption = true;
                    break;
                case '3':
                    // �˶�
                    std::cout << "���뺽���" << std::endl;
                    std::cin >> number;
                    F_MANAGE_H::cancelBooking(g_dbDirectory, number, userID);
                    isValidOption = true;
                    break;
                case '0':
                    // ȡ��
                    isValidOption = true;
                    break;
                default:
                    // ��Чѡ���ʾ������Ϣ��������ѭ��
                    std::cout << "��Чѡ����������롣\n";
                    break;
                }
            }

            break;
        default:
            // ��Чѡ���ʾ������Ϣ��������ѭ��
            std::cout << "��Чѡ����������롣\n";
            break;
        }
    }
}