#ifndef PAYDIALOG_H
#define PAYDIALOG_H

#include <QDialog>
#include <QtNetwork>

#include "account.h"
#include "product.h"

namespace Ui {
class PayDialog;
}

class PayDialog : public QDialog
{
   Q_OBJECT

public:
   PayDialog(QWidget *parent, Account a, QString coststr, QVector<OffAbove> oalist);
   ~PayDialog();

private slots:
//   void changeConnectState();
   void displayError(QAbstractSocket::SocketError e);
   void requestToPay();
   void getPayResult();

   void on_lineEditPayPwd_textEdited(const QString& arg1);

private:
   Ui::PayDialog *ui;

   QTcpSocket *socket;

   Account acc;

   double cost;
};

#endif // PAYDIALOG_H
