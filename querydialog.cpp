#pragma execution_character_set("utf-8")

#include "querydialog.h"
#include "ui_querydialog.h"

#include <QtWidgets>

QueryDialog::QueryDialog(QWidget *parent, QString cid, QString pp)
   : QDialog(parent)
   , ui(new Ui::QueryDialog)
   , cardid(cid)
   , pwdpay(pp)
{
   ui->setupUi(this);
   setWindowFlags(Qt::WindowMinimizeButtonHint
                  | Qt::WindowCloseButtonHint);

   ui->labelCardID->setText(cardid);

   socket = new QTcpSocket(this);

   connect(socket, SIGNAL(connected()),
           this, SLOT(requestToQuery()));

   connect(socket, SIGNAL(readyRead()),
           this, SLOT(getQueryResult()));

   connect(socket, SIGNAL(error(QAbstractSocket::SocketError)),
           this, SLOT(displayError(QAbstractSocket::SocketError)));

   socket->connectToHost(QHostAddress::LocalHost, 10086);
}


QueryDialog::~QueryDialog()
{
   delete ui;
}


void QueryDialog::requestToQuery()
{
   ui->labelBank->setText(tr("查询中..."));
   ui->labelBalance->setText(tr("查询中..."));

   // query a card
   // cmd = 01;#cardid;#paypwd
   // out = 1|#bankname;#balance
   QStringList cmd;
   cmd << "01" << cardid << pwdpay;

   QByteArray out = cmd.join(";").toUtf8();
   socket->write(out);
}


void QueryDialog::getQueryResult()
{
   QByteArray  in  = socket->readAll();
   QStringList res = QString(in).split(";");

   // query a card
   // cmd = 01;#cardid;#paypwd
   // out = 1|#bankname;#balance
   if (in == "1")
   {
      ui->labelBank->setText(tr("密码错误"));
      ui->labelBalance->setText(tr("密码错误"));
      QMessageBox::warning(this, tr("查询"), tr("密码错误！"));
   }
   else if (res.size() == 2)
   {
      ui->labelBank->setText(res.at(0));
      ui->labelBalance->setText(res.at(1));
   }
   else
   {
      ui->labelBank->setText(tr("未知错误"));
      ui->labelBalance->setText(tr("未知错误"));
      QMessageBox::warning(this, tr("查询"), tr("发生未知错误。"));
   }
}


void QueryDialog::displayError(QAbstractSocket::SocketError e)
{
   switch (e)
   {
   case QAbstractSocket::RemoteHostClosedError:
      return;

   case QAbstractSocket::HostNotFoundError:
      QMessageBox::warning(this, tr("支付"),
                           tr("未找到主机。"));
      break;

   case QAbstractSocket::ConnectionRefusedError:
      QMessageBox::warning(this, tr("支付"),
                           tr("连接被拒绝，可能是银行系统未开启。"));
      break;

   default:
      QMessageBox::warning(this, tr("支付"),
                           tr("发生错误：%1")
                              .arg(socket->errorString()));
   }
   reject();
}
