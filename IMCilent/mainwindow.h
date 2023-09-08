#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include"useritem.h"
#include<QVBoxLayout>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    //添加好友到列表上
    void addFriend(userItem* item);
    //从列表上删除好友
    void deleteFriend();

private:
    Ui::MainWindow *ui;
    //定义垂直布局的层
    QVBoxLayout* m_pLayput;
};
#endif // MAINWINDOW_H
