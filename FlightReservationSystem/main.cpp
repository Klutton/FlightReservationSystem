#include <iostream>
#include <fstream>
#include "sqlite3.h"
#include "dashBoard.h"
#include "fileIo.h"
#include "flightManage.h"

// 全局变量
 // 数据库地址
std::string g_dbDirectory;

void clearScreen()
{
    std::cout << "\033[2J\033[1;1H"; // ANSI 转义序列，清除屏幕并将光标移动到第一行第一列
}

bool startup() {

	// 欢迎
	Dashboard::printWelcomeMessage();

	// 读取配置文件，如果没有，询问是否创建默认配置文件
	std::string db_directory = FILEIO_H::readConfig();

	// 为空代表读取失败
	if (db_directory.empty()) {
		Dashboard::printConfigFileError();

        // 输入选项
		char s;
        bool cs;
        bool isValidOption = false;
        while (!isValidOption) {
            std::cin >> s;
            switch (s) {
            case '1':
                // 尝试新建一个默认配置文件
                cs = FILEIO_H::createDefaultConfigFile();
                 // 创建失败
                if (!cs) return false;
                // 默认的数据库文件地址
                g_dbDirectory = "./flights.db";
                isValidOption = true;
                break;
            case '2':
                // 退出
                return false;
                break;
            default:
                // 无效选项，显示错误信息，并继续循环
                std::cout << "无效选项，请重新输入。\n";
                break;
            }
        }
    }
    else {
        // 赋值
        g_dbDirectory = db_directory;
    }


	// 初始化数据库，如果没有，询问是否新建
    bool s = checkTable(g_dbDirectory);
    if (!s) {
        Dashboard::printDatabaseConnectionFailed();

        // 输入选项
        char s;
        bool cs;
        bool isValidOption = false;
        while (!isValidOption) {
            std::cin >> s;
            switch (s) {
            case '1':
                // 尝试新建一个空数据库
                cs = FILEIO_H::createDefaultDatabase(g_dbDirectory);
                // 创建失败
                if (!cs) return false;
                isValidOption = true;
                break;
            case '2':
                // 退出
                return false;
                break;
            default:
                // 无效选项，显示错误信息，并继续循环
                std::cout << "无效选项，请重新输入。\n";
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

        // 打印用户面板
        Dashboard::printMenu();
        std::cin >> s;
        switch (s) {


        case '0':
            // 退出
            return 0;
            break;


        case '1':
            // 录入航班信息
            F_MANAGE_H::insertFlight(g_dbDirectory);
            break;


        case '2':
            // 浏览航班信息
            Dashboard::printBrowseSelection();
            isValidOption = false;
            while (!isValidOption) {
                std::cin >> ss;
                switch (ss) {
                case '1':
                    // 24小时内航班
                    F_MANAGE_H::browseFlights24(g_dbDirectory);
                    isValidOption = true;
                    break;
                case '2':
                    // 所有航班
                    F_MANAGE_H::browseFlights(g_dbDirectory);
                    isValidOption = true;
                    break;
                case '0':
                    // 目的地查询
                    isValidOption = true;
                    break;
                default:
                    // 无效选项，显示错误信息，并继续循环
                    std::cout << "无效选项，请重新输入。\n";
                    break;
                }
            }

            break;


        case '3':
            // 航线查询
            Dashboard::printSearchSelection();
            isValidOption = false;
            while (!isValidOption) {
                std::cin >> ss;
                switch (ss) {
                case '1':
                    // 航班号查询
                    F_MANAGE_H::searchFlightViaNumber(g_dbDirectory);
                    isValidOption = true;
                    break;
                case '2':
                    // 目的地查询
                    F_MANAGE_H::searchFlightViaDst(g_dbDirectory);
                    isValidOption = true;
                    break;
                case '0':
                    // 取消
                    isValidOption = true;
                    break;
                default:
                    // 无效选项，显示错误信息，并继续循环
                    std::cout << "无效选项，请重新输入。\n";
                    break;
                }
            }

            break;


        case '4':
            // 订票退票业务
            std::cout << "输入您的ID" << std::endl;
            std::cin >> userID;
            isValidOption = false;
            while (!isValidOption) {
                Dashboard::printReserveSelection();
                std::cin >> ss;
                switch (ss) {
                case '1':
                    // 查询订单
                    F_MANAGE_H::searchUserFlights(g_dbDirectory, userID);
                    isValidOption = true;
                    break;
                case '2':
                    // 预订
                    std::cout << "输入航班号" << std::endl;
                    std::cin >> number;
                    F_MANAGE_H::bookFlight(g_dbDirectory, number, userID);
                    isValidOption = true;
                    break;
                case '3':
                    // 退订
                    std::cout << "输入航班号" << std::endl;
                    std::cin >> number;
                    F_MANAGE_H::cancelBooking(g_dbDirectory, number, userID);
                    isValidOption = true;
                    break;
                case '0':
                    // 取消
                    isValidOption = true;
                    break;
                default:
                    // 无效选项，显示错误信息，并继续循环
                    std::cout << "无效选项，请重新输入。\n";
                    break;
                }
            }

            break;
        default:
            // 无效选项，显示错误信息，并继续循环
            std::cout << "无效选项，请重新输入。\n";
            break;
        }
    }
}