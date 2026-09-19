#ifndef BANKSYSTEM_H
#define BANKSYSTEM_H

#include <QList>
#include <QString>
#include <QJsonObject>

#include "customer.h"
#include "transaction.h"

class BankSystem
{
public:
    BankSystem();

    bool addCustomer(const Customer &customer);
    bool deleteCustomer(const QString &accountNumber);

    bool freezeCustomer(const QString &accountNumber);
    bool unfreezeCustomer(const QString &accountNumber);

    Customer *findCustomer(const QString &accountNumber);

    bool login(const QString &accountNumber,
               const QString &password);

    QList<Customer> getAllCustomers() const;

    bool deposit(const QString &accountNumber,
                 double amount,
                 const QString &employeeId);

    bool withdraw(const QString &accountNumber,
                  double amount,
                  const QString &employeeId);

    bool transfer(const QString &fromAccount,
                  const QString &toAccount,
                  double amount,
                  const QString &remark);

    bool saveToFile(const QString &fileName);
    bool loadFromFile(const QString &fileName);

private:
    QList<Customer> customers;
};

#endif // BANKSYSTEM_H