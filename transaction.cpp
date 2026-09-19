#include "transaction.h"

Transaction::Transaction()
    : id(0),
    date(QDateTime::currentDateTime()),
    amount(0.0),
    type(""),
    employeeId(""),
    otherAccount(""),
    remark("")
{
}

Transaction::Transaction(int id,
                         double amount,
                         const QString &type,
                         const QString &employeeId)
    : id(id),
    date(QDateTime::currentDateTime()),
    amount(amount),
    type(type),
    employeeId(employeeId),
    otherAccount(""),
    remark("")
{
}

Transaction::Transaction(int id,
                         const QDateTime &date,
                         double amount,
                         const QString &type,
                         const QString &employeeId,
                         const QString &otherAccount,
                         const QString &remark)
    : id(id),
    date(date),
    amount(amount),
    type(type),
    employeeId(employeeId),
    otherAccount(otherAccount),
    remark(remark)
{
}

int Transaction::getId() const
{
    return id;
}

QDateTime Transaction::getDate() const
{
    return date;
}

double Transaction::getAmount() const
{
    return amount;
}

QString Transaction::getType() const
{
    return type;
}

QString Transaction::getEmployeeId() const
{
    return employeeId;
}

QString Transaction::getOtherAccount() const
{
    return otherAccount;
}

QString Transaction::getRemark() const
{
    return remark;
}