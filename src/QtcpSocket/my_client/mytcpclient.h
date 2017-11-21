#ifndef MYTCPCLIENT_H
#define MYTCPCLIENT_H

#include <QtNetwork>
#include <QtCore>

class myTCPclient : public QObject
{
public:
    explicit myTCPclient(QObject *parent=0);
public slots:
    bool connectToHost(QString _host, qint16 _port);
    bool writeData(QByteArray _dataWrited);
    bool receiveData(char *_dataReceived);

private:
    QTcpSocket *socket;
};

#endif // MYTCPCLIENT_H
