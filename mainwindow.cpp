#include "mainwindow.h"
#include "ui_mainwindow.h"


#include <QMessageBox>
#include <QInputDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    bankSystem.loadFromFile("bank_data.json");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_loginButton_clicked()
{
    QString accountNumber = ui->accountLineEdit->text();
    QString password = ui->passwordLineEdit->text();

    if (accountNumber.isEmpty() || password.isEmpty()) {
        QMessageBox::warning(
            this,
            "提示",
            "帐号和密码不能为空！"
            );
        return;
    }

    bool success = bankSystem.login(accountNumber, password);

    if (success) {
        QMessageBox::information(
            this,
            "登录成功",
            "欢迎进入银行储蓄管理系统！"
            );

        userWindow = new UserWindow(accountNumber, &bankSystem);

        connect(userWindow, &UserWindow::backToLogin,
                this, [this]()
                {
                    userWindow = nullptr;
                    this->show();
                });

        userWindow->show();
        this->hide();
    } else {
        Customer *customer =
            bankSystem.findCustomer(accountNumber);

        if (customer != nullptr &&
            customer->getIsFrozen())
        {
            QMessageBox::warning(
                this,
                "登录失败",
                "该账户已被冻结，请联系管理员！"
                );
        }
        else
        {
            QMessageBox::warning(
                this,
                "登录失败",
                "帐号或密码错误！"
                );
        }
    }
}

void MainWindow::on_registerButton_clicked()
{
    bool ok;

    QString accountNumber = QInputDialog::getText(
        this,
        "开户",
        "请输入帐号：",
        QLineEdit::Normal,
        "",
        &ok
        );

    if (!ok || accountNumber.isEmpty()) {
        return;
    }

    QString name = QInputDialog::getText(
        this,
        "开户",
        "请输入姓名：",
        QLineEdit::Normal,
        "",
        &ok
        );

    if (!ok || name.isEmpty()) {
        return;
    }

    QString password = QInputDialog::getText(
        this,
        "开户",
        "请输入密码：",
        QLineEdit::Password,
        "",
        &ok
        );

    if (!ok || password.isEmpty()) {
        return;
    }

    QString address = QInputDialog::getText(
        this,
        "开户",
        "请输入地址：",
        QLineEdit::Normal,
        "",
        &ok
        );

    if (!ok || address.isEmpty()) {
        return;
    }

    QStringList depositTypes;
    depositTypes << "定期1年"
                 << "定期3年"
                 << "定期5年";

    QString depositType = QInputDialog::getItem(
        this,
        "开户",
        "请选择储种：",
        depositTypes,
        0,
        false,
        &ok
        );

    if (!ok) {
        return;
    }

    double interestRate = 0.0198;

    if (depositType == "定期3年") {
        interestRate = 0.0225;
    } else if (depositType == "定期5年") {
        interestRate = 0.035;
    }

    Customer customer(
        accountNumber,
        name,
        password,
        address,
        depositType,
        interestRate,
        0.0
        );

    if (bankSystem.addCustomer(customer)) {
        if (bankSystem.saveToFile("bank_data.json")) {
            QMessageBox::information(
                this,
                "开户成功",
                "储户开户成功，数据已保存！"
                );
        } else {
            QMessageBox::warning(
                this,
                "保存失败",
                "开户成功，但数据保存失败！"
                );
        }
    } else {
        QMessageBox::warning(
            this,
            "开户失败",
            "该帐号已经存在！"
            );
    }

    // bool success = bankSystem.deposit("10001", 500.0, "EMP001");

    // if (success) {
    //     QMessageBox::information(this, "提示", "存款测试成功");
    // } else {
    //     QMessageBox::warning(this, "提示", "存款测试失败");
    // }
}

void MainWindow::on_adminLoginButton_clicked()
{
    bool ok = false;

    QString password = QInputDialog::getText(
        this,
        "管理员登录",
        "请输入管理员密码：",
        QLineEdit::Password,
        "",
        &ok
        );

    if (!ok)
    {
        return;
    }

    if (password != "admin123")
    {
        QMessageBox::warning(
            this,
            "登录失败",
            "管理员密码错误！"
            );
        return;
    }

    adminWindow = new AdminWindow(&bankSystem);

    connect(adminWindow, &AdminWindow::backToLogin,
            this, [this]()
            {
                this->show();
            });

    adminWindow->show();
    this->hide();
}