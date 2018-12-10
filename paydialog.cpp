#pragma execution_character_set("utf-8")

#include "paydialog.h"
#include "ui_paydialog.h"

#include <QtWidgets>

PayDialog::PayDialog(QWidget *parent, Account a, QString coststr, QVector<OffAbove> oaList)
   : QDialog(parent)
   , ui(new Ui::PayDialog)
   , acc(a)
{
   ui->setupUi(this);
   setWindowFlags(Qt::WindowMinimizeButtonHint
                  | Qt::WindowCloseButtonHint);

   // get original cost
   QRegularExpression              rx("(-?\\d+(?:[\\.,]\\d+(?:e\\d+)?)?)");
   QRegularExpressionMatchIterator i = rx.globalMatch(coststr);
   QRegularExpressionMatch         m = i.next();
   cost = m.captured().toDouble();

   // get x off when cost above y
   QVectorIterator<OffAbove> ii(oaList);
   double maxoff   = -1;
   double maxabove = -1;
   while (ii.hasNext())
   {
      OffAbove oa = ii.next();
      double   o  = oa.off.toDouble();
      double   a  = oa.above.toDouble();
      if ((cost >= a) && (a > maxabove))
      {
         // select max off
         maxabove = a;
         maxoff   = o;
      }
   }

   if (maxabove < 0)
   {
      ui->labelPriceTotal->setText(tr("总金额：¥") + QString::number(cost, 'f', 2));
   }
   else
   {
      cost -= maxoff;
      ui->labelPriceTotal->setText(tr("总金额：¥") + QString::number(cost, 'f', 2)
                                   + tr("（满") + QString::number(maxabove, 'f', 2)
                                   + tr("已减") + QString::number(maxoff, 'f', 2) + tr("）"));
   }


   // get cards
   ui->comboBoxCards->clear();
   ui->comboBoxCards->addItems(acc.getCardList());

   // connect to bank
   socket = new QTcpSocket(this);

   ui->labelConnState->setText(tr("等待输入密码"));

   connect(socket, SIGNAL(connected()), this, SLOT(requestToPay()));

   connect(socket, SIGNAL(readyRead()), this, SLOT(getPayResult()));

   connect(socket, SIGNAL(error(QAbstractSocket::SocketError)),
           this, SLOT(displayError(QAbstractSocket::SocketError)));
}


PayDialog::~PayDialog()
{
   delete ui;
}


void PayDialog::requestToPay()
{
   QString id  = ui->comboBoxCards->currentText();
   QString pwd = ui->lineEditPayPwd->text();


   // pay with a card
   // cmd = 02;#cardid;#paypwd;#value
   // out = 0|1
   QStringList cmd;

   cmd << "02" << id << pwd << QString::number(cost, 'f', 2);

   QByteArray out = cmd.join(";").toUtf8();
   socket->write(out);
}


void PayDialog::getPayResult()
{
   QByteArray in = socket->readAll();

   if (in == "0")
   {
      QMessageBox::information(this, tr("支付"), tr("支付成功！"));
      accept();
   }
   else if (in == "1")
   {
      QMessageBox::critical(this, tr("支付"), tr("支付命令错误！"));
   }
   else if (in == "2")
   {
      QMessageBox::warning(this, tr("支付"), tr("卡号与支付密码不匹配！"));
   }
   else if (in == "3")
   {
      QMessageBox::warning(this, tr("支付"), tr("卡内余额不足！"));
   }

   ui->lineEditPayPwd->clear();
   ui->lineEditPayPwd->setFocus();
}


void PayDialog::on_lineEditPayPwd_textEdited(const QString& arg1)
{
   if (arg1.length() == 6)
   {
      ui->labelConnState->setText(tr("正在支付..."));
      socket->connectToHost(QHostAddress::LocalHost, 10086);
   }
   else
   {
      ui->labelConnState->setText(tr("等待输入密码"));
   }
}


//void PayDialog::changeConnectState()
//{
//   ui->labelConnState->setText(tr("已连接"));
//}


void PayDialog::displayError(QAbstractSocket::SocketError e)
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
