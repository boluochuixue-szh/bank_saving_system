#ifndef CUSTOMER_H
#define CUSTOMER_H

#include <QString>
#include <QDateTime>
#include <QList>
#include "transaction.h"

class Customer
{
public:
    Customer();

    Customer(const QString &accountNumber,
             const QString &name,
             const QString &password,
             const QString &address,
             const QString &depositType,
             double interestRate,
             double balance);

    QString getAccountNumber() const;
    QString getName() const;
    QString getPassword() const;
    QString getAddress() const;
    QString getDepositType() const;
    double getInterestRate() const;

    void setName(const QString &name);
    void setPassword(const QString &password);
    void setAddress(const QString &address);
    void setDepositType(const QString &depositType);

    void reportLoss();
    bool getIsLost() const;
    QDateTime getLostDate() const;

    void freeze();
    void unfreeze();

    bool getIsFrozen() const;
    QDateTime getFrozenDate() const;

    void setFrozenInfo(bool isFrozen, const QDateTime &frozenDate);

    QList<Transaction> getTransactions() const;
    void addTransaction(const Transaction &transaction);

    double getBalance() const;

    double calculateInterest() const;
    double calculateTotalAmount() const;

    bool deposit(double amount);
    bool withdraw(double amount);


    void setBalance(double balance);
    void setLostInfo(bool isLost, const QDateTime &lostDate);
    void setInterestRate(double interestRate);

private:
    QString accountNumber;  // 帐号
    QString name;           // 姓名
    QString password;       // 密码
    QString address;        // 地址
    QString depositType;    // 储种
    double interestRate;    // 利率
    double balance;         // 余额

    bool isFrozen;
    QDateTime frozenDate;

    bool isLost;           // 是否挂失
    QDateTime lostDate;    // 挂失日期

    QList<Transaction> transactions;

};

#endif // CUSTOMER_H