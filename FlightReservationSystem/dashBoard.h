#ifndef DASHBOARD_H
#define DASHBOARD_H

class Dashboard {
public:
    static void printWelcomeMessage();
    static void printConfigFileError();
    static void printDatabaseConnectionFailed();
    static void printBrowseSelection();
    static void printSearchSelection();
    static void printReserveSelection();
    static void printMenu();
};

#endif // DASHBOARD_H
