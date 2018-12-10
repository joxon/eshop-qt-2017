#ifndef CATEGORYTABLEMODEL_H
#define CATEGORYTABLEMODEL_H

#include <QtSql>

class CategoryTableModel : public QSqlTableModel
{
   Q_OBJECT

public:
   CategoryTableModel(QObject *parent, QSqlDatabase db);

   QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;

private:
};

#endif // CATEGORYTABLEMODEL_H
