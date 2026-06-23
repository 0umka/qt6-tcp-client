#include <QCoreApplication>
#include "emulator.h"
#include <iostream>

int main(int argc, char* argv[]) {
    QCoreApplication app(argc, argv);

    std::cout << "Clients count: ";
    int clients = 0;
    if (!(std::cin >> clients) || clients < 1) {
        std::cerr << "Clients count must be positive.\n";
        return 1;
    }

    for (int i = 0; i < clients; ++i) {
        new Emulator(&app);
    }

    return app.exec();
}
