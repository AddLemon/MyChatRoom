#ifndef MEMBERSDELEGATE_H
#define MEMBERSDELEGATE_H

#include <QStyledItemDelegate>
#include <QModelIndex>
#include <QPainter>
#include "public.h"

class MembersDelegate : public QStyledItemDelegate
{
public:
    explicit MembersDelegate(QObject *parent = nullptr);

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;

    //QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override;
};

#endif // MEMBERSDELEGATE_H
