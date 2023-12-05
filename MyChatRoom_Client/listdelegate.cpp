#include "listdelegate.h"
#include "public.h"

#include <QIcon>
#include <QPixmap>
#include <QApplication>
#include <QRect>
#include <QString>

listDelegate::listDelegate(QObject *parent)
    : QStyledItemDelegate{parent}
{

}

void listDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    painter->save();

    // 获取数据
    QIcon avatar = qvariant_cast<QIcon>(index.data(Qt::DecorationRole)); // 假设头像以图标形式存储
    QString name = index.data(CustomRoles::nameRole).toString();
    QString id = index.data(CustomRoles::idRole).toString();
    bool winStatus = index.data(CustomRoles::winStatusRole).toBool();
    int msgNum = index.data(CustomRoles::msgNumberRole).toInt();
    QPixmap statusPixmap;
    if(index.data(CustomRoles::statusRole).isValid()){
        //为好友model
        if(index.data(CustomRoles::statusRole).toBool() == true){
            statusPixmap = QPixmap(":/res/green.png"); //绘制在线状态
        }
        else{
            statusPixmap = QPixmap(":/res/red.jpg"); //绘制离线状态
        }
    }


    // 设置头像和文本的绘制区域
    QRect rect = option.rect;
    QRect avatarRect = rect;
    avatarRect.setWidth(avatarRect.height()); // 正方形头像
    QRect nameRect = QRect(avatarRect.right() + 10, rect.top(), rect.width() - avatarRect.width() - 10, rect.height());
    QRect statusRect = QRect(avatarRect.right() - 10, rect.bottom() - 10, 10, 10);

    // 绘制头像
    painter->save();
    if (!avatar.isNull()) {
        painter->drawPixmap(avatarRect, avatar.pixmap(avatarRect.size()));
        painter->drawPixmap(statusRect, statusPixmap);
    }
    painter->restore();

    // 绘制名称
    painter->save();
    painter->drawText(nameRect, Qt::AlignVCenter, name+" ("+id+")");
    painter->restore();

    //绘制未读数据提示
    painter->save();
    if(!winStatus && msgNum > 0){
        // 绘制圆点
        //rect = option.rect.adjusted(0, 0, -10, 0); // 调整位置
        painter->setBrush(QColor(255, 128, 128)); // 设置填充颜色
        painter->setPen(Qt::NoPen); // 不绘制边框
        painter->drawEllipse(rect.right()-20, rect.center().y() - 10, 17, 17); // 绘制圆点

        // 绘制数字
        painter->setPen(Qt::white); // 设置文字颜色
        QFont font = painter->font();    // 创建字体对象
        font.setBold(true);         // 设置为粗体
        painter->setFont(font);     // 将字体应用于 QPainter
        painter->drawText(rect.right()-15, rect.center().y() - 5, 10, 10, Qt::AlignCenter, QString::number(msgNum));
    }
    painter->restore();

    // 检查悬停状态
    painter->save();
    if (option.state & QStyle::State_MouseOver) {
        painter->fillRect(option.rect, QColor(173, 216, 230, 100)); // 悬停时的背景色
    }

    if (option.state & QStyle::State_Selected) {
        painter->drawRect(rect); // 绘制选中时的边框
    }

    painter->restore();
}

QSize listDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    return QSize(180, 50);
}
