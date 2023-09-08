#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>
#include<QString>
#include<QMessageBox>
#include"packDef.h"
#include<QDebug>

namespace Ui {
class LoginDialog;
}

class LoginDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LoginDialog(QWidget *parent = nullptr);
    ~LoginDialog();

signals:
    //把用户注册的信息传给kernel
    void SIG_RegisterCommit(QString name , QString tel , QString password);
    //把用户登录的信息传给kernel
    void SIG_LoginCommit(QString tel , QString password);

private slots:
    void on_pb_commit_clicked();

    void on_pb_clear_clicked();

    void on_pb_commit_register_clicked();

    void on_pb_clear_register_clicked();

private:
    Ui::LoginDialog *ui;
};

#endif // LOGINDIALOG_H
