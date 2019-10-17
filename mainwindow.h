#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <memory>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class LibnetWrapper;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void send_packet();

private:
    Ui::MainWindow *ui;
    std::unique_ptr<LibnetWrapper> netlib_wrapper;
};
#endif // MAINWINDOW_H
