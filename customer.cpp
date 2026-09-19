#include "customer.h"

Customer::Customer()
    : accountNumber(""),
    name(""),
    password(""),
    address(""),
    depositType(""),
    interestRate(0.0),
    balance(0.0),
    isFrozen(false),
    frozenDate(),
    isLost(false),
    lostDate()
{
}

Customer::Customer(const QString &accountNumber,
                   const QString &name,
                   const QString &password,
                   const QString &address,
                   const QString &depositType,
                   double interestRate,
                   double balance)
    : accountNumber(accountNumber),
    name(name),
    password(password),
    address(address),
    depositType(depositType),
    interestRate(interestRate),
    balance(balance),
    isFrozen(false),
    frozenDate(),
    isLost(false),
    lostDate()
{
}

QString Customer::getAccountNumber() const
{
    return accountNumber;
}

QString Customer::getName() const
{
    return name;
}

QString Customer::getPassword() const
{
    return password;
}

QString Customer::getAddress() const
{
    return address;
}

QString Customer::getDepositType() const
{
    return depositType;
}

double Customer::getInterestRate() const
{
    return interestRate;
}

void Customer::setName(const QString &name)
{
    this->name = name;
}

void Customer::setPassword(const QString &password)
{
    this->password = password;
}

void Customer::setAddress(const QString &address)
{
    this->address = address;
}

void Customer::setDepositType(const QString &depositType)
{
    this->depositType = depositType;
}

void Customer::reportLoss()
{
    isLost = true;
    lostDate = QDateTime::currentDateTime();
}

bool Customer::getIsLost() const
{
    return isLost;
}

QDateTime Customer::getLostDate() const
{
    return lostDate;
}

void Customer::freeze()
{
    isFrozen = true;
    frozenDate = QDateTime::currentDateTime();
}

void Customer::unfreeze()
{
    isFrozen = false;
    frozenDate = QDateTime();
}

bool Customer::getIsFrozen() const
{
    return isFrozen;
}

QDateTime Customer::getFrozenDate() const
{
    return frozenDate;
}

void Customer::setFrozenInfo(bool isFrozen,
                             const QDateTime &frozenDate)
{
    this->isFrozen = isFrozen;
    this->frozenDate = frozenDate;
}

QList<Transaction> Customer::getTransactions() const
{
    return transactions;
}

void Customer::addTransaction(const Transaction &transaction)
{
    transactions.append(transaction);
}

double Customer::getBalance() const
{
    return balance;
}

bool Customer::deposit(double amount)
{
    if (amount <= 0) {
        return false;
    }

    if (isLost) {
        return false;
    }

    balance += amount;
    return true;
}

bool Customer::withdraw(double amount)
{
    if (amount <= 0) {
        return false;
    }

    if (isLost) {
        return false;
    }

    if (amount > balance) {
        return false;
    }

    balance -= amount;
    return true;
}

void Customer::setBalance(double balance)
{
    this->balance = balance;
}

void Customer::setLostInfo(bool isLost,
                           const QDateTime &lostDate)
{
    this->isLost = isLost;
    this->lostDate = lostDate;
}

double Customer::calculateInterest() const
{
    double years = 1;

    if (depositType == "定期1年")
    {
        years = 1;
    }
    else if (depositType == "定期3年")
    {
        years = 3;
    }
    else if (depositType == "定期5年")
    {
        years = 5;
    }

    return balance * interestRate * years;
}

double Customer::calculateTotalAmount() const
{
    return balance + calculateInterest();
}

void Customer::setInterestRate(double interestRate)
{
    this->interestRate = interestRate;
}