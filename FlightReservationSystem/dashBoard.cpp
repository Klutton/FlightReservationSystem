#include <iostream>
#include "dashBoard.h"

void Dashboard::printWelcomeMessage() {
    std::cout << "======================================" << std::endl;
    std::cout << "��ӭʹ�÷ɻ���Ʊϵͳ��" << std::endl;
    std::cout << "======================================" << std::endl;
    std::cout << std::endl;
}

void Dashboard::printConfigFileError() {
    std::cout << "δ�ҵ������ļ��������ļ��𻵣������ļ�λ�ڣ�" << std::endl;
    std::cout << "  .\\config.json" << std::endl;
    std::cout << "ѡ����ʽ" << std::endl;
    std::cout << "1. �Զ�����һ�������ļ�" << std::endl;
    std::cout << "2. �˳�����" << std::endl;
    std::cout << "��ѡ�����: ";
}

void Dashboard::printDatabaseConnectionFailed() {
    std::cout << "ѡ����ʽ" << std::endl;
    std::cout << "1. �������ļ�ָ���ĵ�ַ�´���һ���յ����ݿ�" << std::endl;
    std::cout << "2. �˳�����" << std::endl;
    std::cout << "��ѡ�����: ";
}

void Dashboard::printSearchSelection() {
    std::cout << "ѡ���ѯ��ʽ" << std::endl;
    std::cout << "1. ͨ�������" << std::endl;
    std::cout << "2. ͨ��Ŀ�ĵ�" << std::endl;
    std::cout << "0. ȡ��" << std::endl;
    std::cout << "��ѡ�����: ";
}

void Dashboard::printBrowseSelection() {
    std::cout << "ѡ���ѯ����" << std::endl;
    std::cout << "1. 24Сʱ����" << std::endl;
    std::cout << "2. ȫ��" << std::endl;
    std::cout << "0. ȡ��" << std::endl;
    std::cout << "��ѡ�����: ";
}

void Dashboard::printReserveSelection() {
    std::cout << "ѡ��ҵ��" << std::endl;
    std::cout << "1. ��ѯ����" << std::endl;
    std::cout << "2. Ԥ������" << std::endl;
    std::cout << "3. �˶�����" << std::endl;
    std::cout << "0. ȡ��" << std::endl;
    std::cout << "��ѡ�����: ";
}

void Dashboard::printMenu() {
    std::cout << "========= �ɻ���Ʊϵͳ =========" << std::endl;
    std::cout << "1. ¼�뺽����Ϣ" << std::endl;
    std::cout << "2. ���������Ϣ" << std::endl;
    std::cout << "3. ���߲�ѯ" << std::endl;
    std::cout << "4. ��Ʊ��Ʊҵ��" << std::endl;
    std::cout << "0. �˳�ϵͳ" << std::endl;
    std::cout << "===============================" << std::endl;
    std::cout << "�������Ӧ������ѡ����: ";
}
