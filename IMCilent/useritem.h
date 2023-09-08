#ifndef USERITEM_H
#define USERITEM_H

#include <QWidget>
#include<QIcon>
#include<QBitmap>
#include"packDef.h"

namespace Ui {
class userItem;
}

class userItem : public QWidget
{
    Q_OBJECT

public:
    explicit userItem(QWidget *parent = nullptr);
    ~userItem();

    //设置好友属性
    void setFriendInfo(int id,int state,int iconId,QString name,QString feeling);

private slots:
    void on_pb_icon_clicked();

signals:
    void SIG_showChatDialog(int id);

private:
    Ui::userItem *ui;
    int m_id;
    QString m_name;
    int m_state;
    int m_iconId;
    QString m_feeling;
};

#endif // USERITEM_H
