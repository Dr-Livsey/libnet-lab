#ifndef LIBNETWRAPPER_H
#define LIBNETWRAPPER_H

#include <iostream>
#include <string>
#include <libnet.h>
#include <pcap/pcap.h>

#include <QVector>
#include <QString>

class TCPHeader;

class LibnetWrapperException : public std::runtime_error
{
public:
    LibnetWrapperException();
    LibnetWrapperException( const std::string &title, const std::string&msg );

    void show_msg() const;

    ~LibnetWrapperException() = default;
private:
    std::string title, msg;
};

class LibnetWrapper
{
public:
    LibnetWrapper( const char *dev_name );

    static void build_tcp_header(const libnet_tcp_hdr &, const QString &payload);
    static QVector<QString> get_alldevs( void );

    ~LibnetWrapper();
private:
    libnet_t *libnet_context;
    LibnetWrapper() {}
};

#endif // LIBNETWRAPPER_H
