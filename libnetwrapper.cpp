#include "libnetwrapper.h"
#include <QMessageBox>


LibnetWrapper::LibnetWrapper(const char *dev_name)
{
    char errbuf[LIBNET_ERRBUF_SIZE];
    libnet_context = libnet_init(LIBNET_RAW4_ADV, dev_name, errbuf);

    if (NULL == libnet_context){
        throw LibnetWrapperException("libnet_init()", std::string(errbuf));
    }
    else{
        throw LibnetWrapperException("libnet_init()", std::string("Successfully bind to \"") + dev_name + "\" device!");
    }
}

QVector<QString> LibnetWrapper::get_alldevs()
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
        libnet_destroy(libnet_context);
}

LibnetWrapperException::LibnetWrapperException() :
    std::runtime_error("") {}

LibnetWrapperException::LibnetWrapperException( const std::string &t, const std::string &m) :
    std::runtime_error(t + m), title(t), msg(m) {}

void LibnetWrapperException::show_msg() const
{
    QMessageBox::about(nullptr, QString::fromStdString(title), QString::fromStdString(msg));
}
