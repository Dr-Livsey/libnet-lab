#ifndef PROTO_H
#define PROTO_H

#include <QWidget>
#include <memory>

class LibnetHeader;
class LibnetIPv4Header;
class LibnetWrapper;

class Header
{
public:
    Header() = default;
    virtual ~Header() = default;

    virtual LibnetHeader* parse( const QString & ) = 0;
};

class IPv4Header : public QWidget
{
    Q_OBJECT
public:
    explicit IPv4Header(QWidget *parent = nullptr);

    LibnetIPv4Header* parse( std::unique_ptr<LibnetWrapper> &context );

    ~IPv4Header() override = default;
};


class TCPHeader : public QWidget, public Header
{
    Q_OBJECT
public:
    explicit TCPHeader(QWidget *parent = nullptr);

    LibnetHeader* parse( const QString & ) override;

    ~TCPHeader() override = default;
};

class UDPHeader : public QWidget, public Header
{
    Q_OBJECT
public:
    explicit UDPHeader(QWidget *parent = nullptr);

    LibnetHeader *parse( const QString & ) override;

    ~UDPHeader() override = default;
};

class ICMPHeader : public QWidget, public Header
{
    Q_OBJECT
public:
    explicit ICMPHeader(QWidget *parent = nullptr);

    LibnetHeader *parse( const QString & ) override;

    ~ICMPHeader() override = default;
};

#endif // PROTO_H
