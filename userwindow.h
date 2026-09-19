#ifndef USERWINDOW_H
#define USERWINDOW_H

#include <QWidget>

#include "banksystem.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class UserWindow;
}
QT_END_NAMESPACE

class UserWindow : public QWidget
{
    Q_OBJECT

public:
    explicit UserWindow(
        const QString &accountNumber,
        BankSystem *bankSystem,
        QWidget *parent = nullptr
        );

    ~UserWindow() override;

signals:
    void backToLogin();

private slots:
    void on_depositButton_clicked();

    void on_withdrawButton_clicked();

    void on_balanceButton_clicked();

    void on_recordButton_clicked();

    void on_reportLossButton_clicked();

    void on_modifyButton_clicked();

    void on_interestButton_clicked();

    void on_exitButton_clicked();

    void on_transferButton_clicked();

private:
    Ui::UserWindow *ui;
    QString accountNumber;
    BankSystem *bankSystem;
};

#endif // USERWINDOW_H