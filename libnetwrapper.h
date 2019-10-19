#ifndef LIBNETWRAPPER_H
#define LIBNETWRAPPER_H

#include <iostream>
#include <string>
#include <libnet.h>
#include <pcap/pcap.h>

#include <QVector>
#include <QString>

class LibnetIPv4Header;
class LibnetHeader;

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

    struct icmpv4_echo_hdr
    {
        uint8_t     type;
        uint16_t    sum;
        uint16_t    seq;
    };

    LibnetWrapper( const char *dev_name );

    libnet_ptag_t   build_tcp_header(   const libnet_tcp_hdr  &, const QString &payload );
    libnet_ptag_t   build_udp_header(   const libnet_udp_hdr  &, const QString &payload );
    libnet_ptag_t   build_icmp_header(  const icmpv4_echo_hdr &, const QString &payload );
    libnet_ptag_t   build_ipv4_header(  const libnet_ipv4_hdr & );

    uint32_t        ipv4_str_to_addr(   const QString &addr );
    uint32_t        write( void );
    QString         last_error();
    QString         get_own_ipv4();
    void            clear_packet();
    void            build_packet(LibnetIPv4Header *, LibnetHeader *);

    static QVector<QString> get_alldevs( void );

    ~LibnetWrapper();

private:
    libnet_t *libnet_context;
    LibnetWrapper() {}
};

#endif // LIBNETWRAPPER_H
