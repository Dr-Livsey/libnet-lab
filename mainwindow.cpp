#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "proto.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->send_packet_button->setDisabled(true);
    ui->get_ip_button->setDisabled(true);

    //Add devs names to QListWidget
    for ( auto dev_name : LibnetWrapper::get_alldevs() ){
        ui->devices_lw->addItem(dev_name);
    }

    //Connect select device and creation of LibnetWrapper
    connect(ui->devices_lw, &QListWidget::itemDoubleClicked, [=](QListWidgetItem *item)
    {
        try
        {
            auto new_netlib_wrapper = std::make_unique<LibnetWrapper>(item->text().toStdString().c_str());

            // If device exists, swap with older instance
            netlib_wrapper.swap(new_netlib_wrapper);

            ui->src_line->setText(netlib_wrapper->get_own_ipv4());
        }
        catch ( LibnetWrapperException &ex)
        {
            ex.show_msg();
            return;
        }

        // Set current dev QLineEdit
        ui->cur_dev_line->setText(item->text());
        ui->send_packet_button->setEnabled(true);
        ui->get_ip_button->setEnabled(true);
    });

    //Connect 'Send Packet' button to appropriate slot
    connect(ui->send_packet_button, &QPushButton::clicked, this, &MainWindow::send_packet);

    connect(ui->get_ip_button, &QPushButton::clicked, [=]()
    {
        try
        {
            ui->src_line->setText(netlib_wrapper->get_own_ipv4());
        }
        catch ( LibnetWrapperException &libnet_ex )
        {
            libnet_ex.show_msg();
        }
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::send_packet()
{
    // Get network layer header pointer
    IPv4Header *ipv4_header  = ui->NetworkLayerWidget;
    TransportHeader *transport_header = dynamic_cast<TransportHeader*>(ui->TransportLayerTabWidget->currentWidget());

    uint32_t bytes_written;

    try
    {
        if (transport_header == nullptr)
            throw LibnetWrapperException("send_packet()", "Bad dynamic cast Header -> Transport Header");

        // Clear raw info of headers
        ipv4_header->clear();
        transport_header->clear();

        // Build transport layer + ICMP
        transport_header->build(netlib_wrapper, ui->payload_text->toPlainText());

        // Build network layer
        ipv4_header->build(netlib_wrapper, transport_header);

        // Write packet to wire
        bytes_written = netlib_wrapper->write();

    }
    catch ( const LibnetWrapperException &libnet_ex )
    {
        libnet_ex.show_msg();
        netlib_wrapper->clear_packet();
        return;
    }

    ui->message_label->setText(
                QString::fromStdString(std::to_string(bytes_written)) + " bytes sended");
}

