#ifndef BINDDIALOG_H
#define BINDDIALOG_H

#include <QDialog>
#include <QtNetwork>

namespace Ui {
class BindDialog;
}

class BindDialog : public QDialog
{
   Q_OBJECT

public:
   BindDialog(QWidget *parent, QString cid, QString ph, QString pp);
   ~BindDialog();

private slots:
   void sendBindInfo();
   void getBindResult();
   void displayError(QAbstractSocket::SocketError);

private:
   Ui::BindDialog *ui;

   QTcpSocket *socket;

   // bind a card
   // cmd = 00;#cardid;#phone;#paypwd
   // out = 0|1 (0=success, 1=fail)

   QString cardid;
   QString phone;
   QString paypwd;
};

#endif // BINDDIALOG_H
