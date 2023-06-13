#include <iostream>
#include "dashBoard.h"

void Dashboard::printWelcomeMessage() {
    std::cout << "======================================" << std::endl;
    std::cout << "欢迎使用飞机订票系统！" << std::endl;
    std::cout << "======================================" << std::endl;
    std::cout << std::endl;
}

void Dashboard::printConfigFileError() {
    std::cout << "未找到配置文件或配置文件损坏，配置文件位于：" << std::endl;
    std::cout << "  .\\config.json" << std::endl;
    std::cout << "选择处理方式" << std::endl;
    std::cout << "1. 自动创建一个配置文件" << std::endl;
    std::cout << "2. 退出程序" << std::endl;
    std::cout << "请选择操作: ";
}

void Dashboard::printDatabaseConnectionFailed() {
    std::cout << "选择处理方式" << std::endl;
    std::cout << "1. 在配置文件指定的地址下创建一个空的数据库" << std::endl;
    std::cout << "2. 退出程序" << std::endl;
    std::cout << "请选择操作: ";
}

void Dashboard::printSearchSelection() {
    std::cout << "选择查询方式" << std::endl;
    std::cout << "1. 通过航班号" << std::endl;
    std::cout << "2. 通过目的地" << std::endl;
    std::cout << "0. 取消" << std::endl;
    std::cout << "请选择操作: ";
}

void Dashboard::printBrowseSelection() {
    std::cout << "选择查询内容" << std::endl;
    std::cout << "1. 24小时以内" << std::endl;
    std::cout << "2. 全部" << std::endl;
    std::cout << "0. 取消" << std::endl;
    std::cout << "请选择操作: ";
}

void Dashboard::printReserveSelection() {
    std::cout << "选择业务" << std::endl;
    std::cout << "1. 查询订单" << std::endl;
    std::cout << "2. 预订航班" << std::endl;
    std::cout << "3. 退订航班" << std::endl;
    std::cout << "0. 取消" << std::endl;
    std::cout << "请选择操作: ";
}

void Dashboard::printMenu() {
    std::cout << "========= 飞机订票系统 =========" << std::endl;
    std::cout << "1. 录入航班信息" << std::endl;
    std::cout << "2. 浏览航班信息" << std::endl;
    std::cout << "3. 航线查询" << std::endl;
    std::cout << "4. 订票退票业务" << std::endl;
    std::cout << "0. 退出系统" << std::endl;
    std::cout << "===============================" << std::endl;
    std::cout << "请输入对应的数字选择功能: ";
}
