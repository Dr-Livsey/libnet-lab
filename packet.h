#ifndef PACKET_H
#define PACKET_H

#include <QMetaType>
#include "proto.h"

class QListWidgetItem;
class RawPacket;

class RawPacket
{
public:
    RawPacket(LibnetIPv4Header *ipv4_hdr, LibnetHeader *t_hdr);

    QListWidgetItem*    as_QLItem() const;
    void                build( std::unique_ptr<LibnetWrapper> &) const;

    ~RawPacket();
private:
    RawPacket();
    LibnetIPv4Header *ipv4_hdr;
    LibnetHeader     *t_hdr;
};

Q_DECLARE_METATYPE(const RawPacket*)

#endif // PACKET_H
