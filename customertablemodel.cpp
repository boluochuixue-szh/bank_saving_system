#include "customertablemodel.h"

CustomerTableModel::CustomerTableModel(
    const QList<Customer> &customers,
    QObject *parent
    )
    : QAbstractTableModel(parent)
    , customers(customers)
{
}

int CustomerTableModel::rowCount(
    const QModelIndex &parent
    ) const
{
    if (parent.isValid())
        return 0;

    return customers.size();
}

int CustomerTableModel::columnCount(
    const QModelIndex &parent
    ) const
{
    if (parent.isValid())
        return 0;

    return 8;
}

QVariant CustomerTableModel::data(
    const QModelIndex &index,
    int role
    ) const
{
    if (!index.isValid())
        return QVariant();

    if (role == Qt::TextAlignmentRole)
    {
        return Qt::AlignCenter;
    }

    if (index.row() < 0 ||
        index.row() >= customers.size())
        return QVariant();

    if (role != Qt::DisplayRole)
        return QVariant();

    const Customer &customer = customers[index.row()];

    switch (index.column())
    {
    case 0:
        return customer.getAccountNumber();

    case 1:
        return customer.getName();

    case 2:
        return customer.getAddress();

    case 3:
        return customer.getDepositType();

    case 4:
        return QString::number(
            customer.getBalance(),
            'f',
            2
            );

    case 5:
        if (customer.getIsFrozen())
        {
            return "已冻结";
        }

        if (customer.getIsLost())
        {
            return "已挂失";
        }

        return "正常";

    case 6:
        if (customer.getIsLost())
        {
            return customer.getLostDate()
            .toString("yyyy-MM-dd HH:mm:ss");
        }

        return "—";

    case 7:
        if (customer.getIsFrozen())
            return customer.getFrozenDate().toString("yyyy-MM-dd HH:mm:ss");
        return "—";

    default:
        return QVariant();
    }
}

QVariant CustomerTableModel::headerData(
    int section,
    Qt::Orientation orientation,
    int role
    ) const
{
    if (role != Qt::DisplayRole)
        return QVariant();

    if (orientation == Qt::Horizontal)
    {
        QStringList headers;

        headers << "帐号"
                << "姓名"
                << "地址"
                << "储种"
                << "余额"
                << "账户状态"
                << "挂失日期"
                << "冻结日期";

        if (section >= 0 && section < headers.size())
            return headers[section];
    }

    return QVariant();
}

void CustomerTableModel::setCustomers(
    const QList<Customer> &customers)
{
    beginResetModel();

    this->customers = customers;

    endResetModel();
}