#ifndef PROTO_H
#define PROTO_H

#include <QWidget>
#include "libnetwrapper.h"

class QGroupBox;

class Header
{
public:
    Header() = default;
    virtual ~Header() = default;

    /*Build header with libnet*/
    virtual void build( const QString &payload ) = 0;
};

class TCPHeader : public QWidget, public Header
{
    Q_OBJECT
public:
    explicit TCPHeader(QWidget *parent = nullptr);

    void build( const QString &payload ) override;

    ~TCPHeader() override = default;

private:
    libnet_tcp_hdr info;
    QString     payload;    /*Payload*/
};

class UDPHeader : public QWidget, public Header
{
    Q_OBJECT
public:
    explicit UDPHeader(QWidget *parent = nullptr);

    void build( const QString &payload ) override;

    ~UDPHeader() override = default;

signals:

public slots:
};

class ICMPHeader : public QWidget, public Header
{
    Q_OBJECT
public:
    explicit ICMPHeader(QWidget *parent = nullptr);

    void build( const QString &payload ) override;

    ~ICMPHeader() override = default;

signals:

public slots:
};

#endif // PROTO_H
