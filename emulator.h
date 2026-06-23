#ifndef EMULATOR_H
#define EMULATOR_H

#include <QObject>

#include <QJsonDocument>
#include <QJsonObject>
#include <QRandomGenerator>
#include <QTcpSocket>
#include <QTimer>
#include <iostream>

class Emulator : public QObject {
    Q_OBJECT
public:
    explicit Emulator(QObject* parent = nullptr);

private slots:
    void Connect();
    void Send();

private:
    QTcpSocket* sock_;
    QTimer recon_, data_;
};

#endif // EMULATOR_H

