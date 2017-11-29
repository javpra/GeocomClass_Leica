#include "mytcpclient.h"
#include <iostream>


//static inline QByteArray IntToArray(qint32 source);

myTCPclient::myTCPclient(QObject *parent) : QObject(parent)
{
    socket = new QTcpSocket(this);
}

bool myTCPclient::connectToHost(QString _host, qint16 _port)
{
    socket->connectToHost(_host,_port);
    return socket->waitForConnected();
}

bool myTCPclient::writeData(QByteArray _dataWrited)
{
    if(socket->state()== QAbstractSocket::ConnectedState)
    {
        //socket->write(IntToArray(data.size()));
        qint64 size;
        size = socket->write(_dataWrited);
        //std::cout << "Bytes number written: " << dat << std::endl;
        return socket->waitForBytesWritten();
    }
    else
        return false;
}

bool myTCPclient::receiveData(char *_dataReceived)
{

    if(socket->state()== QAbstractSocket::ConnectedState)
    {
        qint64 size;
        socket->waitForReadyRead();
        size = socket->read(_dataReceived,1024);
        //std::cout << "Bytes received: " << size << std::endl;
        return true;
    }
    else
        return false;
}
