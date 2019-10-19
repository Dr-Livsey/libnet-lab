#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "proto.h"
#include "libnetheader.h"
#include "packet.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->send_packet_button->setDisabled(true);
    ui->get_ip_button->setDisabled(true);
    ui->AddToQueueButton->setDisabled(true);

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
        ui->AddToQueueButton->setEnabled(true);
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

    // Clear packets queue
    on_ClearQueueButton_clicked();
}

void MainWindow::send_packet()
{
    // Get network layer header pointer
    IPv4Header *ipv4_header  = ui->NetworkLayerWidget;
    Header *transport_header = dynamic_cast<Header*>(ui->TransportLayerTabWidget->currentWidget());

    uint32_t bytes_written;

    LibnetHeader*       libnet_t_h      = nullptr;
    LibnetIPv4Header*   libnet_ipv4_h   = nullptr;
    try
    {
        if (transport_header == nullptr)
            throw LibnetWrapperException("send_packet()", "Bad dynamic cast Header -> Transport Header");

        QString payload = ui->payload_text->toPlainText();

        // Parse QWidgets
        libnet_t_h      = transport_header->parse(payload);
        libnet_ipv4_h   = ipv4_header->parse(netlib_wrapper);

        // Build packet
        netlib_wrapper->build_packet(libnet_ipv4_h, libnet_t_h);

        // Write packet to wire
        bytes_written = netlib_wrapper->write();

        ui->message_label->setText(
                    QString::fromStdString(std::to_string(bytes_written)) + " bytes sended");
    }
    catch ( const LibnetWrapperException &libnet_ex )
    {
        libnet_ex.show_msg();
    }

    // Clear env
    netlib_wrapper->clear_packet();
    if (libnet_t_h != nullptr)      delete libnet_t_h;
    if (libnet_ipv4_h != nullptr)   delete libnet_ipv4_h;
}


void MainWindow::on_AddToQueueButton_clicked()
{
    QTabWidget *Tl_tabwidget = ui->TransportLayerTabWidget;

    // Current transport layer widget
    QWidget *current_tl_widget = Tl_tabwidget->currentWidget();
    QString  current_tl_name   = Tl_tabwidget->tabText(Tl_tabwidget->indexOf(current_tl_widget));

    // Get network layer header pointer
    IPv4Header *ipv4_header  = ui->NetworkLayerWidget;
    // Get transport layer header pointer
    Header *transport_header = dynamic_cast<Header*>(current_tl_widget);

    QString payload = ui->payload_text->toPlainText();

    RawPacket *raw_packet = nullptr;

    try
    {
        // Redundent check
        if (transport_header == nullptr){
            throw LibnetWrapperException(
                    "on_AddToQueueButton_clicked()", "Bad dynamic cast Header -> Transport Header");
        }

        // Create raw packet
        raw_packet = new RawPacket(ipv4_header->parse(netlib_wrapper), transport_header->parse(payload));

        QListWidgetItem *packet_qlw = raw_packet->as_QLItem();

        // Packet number in queue
        std::string packet_num = std::to_string(ui->PacketsQueueListWidget->count());

        // Set packet name
        packet_qlw->setText(QString::fromStdString("Packet ") + current_tl_name   + " #" +
                            QString::fromStdString(packet_num));

        // Add item to packets queue
        ui->PacketsQueueListWidget->addItem(packet_qlw);

    }
    catch ( const LibnetWrapperException &libnet_ex )
    {
        if (raw_packet != nullptr) delete raw_packet;
        libnet_ex.show_msg();
    }
}

void MainWindow::on_SendAllPacketsButton_clicked()
{
    for (int i = 0; i < ui->PacketsQueueListWidget->count(); i++)
    {
       QListWidgetItem *item       = ui->PacketsQueueListWidget->item(i);
       const RawPacket *raw_packet = RawPacket::from_QLItem(item);

        try
        {
           // Write packet to wire
           netlib_wrapper->write_packet(raw_packet);
        }
        catch ( const LibnetWrapperException &libnet_ex )
        {
            libnet_ex.show_msg();
        }

        // Delete packet
        delete raw_packet;
        //Clear
        netlib_wrapper->clear_packet();
    }
    // Clear queue
    ui->PacketsQueueListWidget->clear();
}

void MainWindow::on_ClearQueueButton_clicked()
{
    for (int i = 0; i < ui->PacketsQueueListWidget->count(); i++)
    {
       QListWidgetItem *item = ui->PacketsQueueListWidget->item(i);

       // Clear raw packet data
       delete RawPacket::from_QLItem(item);
    }
    // Clear queue
    ui->PacketsQueueListWidget->clear();
}

void MainWindow::on_PacketsQueueListWidget_itemDoubleClicked(QListWidgetItem *item)
{
    const RawPacket *raw_packet = RawPacket::from_QLItem(item);

    try
    {
       // Write packet to wire
       netlib_wrapper->write_packet(raw_packet);
    }
    catch ( const LibnetWrapperException &libnet_ex )
    {
        libnet_ex.show_msg();
    }

    // Delete pointers;
    delete raw_packet;
    delete ui->PacketsQueueListWidget->takeItem(ui->PacketsQueueListWidget->row(item));

    //Clear packet
    netlib_wrapper->clear_packet();
}
