#ifndef CHATDIALOG_H
#define CHATDIALOG_H

#include <QDialog>
#include<qdebug.h>

namespace Ui {
class ChatDialog;
}

class ChatDialog : public QDialog
{
    Q_OBJECT
signals:
    //发送聊天内容和ip给kernel
    void SIG_sendMsg(QString content,int id);

public:
    explicit ChatDialog(QWidget *parent = nullptr);
    ~ChatDialog();
    //设置窗口信息
    void setInfo(QString name , int id);
    //设置聊天内容到窗口上
    void setContent(QString content);

private slots:
    void on_pb_send_clicked();

private:
    Ui::ChatDialog *ui;
    int m_id;
    QString m_name;
};

#endif // CHATDIALOG_H
