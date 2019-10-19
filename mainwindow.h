#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <memory>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class LibnetWrapper;
class QListWidgetItem;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void send_packet();

private slots:
    void on_AddToQueueButton_clicked();

    void on_SendAllPacketsButton_clicked();

    void on_ClearQueueButton_clicked();

    void on_PacketsQueueListWidget_itemDoubleClicked(QListWidgetItem *item);

private:
    Ui::MainWindow *ui;
    std::unique_ptr<LibnetWrapper> netlib_wrapper;
};
#endif // MAINWINDOW_H
