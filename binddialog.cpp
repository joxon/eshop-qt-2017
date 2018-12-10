#pragma execution_character_set("utf-8")

#include "binddialog.h"
#include "ui_binddialog.h"

#include <QtWidgets>
#include <QMovie>

BindDialog::BindDialog(QWidget *parent, QString cid, QString ph, QString pp)
   : QDialog(parent)
   , ui(new Ui::BindDialog)
   , cardid(cid)
   , phone(ph)
   , paypwd(pp)
{
   ui->setupUi(this);
   setWindowFlags(Qt::WindowMinimizeButtonHint
                  | Qt::WindowCloseButtonHint);

   QMovie *mv = new QMovie(":/pics/loading.gif");
   mv->setScaledSize(QSize(50, 50));
   mv->start();
   ui->labelGif->setAttribute(Qt::WA_NoSystemBackground);
   ui->labelGif->setMovie(mv);

   socket = new QTcpSocket(this);
   socket->connectToHost(QHostAddress::LocalHost, 10086);

   connect(socket, SIGNAL(connected()),
           this, SLOT(sendBindInfo()));

   connect(socket, SIGNAL(readyRead()),
           this, SLOT(getBindResult()));

   connect(socket, SIGNAL(error(QAbstractSocket::SocketError)),
           this, SLOT(displayError(QAbstractSocket::SocketError)));
}


BindDialog::~BindDialog()
{
   delete ui;
}


void BindDialog::sendBindInfo()
{
   QStringList cmd;

   cmd << "00" << cardid << phone << paypwd;

   QByteArray out = cmd.join(";").toUtf8();
   //QDataStream
   socket->write(out);
}


void BindDialog::getBindResult()
{
   QByteArray in = socket->readAll();

   if (in == "0")
   {
      accept();
   }
   else
   {
      reject();
   }
}


void BindDialog::displayError(QAbstractSocket::SocketError e)
{
   switch (e)
   {
   case QAbstractSocket::RemoteHostClosedError:
      break;

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
