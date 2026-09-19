#include "banksystem.h"
#include "transaction.h"

#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>

BankSystem::BankSystem()
{
}

bool BankSystem::addCustomer(const Customer &customer)
{
    // 如果帐号已经存在，则添加失败
    if (findCustomer(customer.getAccountNumber()) != nullptr) {
        return false;
    }

    customers.append(customer);
    return true;
}

bool BankSystem::deleteCustomer(const QString &accountNumber)
{
    for (int i = 0; i < customers.size(); ++i)
    {
        if (customers[i].getAccountNumber() == accountNumber)
        {
            customers.removeAt(i);
            return true;
        }
    }

    return false;
}

bool BankSystem::freezeCustomer(const QString &accountNumber)
{
    Customer *customer = findCustomer(accountNumber);

    if (customer == nullptr)
    {
        return false;
    }

    if (customer->getIsFrozen())
    {
        return false;
    }

    customer->freeze();

    return true;
}

bool BankSystem::unfreezeCustomer(const QString &accountNumber)
{
    Customer *customer = findCustomer(accountNumber);

    if (customer == nullptr)
    {
        return false;
    }

    if (!customer->getIsFrozen())
    {
        return false;
    }

    customer->unfreeze();

    return true;
}

Customer *BankSystem::findCustomer(const QString &accountNumber)
{
    for (Customer &customer : customers) {
        if (customer.getAccountNumber() == accountNumber) {
            return &customer;
        }
    }

    return nullptr;
}

bool BankSystem::login(const QString &accountNumber,
                       const QString &password)
{
    Customer *customer = findCustomer(accountNumber);

    if (customer == nullptr) {
        return false;
    }

    if (customer->getIsFrozen())
    {
        return false;
    }

    return customer->getPassword() == password;
}

QList<Customer> BankSystem::getAllCustomers() const
{
    return customers;
}

bool BankSystem::deposit(const QString &accountNumber,
                         double amount,
                         const QString &employeeId)
{
    Customer *customer = findCustomer(accountNumber);

    if (customer == nullptr) {
        return false;
    }

    if (!customer->deposit(amount)) {
        return false;
    }

    int transactionId = customer->getTransactions().size() + 1;

    Transaction transaction(
        transactionId,
        amount,
        "存款",
        employeeId
        );

    customer->addTransaction(transaction);

    return true;
}

bool BankSystem::withdraw(const QString &accountNumber,
                          double amount,
                          const QString &employeeId)
{
    Customer *customer = findCustomer(accountNumber);

    if (customer == nullptr) {
        return false;
    }

    if (!customer->withdraw(amount)) {
        return false;
    }

    int transactionId = customer->getTransactions().size() + 1;

    Transaction transaction(
        transactionId,
        amount,
        "取款",
        employeeId
        );

    customer->addTransaction(transaction);

    return true;
}

bool BankSystem::transfer(const QString &fromAccount,
                          const QString &toAccount,
                          double amount,
                          const QString &remark)
{
    // 1. 查找转出账户
    Customer *fromCustomer =
        findCustomer(fromAccount);

    // 2. 查找转入账户
    Customer *toCustomer =
        findCustomer(toAccount);

    // 3. 检查账户是否存在
    if (fromCustomer == nullptr ||
        toCustomer == nullptr)
    {
        return false;
    }

    // 4. 不能给自己转账
    if (fromAccount == toAccount)
    {
        return false;
    }

    // 5. 检查转账金额
    if (amount <= 0)
    {
        return false;
    }

    // 6. 单笔转账不能超过50000元
    if (amount > 50000)
    {
        return false;
    }

    // 7. 转出账户不能被冻结或挂失
    if (fromCustomer->getIsFrozen() ||
        fromCustomer->getIsLost())
    {
        return false;
    }

    // 8. 转入账户不能被冻结或挂失
    if (toCustomer->getIsFrozen() ||
        toCustomer->getIsLost())
    {
        return false;
    }

    // 9. 检查余额是否足够
    if (fromCustomer->getBalance() < amount)
    {
        return false;
    }

    // 10. 转出账户扣款
    if (!fromCustomer->withdraw(amount))
    {
        return false;
    }

    // 11. 转入账户入账
    if (!toCustomer->deposit(amount))
    {
        // 理论上这里不会发生
        // 如果发生，则把钱退回转出账户
        fromCustomer->deposit(amount);
        return false;
    }

    // 12. 给转出账户生成交易记录
    int fromTransactionId =
        fromCustomer->getTransactions().size() + 1;

    Transaction fromTransaction(
        fromTransactionId,
        QDateTime::currentDateTime(),
        amount,
        "转账",
        "",
        toAccount,
        remark
        );

    fromCustomer->addTransaction(
        fromTransaction
        );

    // 13. 给转入账户生成交易记录
    int toTransactionId =
        toCustomer->getTransactions().size() + 1;

    Transaction toTransaction(
        toTransactionId,
        QDateTime::currentDateTime(),
        amount,
        "转账",
        "",
        fromAccount,
        remark
        );

    toCustomer->addTransaction(
        toTransaction
        );

    return true;
}

bool BankSystem::saveToFile(const QString &fileName)
{
    QJsonArray customerArray;

    for (const Customer &customer : customers) {
        QJsonObject customerObject;

        customerObject["accountNumber"] =
            customer.getAccountNumber();

        customerObject["name"] =
            customer.getName();

        customerObject["password"] =
            customer.getPassword();

        customerObject["address"] =
            customer.getAddress();

        customerObject["depositType"] =
            customer.getDepositType();

        customerObject["interestRate"] =
            customer.getInterestRate();

        customerObject["balance"] =
            customer.getBalance();

        customerObject["isLost"] =
            customer.getIsLost();

        customerObject["lostDate"] =
            customer.getLostDate().toString(Qt::ISODate);

        customerObject["isFrozen"] =
            customer.getIsFrozen();

        customerObject["frozenDate"] =
            customer.getFrozenDate().toString(Qt::ISODate);

        QJsonArray transactionArray;

        for (const Transaction &transaction :
             customer.getTransactions()) {

            QJsonObject transactionObject;

            transactionObject["id"] =
                transaction.getId();

            transactionObject["date"] =
                transaction.getDate().toString(Qt::ISODate);

            transactionObject["amount"] =
                transaction.getAmount();

            transactionObject["type"] =
                transaction.getType();

            transactionObject["employeeId"] =
                transaction.getEmployeeId();

            transactionObject["otherAccount"] =
                transaction.getOtherAccount();

            transactionObject["remark"] =
                transaction.getRemark();

            transactionArray.append(transactionObject);
        }

        customerObject["transactions"] = transactionArray;

        customerArray.append(customerObject);
    }

    QJsonDocument document(customerArray);

    QFile file(fileName);

    if (!file.open(QIODevice::WriteOnly)) {
        return false;
    }

    file.write(document.toJson(QJsonDocument::Indented));
    file.close();

    return true;
}

bool BankSystem::loadFromFile(const QString &fileName)
{
    QFile file(fileName);

    if (!file.open(QIODevice::ReadOnly)) {
        return false;
    }

    QByteArray data = file.readAll();
    file.close();

    QJsonParseError error;
    QJsonDocument document =
        QJsonDocument::fromJson(data, &error);

    if (error.error != QJsonParseError::NoError) {
        return false;
    }

    if (!document.isArray()) {
        return false;
    }

    customers.clear();

    QJsonArray customerArray = document.array();

    for (const QJsonValue &customerValue : customerArray) {
        QJsonObject customerObject =
            customerValue.toObject();

        QString accountNumber =
            customerObject["accountNumber"].toString();

        QString name =
            customerObject["name"].toString();

        QString password =
            customerObject["password"].toString();

        QString address =
            customerObject["address"].toString();

        QString depositType =
            customerObject["depositType"].toString();

        double interestRate =
            customerObject["interestRate"].toDouble();

        double balance =
            customerObject["balance"].toDouble();

        Customer customer(
            accountNumber,
            name,
            password,
            address,
            depositType,
            interestRate,
            balance
            );

        bool isLost =
            customerObject["isLost"].toBool();

        QDateTime lostDate =
            QDateTime::fromString(
                customerObject["lostDate"].toString(),
                Qt::ISODate
                );

        customer.setLostInfo(isLost, lostDate);

        bool isFrozen =
            customerObject["isFrozen"].toBool();

        QDateTime frozenDate =
            QDateTime::fromString(
                customerObject["frozenDate"].toString(),
                Qt::ISODate
                );

        customer.setFrozenInfo(isFrozen, frozenDate);

        QJsonArray transactionArray =
            customerObject["transactions"].toArray();

        for (const QJsonValue &transactionValue :
             transactionArray) {

            QJsonObject transactionObject =
                transactionValue.toObject();

            int id =
                transactionObject["id"].toInt();

            QDateTime date =
                QDateTime::fromString(
                    transactionObject["date"].toString(),
                    Qt::ISODate
                    );

            double amount =
                transactionObject["amount"].toDouble();

            QString type =
                transactionObject["type"].toString();

            QString employeeId =
                transactionObject["employeeId"].toString();

            QString otherAccount =
                transactionObject["otherAccount"].toString();

            QString remark =
                transactionObject["remark"].toString();

            Transaction transaction(
                id,
                date,
                amount,
                type,
                employeeId,
                otherAccount,
                remark
                );

            customer.addTransaction(transaction);
        }

        customers.append(customer);
    }

    return true;
}