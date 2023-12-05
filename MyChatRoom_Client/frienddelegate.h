#ifndef FRIENDDELEGATE_H
#define FRIENDDELEGATE_H

#include <QModelIndex>
#include <QPainter>
#include <QStyledItemDelegate>

class friendDelegate : public QStyledItemDelegate
{
public:
    friendDelegate(QObject *parent = nullptr);

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;

    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override;
};

#endif // FRIENDDELEGATE_H
