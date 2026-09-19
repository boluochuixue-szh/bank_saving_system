#ifndef TRANSACTION_H
#define TRANSACTION_H

#include <QDateTime>
#include <QString>

class Transaction
{
public:
    Transaction();

    Transaction(int id,
                double amount,
                const QString &type,
                const QString &employeeId);

    Transaction(int id,
                const QDateTime &date,
                double amount,
                const QString &type,
                const QString &employeeId,
                const QString &otherAccount = "",
                const QString &remark = "");

    int getId() const;
    QDateTime getDate() const;
    double getAmount() const;
    QString getType() const;
    QString getEmployeeId() const;

    QString getOtherAccount() const;
    QString getRemark() const;

private:
    int id;                  // 业务记录编号
    QDateTime date;          // 业务日期
    double amount;           // 业务金额
    QString type;            // 业务类型：存款、取款或转账
    QString employeeId;      // 营业员工号

    QString otherAccount;    // 转账对方账号
    QString remark;          // 转账备注
};

#endif // TRANSACTION_H