#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "libnetwrapper.h"
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
}

MainWindow::~MainWindow()
{
    delete ui;
}

