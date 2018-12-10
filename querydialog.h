#ifndef QUERYDIALOG_H
#define QUERYDIALOG_H

#include <QDialog>
#include <QtNetwork>

namespace Ui {
class QueryDialog;
}

class QueryDialog : public QDialog
{
   Q_OBJECT

public:
   QueryDialog(QWidget *parent, QString cid, QString pp);
   ~QueryDialog();

private slots:
   void requestToQuery();
   void getQueryResult();
   void displayError(QAbstractSocket::SocketError e);

private:
   Ui::QueryDialog *ui;

   QTcpSocket *socket;

   QString cardid;
   QString pwdpay;
};

#endif // QUERYDIALOG_H
