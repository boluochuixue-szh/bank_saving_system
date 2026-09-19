#include "adminwindow.h"
#include "ui_adminwindow.h"

#include <QTableWidgetItem>
#include <QMessageBox>
#include <QDialog>
#include <QVBoxLayout>
#include <QListWidget>
#include <QInputDialog>
#include <algorithm>

AdminWindow::AdminWindow(BankSystem *bankSystem,
                         QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::AdminWindow)
    , bankSystem(bankSystem)
    , customerModel(nullptr)
{
    ui->setupUi(this);

    // 隔行显示
    ui->customerTable->setAlternatingRowColors(true);

    // 点击表格时整行选中
    ui->customerTable->setSelectionBehavior(
        QAbstractItemView::SelectRows
        );

    // 只能选择一行
    ui->customerTable->setSelectionMode(
        QAbstractItemView::SingleSelection
        );

    customerModel = new CustomerTableModel(
        QList<Customer>(),
        this
        );

    ui->customerTable->setModel(customerModel);

    setWindowTitle("管理员管理界面");

//   ui->customerTable->setColumnCount(7);
//    QStringList headers;
//    headers << "帐号" << "姓名" << "地址" << "储种" << "余额" << "账户状态" << "挂失日期";
//  ui->customerTable->setHorizontalHeaderLabels(headers);

    ui->customerTable->setEditTriggers(QAbstractItemView::NoEditTriggers);

    // 设置列宽
    ui->customerTable->setColumnWidth(0, 120);  // 帐号
    ui->customerTable->setColumnWidth(1, 90);   // 姓名
    ui->customerTable->setColumnWidth(2, 180);  // 地址
    ui->customerTable->setColumnWidth(3, 100);  // 储种
    ui->customerTable->setColumnWidth(4, 100);  // 余额
    ui->customerTable->setColumnWidth(5, 100);  // 账户状态
    ui->customerTable->setColumnWidth(6, 160);  // 挂失日期
    ui->customerTable->setColumnWidth(7, 160);  // 冻结日期

    // 设置行高
    ui->customerTable->verticalHeader()->setDefaultSectionSize(35);

    // 设置表头高度
    ui->customerTable->horizontalHeader()->setDefaultSectionSize(100);

    // 表头文字居中
    ui->customerTable->horizontalHeader()
        ->setDefaultAlignment(Qt::AlignCenter);

    // 单元格文字居中
    ui->customerTable->setStyleSheet(
        "QTableView::item {"
        "    text-align: center;"
        "}"
        );
}

AdminWindow::~AdminWindow()
{
    delete ui;
}

void AdminWindow::on_showAllButton_clicked()
{
    QList<Customer> customers =
        bankSystem->getAllCustomers();

    // 按储种筛选
    int depositTypeIndex =
        ui->depositTypeComboBox->currentIndex();

    if (depositTypeIndex != 0)
    {
        QString selectedType =
            ui->depositTypeComboBox->currentText();

        for (int i = customers.size() - 1; i >= 0; --i)
        {
            if (customers[i].getDepositType() != selectedType)
            {
                customers.removeAt(i);
            }
        }
    }

    // 按账户状态筛选
    int statusIndex =
        ui->statusComboBox->currentIndex();

    if (statusIndex == 1)
    {
        // 只显示正常账户
        for (int i = customers.size() - 1; i >= 0; --i)
        {
            if (customers[i].getIsLost() ||
                customers[i].getIsFrozen())
            {
                customers.removeAt(i);
            }
        }
    }
    else if (statusIndex == 2)
    {
        // 只显示已挂失账户
        for (int i = customers.size() - 1; i >= 0; --i)
        {
            if (!customers[i].getIsLost())
            {
                customers.removeAt(i);
            }
        }
    }
    else if (statusIndex == 3)
    {
        // 只显示已冻结账户
        for (int i = customers.size() - 1; i >= 0; --i)
        {
            if (!customers[i].getIsFrozen())
            {
                customers.removeAt(i);
            }
        }
    }

    // 排序
    int sortIndex =
        ui->sortComboBox->currentIndex();

    std::sort(
        customers.begin(),
        customers.end(),
        [sortIndex](const Customer &a, const Customer &b)
        {
            if (sortIndex == 0)
            {
                return a.getAccountNumber().toInt()
                < b.getAccountNumber().toInt();
            }
            else if (sortIndex == 1)
            {
                return a.getAccountNumber().toInt()
                > b.getAccountNumber().toInt();
            }
            else if (sortIndex == 2)
            {
                return a.getBalance()
                > b.getBalance();
            }
            else
            {
                return a.getBalance()
                < b.getBalance();
            }
        }
        );

    customerModel->setCustomers(customers);
}

void AdminWindow::on_exitButton_clicked()
{
    emit backToLogin();
    this->close();
}

void AdminWindow::on_searchButton_clicked()
{
    QString accountNumber =
        ui->searchAccountLineEdit->text().trimmed();

    if (accountNumber.isEmpty())
    {
        QMessageBox::warning(
            this,
            "提示",
            "请输入储户帐号！"
            );
        return;
    }

    Customer *customer =
        bankSystem->findCustomer(accountNumber);

    if (customer == nullptr)
    {
        QMessageBox::warning(
            this,
            "查询失败",
            "没有找到该储户！"
            );
        return;
    }

    QList<Customer> result;
    result.append(*customer);

    // 更新原来的模型，不再创建新的模型
    customerModel->setCustomers(result);
}

void AdminWindow::on_modifyButton_clicked()
{
    QString accountNumber =
        ui->searchAccountLineEdit->text().trimmed();

    if (accountNumber.isEmpty())
    {
        QMessageBox::warning(
            this,
            "提示",
            "请先输入储户帐号！"
            );
        return;
    }

    Customer *customer =
        bankSystem->findCustomer(accountNumber);

    if (customer == nullptr)
    {
        QMessageBox::warning(
            this,
            "修改失败",
            "没有找到该储户！"
            );
        return;
    }

    bool ok = false;

    QString name = QInputDialog::getText(
        this,
        "修改姓名",
        "请输入新姓名：",
        QLineEdit::Normal,
        customer->getName(),
        &ok
        );

    if (!ok)
        return;

    QString password = QInputDialog::getText(
        this,
        "修改密码",
        "请输入新密码：",
        QLineEdit::Password,
        customer->getPassword(),
        &ok
        );

    if (!ok)
        return;

    QString address = QInputDialog::getText(
        this,
        "修改地址",
        "请输入新地址：",
        QLineEdit::Normal,
        customer->getAddress(),
        &ok
        );

    if (!ok)
        return;

    QStringList depositTypes;
    depositTypes << "定期1年"
                 << "定期3年"
                 << "定期5年";

    int currentIndex =
        depositTypes.indexOf(customer->getDepositType());

    if (currentIndex < 0)
    {
        currentIndex = 0;
    }

    QString depositType = QInputDialog::getItem(
        this,
        "修改储种",
        "请选择新储种：",
        depositTypes,
        currentIndex,
        false,
        &ok
        );

    if (!ok)
        return;

    double interestRate = 0.0;

    if (depositType == "定期1年")
    {
        interestRate = 0.0198;
    }
    else if (depositType == "定期3年")
    {
        interestRate = 0.0225;
    }
    else if (depositType == "定期5年")
    {
        interestRate = 0.035;
    }

    customer->setName(name.trimmed());
    customer->setPassword(password);
    customer->setAddress(address.trimmed());
    customer->setDepositType(depositType);
    customer->setInterestRate(interestRate);

    bankSystem->saveToFile("bank_data.json");

    customerModel->setCustomers(
        bankSystem->getAllCustomers()
        );

    QMessageBox::information(
        this,
        "修改成功",
        "储户信息已经修改并保存！"
        );

//    on_searchButton_clicked();
}

void AdminWindow::on_deleteButton_clicked()
{
    QString accountNumber =
        ui->searchAccountLineEdit->text().trimmed();

    if (accountNumber.isEmpty())
    {
        QMessageBox::warning(
            this,
            "提示",
            "请先输入储户帐号！"
            );
        return;
    }

    Customer *customer =
        bankSystem->findCustomer(accountNumber);

    if (customer == nullptr)
    {
        QMessageBox::warning(
            this,
            "删除失败",
            "没有找到该储户！"
            );
        return;
    }

    if (customer->getBalance() != 0)
    {
        QMessageBox::warning(
            this,
            "无法删除",
            "该储户余额不为零，不能删除！"
            );
        return;
    }

    QMessageBox::StandardButton result =
        QMessageBox::question(
            this,
            "确认删除",
            "确定要删除该储户吗？",
            QMessageBox::Yes | QMessageBox::No
            );

    if (result != QMessageBox::Yes)
    {
        return;
    }

    if (bankSystem->deleteCustomer(accountNumber))
    {
        bankSystem->saveToFile("bank_data.json");

        customerModel->setCustomers(
            bankSystem->getAllCustomers()
            );

        QMessageBox::information(
            this,
            "删除成功",
            "储户已经删除！"
            );

        ui->searchAccountLineEdit->clear();
    }
    else
    {
        QMessageBox::warning(
            this,
            "删除失败",
            "删除储户时发生错误！"
            );
    }
}

void AdminWindow::on_recordButton_clicked()
{
    QString accountNumber =
        ui->searchAccountLineEdit->text().trimmed();

    if (accountNumber.isEmpty())
    {
        QMessageBox::warning(
            this,
            "提示",
            "请输入储户帐号！"
            );
        return;
    }

    Customer *customer =
        bankSystem->findCustomer(accountNumber);

    if (customer == nullptr)
    {
        QMessageBox::warning(
            this,
            "查询失败",
            "没有找到该储户！"
            );
        return;
    }

    QDialog *dialog = new QDialog(this);
    dialog->setWindowTitle("储户存取款记录");

    QVBoxLayout *layout = new QVBoxLayout(dialog);
    QListWidget *listWidget = new QListWidget(dialog);

    const QList<Transaction> &transactions =
        customer->getTransactions();

    if (transactions.isEmpty())
    {
        listWidget->addItem("该储户暂无存取款记录。");
    }
    else
    {
        for (const Transaction &transaction : transactions)
        {
            QString text = QString(
                               "编号：%1    日期：%2    类型：%3    金额：%4    营业员工号：%5"
                               )
                               .arg(transaction.getId())
                               .arg(transaction.getDate().toString("yyyy-MM-dd HH:mm:ss"))
                               .arg(transaction.getType())
                               .arg(transaction.getAmount(), 0, 'f', 2)
                               .arg(transaction.getEmployeeId());

            listWidget->addItem(text);
        }
    }

    layout->addWidget(listWidget);
    dialog->resize(700, 400);
    dialog->exec();

    dialog->deleteLater();
}

void AdminWindow::on_sortComboBox_currentIndexChanged(int index)
{
    Q_UNUSED(index);

    on_showAllButton_clicked();
}

void AdminWindow::on_depositTypeComboBox_currentIndexChanged(int index)
{
    Q_UNUSED(index);

    on_showAllButton_clicked();
}

void AdminWindow::on_statusComboBox_currentIndexChanged(int index)
{
    Q_UNUSED(index);
    on_showAllButton_clicked();
}

void AdminWindow::on_customerTable_clicked(
    const QModelIndex &index)
{
    if (!index.isValid())
    {
        return;
    }

    QString accountNumber =
        customerModel->data(
                         customerModel->index(index.row(), 0),
                         Qt::DisplayRole
                         ).toString();

    ui->searchAccountLineEdit->setText(accountNumber);
}

void AdminWindow::on_interestButton_clicked()
{
    QString accountNumber =
        ui->searchAccountLineEdit->text().trimmed();

    if (accountNumber.isEmpty())
    {
        QMessageBox::warning(
            this,
            "提示",
            "请先输入要查询的储户帐号！"
            );
        return;
    }

    Customer *customer =
        bankSystem->findCustomer(accountNumber);

    if (customer == nullptr)
    {
        QMessageBox::warning(
            this,
            "查询失败",
            "没有找到该储户！"
            );
        return;
    }

    double balance = customer->getBalance();
    double interest = customer->calculateInterest();
    double totalAmount = customer->calculateTotalAmount();

    QString message = QString(
                          "储户帐号：%1\n"
                          "储户姓名：%2\n"
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

void AdminWindow::on_addCustomerButton_clicked()
{
    bool ok = false;

    QString accountNumber = QInputDialog::getText(
        this,
        "新增储户",
        "请输入储户帐号：",
        QLineEdit::Normal,
        "",
        &ok
        );

    if (!ok || accountNumber.trimmed().isEmpty())
    {
        return;
    }

    accountNumber = accountNumber.trimmed();

    if (bankSystem->findCustomer(accountNumber) != nullptr)
    {
        QMessageBox::warning(
            this,
            "开户失败",
            "该帐号已经存在！"
            );
        return;
    }

    QString name = QInputDialog::getText(
        this,
        "新增储户",
        "请输入储户姓名：",
        QLineEdit::Normal,
        "",
        &ok
        );

    if (!ok || name.trimmed().isEmpty())
    {
        return;
    }

    QString password = QInputDialog::getText(
        this,
        "新增储户",
        "请输入登录密码：",
        QLineEdit::Password,
        "",
        &ok
        );

    if (!ok || password.isEmpty())
    {
        return;
    }

    QString address = QInputDialog::getText(
        this,
        "新增储户",
        "请输入储户地址：",
        QLineEdit::Normal,
        "",
        &ok
        );

    if (!ok || address.trimmed().isEmpty())
    {
        return;
    }

    QStringList depositTypes;
    depositTypes << "定期1年"
                 << "定期3年"
                 << "定期5年";

    QString depositType = QInputDialog::getItem(
        this,
        "新增储户",
        "请选择储种：",
        depositTypes,
        0,
        false,
        &ok
        );

    if (!ok)
    {
        return;
    }

    double interestRate = 0.0;

    if (depositType == "定期1年")
    {
        interestRate = 0.0198;
    }
    else if (depositType == "定期3年")
    {
        interestRate = 0.0225;
    }
    else if (depositType == "定期5年")
    {
        interestRate = 0.035;
    }

    double initialBalance = QInputDialog::getDouble(
        this,
        "新增储户",
        "请输入初始存款金额：",
        0.0,
        0.0,
        100000000.0,
        2,
        &ok
        );

    if (!ok)
    {
        return;
    }

    Customer customer(
        accountNumber,
        name.trimmed(),
        password,
        address.trimmed(),
        depositType,
        interestRate,
        initialBalance
        );

    Transaction transaction(
        1,
        initialBalance,
        "开户存款",
        "管理员"
        );

    customer.addTransaction(transaction);

    bankSystem->addCustomer(customer);

    bankSystem->saveToFile("bank_data.json");

    customerModel->setCustomers(
        bankSystem->getAllCustomers()
        );

    QMessageBox::information(
        this,
        "开户成功",
        "储户新增成功！"
        );
}

void AdminWindow::on_freezeButton_clicked()
{
    QString accountNumber =
        ui->searchAccountLineEdit->text().trimmed();

    if (accountNumber.isEmpty())
    {
        QMessageBox::warning(
            this,
            "提示",
            "请先输入储户帐号！"
            );
        return;
    }

    Customer *customer =
        bankSystem->findCustomer(accountNumber);

    if (customer == nullptr)
    {
        QMessageBox::warning(
            this,
            "冻结失败",
            "没有找到该储户！"
            );
        return;
    }

    if (customer->getIsFrozen())
    {
        QMessageBox::information(
            this,
            "提示",
            "该账户已经被冻结！"
            );
        return;
    }

    bool success =
        bankSystem->freezeCustomer(accountNumber);

    if (!success)
    {
        QMessageBox::warning(
            this,
            "冻结失败",
            "账户冻结失败！"
            );
        return;
    }

    bankSystem->saveToFile("bank_data.json");

    customerModel->setCustomers(
        bankSystem->getAllCustomers()
        );

    QMessageBox::information(
        this,
        "冻结成功",
        "该账户已被冻结！"
        );
}


void AdminWindow::on_unfreezeButton_clicked()
{
    QString accountNumber =
        ui->searchAccountLineEdit->text().trimmed();

    if (accountNumber.isEmpty())
    {
        QMessageBox::warning(
            this,
            "提示",
            "请先输入储户帐号！"
            );
        return;
    }

    Customer *customer =
        bankSystem->findCustomer(accountNumber);

    if (customer == nullptr)
    {
        QMessageBox::warning(
            this,
            "解冻失败",
            "没有找到该储户！"
            );
        return;
    }

    if (!customer->getIsFrozen())
    {
        QMessageBox::information(
            this,
            "提示",
            "该账户当前没有被冻结！"
            );
        return;
    }

    bool success =
        bankSystem->unfreezeCustomer(accountNumber);

    if (!success)
    {
        QMessageBox::warning(
            this,
            "解冻失败",
            "账户解冻失败！"
            );
        return;
    }

    bankSystem->saveToFile("bank_data.json");

    customerModel->setCustomers(
        bankSystem->getAllCustomers()
        );

    QMessageBox::information(
        this,
        "解冻成功",
        "该账户已解除冻结，可以重新登录！"
        );
}