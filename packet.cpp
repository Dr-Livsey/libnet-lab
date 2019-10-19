#include "packet.h"
#include "libnetwrapper.h"
#include "libnetheader.h"
#include <QListWidgetItem>

RawPacket::RawPacket(LibnetIPv4Header *_ipv4_hdr, LibnetHeader *_t_hdr)
    : ipv4_hdr(_ipv4_hdr), t_hdr(_t_hdr)
{
    /**/
}

QListWidgetItem *RawPacket::as_QLItem() const
{
    // Wrap raw packet into QListWidgetItem
    QListWidgetItem *packet_qlw = new QListWidgetItem;

    QVariant wrapped_packet;
    wrapped_packet.setValue(this);

    packet_qlw->setData(Qt::UserRole, wrapped_packet);

    return packet_qlw;
}

void
RawPacket::build( std::unique_ptr<LibnetWrapper> &netlib_wrapper ) const
{
    netlib_wrapper->build_packet(ipv4_hdr, t_hdr);
}

RawPacket::~RawPacket()
{
    if (ipv4_hdr != nullptr)
        delete ipv4_hdr;

    if (t_hdr != nullptr)
        delete t_hdr;
}
