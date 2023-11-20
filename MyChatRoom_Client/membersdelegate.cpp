#include "membersdelegate.h"

MembersDelegate::MembersDelegate(QObject *parent)
    : QStyledItemDelegate{parent}
{

}

void MembersDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    painter->save();

    // 获取数据
    QIcon avatar = qvariant_cast<QIcon>(index.data(Qt::DecorationRole)); // 假设头像以图标形式存储
    QString name = index.data(CustomRoles::nameRole).toString();

    // 设置头像和文本的绘制区域
    QRect rect = option.rect;
    QRect avatarRect = rect;
    avatarRect.setWidth(avatarRect.height()); // 正方形头像
    QRect nameRect = QRect(avatarRect.right() + 10, rect.top(), rect.width() - avatarRect.width() - 10, rect.height());

    // 绘制头像
    if (!avatar.isNull()) {
        painter->drawPixmap(avatarRect, avatar.pixmap(avatarRect.size()));
    }

    // 绘制名称
    painter->drawText(nameRect, Qt::AlignVCenter, name);

    // 检查悬停状态
    if (option.state & QStyle::State_MouseOver) {
        painter->fillRect(option.rect, QColor(173, 216, 230, 100)); // 悬停时的背景色
    }

    painter->restore();
}

//QSize MembersDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
//{
//    return QSize(180, 50);
//}
