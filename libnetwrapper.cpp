#include "proto.h"
#include "libnetheader.h"

#include <QMessageBox>
#include <QDebug>


LibnetWrapper::LibnetWrapper(const char *dev_name)
{
    char errbuf[LIBNET_ERRBUF_SIZE];
    libnet_context = libnet_init(LIBNET_RAW4_ADV, dev_name, errbuf);

    if (NULL == libnet_context){
        throw LibnetWrapperException("libnet_init()", std::string(errbuf));
    }
    else{
        LibnetWrapperException("libnet_init()",
                               std::string("Successfully bind to \"") + dev_name + "\" device!").show_msg();
    }
}

libnet_ptag_t
LibnetWrapper::build_tcp_header(const libnet_tcp_hdr &t_hdr, const QString &payload_str )
{
    char*    c_payload  = const_cast<char*>(payload_str.toStdString().c_str());
    uint8_t* payload    = reinterpret_cast<uint8_t*>(c_payload);
    uint32_t payload_s  = static_cast<uint32_t>(payload_str.size());

    libnet_ptag_t pack_tag;

    pack_tag = libnet_build_tcp(
                    t_hdr.th_sport,
                    t_hdr.th_dport,
                    t_hdr.th_seq,
                    t_hdr.th_ack,
                    t_hdr.th_flags,
                    t_hdr.th_win,
                    t_hdr.th_sum,
                    t_hdr.th_urp,
                    LIBNET_TCP_H,
                    payload,
                    payload_s,
                    libnet_context,
                    0);

    if ( -1 == pack_tag )
    {
        throw LibnetWrapperException("build_tcp_header()", last_error().toStdString());
    }

    return pack_tag;
}

libnet_ptag_t LibnetWrapper::build_udp_header(const libnet_udp_hdr &udp_hdr, const QString &payload_str)
{
    char*    c_payload  = const_cast<char*>(payload_str.toStdString().c_str());
    uint8_t* payload    = reinterpret_cast<uint8_t*>(c_payload);
    uint32_t payload_s  = static_cast<uint32_t>(payload_str.size());

    libnet_ptag_t pack_tag;

    pack_tag = libnet_build_udp(udp_hdr.uh_sport,
                                udp_hdr.uh_dport,
                                LIBNET_UDP_H,
                                udp_hdr.uh_sum,
                                payload,
                                payload_s,
                                libnet_context,
                                0);

    if ( -1 == pack_tag )
    {
        throw LibnetWrapperException("build_udp_header()", last_error().toStdString());
    }

    return pack_tag;
}

libnet_ptag_t LibnetWrapper::build_icmp_header(const icmpv4_echo_hdr &icmp_hdr, const QString &payload_str)
{
    char*    c_payload  = const_cast<char*>(payload_str.toStdString().c_str());
    uint8_t* payload    = reinterpret_cast<uint8_t*>(c_payload);
    uint32_t payload_s  = static_cast<uint32_t>(payload_str.size());

    libnet_ptag_t pack_tag;

    pack_tag = libnet_build_icmpv4_echo(icmp_hdr.type,
                                        0,
                                        icmp_hdr.sum,
                                        0,
                                        icmp_hdr.seq,
                                        payload,
                                        payload_s,
                                        libnet_context,
                                        0);

    if ( -1 == pack_tag )
    {
        throw LibnetWrapperException("build_icmp_header()", last_error().toStdString());
    }

    return pack_tag;
}

libnet_ptag_t LibnetWrapper::build_ipv4_header(const libnet_ipv4_hdr &ipv4_hdr)
{
    libnet_ptag_t pack_tag;

    pack_tag = libnet_build_ipv4(
                    ipv4_hdr.ip_len,
                    ipv4_hdr.ip_tos,
                    ipv4_hdr.ip_id,
                    ipv4_hdr.ip_off,
                    ipv4_hdr.ip_ttl,
                    ipv4_hdr.ip_p,
                    ipv4_hdr.ip_sum,
                    ipv4_hdr.ip_src.s_addr,
                    ipv4_hdr.ip_dst.s_addr,
                    NULL,
                    0,
                    libnet_context,
                    0);

    if ( -1 == pack_tag )
    {
        throw LibnetWrapperException("build_ipv4_header()", last_error().toStdString());
    }

    return pack_tag;
}

uint32_t LibnetWrapper::ipv4_str_to_addr(const QString &addr)
{
    uint32_t ip_addr = libnet_name2addr4(libnet_context,
                                         addr.toStdString().c_str(),
                                         LIBNET_DONT_RESOLVE);

    if ( static_cast<uint32_t>(-1) == ip_addr )
    {
        QString error = "Error while converting \"" + addr + "\" to IPv4 address.";
        std::string descr = "\nDescription: " + last_error().toStdString();
        throw LibnetWrapperException("ipv4_str_to_addr()", error.toStdString() + descr);
    }

    return ip_addr;
}

uint32_t LibnetWrapper::write()
{
    int bytes_written = libnet_write(libnet_context);

    if ( -1 == bytes_written )
    {
        throw LibnetWrapperException("libnet_write()", last_error().toStdString());
    }

    return static_cast<uint32_t>(bytes_written);
}

QString LibnetWrapper::last_error()
{
    return QString::fromStdString(libnet_geterror(libnet_context));
}

QString LibnetWrapper::get_own_ipv4()
{
    uint32_t ipv4_addr = libnet_get_ipaddr4(libnet_context);

    if ( static_cast<uint32_t>(-1) == ipv4_addr )
        throw LibnetWrapperException("get_own_ipv4()", last_error().toStdString());
    else
    {
        char *c_ipv4 = libnet_addr2name4(ipv4_addr, LIBNET_DONT_RESOLVE);

        if ( NULL == c_ipv4 )
            throw LibnetWrapperException("get_own_ipv4()", last_error().toStdString());
        else
        {
            return QString::fromStdString(c_ipv4);
        }
    }

    return QString();
}

void LibnetWrapper::clear_packet()
{
    libnet_clear_packet(libnet_context);
}

void LibnetWrapper::build_packet(LibnetIPv4Header *ipv4_hdr, LibnetHeader *upper_layer_hdr)
{
    /*Build transport layer*/
    upper_layer_hdr->build(*this);

    /*Build network layer*/
    ipv4_hdr->build(*this, upper_layer_hdr);
}

QVector<QString>
LibnetWrapper::get_alldevs()
{
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_if_t *alldevsp;
    QVector<QString> devs;

    // Get all free devices
    if (PCAP_ERROR == pcap_findalldevs(&alldevsp, errbuf))
    {
        throw LibnetWrapperException("pcap_findalldevs()", std::string(errbuf));
    }
    else
    {
        for (pcap_if_t *it = alldevsp; it != NULL; it = it->next)
        {
            devs.push_back(QString::fromStdString(std::string(it->name)));
        }

        if (alldevsp != NULL)
            pcap_freealldevs(alldevsp);
    }

    return devs;
}

LibnetWrapper::~LibnetWrapper()
{
    if (libnet_context != NULL)
    {
        libnet_destroy(libnet_context);
    }
}

LibnetWrapperException::LibnetWrapperException() :
    std::runtime_error("") {}

LibnetWrapperException::LibnetWrapperException( const std::string &t, const std::string &m) :
    std::runtime_error(t + m), title(t), msg(m) {}

void LibnetWrapperException::show_msg() const
{
    QMessageBox::about(nullptr, QString::fromStdString(title), QString::fromStdString(msg));
}
