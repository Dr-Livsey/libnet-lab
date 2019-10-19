#include "libnetheader.h"

LibnetIPv4Header::LibnetIPv4Header(libnet_ipv4_hdr init) : info(init)
{
    /**/
}

LibnetTCPHeader::LibnetTCPHeader(libnet_tcp_hdr init, const QString &_payload)
    : LibnetHeader(), info(init), payload(_payload)
{
    type    = IPPROTO_TCP;
    size_h  = LIBNET_TCP_H;
}

LibnetUDPHeader::LibnetUDPHeader(libnet_udp_hdr init, const QString &_payload)
    : LibnetHeader(), info(init), payload(_payload)
{
    type    = IPPROTO_UDP;
    size_h  = LIBNET_UDP_H;
}

LibnetICMPHeader::LibnetICMPHeader(LibnetWrapper::icmpv4_echo_hdr init, const QString &_payload)
    : LibnetHeader(), info(init), payload(_payload)
{
    type    = IPPROTO_ICMP;
    size_h  = LIBNET_ICMPV4_ECHO_H;
}

void LibnetICMPHeader::build(LibnetWrapper &context)
{
    context.build_icmp_header(info, payload);
}

void LibnetIPv4Header::build(LibnetWrapper &context, LibnetHeader *upper)
{
    uint16_t th_len  = static_cast<uint16_t>(upper->get_size());
    uint8_t  th_type = static_cast<uint8_t>(upper->get_type());

    info.ip_len = LIBNET_IPV4_H + th_len;
    info.ip_p   = th_type;

    context.build_ipv4_header(info);
}

void LibnetTCPHeader::build(LibnetWrapper &context)
{
    context.build_tcp_header(info, payload);
}

void LibnetUDPHeader::build(LibnetWrapper &context)
{
    context.build_udp_header(info, payload);
}

uint32_t LibnetHeader::get_size()
{
    return size_h;
}

uint32_t LibnetHeader::get_type()
{
    return type;
}
