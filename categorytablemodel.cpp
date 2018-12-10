#include "categorytablemodel.h"

CategoryTableModel::CategoryTableModel(QObject *parent, QSqlDatabase db)
   : QSqlTableModel(parent)
{
   QSqlTableModel::QSqlTableModel(parent, db);
}


QVariant CategoryTableModel::data(const QModelIndex& index, int role) const
{
   if (role == Qt::TextAlignmentRole)
   {
      return Qt::AlignCenter;
   }
   else
   {
      return QSqlTableModel::data(index, role);
   }
}
