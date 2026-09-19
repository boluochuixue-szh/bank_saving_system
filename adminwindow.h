#ifndef ADMINWINDOW_H
#define ADMINWINDOW_H

#include <QWidget>
#include "banksystem.h"
#include "customertablemodel.h"

namespace Ui {
class AdminWindow;
}

class AdminWindow : public QWidget
{
    Q_OBJECT

public:
    explicit AdminWindow(BankSystem *bankSystem,
                         QWidget *parent = nullptr);
    ~AdminWindow();

private slots:
    void on_showAllButton_clicked();
    void on_exitButton_clicked();
    void on_searchButton_clicked();
    void on_recordButton_clicked();
    void on_modifyButton_clicked();
    void on_deleteButton_clicked();

    void on_sortComboBox_currentIndexChanged(int index);
    void on_depositTypeComboBox_currentIndexChanged(int index);
    void on_statusComboBox_currentIndexChanged(int index);
    void on_customerTable_clicked(const QModelIndex &index);

    void on_interestButton_clicked();

    void on_addCustomerButton_clicked();

    void on_freezeButton_clicked();

    void on_unfreezeButton_clicked();

signals:
    void backToLogin();

private:
    Ui::AdminWindow *ui;
    BankSystem *bankSystem;
    CustomerTableModel *customerModel;
};

#endif // ADMINWINDOW_H