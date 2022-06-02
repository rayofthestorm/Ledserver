#include "ledserver.h"
#include "config.h"

LedServer::LedServer(quint16 port, QObject *parent) : QObject(parent), m_port(port)
{
    m_gpio = new Gpio(this);
    m_server = new QTcpServer(this);
    connect(m_server, &QTcpServer::newConnection, this, &LedServer::myNewConnection);
}

void LedServer::start()
{
    m_server->listen(QHostAddress::Any, PORT);
    qDebug() << "server lauscht auf port " << PORT;
}

// Client hat Verbindung zum Server aufgebaut
void LedServer::myNewConnection()
{
    m_socket = m_server->nextPendingConnection();
    qDebug() << "verbindung zum Client hergestellt";
    connect(m_socket, &QTcpSocket::disconnected, this, &LedServer::myClientDisconnect);
    connect(m_socket, &QTcpSocket::readyRead, this, &LedServer::myServerRead);
}

// Client hat Verbindung zum Server getrennt
void LedServer::myClientDisconnect()
{
    m_gpio->set(0);
    m_socket->disconnect();

    qDebug() << "Verbindung zum Client getrennt";
}

// Client hat eine Zahl (0...15) zum Server gesendet
void LedServer::myServerRead()
{
        QString UserNumber = m_socket->readAll();
        qDebug() << "Daten vom client: " << UserNumber;
        m_socket->write("Bitte geben Sie eine Zahl zwischen 0 und 15 ein (0 & 15 includiert) \n");

            if(UserNumber.toInt() <= 15 && UserNumber.toInt() >= 0)
            {
               qDebug() << "Daten ok liegen zwischen 0 und 15(0 & 15 includiert)";
               m_socket->write("Daten ok liegen zwischen 0 und 15(0 & 15 includiert)\n");
               int IntUserNumber = UserNumber.toInt();
               m_gpio->set(IntUserNumber);
            }

            else
            {
                qDebug() << "Daten Liegen auserhalb des Zulaessigen Bereichs!!";
                m_socket->write("Daten Liegen auserhalb des Zulaessigen Bereichs!!\n");


            }


}
