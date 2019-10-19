#include <QDebug>
#include <QLineEdit>
#include <QCheckBox>
#include <QGroupBox>
#include <QRadioButton>
#include <iostream>

#include "libnetheader.h"
#include "proto.h"

TCPHeader::TCPHeader(QWidget *parent) : QWidget(parent), Header()
{
    /**/
}

UDPHeader::UDPHeader(QWidget *parent) : QWidget(parent), Header()
{
    /**/
}

ICMPHeader::ICMPHeader(QWidget *parent) : QWidget(parent), Header()
{
    /**/
}

IPv4Header::IPv4Header(QWidget *parent) : QWidget(parent)
{
    /**/
}

LibnetHeader*
TCPHeader::parse( const QString &payload )
{
    libnet_tcp_hdr info;

    try
    {
        //Set fields before build
        for ( QLineEdit *field : this->findChildren<QLineEdit*>())
        {
            if  ( field->objectName() == "src_port_line" )
                info.th_sport   =   static_cast<uint16_t>(std::stoul(field->text().toStdString()));
            else if ( field->objectName() == "dst_port_line" )
                info.th_dport   =   static_cast<uint16_t>(std::stoul(field->text().toStdString()));
            else if ( field->objectName() == "seq_line" )
                info.th_seq     =   static_cast<uint32_t>(std::stoul(field->text().toStdString()));
            else if ( field->objectName() == "ack_line" )
                info.th_ack     =   static_cast<uint32_t>(std::stoul(field->text().toStdString()));
            else if ( field->objectName() == "wind_size_line" )
                info.th_win     =   static_cast<uint16_t>(std::stoul(field->text().toStdString()));
            else if ( field->objectName() == "checksum_line_tcp" )
                info.th_sum     =   static_cast<uint16_t>(std::stoul(field->text().toStdString()));
            else if ( field->objectName() == "urg_ptr_line" )
                info.th_urp     =   static_cast<uint16_t>(std::stoul(field->text().toStdString()));
        }

        // Set control flags
        QGroupBox *tcp_flags = this->findChild<QGroupBox*>();
        for ( QCheckBox *flag : tcp_flags->findChildren<QCheckBox*>())
        {
            if (flag->isChecked() == false)
                continue;

            if ( flag->objectName() == "cwr")
                info.th_flags |= TH_CWR;
            else if ( flag->objectName() == "ece" )
                info.th_flags |= TH_ECE;
            else if ( flag->objectName() == "urg" )
                info.th_flags |= TH_URG;
            else if ( flag->objectName() == "ack" )
                info.th_flags |= TH_ACK;
            else if ( flag->objectName() == "psh" )
                info.th_flags |= TH_PUSH;
            else if ( flag->objectName() == "rst" )
                info.th_flags |= TH_RST;
            else if ( flag->objectName() == "syn" )
                info.th_flags |= TH_SYN;
            else if ( flag->objectName() == "fin" )
                info.th_flags |= TH_FIN;
        }
    }
    catch (const std::exception &ex)
    {
        throw LibnetWrapperException("TCP Fields cast", ex.what());
    }

    return new LibnetTCPHeader(info, payload);
}

LibnetHeader*
UDPHeader::parse( const QString &payload )
{
    libnet_udp_hdr info;

    try
    {
        //Set fields before build
        for ( QLineEdit *field : this->findChildren<QLineEdit*>())
        {
            if  ( field->objectName() == "src_port_udp_line" )
                info.uh_sport   =   static_cast<uint16_t>(std::stoul(field->text().toStdString()));
            else if ( field->objectName() == "dst_port_udp_line" )
                info.uh_dport   =   static_cast<uint16_t>(std::stoul(field->text().toStdString()));
            else if ( field->objectName() == "checksum_udp_line" )
                info.uh_sum     =   static_cast<uint16_t>(std::stoul(field->text().toStdString()));
        }
    }
    catch (const std::exception &ex)
    {
        throw LibnetWrapperException("UDP Fields cast", ex.what());
    }

    return new LibnetUDPHeader(info, payload);
}

LibnetHeader*
ICMPHeader::parse( const QString &payload )
{
    LibnetWrapper::icmpv4_echo_hdr info;

    try
    {
        //Set fields before build
        for ( QLineEdit *field : this->findChildren<QLineEdit*>())
        {
            if  ( field->objectName() == "checksum_icmp_line" )
                info.sum   =   static_cast<uint16_t>(std::stoul(field->text().toStdString()));
            else if ( field->objectName() == "seq_icmp_line" )
                info.seq   =   static_cast<uint16_t>(std::stoul(field->text().toStdString()));
        }

        QGroupBox *icmp_type_box = this->findChild<QGroupBox*>();
        for ( QRadioButton *icmp_type : icmp_type_box->findChildren<QRadioButton*>())
        {
            if (icmp_type->isChecked() == false)
                continue;

            if ( icmp_type->objectName() == "request_radio_b")
                info.type = ICMP_ECHO;
            else if ( icmp_type->objectName() == "reply_radio_b")
                info.type = ICMP_ECHOREPLY;
        }
    }
    catch (const std::exception &ex)
    {
        throw LibnetWrapperException("ICMP Fields cast", ex.what());
    }

    return new LibnetICMPHeader(info, payload);
}

LibnetIPv4Header*
IPv4Header::parse( std::unique_ptr<LibnetWrapper> &context )
{
    libnet_ipv4_hdr info;

    info.ip_len = 0;
    info.ip_p   = 0;

    try
    {
        //Set fields before build
        for ( QLineEdit *field : this->findChildren<QLineEdit*>())
        {
            if  ( field->objectName() == "checksum_line" ){
                info.ip_sum   =   static_cast<uint16_t>(std::stoul(field->text().toStdString()));
             }
            else if ( field->objectName() == "offset_line" ){
                info.ip_off   =   static_cast<uint16_t>(std::stoul(field->text().toStdString()));
             }
            else if ( field->objectName() == "ttl_line" ){
                info.ip_ttl   =   static_cast<uint8_t>(std::stoul(field->text().toStdString()));
            }
            else if ( field->objectName() == "src_line" ){
                info.ip_src.s_addr = context->ipv4_str_to_addr(field->text());
            }
            else if ( field->objectName() == "dst_line" ){
                info.ip_dst.s_addr = context->ipv4_str_to_addr(field->text());
            }
        }

        // Set control flags
        QGroupBox *ipv4_flags = this->findChild<QGroupBox*>();
        for ( QLineEdit *flag : ipv4_flags->findChildren<QLineEdit*>())
        {
            if ( flag->objectName() == "dscp_line" ){
                info.ip_tos |= (0b00111111 & static_cast<uint8_t>(std::stoul(flag->text().toStdString()))) << 2;
            }
            else if ( flag->objectName() == "ecn_line" )
                info.ip_tos |= (0b00000011 & static_cast<uint8_t>(std::stoul(flag->text().toStdString())));
        }
    }
    catch ( const std::exception &ex )
    {
        throw LibnetWrapperException("IPv4 Fields cast", ex.what());
    }

    return new LibnetIPv4Header(info);
}
