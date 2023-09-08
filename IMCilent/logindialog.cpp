#include "logindialog.h"
#include "ui_logindialog.h"

LoginDialog::LoginDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoginDialog)
{
    ui->setupUi(this);
    setWindowTitle("聊天系统1.0");
}

LoginDialog::~LoginDialog()
{
    delete ui;
}

void LoginDialog::on_pb_commit_clicked()
{
    qDebug() << __func__;
    //1.获取文本信息
    QString telTmp =  ui->le_tel->text();
    QString passwordTmp =  ui->le_password->text();

    //2.合法性校验
    //2.1字符串不能全为空空格
    if(telTmp.isEmpty()||passwordTmp.isEmpty()||
            telTmp.remove(" ").isEmpty()||passwordTmp.remove(" ").isEmpty()){
        QMessageBox::about(this,"提示","字符串不能全为空空格");
        return;
    }
    //2.2长度不能超过15
    if(telTmp.length()!=11||passwordTmp.length()>15){
        QMessageBox::about(this,"提示","长度不能超过15");
        return;
    }
    //2.3不能有下划线等。。TODO正则表达式

    //3.给kernel发信号
    Q_EMIT SIG_LoginCommit(telTmp ,passwordTmp);
}


void LoginDialog::on_pb_clear_clicked()
{
    qDebug() << __func__;
    ui->le_password->setText("");
    ui->le_tel->setText("");
}


void LoginDialog::on_pb_commit_register_clicked()
{
    qDebug() << __func__;
    //1.获取文本信息
    QString nameTmp =  ui->le_name_register->text();
    QString telTmp =  ui->le_tel_register->text();
    QString passwordTmp =  ui->le_password_register->text();

    //2.合法性校验
    //2.1字符串不能全为空空格
    if(nameTmp.isEmpty()||telTmp.isEmpty()||passwordTmp.isEmpty()||
            nameTmp.remove(" ").isEmpty()||telTmp.remove(" ").isEmpty()||passwordTmp.remove(" ").isEmpty()){
        QMessageBox::about(this,"提示","字符串不能全为空空格");
        return;
    }
    //2.2长度不能超过15
    if(nameTmp.length()>15||telTmp.length()!=11||passwordTmp.length()>15){
        QMessageBox::about(this,"提示","长度不能超过15");
        return;
    }
    //2.3不能有下划线等。。TODO正则表达式

    //3.给kernel发信号
    Q_EMIT SIG_RegisterCommit(nameTmp ,telTmp ,passwordTmp);
}


void LoginDialog::on_pb_clear_register_clicked()
{
    qDebug() << __func__;
    ui->le_name_register->setText("");
    ui->le_password_register->setText("");
    ui->le_tel_register->setText("");
}

