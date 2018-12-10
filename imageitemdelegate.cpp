#include "imageitemdelegate.h"

#include <QApplication>

ImageItemDelegate::ImageItemDelegate(QObject *parent)
   : QStyledItemDelegate(parent)
{
}


void ImageItemDelegate::paint(QPainter *painter
                              , const QStyleOptionViewItem& option
                              , const QModelIndex& index) const
{
   QStyledItemDelegate::paint(painter, option, index);
   QStyleOptionViewItem opt(option);

   initStyleOption(&opt, index);

   QPixmap pixmap;
   if (!pixmap.loadFromData(index.data().toByteArray()))
   //, "jpg", Qt::AutoColor))
   {
      //qDebug() << "ImageItemDelegate::paint(): no image data" << pixmap << pixmap.size();
      pixmap.load(":/icons-light/tag_white.png", "png");
      QApplication::style()->drawItemPixmap(painter
                                            , opt.rect
                                            , Qt::AlignCenter
                                            , pixmap);
      return;
   }

   int shorterLen = opt.rect.width() > opt.rect.height() ?
                    opt.rect.height() : opt.rect.width();
   QApplication::style()->drawItemPixmap(painter
                                         , opt.rect
                                         , Qt::AlignCenter
                                         , pixmap.scaled(QSize(shorterLen, shorterLen)
                                                         , Qt::KeepAspectRatio
                                                         , Qt::SmoothTransformation));
}


QSize ImageItemDelegate::sizeHint(const QStyleOptionViewItem& option
                                  , const QModelIndex& index) const
{
   return QStyledItemDelegate::sizeHint(option, index);
}
