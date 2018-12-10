#ifndef IMAGEITEMDELEGATE_H
#define IMAGEITEMDELEGATE_H

#include <QStyledItemDelegate>

class ImageItemDelegate : public QStyledItemDelegate
{
   Q_OBJECT
public:
   explicit ImageItemDelegate(QObject *parent = nullptr);

   void paint(QPainter *painter, const QStyleOptionViewItem& option, const QModelIndex& index) const;

   QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const;
};

#endif // IMAGEITEMDELEGATE_H
