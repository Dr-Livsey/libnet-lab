#ifndef PROTO_H
#define PROTO_H

#include <QWidget>
#include <memory>
#include "libnetwrapper.h"

class QGroupBox;

class TransportHeader
{
public:
    TransportHeader() = default;
    virtual ~TransportHeader() = default;

    /*Build header with libnet*/
    virtual void build( std::unique_ptr<LibnetWrapper> &context, const QString &payload ) = 0;
    virtual void clear( void ) = 0;

    uint32_t get_size();
    uint32_t get_type();

protected:
    uint32_t type;
    uint32_t size_h;
    QString  payload;
};

class IPv4Header : public QWidget
{
    Q_OBJECT
public:
    explicit IPv4Header(QWidget *parent = nullptr);

    void build( std::unique_ptr<LibnetWrapper> &context, TransportHeader *transport_header );
    void clear( void );

    ~IPv4Header() override = default;

private:
    libnet_ipv4_hdr info;
};


class TCPHeader : public QWidget, public TransportHeader
{
    Q_OBJECT
public:
    explicit TCPHeader(QWidget *parent = nullptr);

    void build( std::unique_ptr<LibnetWrapper> &context, const QString &payload ) override;
    void clear( void ) override;

    ~TCPHeader() override = default;

private:
    libnet_tcp_hdr info;
};

class UDPHeader : public QWidget, public TransportHeader
{
    Q_OBJECT
public:
    explicit UDPHeader(QWidget *parent = nullptr);

    void build( std::unique_ptr<LibnetWrapper> &context, const QString &payload ) override;
    void clear( void ) override;

    ~UDPHeader() override = default;

private:
    libnet_udp_hdr info;
};

class ICMPHeader : public QWidget, public TransportHeader
{
    Q_OBJECT
public:
    explicit ICMPHeader(QWidget *parent = nullptr);

    void build( std::unique_ptr<LibnetWrapper> &context, const QString &payload ) override;
    void clear( void ) override;

    ~ICMPHeader() override = default;

private:
    LibnetWrapper::icmpv4_echo_hdr info;
};

#endif // PROTO_H
