#include "userwindow.h"
#include "ui_userwindow.h"

#include <QInputDialog>
#include <QMessageBox>
#include <QDialog>
#include <QVBoxLayout>
#include <QListWidget>

UserWindow::UserWindow(
    const QString &accountNumber,
    BankSystem *bankSystem,
    QWidget *parent
    )
    : QWidget(parent)
    , ui(new Ui::UserWindow)
    , accountNumber(accountNumber)
    , bankSystem(bankSystem)
{
    ui->setupUi(this);

    setAttribute(Qt::WA_DeleteOnClose);

    setWindowTitle("用户业务管理");

    ui->accountLabel->setText(
        "当前登录帐号：" + accountNumber
        );
}

UserWindow::~UserWindow()
{
    delete ui;
}

void UserWindow::on_depositButton_clicked()
{
    bool ok = false;

    double amount = QInputDialog::getDouble(
        this,
        "存款",
        "请输入存款金额：",
        0.0,
        0.01,
        100000000.0,
        2,
        &ok
        );

    if (!ok) {
        return;
    }

    bool success = bankSystem->deposit(
        accountNumber,
        amount,
        "EMP001"
        );

    if (success) {
        QMessageBox::information(
            this,
            "存款成功",
            "存款成功，金额："
                + QString::number(amount, 'f', 2)
                + " 元"
            );

        bankSystem->saveToFile("bank_data.json");
    } else {
        QMessageBox::warning(
            this,
            "存款失败",
            "存款失败，账户可能不存在或已挂失。"
            );
    }
}

void UserWindow::on_withdrawButton_clicked()
{
    bool ok = false;

    double amount = QInputDialog::getDouble(
        this,
        "取款",
        "请输入取款金额：",
        0.0,
        0.01,
        100000000.0,
        2,
        &ok
        );

    if (!ok) {
        return;
    }

    bool success = bankSystem->withdraw(
        accountNumber,
        amount,
        "EMP001"
        );

    if (success) {
        QMessageBox::information(
            this,
            "取款成功",
            "取款成功，金额："
                + QString::number(amount, 'f', 2)
                + " 元"
            );

        bankSystem->saveToFile("bank_data.json");
    } else {
        QMessageBox::warning(
            this,
            "取款失败",
            "取款失败，可能是余额不足、帐号不存在或账户已挂失。"
            );
    }
}

void UserWindow::on_balanceButton_clicked()
{
    Customer *customer = bankSystem->findCustomer(accountNumber);

    if (customer == nullptr) {
        QMessageBox::warning(
            this,
            "查询失败",
            "没有找到当前账户！"
            );
        return;
    }

    QMessageBox::information(
        this,
        "账户余额",
        "当前余额："
            + QString::number(customer->getBalance(), 'f', 2)
            + " 元"
        );
}

void UserWindow::on_recordButton_clicked()
{
    Customer *customer =
        bankSystem->findCustomer(accountNumber);

    if (customer == nullptr) {
        QMessageBox::warning(
            this,
            "查询失败",
            "没有找到当前账户！"
            );
        return;
    }

    QDialog dialog(this);
    dialog.setWindowTitle("查询交易记录");
    dialog.resize(500, 350);

    QVBoxLayout *layout =
        new QVBoxLayout(&dialog);

    QListWidget *listWidget =
        new QListWidget(&dialog);

    layout->addWidget(listWidget);

    const QList<Transaction> &transactions =
        customer->getTransactions();

    if (transactions.isEmpty())
    {
        listWidget->addItem("暂无交易记录");
    }
    else
    {
        for (const Transaction &transaction : transactions)
        {
            QString employeeText =
                transaction.getEmployeeId();

            if (employeeText.isEmpty())
            {
                employeeText = "非柜台交易";
            }

            QString record =
                "日期：" +
                transaction.getDate().toString(
                    "yyyy-MM-dd HH:mm:ss"
                    )
                + " | 类型：" +
                transaction.getType()
                + " | 金额：" +
                QString::number(
                    transaction.getAmount(),
                    'f',
                    2
                    )
                + " 元"
                + " | 对方账号：" +
                transaction.getOtherAccount()
                + " | 备注：" +
                transaction.getRemark()
                + " | 营业员工号：" +
                employeeText;

            listWidget->addItem(record);
        }
    }

    dialog.exec();
}

void UserWindow::on_reportLossButton_clicked()
{
    Customer *customer = bankSystem->findCustomer(accountNumber);

    if (customer == nullptr) {
        QMessageBox::warning(
            this,
            "挂失失败",
            "没有找到当前账户！"
            );
        return;
    }

    if (customer->getIsLost()) {
        QMessageBox::information(
            this,
            "提示",
            "该账户已经挂失，无需重复操作。"
            );
        return;
    }

    QMessageBox::StandardButton result =
        QMessageBox::question(
            this,
            "账户挂失",
            "确定要挂失当前账户吗？",
            QMessageBox::Yes | QMessageBox::No
            );

    if (result != QMessageBox::Yes) {
        return;
    }

    customer->reportLoss();

    bankSystem->saveToFile("bank_data.json");

    QMessageBox::information(
        this,
        "挂失成功",
        "账户已成功挂失，挂失后不能进行存取款操作。"
        );
}

void UserWindow::on_modifyButton_clicked()
{
    Customer *customer =
        bankSystem->findCustomer(accountNumber);

    if (customer == nullptr) {
        QMessageBox::warning(
            this,
            "修改失败",
            "没有找到当前账户！"
            );
        return;
    }

    bool ok = false;

    QString name = QInputDialog::getText(
        this,
        "修改信息",
        "请输入新的姓名：",
        QLineEdit::Normal,
        customer->getName(),
        &ok
        );

    if (!ok || name.isEmpty()) {
        return;
    }

    QString password = QInputDialog::getText(
        this,
        "修改信息",
        "请输入新的密码：",
        QLineEdit::Password,
        customer->getPassword(),
        &ok
        );

    if (!ok || password.isEmpty()) {
        return;
    }

    QString address = QInputDialog::getText(
        this,
        "修改信息",
        "请输入新的地址：",
        QLineEdit::Normal,
        customer->getAddress(),
        &ok
        );

    if (!ok || address.isEmpty()) {
        return;
    }

    customer->setName(name);
    customer->setPassword(password);
    customer->setAddress(address);

    if (bankSystem->saveToFile("bank_data.json")) {
        QMessageBox::information(
            this,
            "修改成功",
            "储户信息已修改并保存！"
            );
    } else {
        QMessageBox::warning(
            this,
            "保存失败",
            "信息已修改，但保存文件失败！"
            );
    }
}

void UserWindow::on_interestButton_clicked()
{
    Customer *customer =
        bankSystem->findCustomer(accountNumber);

    if (customer == nullptr)
    {
        QMessageBox::warning(
            this,
            "查询失败",
            "没有找到当前账户！"
            );
        return;
    }

    double balance = customer->getBalance();
    double interest = customer->calculateInterest();
    double totalAmount = customer->calculateTotalAmount();

    QString message = QString(
                          "帐号：%1\n"
                          "姓名：%2\n"
                          "储种：%3\n"
                          "本金：%4 元\n"
                          "预计利息：%5 元\n"
                          "本息合计：%6 元"
                          )
                          .arg(customer->getAccountNumber())
                          .arg(customer->getName())
                          .arg(customer->getDepositType())
                          .arg(balance, 0, 'f', 2)
                          .arg(interest, 0, 'f', 2)
                          .arg(totalAmount, 0, 'f', 2);

    QMessageBox::information(
        this,
        "利息查询结果",
        message
        );
}

void UserWindow::on_exitButton_clicked()
{
    emit backToLogin();
    this->close();
}

void UserWindow::on_transferButton_clicked()
{
    bool ok = false;

    // 输入转入账号
    QString toAccount = QInputDialog::getText(
        this,
        "转账",
        "请输入转入账号：",
        QLineEdit::Normal,
        "",
        &ok
        );

    if (!ok)
        return;

    // 输入转账金额
    double amount = QInputDialog::getDouble(
        this,
        "转账",
        "请输入转账金额：",
        0.0,
        0.01,
        50000.0,
        2,
        &ok
        );

    if (!ok)
        return;

    // 输入转账备注
    QString remark = QInputDialog::getText(
        this,
        "转账",
        "请输入转账备注：",
        QLineEdit::Normal,
        "",
        &ok
        );

    if (!ok)
        return;

    bool success = bankSystem->transfer(
        accountNumber,
        toAccount,
        amount,
        remark
        );

    if (success)
    {
        bankSystem->saveToFile("bank_data.json");

        QMessageBox::information(
            this,
            "转账成功",
            "转账已经成功！"
            );
    }
    else
    {
        QMessageBox::warning(
            this,
            "转账失败",
            "转账失败！\n\n"
            "可能原因：\n"
            "1. 转入账号不存在\n"
            "2. 不能向自己的账号转账\n"
            "3. 转账金额无效\n"
            "4. 余额不足\n"
            "5. 账户已被冻结或挂失\n"
            "6. 单笔转账超过50000元"
            );
    }
}