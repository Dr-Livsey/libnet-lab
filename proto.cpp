#include "proto.h"

#include <QDebug>
#include <QLineEdit>
#include <QCheckBox>
#include <QGroupBox>
#include <iostream>

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

void TCPHeader::build( const QString &payload_field )
{
    payload = payload_field;

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
            if ( flag->objectName() == "cwr" )
                info.th_flags |= 0b10000000;
            else if ( flag->objectName() == "ece" )
                info.th_flags |= 0b01000000;
            else if ( flag->objectName() == "urg" )
                info.th_flags |= 0b00100000;
            else if ( flag->objectName() == "ack" )
                info.th_flags |= 0b00010000;
            else if ( flag->objectName() == "psh" )
                info.th_flags |= 0b00001000;
            else if ( flag->objectName() == "rst" )
                info.th_flags |= 0b00000100;
            else if ( flag->objectName() == "syn" )
                info.th_flags |= 0b00000010;
            else if ( flag->objectName() == "fin" )
                info.th_flags |= 0b00000001;
        }
    }
    catch (const std::exception &ex)
    {
        throw LibnetWrapperException("TCP Fields cast", ex.what());
    }

    LibnetWrapper::build_tcp_header(this->info, payload);
}

void UDPHeader::build( const QString &payload_field )
{
    qDebug() << "hello udp header" << payload_field;
}

void ICMPHeader::build( const QString &payload_field )
{
    qDebug() << "hello icmp header" << payload_field;
}
