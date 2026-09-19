#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "banksystem.h"
#include "userwindow.h"
#include "adminwindow.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private slots:
    void on_loginButton_clicked();
    void on_registerButton_clicked();

    void on_adminLoginButton_clicked();

private:
    Ui::MainWindow *ui;
    BankSystem bankSystem;
    UserWindow *userWindow = nullptr;
    AdminWindow *adminWindow = nullptr;
};
#endif // MAINWINDOW_H
