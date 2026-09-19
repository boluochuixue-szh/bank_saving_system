#ifndef CUSTOMERTABLEMODEL_H
#define CUSTOMERTABLEMODEL_H

#include <QAbstractTableModel>
#include "customer.h"

class CustomerTableModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    explicit CustomerTableModel(
        const QList<Customer> &customers,
        QObject *parent = nullptr
        );

    int rowCount(
        const QModelIndex &parent = QModelIndex()
        ) const override;

    int columnCount(
        const QModelIndex &parent = QModelIndex()
        ) const override;

    QVariant data(
        const QModelIndex &index,
        int role = Qt::DisplayRole
        ) const override;

    QVariant headerData(
        int section,
        Qt::Orientation orientation,
        int role = Qt::DisplayRole
        ) const override;

    // 刷新表格数据
    void setCustomers(const QList<Customer> &customers);

private:
    QList<Customer> customers;
};

#endif // CUSTOMERTABLEMODEL_H