#ifndef SHAREFILE_H
#define SHAREFILE_H

#include <QWidget>
#include<QVBoxLayout>
#include<QHBoxLayout>
#include<QListWidget>
#include<QButtonGroup>
#include<QScrollArea>
#include<QPushButton>

class ShareFile : public QWidget
{
    Q_OBJECT
public:
    explicit ShareFile(QWidget *parent = nullptr);
    //单例模式
    static ShareFile &getInstance();
    //上传分享好友信息
    void updataFriend(QListWidget* m_pFriendList);

private:
    QPushButton* m_pSelectAllpb;//全选
    QPushButton* m_pCancelSelectpb;//取消全选

    QPushButton* m_pOKpb;//ok
    QPushButton* m_pCancelpb;//取消

    QScrollArea* m_pSA;//滚动区
    QWidget* m_pFriendw;//好友列表窗口

    QButtonGroup* m_pButtonGroup;

    QVBoxLayout* m_pFriendWVbl;
public slots:
    //全选槽函数
    void selectAll();

    //取消选择槽函数
    void cancelSelect();

    //确定槽函数
    void OKshare();

    //取消槽函数
    void cancelShare();
signals:
};

#endif // SHAREFILE_H
