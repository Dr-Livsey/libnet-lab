#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "proto.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //Add devs names to QListWidget
    for ( auto dev_name : LibnetWrapper::get_alldevs() ){
        ui->devices_lw->addItem(dev_name);
    }

    //Connect select device and creation of LibnetWrapper
    connect(ui->devices_lw, &QListWidget::itemDoubleClicked, [=](QListWidgetItem *item)
    {
        if (netlib_wrapper){
            netlib_wrapper.reset();
        }

        try
        {
            netlib_wrapper = std::unique_ptr<LibnetWrapper>(new LibnetWrapper(item->text().toStdString().c_str()));
        }
        catch (const LibnetWrapperException &ex)
        {
            ex.show_msg();
        }
    });

    //Connect 'Send Packet' button to appropriate slot
    connect(ui->send_packet_button, &QPushButton::clicked, this, &MainWindow::send_packet);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::send_packet()
{
    // Get network layer header pointer
    Header *transport_header = dynamic_cast<Header*>(ui->TransportLayerTabWidget->currentWidget());

    if (transport_header == nullptr)
        throw LibnetWrapperException("send_packet()", "Bad dynamic cast Header -> Transport Header");

    transport_header->build(ui->payload_text->toPlainText());
    //network_hedaer->build();
    //send_packet();

}

