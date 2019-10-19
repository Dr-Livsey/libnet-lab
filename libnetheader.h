#ifndef LIBNETHEADER_H
#define LIBNETHEADER_H

#include "libnetwrapper.h"

/*
 * Wrapper of Libnet header structs
 * Such as libnet_ipv4_hdr etc.
*/
class LibnetHeader
{
public:
    LibnetHeader() = default;
    virtual void build( LibnetWrapper & ) = 0;
    virtual ~LibnetHeader() = default;

    uint32_t get_size();
    uint32_t get_type();

protected:
    uint32_t type;
    uint32_t size_h;
};

struct LibnetIPv4Header
{
    LibnetIPv4Header(libnet_ipv4_hdr);
    void build( LibnetWrapper & , LibnetHeader *upper_layer);
    ~LibnetIPv4Header() = default;
private:
    LibnetIPv4Header();
    libnet_ipv4_hdr info;
};

struct LibnetTCPHeader : public LibnetHeader
{
    LibnetTCPHeader(libnet_tcp_hdr, const QString &payload);
    void build( LibnetWrapper & ) override;
    ~LibnetTCPHeader() override = default;
private:
    LibnetTCPHeader();
    libnet_tcp_hdr info;
    QString payload;
};

struct LibnetUDPHeader : public LibnetHeader
{
    LibnetUDPHeader(libnet_udp_hdr, const QString &payload);
    void build( LibnetWrapper & ) override;
    ~LibnetUDPHeader() override = default;
private:
    LibnetUDPHeader();
    libnet_udp_hdr info;
    QString payload;
};

struct LibnetICMPHeader : public LibnetHeader
{
    LibnetICMPHeader(LibnetWrapper::icmpv4_echo_hdr, const QString &payload);
    void build( LibnetWrapper & ) override;
    ~LibnetICMPHeader() override = default;
private:
    LibnetICMPHeader();
    LibnetWrapper::icmpv4_echo_hdr info;
    QString payload;
};


#endif // LIBNETHEADER_H
