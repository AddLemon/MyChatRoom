#ifndef LISTDELEGATE_H
#define LISTDELEGATE_H

#include <QModelIndex>
#include <QPainter>
#include <QStyledItemDelegate>

class listDelegate : public QStyledItemDelegate
{
public:
    listDelegate(QObject *parent = nullptr);

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;

    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override;
};

#endif // LISTDELEGATE_H
