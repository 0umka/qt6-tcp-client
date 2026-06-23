#include "emulator.h"

namespace {

constexpr int kReconnectMs = 5000;
constexpr int kMinDelayMs = 10;
constexpr int kMaxDelayMs = 100;

/**
 * @brief makes json messages to the server
 * @return different info
 */
QJsonObject MakeData() {
    switch (QRandomGenerator::global()->bounded(3)) {
    case 0: return QJsonObject{{"type", "NetworkMetrics"},
                           {"bandwidth", 100.5 + QRandomGenerator::global()->generateDouble()},
                           {"latency", 12.3 + QRandomGenerator::global()->generateDouble()},
                           {"packet_loss", QRandomGenerator::global()->generateDouble()}};
    case 1: return QJsonObject{{"type", "DeviceStatus"},
                           {"uptime", QRandomGenerator::global()->bounded(86400)},
                           {"cpu_usage", QRandomGenerator::global()->bounded(101)},
                           {"memory_usage", QRandomGenerator::global()->bounded(101)}};
    default: {
        static const QStringList msgs = {
            "Ping OK",
            "User admin logged in from 192.168.1.45 at 14:32:01.",
            "CRITICAL: Multiple packet drops detected on interface eth1 over the last 5 minutes. "
            "Total dropped packets: 15432. Average latency spiked to 450ms. Recommended actions: "
            "check cable integrity, verify switch port configuration, and review QoS policies."
        };
        static const QStringList sev = {"DEBUG", "INFO", "WARNING", "ERROR", "CRITICAL"};
        return QJsonObject{{"type", "Log"},
                           {"severity", sev[QRandomGenerator::global()->bounded(sev.size())]},
                           {"message", msgs[QRandomGenerator::global()->bounded(msgs.size())]}};
    }
    }
}

}  // namespace

Emulator::Emulator(QObject* parent)
    : QObject{parent}
    , sock_(new QTcpSocket(this))
{
    connect(sock_, &QTcpSocket::connected, this, [this]() {
        std::cout << "Connected.\n";
        recon_.stop();
        Send();
    });
    connect(sock_, &QTcpSocket::disconnected, this, [this]() {
        std::cout << "Disconnected.\n";
        data_.stop();
        recon_.start();
    });
    connect(sock_, &QTcpSocket::errorOccurred, this, [this]() {
        if (sock_->state() != QTcpSocket::ConnectedState) recon_.start();
    });
    connect(&recon_, &QTimer::timeout, this, &Emulator::Connect);
    connect(&data_, &QTimer::timeout, this, &Emulator::Send);
    recon_.setInterval(kReconnectMs);
    data_.setSingleShot(true);
    Connect();
}
/**
 * @brief connects socket to the host
 */
void Emulator::Connect() {
    if (sock_->state() != QAbstractSocket::ConnectedState && sock_->state() != QAbstractSocket::ConnectingState) {
        std::cout << "Connecting to localhost:12345...\n";
        sock_->connectToHost("localhost", 12345);
    }
}

/**
 * @brief writes json data to the tcp socket
 */
void Emulator::Send() {
    if (sock_->state() != QAbstractSocket::ConnectedState)
        return;
    auto d = QJsonDocument(MakeData());
    auto b = d.toJson();
    sock_->write(b + "\n");
    std::cout << "Sent (" << b.size() << " bytes)\n";
    data_.start(QRandomGenerator::global()->bounded(kMinDelayMs, kMaxDelayMs + 1));
}
