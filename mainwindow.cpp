#pragma execution_character_set("utf-8")

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QtCore>
#include <QtWidgets>

#include "imageitemdelegate.h"

#include "paydialog.h"
#include "binddialog.h"
#include "querydialog.h"

MainWindow::MainWindow(QWidget *parent, QApplication& app)
   : QMainWindow(parent)
   , ui(new Ui::MainWindow)
   , rApp(app)
{
   ui->setupUi(this);
   setWindowFlags(Qt::CustomizeWindowHint
                  | Qt::WindowMinMaxButtonsHint
                  | Qt::WindowCloseButtonHint);

   ui->stackedWidgetMain->setCurrentWidget(ui->pageHome);
   ui->stackedWidgetAccount->setCurrentWidget(ui->pageLogin);
   ui->stackedWidgetCategory->setCurrentWidget(ui->pageProductAll);

   openDB(QApplication::applicationDirPath() + "/eshop.db");

   /*
    * Load settings
    */

   QSqlQuery q;
   q.exec("select * from appsettings");
   q.next();
   if (q.value("remUid").toBool())
   {
      ui->lineEditLoginName->setText(q.value("lastUid").toString().trimmed());
      ui->checkBoxRemUid->setChecked(true);
   }
   if (q.value("remPwd").toBool())
   {
      ui->lineEditLoginPassword->setText(q.value("lastPwd").toString().trimmed());
      ui->checkBoxRemPwd->setChecked(true);
   }

   QString lastTheme = q.value("lastTheme").toString();
   if (lastTheme == "taobao.qss")
   {
      on_actionTaobao_triggered();
   }
   else if (lastTheme == "blue.qss")
   {
      on_actionBlue_triggered();
   }
   else if (lastTheme == "blue-md.qss")
   {
      on_actionBlueMD_triggered();
   }
   else if (lastTheme == "dark.qss")
   {
      on_actionDark_triggered();
   }
   else if (lastTheme == "dark-vs.qss")
   {
      on_actionDarkVS_triggered();
   }
   else if (lastTheme == "gray.qss")
   {
      on_actionGray_triggered();
   }

   /*
    * Load OffAbove Acts
    */

   q.exec("select * from offAbove");
   while (q.next())
   {
      if (q.value("enable").toBool())
      {
         OffAbove oa;
         oa.above = q.value("above").toString();
         oa.off   = q.value("off").toString();
         oaList.append(oa);
      }
   }

   /*
    *  Load products
    */

   categoryTableModel = new CategoryTableModel(this, database);
   categoryTableModel->setTable("products");
   if (!categoryTableModel->select())
   {
      qDebug() << "QSqlTableModel select failed";
      qDebug() << categoryTableModel->lastError();
   }

   categoryTableModel->setHeaderData(1, Qt::Horizontal, tr("图片"));
   categoryTableModel->setHeaderData(2, Qt::Horizontal, tr("品名"));
   categoryTableModel->setHeaderData(3, Qt::Horizontal, tr("类型"));
   categoryTableModel->setHeaderData(4, Qt::Horizontal, tr("描述"));
   categoryTableModel->setHeaderData(5, Qt::Horizontal, tr("库存"));
   categoryTableModel->setHeaderData(6, Qt::Horizontal, tr("价格"));

   QTableView *category = ui->tableViewCategory;
   category->setItemDelegateForColumn(1, new ImageItemDelegate(this));
   category->setModel(categoryTableModel);
   category->hideColumn(0); // id
   category->hideColumn(3); // type
   category->hideColumn(4); // des
   category->hideColumn(5); // stock
   //tableCat->hideColumn(6);// price
   category->hideColumn(7); // img
   category->hideColumn(8);
   category->hideColumn(9);
   category->hideColumn(10);

   category->resizeColumnsToContents();
   category->setColumnWidth(1, 200);
   category->setColumnWidth(2, 600);
   category->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);

   /*
    * banner
    */

   QTimer *bannerTimer = new QTimer(this);
   connect(bannerTimer, SIGNAL(timeout()), this, SLOT(changeBanner()));
   bannerTimer->start(1000);
}


MainWindow::~MainWindow()
{
   delete ui;
   closeDB();
}


bool MainWindow::openDB(const QString dbname)
{
   if (database.isOpen())
   {
      qDebug() << "MainWindow::openDB(): db is already open";
      return false;
   }

   database = QSqlDatabase::addDatabase("QSQLITE");
   database.setDatabaseName(dbname);

   if (!database.open())
   {
      qDebug() << "MainWindow::openDB(): db.open() failed";
      QMessageBox::critical(this, tr("错误"), tr("无法打开数据库！"));
      return false;
   }

   QSqlQuery q(database);
   q.exec("select * from productTypes");
   while (q.next())
   {
      ProductType type;
      type.setName(q.value("name").toString());
      type.setPrefix(q.value("prefix").toString());
      type.setDiscount(q.value("discount").toDouble());
      type.setCount(q.value("count").toInt());
      typeList.append(type);
   }

   return true;
}


void MainWindow::closeDB()
{
   database.close();
}


void MainWindow::on_actionAboutQt_triggered()
{
   QApplication::aboutQt();
}


/*
 *
 *  Change styles
 *
 */
void MainWindow::on_actionTaobao_triggered()
{
   QFile f(":/qss/taobao.qss");

   if (f.open(QFile::ReadOnly | QFile::Text))
   {
      QTextStream ts(&f);
      rApp.setStyleSheet(ts.readAll());
   }

   QSqlQuery q("update appsettings set lastTheme = 'taobao.qss'");
   if (!q.exec())
   {
      qDebug() << q.lastError();
      qDebug() << "MainWindow::on_actionTaobao_triggered(): update appsettings failed";
   }
   QPixmap pm;

   pm.load(":/pics/empty-cart-darkfont.png");
   ui->labelCartEmpty->setPixmap(pm);
}


void MainWindow::on_actionBlueMD_triggered()
{
   QFile f(":/qss/blue-md.qss");

   if (f.open(QFile::ReadOnly | QFile::Text))
   {
      QTextStream ts(&f);
      rApp.setStyleSheet(ts.readAll());
   }

   QSqlQuery q("update appsettings set lastTheme = 'blue-md.qss'");
   if (!q.exec())
   {
      qDebug() << q.lastError();
      qDebug() << "MainWindow::on_actionBlueMD_triggered(): update appsettings failed";
   }
   QPixmap pm;

   pm.load(":/pics/empty-cart-lightfont.png");
   ui->labelCartEmpty->setPixmap(pm);
}


void MainWindow::on_actionBlue_triggered()
{
   QFile f(":/qss/blue.qss");

   if (f.open(QFile::ReadOnly | QFile::Text))
   {
      QTextStream ts(&f);
      rApp.setStyleSheet(ts.readAll());
   }

   QSqlQuery q("update appsettings set lastTheme = 'blue.qss'");
   if (!q.exec())
   {
      qDebug() << q.lastError();
      qDebug() << "MainWindow::on_actionBlue_triggered(): update appsettings failed";
   }
   QPixmap pm;

   pm.load(":/pics/empty-cart-darkfont.png");
   ui->labelCartEmpty->setPixmap(pm);
}


void MainWindow::on_actionDarkVS_triggered()
{
   QFile f(":/qss/dark-vs.qss");

   if (f.open(QFile::ReadOnly | QFile::Text))
   {
      QTextStream ts(&f);
      rApp.setStyleSheet(ts.readAll());
   }

   QSqlQuery q("update appsettings set lastTheme = 'dark-vs.qss'");
   if (!q.exec())
   {
      qDebug() << q.lastError();
      qDebug() << "MainWindow::on_actionDarkVS_triggered(): update appsettings failed";
   }
   QPixmap pm;

   pm.load(":/pics/empty-cart-lightfont.png");
   ui->labelCartEmpty->setPixmap(pm);
}


void MainWindow::on_actionDark_triggered()
{
   QFile f(":/qss/dark.qss");

   if (f.open(QFile::ReadOnly | QFile::Text))
   {
      QTextStream ts(&f);
      rApp.setStyleSheet(ts.readAll());
   }

   QSqlQuery q("update appsettings set lastTheme = 'dark.qss'");
   if (!q.exec())
   {
      qDebug() << q.lastError();
      qDebug() << "MainWindow::on_actionDark_triggered(): update appsettings failed";
   }
   QPixmap pm;

   pm.load(":/pics/empty-cart-lightfont.png");
   ui->labelCartEmpty->setPixmap(pm);
}


void MainWindow::on_actionGray_triggered()
{
   QFile f(":/qss/gray.qss");

   if (f.open(QFile::ReadOnly | QFile::Text))
   {
      QTextStream ts(&f);
      rApp.setStyleSheet(ts.readAll());
   }

   QSqlQuery q("update appsettings set lastTheme = 'gray.qss'");
   if (!q.exec())
   {
      qDebug() << q.lastError();
      qDebug() << "MainWindow::on_actionGray_triggered(): update appsettings failed";
   }

   QPixmap pm;

   pm.load(":/pics/empty-cart-darkfont.png");
   ui->labelCartEmpty->setPixmap(pm);
}


void MainWindow::on_actionExit_triggered()
{
   close();
}


void MainWindow::closeEvent(QCloseEvent *ce)
{
   QMessageBox::StandardButton clickedButton;
   clickedButton = QMessageBox::question(this, tr("退出"), tr("您确定要退出吗？")
                                         , QMessageBox::Yes | QMessageBox::No
                                         , QMessageBox::Yes);
   if (clickedButton == QMessageBox::Yes)
   {
      ce->accept();
   }
   else
   {
      ce->ignore();
   }
}


/*
 *
 *  Home page
 *
 */
void MainWindow::on_actionHome_triggered()
{
   ui->stackedWidgetMain->setCurrentWidget(ui->pageHome);
}


void MainWindow::on_lineEditHomeSearch_returnPressed()
{
   ui->pushButtonHomeSearch->click();
}


void MainWindow::on_pushButtonHomeSearch_clicked()
{
   ui->lineEditCatSearch->setText(ui->lineEditHomeSearch->text());
   ui->stackedWidgetMain->setCurrentWidget(ui->pageCategory);
   ui->stackedWidgetCategory->setCurrentWidget(ui->pageProductAll);
   ui->pushButtonCatSearch->click();
}


void MainWindow::on_pushButtonViewClothes_clicked()
{
   ui->lineEditHomeSearch->setText(tr("衣服"));
   ui->pushButtonHomeSearch->click();
}


void MainWindow::on_pushButtonViewBooks_clicked()
{
   ui->lineEditHomeSearch->setText(tr("图书"));
   ui->pushButtonHomeSearch->click();
}


void MainWindow::on_pushButtonViewFoods_clicked()
{
   ui->lineEditHomeSearch->setText(tr("食品"));
   ui->pushButtonHomeSearch->click();
}


/*
 *
 *  Category page
 *
 */
void MainWindow::on_actionCategory_triggered()
{
   ui->stackedWidgetMain->setCurrentWidget(ui->pageCategory);
   ui->stackedWidgetCategory->setCurrentWidget(ui->pageProductAll);
}


void MainWindow::on_lineEditCatSearch_returnPressed()
{
   ui->pushButtonCatSearch->click();
}


void MainWindow::on_lineEditCatSearch_textEdited(const QString& arg1)
{
   categoryTableModel->setFilter("name like '%" + arg1 + "%' " +
                                 "or type like '%" + arg1 + "%' " +
                                 "or description like '%" + arg1 + "%' ");
}


void MainWindow::on_pushButtonCatSearch_clicked()
{
   QString search = ui->lineEditCatSearch->text();

   categoryTableModel->setFilter("name like '%" + search + "%' " +
                                 "or type like '%" + search + "%' " +
                                 "or description like '%" + search + "%' ");
}


void MainWindow::on_tableViewCategory_clicked(const QModelIndex& index)
{
   QString   pid    = index.sibling(index.row(), 0).data().toString();
   QString   prefix = pid.left(3);
   QSqlQuery q;

   if (!q.exec("select * from products where id = '" + pid + "'"))
   {
      qDebug() << "MainWindow::on_tableViewCategory_clicked: exec failed";
   }
   if (!q.next())
   {
      qDebug() << "MainWindow::on_tableViewCategory_clicked: next failed";
   }

   QPixmap    pImgPm;
   QByteArray pImgBytes = q.value("image").toByteArray();
   if (pImgBytes.isEmpty())
   {
      pImgPm.load(":/icons-shop/basket.png");
   }
   else
   {
      pImgPm.loadFromData(pImgBytes);
   }
   // left
   ui->labelPImage->setPixmap(pImgPm.scaled(400, 400));

   // upper right
   ui->labelPName->setText(q.value("name").toString());
   ui->labelPDescription->setText(q.value("description").toString());

   // bottom right
   ui->labelPID->setText(q.value("id").toString());
   ui->labelPType->setText(q.value("type").toString());
   ui->labelPStock->setText(q.value("stock").toString());
   ui->labelPPrice->setText("¥" + q.value("price").toString());
   int stock = q.value("stock").toInt();
   if (stock <= 0)
   {
      ui->spinBoxPNumber->setValue(0);
      ui->spinBoxPNumber->setMinimum(0);
      ui->spinBoxPNumber->setMaximum(0);
   }
   else
   {
      ui->spinBoxPNumber->setValue(1);
      ui->spinBoxPNumber->setMinimum(1);
      ui->spinBoxPNumber->setMaximum(stock);
   }

   // load discount info
   q.prepare("select * from productTypes where prefix = :p");
   q.bindValue(":p", prefix);
   q.exec();
   q.next();
   QString dis = q.value("discount").toString();
   ui->labelPDiscount->setText(dis);
   if (dis == "1")
   {
      ui->labelPDiscountText->hide();
      ui->labelPDiscount->hide();
   }
   else
   {
      ui->labelPDiscountText->show();
      ui->labelPDiscount->show();
   }


   // prevent mem leaks
//   static QWidget *handle;
//   if (handle != nullptr)
//   {
//      delete handle;
//   }
//   handle = new QWidget;

//   if (ui->gridLayoutProductDetails != nullptr)
//   {
//      delete ui->gridLayoutProductDetails;
//   }
//   ui->gridLayoutProductDetails = new QGridLayout();
//   ui->gridLayoutProductDetails->setSpacing(6);
//   ui->gridLayoutProductDetails->setObjectName(QStringLiteral("gridLayoutProductDetails"));
//   ui->gridLayoutProductDetails->setSizeConstraint(QLayout::SetMaximumSize);
//   ui->gridLayoutProductDetails->setContentsMargins(-1, 10, -1, 10);
//   ui->verticalLayout_20->addLayout(ui->gridLayoutProductDetails);
   // load details
   if (prefix == "001")
   {  // books
      q.prepare("select * from productBooks where id = :i");
      q.bindValue(":i", pid);
      q.exec();
      q.next();

      Book bk;

      bk.setName(q.value("name").toString());
      bk.setIsbn(q.value("isbn").toString());
      bk.setPages(q.value("pages").toString());
      bk.setAuthor(q.value("author").toString());
      bk.setPuber(q.value("puber").toString());
      bk.setPubver(q.value("pubver").toString());

      ui->labelPD00->setText(tr("书名：") + bk.getName());
      ui->labelPD01->setText(tr("ISBN：") + bk.getIsbn());
      ui->labelPD02->setText(tr("书页：") + bk.getPages());
      ui->labelPD10->setText(tr("作者：") + bk.getAuthor());
      ui->labelPD11->setText(tr("出版社：") + bk.getPuber());
      ui->labelPD12->setText(tr("版次：") + bk.getPubver());
//      QLabel *l = new QLabel(tr("书名：") + bk.getName(), handle);
//      l->setMaximumSize(300, 50);
//      l->setWordWrap(true);
//      ui->gridLayoutProductDetails->addWidget(l, 0, 0);

//      l = new QLabel(tr("ISBN：") + bk.getIsbn(), handle);
//      l->setMaximumSize(300, 50);
//      l->setWordWrap(true);
//      ui->gridLayoutProductDetails->addWidget(l, 0, 1);

//      l = new QLabel(tr("书页：") + bk.getPages(), handle);
//      l->setMaximumSize(300, 50);
//      l->setWordWrap(true);
//      ui->gridLayoutProductDetails->addWidget(l, 0, 2);

//      l = new QLabel(tr("作者：") + bk.getAuthor(), handle);
//      l->setMaximumSize(300, 50);
//      l->setWordWrap(true);
//      ui->gridLayoutProductDetails->addWidget(l, 1, 0);

//      l = new QLabel(tr("出版社：") + bk.getPuber(), handle);
//      l->setMaximumSize(300, 50);
//      l->setWordWrap(true);
//      ui->gridLayoutProductDetails->addWidget(l, 1, 1);

//      l = new QLabel(tr("版次：") + bk.getPubver(), handle);
//      l->setMaximumSize(300, 50);
//      l->setWordWrap(true);
//      ui->gridLayoutProductDetails->addWidget(l, 1, 2);
   }
   else if (prefix == "002")
   {  // clothes
      q.prepare("select * from productClothes where id = :i");
      q.bindValue(":i", pid);
      q.exec();
      q.next();

      Clothes cl;

      cl.setBrand(q.value("brand").toString());
      cl.setColor(q.value("color").toString());
      cl.setSize(q.value("size").toString());
      cl.setTexture(q.value("texture").toString());

      ui->labelPD00->setText(tr("品牌：") + cl.getBrand());
      ui->labelPD01->setText(tr("颜色：") + cl.getColor());
      ui->labelPD02->setText(tr("尺寸：") + cl.getSize());
      ui->labelPD10->setText(tr("材质：") + cl.getTexture());
      ui->labelPD11->setText("");
      ui->labelPD12->setText("");

//      ui->gridLayoutProductDetails->addWidget(new QLabel(tr("品牌：") + cl.getBrand(), handle), 0, 0);
//      ui->gridLayoutProductDetails->addWidget(new QLabel(tr("颜色：") + cl.getColor(), handle), 0, 1);
//      ui->gridLayoutProductDetails->addWidget(new QLabel("", handle), 0, 2);
//      ui->gridLayoutProductDetails->addWidget(new QLabel(tr("尺寸：") + cl.getSize(), handle), 1, 0);
//      ui->gridLayoutProductDetails->addWidget(new QLabel(tr("材质：") + cl.getTexture(), handle), 1, 1);
//      ui->gridLayoutProductDetails->addWidget(new QLabel("", handle), 1, 2);

//      QLabel *l = new QLabel(tr("品牌：") + cl.getBrand(), handle);
//      l->setMaximumSize(300, 50);
//      l->setWordWrap(true);
//      ui->gridLayoutProductDetails->addWidget(l, 0, 0);

//      l = new QLabel(tr("颜色：") + cl.getColor(), handle);
//      l->setMaximumSize(300, 50);
//      l->setWordWrap(true);
//      ui->gridLayoutProductDetails->addWidget(l, 0, 1);

//      l = new QLabel(tr("尺寸：") + cl.getSize(), handle);
//      l->setMaximumSize(300, 50);
//      l->setWordWrap(true);
//      ui->gridLayoutProductDetails->addWidget(l, 0, 2);

//      l = new QLabel(tr("材质：") + cl.getTexture(), handle);
//      l->setMaximumSize(300, 50);
//      l->setWordWrap(true);
//      ui->gridLayoutProductDetails->addWidget(l, 1, 0);

//      l = new QLabel("", handle);
//      l->setMaximumSize(300, 50);
//      l->setWordWrap(true);
//      ui->gridLayoutProductDetails->addWidget(l, 1, 1);

//      l = new QLabel("", handle);
//      l->setMaximumSize(300, 50);
//      l->setWordWrap(true);
//      ui->gridLayoutProductDetails->addWidget(l, 1, 2);
   }
   else if (prefix == "003")
   {  // foods
      q.prepare("select * from productFoods where id = :i");
      q.bindValue(":i", pid);
      q.exec();
      q.next();

      Food fd;

      fd.setBrand(q.value("brand").toString());
      fd.setName(q.value("name").toString());
      fd.setFlavor(q.value("flavor").toString());
      fd.setPeriod(q.value("period").toString());
      fd.setNetwgt(q.value("netwgt").toString());

      ui->labelPD00->setText(tr("品牌：") + fd.getBrand());
      ui->labelPD01->setText(tr("品名：") + fd.getName());
      ui->labelPD02->setText(tr("口味：") + fd.getFlavor());
      ui->labelPD10->setText(tr("保质期：") + fd.getPeriod());
      ui->labelPD11->setText(tr("净含量：") + fd.getNetwgt());
      ui->labelPD12->setText("");

//      ui->gridLayoutProductDetails->addWidget(new QLabel(tr("品牌：") + fd.getBrand(), handle), 0, 0);
//      ui->gridLayoutProductDetails->addWidget(new QLabel(tr("品名：") + fd.getName(), handle), 0, 1);
//      ui->gridLayoutProductDetails->addWidget(new QLabel(tr("口味：") + fd.getFlavor(), handle), 0, 2);
//      ui->gridLayoutProductDetails->addWidget(new QLabel(tr("保质期：") + fd.getPeriod(), handle), 1, 0);
//      ui->gridLayoutProductDetails->addWidget(new QLabel(tr("净含量：") + fd.getNetwgt(), handle), 1, 1);
//      ui->gridLayoutProductDetails->addWidget(new QLabel("", handle), 1, 2);

//      QLabel *l = new QLabel(tr("品牌：") + fd.getBrand(), handle);
//      l->setMaximumSize(300, 50);
//      l->setWordWrap(true);
//      ui->gridLayoutProductDetails->addWidget(l, 0, 0);

//      l = new QLabel(tr("品名：") + fd.getName(), handle);
//      l->setMaximumSize(300, 50);
//      l->setWordWrap(true);
//      ui->gridLayoutProductDetails->addWidget(l, 0, 1);

//      l = new QLabel(tr("口味：") + fd.getFlavor(), handle);
//      l->setMaximumSize(300, 50);
//      l->setWordWrap(true);
//      ui->gridLayoutProductDetails->addWidget(l, 0, 2);

//      l = new QLabel(tr("保质期：") + fd.getPeriod(), handle);
//      l->setMaximumSize(300, 50);
//      l->setWordWrap(true);
//      ui->gridLayoutProductDetails->addWidget(l, 1, 0);

//      l = new QLabel(tr("净含量：") + fd.getNetwgt(), handle);
//      l->setMaximumSize(300, 50);
//      l->setWordWrap(true);
//      ui->gridLayoutProductDetails->addWidget(l, 1, 1);

//      l = new QLabel("", handle);
//      l->setMaximumSize(300, 50);
//      l->setWordWrap(true);
//      ui->gridLayoutProductDetails->addWidget(l, 1, 2);
   }

//   ui->gridLayoutProductDetails->setSizeConstraint(QLayout::SetMaximumSize);

//   ui->gridLayoutProductDetails->setColumnMinimumWidth(0, 300);
//   ui->gridLayoutProductDetails->setColumnMinimumWidth(1, 300);
//   ui->gridLayoutProductDetails->setColumnMinimumWidth(2, 300);

//   ui->gridLayoutProductDetails->setColumnStretch(0, 1);
//   ui->gridLayoutProductDetails->setColumnStretch(1, 1);
//   ui->gridLayoutProductDetails->setColumnStretch(2, 1);

   ui->stackedWidgetCategory->setCurrentWidget(ui->pageProductSingle);
}


void MainWindow::on_pushButtonCatBack_clicked()
{
   ui->stackedWidgetCategory->setCurrentWidget(ui->pageProductAll);
}


void MainWindow::on_pushButtonPBuyNow_clicked()
{
   int stock = ui->labelPStock->text().toInt();

   if (stock <= 0)
   {
      QMessageBox::warning(this, tr("立即购买"), tr("库存不足！"));
      return;
   }

   if (!currentAccount.isLogined())
   {
      QMessageBox::warning(this, tr("立即购买"), tr("您还没有登录哦！"));
      ui->stackedWidgetMain->setCurrentWidget(ui->pageAccount);
      return;
   }

   checkoutList.clear();

   QSqlQuery     q;
   ProductInCart pro;
   QString       id = ui->labelPID->text();

   q.exec("select * from products where id = '" + id + "'");
   q.next();

   QPixmap pm;
   pm.loadFromData(q.value("image").toByteArray());
   pro.setImage(pm);
   pro.setId(id);
   pro.setPname(q.value("name").toString());
   pro.setPriceOri(q.value("price").toDouble());
   pro.setCount(ui->spinBoxPNumber->value());

   QString prefix = id.left(3);
   q.exec("select * from productTypes where prefix = '" + prefix + "'");
   q.next();
   pro.setTypeDiscount(q.value("discount").toDouble());

   checkoutList.append(pro);

   // Combo box addresses
   QStringList all;
   QStringList alist     = currentAccount.getAddrList();
   QStringList plist     = currentAccount.getPcodeList();
   QStringList rrlist    = currentAccount.getRecverList();
   QStringList rplist    = currentAccount.getRecvphList();
   int         addrCount = alist.size();
   for (int i = 0; i < addrCount; ++i)
   {
      all.append(alist.at(i) + ";" + plist.at(i) + ";" + rrlist.at(i) + ";" + rplist.at(i));
   }
   ui->comboBoxCOAddr->clear();
   ui->comboBoxCOAddr->addItems(all);

   // Table Checkout

   static QWidget *handle;
   if (handle != nullptr)
   {
      delete handle;
   }
   handle = new QWidget;

   QTableWidget *tableCheckout = ui->tableWidgetCheckout;
   QVectorIterator<ProductInCart> i(checkoutList);
   int row = 0, col = 0;
   while (i.hasNext())
   {
      ProductInCart pro = i.next();

      tableCheckout->setRowCount(row + 1);

      // col 0: image
      QLabel *label = new QLabel(handle);
      label->setPixmap(pro.getImage().scaled(100, 100, Qt::KeepAspectRatio));
      tableCheckout->setCellWidget(row, col++, label);

      // col 1: id
      tableCheckout->setItem(row, col++, new QTableWidgetItem(pro.getId()));

      // col 2: name
      tableCheckout->setItem(row, col++, new QTableWidgetItem(pro.getPname()));

      // col 3: price single
      tableCheckout->setItem(row, col++, new QTableWidgetItem(QString::number(pro.getPrice(), 'f', 2)));

      // col 4: count
      tableCheckout->setItem(row, col++, new QTableWidgetItem(QString::number(pro.getCount())));

      // col 5: discount
      tableCheckout->setItem(row, col++, new QTableWidgetItem(QString::number(pro.getTypeDiscount(), 'f', 2)));

      // col 6: price total
      tableCheckout->setItem(row, col++, new QTableWidgetItem(QString::number(pro.getPrice() * pro.getCount(), 'f', 2)));

      ++row;
      col = 0;
   }

   tableCheckout->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);
   tableCheckout->horizontalHeader()->show();

   tableCheckout->resizeColumnsToContents();
   tableCheckout->setColumnWidth(0, 100);  // image
   tableCheckout->setColumnWidth(2, 500);  // name

   double total = 0;
   for (int i = 0; i < row; ++i)
   {
      total += tableCheckout->item(i, 6)->text().toDouble();
   }
   ui->labelCOPriceTotal->setText(tr("总金额：") + QString::number(total, 'f', 2));

   ui->stackedWidgetMain->setCurrentWidget(ui->pageCart);
   ui->stackedWidgetCart->setCurrentWidget(ui->pageCheckout);
}


void MainWindow::on_pushButtonPAddToCart_clicked()
{
   int stock = ui->labelPStock->text().toInt();

   if (stock <= 0)
   {
      QMessageBox::warning(this, tr("加入购物车"), tr("库存不足！"));
      return;
   }

   int addCount = ui->spinBoxPNumber->value();

   if (addCount <= 0)
   {
      QMessageBox::warning(this, tr("加入购物车"), tr("数量输入有误！"));
      return;
   }

   QString currPID = ui->labelPID->text();

   for (QVector<ProductInCart>::iterator i = cartList.begin(); i != cartList.end(); ++i)
   {
      if (i->getId() == currPID)
      {
         i->setCount(i->getCount() + addCount);
         QMessageBox::information(this, tr("购物车"), tr("加入购物车成功！"));

         return;
      }
   }

   ProductInCart pro;
   pro.setImage(*(ui->labelPImage->pixmap()));
   pro.setId(ui->labelPID->text());
   pro.setPname(ui->labelPName->text());
   pro.setDescription(ui->labelPDescription->text());
   pro.setStock(ui->labelPStock->text().toInt());
   pro.setType(ui->labelPType->text());
   pro.setTypeDiscount(ui->labelPDiscount->text().toDouble());
   pro.setPriceOri(ui->labelPPrice->text().remove("¥").toDouble());
   pro.setCount(pro.getCount() + ui->spinBoxPNumber->value());

   cartList.append(pro);

   QMessageBox::information(this, tr("购物车"), tr("加入购物车成功！"));
}


/*
 *
 * Cart Page
 *
 */
void MainWindow::on_actionCart_triggered()
{
   // no product in cart
   if (cartList.isEmpty())
   {
      ui->widgetCartButtons->hide();
      ui->tableWidgetCart->hide();
      ui->labelCartEmpty->show();

      ui->stackedWidgetMain->setCurrentWidget(ui->pageCart);

      return;
   }

   // display products
   QTableWidget *tableCart = ui->tableWidgetCart;

   ui->labelCartEmpty->hide();
   tableCart->show();
   ui->widgetCartButtons->show();

   ui->checkBoxCartSelAll->setChecked(false);
   tableCart->setRowCount(0);
   tableCart->setColumnCount(9);

//   if (sboxMapper != nullptr)
//   {
//      delete sboxMapper;
//   }
//   if (dbtnMapper != nullptr)
//   {
//      delete dbtnMapper;
//   }

//   static QVector<QSpinBox *>    sboxes;
//   static QVector<QPushButton *> delbtns;
//   for (QVector<QSpinBox *>::iterator i = sboxes.begin(); i != sboxes.end(); ++i)
//   {
//      delete *i;
//   }
//   sboxes.clear();
//   for (QVector<QPushButton *>::iterator i = delbtns.begin(); i != delbtns.end(); ++i)
//   {
//      delete *i;
//   }
//   delbtns.clear();

   // prevent memory leaks
   static QWidget *handle;
   if (handle != nullptr)
   {
      delete handle;
   }
   handle = new QWidget();

   QSignalMapper *sboxMapper = new QSignalMapper(handle);
   QSignalMapper *dbtnMapper = new QSignalMapper(handle);
   QSignalMapper *cboxMapper = new QSignalMapper(handle);
   QVectorIterator<ProductInCart> i(cartList);
   int row = 0, col = 0;
   while (i.hasNext())
   {
      ProductInCart pro = i.next();

      tableCart->setRowCount(row + 1);
      // col 0: checkbox
      QCheckBox *cbox = new QCheckBox(handle);
      connect(cbox, SIGNAL(toggled(bool)), cboxMapper, SLOT(map()));
      cboxMapper->setMapping(cbox, row);
      tableCart->setCellWidget(row, col++, cbox);

      // col 1: image
      QLabel *label = new QLabel(handle);
      label->setPixmap(pro.getImage().scaled(100, 100, Qt::KeepAspectRatio));
      tableCart->setCellWidget(row, col++, label);

      // col 2: id
      tableCart->setItem(row, col++, new QTableWidgetItem(pro.getId()));

      // col 3: name
      tableCart->setItem(row, col++, new QTableWidgetItem(pro.getPname()
                                                          + tr("（库存")
                                                          + QString::number(pro.getStock())
                                                          + tr("件）")));

      // col 4: type
      tableCart->setItem(row, col++, new QTableWidgetItem(pro.getType()));

      // col 5: price single
      QString priceSingle = QString::number(pro.getPrice(), 'f', 2)
                            + tr("（原价") + QString::number(pro.getPriceOri(), 'f', 2) + tr("）");
      tableCart->setItem(row, col++, new QTableWidgetItem(priceSingle));

      // col 6: spinbox count
      QSpinBox *box = new QSpinBox(handle);
//      sboxes.append(box);
      box->setMinimum(1);
      box->setMaximum(pro.getStock());
      box->setValue(pro.getCount());
      tableCart->setCellWidget(row, col++, box);
      connect(box, SIGNAL(valueChanged(int)), sboxMapper, SLOT(map()));
      sboxMapper->setMapping(box, row);

      // col 7: price total
      tableCart->setItem(row, col++, new QTableWidgetItem(QString::number(pro.getPrice() * pro.getCount(), 'f', 2)));

      // col 8: del button
      QPushButton *delbtn = new QPushButton(tr("删除"), handle);
//      delbtns.append(delbtn);
      tableCart->setCellWidget(row, col++, delbtn);
      connect(delbtn, SIGNAL(clicked()), dbtnMapper, SLOT(map()));
      dbtnMapper->setMapping(delbtn, row);

      ++row;
      col = 0;
   }

   // important: final step!
   connect(sboxMapper, SIGNAL(mapped(int)), this, SLOT(adjustSingleProductCountInCart(int)));
   connect(dbtnMapper, SIGNAL(mapped(int)), this, SLOT(removeSingleProductFromCart(int)));
   connect(cboxMapper, SIGNAL(mapped(int)), this, SLOT(selectSingleProductInCart(int)));

   tableCart->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);
   tableCart->horizontalHeader()->show();

   tableCart->resizeColumnsToContents();
   tableCart->setColumnWidth(0, 25);  // checkbox
   tableCart->setColumnWidth(1, 100); // image
   tableCart->setColumnWidth(3, 400); // name
   tableCart->setColumnWidth(7, 100); // price total

//   QStringList headers;
//   headers << tr("选择") << tr("图片") << tr("货号") << tr("品名")
//           << tr("类型") << tr("单价") << tr("数量") << tr("小计") << tr("删除");
//   tableCart->setHorizontalHeaderLabels(headers);
   cartPriceTotal = 0;
   ui->labelCartTotalPrice->setText(tr("总金额：0.00"));

   if (oaList.isEmpty())
   {
      ui->labelOffAbove->hide();
   }
   else
   {
      QVectorIterator<OffAbove> ii(oaList);
      QString     oastr = tr("满减活动进行中！");
      QStringList oastl;
      while (ii.hasNext())
      {
         OffAbove oa = ii.next();
         oastl << (tr("满") + oa.above + tr("减") + oa.off);
      }
      oastr.append(oastl.join(";"));
      ui->labelOffAbove->setText(oastr);
   }


   ui->stackedWidgetCart->setCurrentWidget(ui->pageCartInner);
   ui->stackedWidgetMain->setCurrentWidget(ui->pageCart);
}


void MainWindow::removeSingleProductFromCart(int row)
{
   QString selectedID = ui->tableWidgetCart->item(row, 2)->data(Qt::DisplayRole).toString();

   //::iterator = r/w pointer
   //::const_iterator = r/o pointer
   for (QVector<ProductInCart>::iterator i = cartList.begin(); i != cartList.end(); ++i)
   {
      if (i->getId() == selectedID)
      {
         cartList.erase(i);
         ui->tableWidgetCart->removeRow(row);
         if (cartList.isEmpty())
         {
            ui->labelCartEmpty->show();

            ui->tableWidgetCart->hide();
            ui->widgetCartButtons->hide();
         }
         return;
      }
   }
}


void MainWindow::adjustSingleProductCountInCart(int row)
{
   QTableWidget *tableCart = ui->tableWidgetCart;
   int          tableRows  = tableCart->rowCount();
   QString      selectedID = tableCart->item(row, 2)->data(Qt::DisplayRole).toString();
   QSpinBox     *box       = qobject_cast<QSpinBox *>(ui->tableWidgetCart->cellWidget(row, 6));

   for (QVector<ProductInCart>::iterator i = cartList.begin(); i != cartList.end(); ++i)
   {
      if (i->getId() == selectedID)
      {
         i->setCount(box->value());
         tableCart->item(row, 7)->setData(Qt::DisplayRole,
                                          QString::number((i->getPrice()) * (i->getCount()), 'f', 2));
         break;
      }
   }

   cartPriceTotal = 0;
   for (int i = 0; i < tableRows; ++i)
   {
      QCheckBox *box = qobject_cast<QCheckBox *>(tableCart->cellWidget(i, 0));
      if (box->isChecked())
      {
         cartPriceTotal += tableCart->item(row, 7)->text().toDouble();
      }
   }

   ui->labelCartTotalPrice->setText(tr("总金额：") + QString::number(cartPriceTotal, 'f', 2));
}


void MainWindow::selectSingleProductInCart(int row)
{
   QTableWidget *tableCart = ui->tableWidgetCart;

   QCheckBox *box = qobject_cast<QCheckBox *>(tableCart->cellWidget(row, 0));

   if (box->isChecked())
   {
      cartPriceTotal += tableCart->item(row, 7)->text().toDouble();
   }
   else
   {
      cartPriceTotal -= tableCart->item(row, 7)->text().toDouble();
   }

   if (cartPriceTotal < 0)
   {
      cartPriceTotal = 0;
   }
   ui->labelCartTotalPrice->setText(tr("总金额：") + QString::number(cartPriceTotal, 'f', 2));
}


void MainWindow::changeBanner()
{
   QPixmap pm;

   qsrand(QDateTime::currentMSecsSinceEpoch());
   int sel = qrand() % 3;

   switch (sel)
   {
   case 0:
      pm.load(":/pics/banner-books.png");
      break;

   case 1:
      pm.load(":/pics/banner-clothes.png");
      break;

   case 2:
      pm.load(":/pics/banner-foods.png");
      break;

   default:
      break;
   }

   ui->labelBanner->setPixmap(pm);
}


void MainWindow::on_pushButtonCartDelete_clicked()
{
   QTableWidget *tableCart = ui->tableWidgetCart;
   int          tableRows  = tableCart->rowCount();
   int          count      = 0;

   for (int i = tableRows - 1; i >= 0; --i)
   {
      QCheckBox *box = qobject_cast<QCheckBox *>(tableCart->cellWidget(i, 0));

      if (box->isChecked())
      {
         ++count;
         removeSingleProductFromCart(i);
      }
   }
   if (count == 0)
   {
      QMessageBox::warning(this, tr("删除"), tr("您没有选中任何商品哦！"));
   }
}


void MainWindow::on_checkBoxCartSelAll_toggled(bool checked)
{
   QTableWidget *tableCart = ui->tableWidgetCart;
   int          tableRows  = tableCart->rowCount();

   if (checked)
   {
      for (int i = 0; i < tableRows; ++i)
      {
         QCheckBox *box = qobject_cast<QCheckBox *>(tableCart->cellWidget(i, 0));
         box->setChecked(true);
      }
   }
   else
   {
      for (int i = 0; i < tableRows; ++i)
      {
         QCheckBox *box = qobject_cast<QCheckBox *>(tableCart->cellWidget(i, 0));
         box->setChecked(false);
      }
   }
}


void MainWindow::on_tableWidgetCart_cellClicked(int row, int column)
{
   Q_UNUSED(column);

   QTableWidget *tableCart = ui->tableWidgetCart;
   QString      selectedID = tableCart->item(row, 2)->text();
   QSqlQuery    q;

   if (!q.exec("select * from products where id = '" + selectedID + "'"))
   {
      qDebug() << "MainWindow::on_tableWidgetCart_cellClicked: exec failed";
   }
   if (!q.next())
   {
      qDebug() << "MainWindow::on_tableWidgetCart_cellClicked: next failed";
   }

   QPixmap    pImgPm;
   QByteArray pImgBytes = q.value("image").toByteArray();
   if (pImgBytes.isEmpty())
   {
      pImgPm.load(":/icons-shop/basket.png");
   }
   else
   {
      pImgPm.loadFromData(pImgBytes);
   }
   ui->labelPImage->setPixmap(pImgPm);

   ui->labelPName->setText(q.value("name").toString());
   ui->labelPDescription->setText(q.value("description").toString());

   ui->labelPID->setText(q.value("id").toString());
   ui->labelPType->setText(q.value("type").toString());
   ui->labelPStock->setText(q.value("stock").toString());
   ui->labelPPrice->setText("¥" + q.value("price").toString());
   ui->spinBoxPNumber->setValue(1);

   ui->stackedWidgetCategory->setCurrentWidget(ui->pageProductSingle);
   ui->stackedWidgetMain->setCurrentWidget(ui->pageCategory);
}


void MainWindow::on_pushButtonBackToCart_clicked()
{
   on_actionCart_triggered();
   ui->checkBoxCartSelAll->setChecked(false);
   ui->stackedWidgetCart->setCurrentWidget(ui->pageCartInner);
}


void MainWindow::on_pushButtonCartCheckout_clicked()
{
   QTableWidget *tableCart = ui->tableWidgetCart;
   int          tableRows  = tableCart->rowCount();

   if (!currentAccount.isLogined())
   {
      QMessageBox::warning(this, tr("结算"), tr("您还没有登录哦！"));
      ui->stackedWidgetMain->setCurrentWidget(ui->pageAccount);
      return;
   }

   checkoutList.clear();
   for (int i = 0; i < tableRows; ++i)
   {
      QCheckBox *box = qobject_cast<QCheckBox *>(tableCart->cellWidget(i, 0));
      if (box->isChecked())
      {
         QSqlQuery     q;
         ProductInCart pro;
         QString       id = tableCart->item(i, 2)->text();

         q.exec("select * from products where id = '" + id + "'");
         q.next();

         pro.setImage(*(qobject_cast<QLabel *>(tableCart->cellWidget(i, 1))->pixmap()));
         pro.setId(id);
         pro.setPname(q.value("name").toString());
         pro.setPriceOri(q.value("price").toDouble());
         pro.setCount(qobject_cast<QSpinBox *>(tableCart->cellWidget(i, 6))->value());

         QString prefix = id.left(3);
         q.exec("select * from productTypes where prefix = '" + prefix + "'");
         q.next();
         pro.setTypeDiscount(q.value("discount").toDouble());

         checkoutList.append(pro);
      }
   }

   if (checkoutList.isEmpty())
   {
      QMessageBox::warning(this, tr("结算"), tr("未选中任何商品！"));
      return;
   }

   // Combo box addresses
   QStringList all;
   QStringList alist  = currentAccount.getAddrList();
   QStringList plist  = currentAccount.getPcodeList();
   QStringList rrlist = currentAccount.getRecverList();
   QStringList rplist = currentAccount.getRecvphList();
   int         count  = alist.size();
   for (int i = 0; i < count; ++i)
   {
      all.append(alist.at(i) + ";" + plist.at(i) + ";" + rrlist.at(i) + ";" + rplist.at(i));
   }
   ui->comboBoxCOAddr->clear();
   ui->comboBoxCOAddr->addItems(all);

   // Table Checkout
   static QWidget *handle;
   if (handle != nullptr)
   {
      delete handle;
   }
   handle = new QWidget;

   QTableWidget *tableCheckout = ui->tableWidgetCheckout;
   QVectorIterator<ProductInCart> i(checkoutList);
   int row = 0, col = 0;
   while (i.hasNext())
   {
      ProductInCart pro = i.next();

      tableCheckout->setRowCount(row + 1);

      // col 0: image
      QLabel *label = new QLabel(handle);
      label->setPixmap(pro.getImage().scaled(100, 100, Qt::KeepAspectRatio));
      tableCheckout->setCellWidget(row, col++, label);

      // col 1: id
      tableCheckout->setItem(row, col++, new QTableWidgetItem(pro.getId()));

      // col 2: name
      tableCheckout->setItem(row, col++, new QTableWidgetItem(pro.getPname()));

      // col 3: price single
      tableCheckout->setItem(row, col++, new QTableWidgetItem(QString::number(pro.getPrice(), 'f', 2)));

      // col 4: count
      tableCheckout->setItem(row, col++, new QTableWidgetItem(QString::number(pro.getCount())));

      // col 5: discount
      tableCheckout->setItem(row, col++, new QTableWidgetItem(QString::number(pro.getTypeDiscount(), 'f', 2)));

      // col 6: price total
      tableCheckout->setItem(row, col++, new QTableWidgetItem(QString::number(pro.getPrice() * pro.getCount(), 'f', 2)));

      ++row;
      col = 0;
   }

   tableCheckout->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);
   tableCheckout->horizontalHeader()->show();

   tableCheckout->resizeColumnsToContents();
   tableCheckout->setColumnWidth(0, 100); // image
   tableCheckout->setColumnWidth(2, 500); // name

   // Price total
   double total = 0;
   for (int i = 0; i < row; ++i)
   {
      total += tableCheckout->item(i, 6)->text().toDouble();
   }

   ui->labelCOPriceTotal->setText(tr("总金额：") + QString::number(total, 'f', 2));

   ui->stackedWidgetCart->setCurrentWidget(ui->pageCheckout);
}


void MainWindow::on_pushButtonCOPay_clicked()
{
   // create a new order
   // gen new order id
   qsrand(QDateTime::currentMSecsSinceEpoch());
   QString oid = QDateTime::currentDateTime().toString("yyyyMMddhhmmss")
                 + QString::number(qrand()).leftJustified(4, '0', true);

//   QStringList olist   = currentAccount.getOrderList();
//   QString     ostring = olist.join(";");
//   if (ostring.isEmpty())
//   {
//      ostring.append(oid);
//   }
//   else
//   {
//      ostring.append(";" + oid);
//   }
//   currentAccount.setOrderList(ostring.split(";"));

//   // write back
//   QSqlQuery q;
//   q.prepare("update accounts set orders = :o where id = :i");
//   q.bindValue(":o", ostring);
//   q.bindValue(":i", currentAccount.getId());
//   if (!q.exec())
//   {
//      qDebug() << "on_pushButtonCOPay_clicked(): update failed";
//      qDebug() << q.lastError();
//   }

   // construct order
   Order o;
   o.id         = oid;
   o.state      = "待支付";
   o.timeCreate = QDateTime::currentDateTime().toString("yyyy-MM-dd-hh:mm:ss");
   o.timePay    = "";

   // sava order address
   //all.append(alist.at(i) + ";" + plist.at(i) + ";" + rrlist.at(i) + ";" + rplist.at(i));
   QStringList a = ui->comboBoxCOAddr->currentText().split(";");
   o.address.address  = a.at(0);
   o.address.postcode = a.at(1);
   o.address.receiver = a.at(2);
   o.address.recphone = a.at(3);

   // save product info
   // and decrease stock
   int         count = ui->tableWidgetCheckout->rowCount();
   QStringList pnames, pprices, pdiscount, pcounts;
   QSqlQuery   q;
   for (int i = 0; i < count; ++i)
   {
      /* tableWidgetCheckout
       * 0=image
       * 1=id
       * 2=name
       * 3=price
       * 4=count
       * 5=discount
       */
      QString pid = ui->tableWidgetCheckout->item(i, 1)->text();
      QString pcs = ui->tableWidgetCheckout->item(i, 4)->text();

      q.prepare("select * from products where id = :i");
      q.bindValue(":i", pid);
      q.exec();
      q.next();

      int stock = q.value("stock").toInt();
      int pci   = pcs.toInt();
      stock -= pci;

      q.prepare("update products set stock = :s where id = :i");
      q.bindValue(":s", stock);
      q.bindValue(":i", pid);
      q.exec();

      pnames.append(ui->tableWidgetCheckout->item(i, 2)->text());
      pprices.append(ui->tableWidgetCheckout->item(i, 3)->text());
      pcounts.append(pcs);
      pdiscount.append(ui->tableWidgetCheckout->item(i, 5)->text());
   }
   o.pnames    = pnames.join(";");
   o.pprices   = pprices.join(";");
   o.pcounts   = pcounts.join(";");
   o.pdiscount = pdiscount.join(";");

   // write to db
   q.prepare("insert into"
             " accountOrders (id, uid, state, timeCreate, timePay, "
             " address, postcode, receiver, recphone, "
             " pnames, pprices, pdiscount, pcounts)"
             " values (:id, :uid, :state, :timeCreate, :timePay, "
             " :address, :postcode, :receiver, :recphone, "
             " :pnames, :pprices, :pdiscount, :pcounts)");
   q.bindValue(":id", o.id);
   q.bindValue(":uid", currentAccount.getId());
   q.bindValue(":state", o.state);
   q.bindValue(":timeCreate", o.timeCreate);
   q.bindValue(":timePay", o.timePay);

   q.bindValue(":address", o.address.address);
   q.bindValue(":postcode", o.address.postcode);
   q.bindValue(":receiver", o.address.receiver);
   q.bindValue(":recphone", o.address.recphone);

   q.bindValue(":pnames", o.pnames);
   q.bindValue(":pprices", o.pprices);
   q.bindValue(":pdiscount", o.pdiscount);
   q.bindValue(":pcounts", o.pcounts);

   if (!q.exec())
   {
      qDebug() << "on_pushButtonCOPay_clicked(): insert order failed";
      qDebug() << q.lastError();
   }

   if (currentAccount.getCardList().isEmpty())
   {
      QMessageBox::warning(this, tr("支付"), tr("您还没有绑定卡哦！（订单已提交，您可以在个人中心查看并支付）"));
      ui->stackedWidgetMain->setCurrentWidget(ui->pageAccount);
      on_pushButtonMyCards_clicked();
      return;
   }

   QString   cost = ui->labelCOPriceTotal->text();
   PayDialog pd(nullptr, currentAccount, cost, oaList);

   if (pd.exec() == QDialog::Accepted)
   {
      q.exec("update accountOrders "
             " set state = '待发货', timePay = '" + QDateTime::currentDateTime().toString("yyyy-MM-dd-hh:mm:ss") + "' " +
             " where id = '" + oid + "'");
//      QMessageBox::information(this, tr("支付"), tr("支付成功！"));
   }
   else
   {
      QMessageBox::information(this, tr("支付"), tr("支付未成功！请到个人中心重新支付。"));
   }

   ui->stackedWidgetCart->setCurrentWidget(ui->pageCartInner);
   ui->stackedWidgetMain->setCurrentWidget(ui->pageAccount);
   ui->stackedWidgetAccount->setCurrentWidget(ui->pageLogined);

   on_tabWidgetOrder_tabBarClicked(0);
}


/*
 *
 *  Account page
 *
 */
void MainWindow::on_actionAccount_triggered()
{
   static QWidget *handle;

   if (handle != nullptr)
   {
      delete handle;
   }
   handle = new QWidget;

   orderTableModel = new QSqlTableModel(handle);
   orderTableModel->setTable("accountOrders");
   orderTableModel->select();
   orderTableModel->setFilter("uid = '" + currentAccount.getId() + "'");
   orderTableModel->setHeaderData(2, Qt::Horizontal, tr("订单状态"), Qt::DisplayRole);
   orderTableModel->setHeaderData(3, Qt::Horizontal, tr("订单创建时间"), Qt::DisplayRole);
   orderTableModel->setHeaderData(9, Qt::Horizontal, tr("商品列表"), Qt::DisplayRole);

   ui->tableViewOrderAll->setModel(orderTableModel);
   ui->tableViewOrderAll->hideColumn(0);
   ui->tableViewOrderAll->hideColumn(1);
   ui->tableViewOrderAll->hideColumn(4);
   ui->tableViewOrderAll->hideColumn(5);
   ui->tableViewOrderAll->hideColumn(6);
   ui->tableViewOrderAll->hideColumn(7);
   ui->tableViewOrderAll->hideColumn(8);
   ui->tableViewOrderAll->hideColumn(10);
   ui->tableViewOrderAll->hideColumn(11);
   ui->tableViewOrderAll->hideColumn(12);
   ui->tableViewOrderAll->resizeColumnsToContents();
   ui->tableViewOrderAll->setColumnWidth(9, 500);

   ui->stackedWidgetMain->setCurrentWidget(ui->pageAccount);
}


void MainWindow::on_pushButtonRegister_clicked()
{
   ui->stackedWidgetAccount->setCurrentWidget(ui->pageRegister);
}


void MainWindow::on_pushButtonRegCancel_clicked()
{
   ui->stackedWidgetAccount->setCurrentWidget(ui->pageLogin);
}


void MainWindow::on_pushButtonLogout_clicked()
{
   currentAccount.setLogined(false);
   if (!ui->checkBoxRemPwd->isChecked())
   {
      ui->lineEditLoginPassword->clear();
   }
   if (!ui->checkBoxRemUid->isChecked())
   {
      ui->lineEditLoginName->clear();
   }
   ui->stackedWidgetAccount->setCurrentWidget(ui->pageLogin);
}


void MainWindow::on_lineEditLoginName_returnPressed()
{
   on_pushButtonLogin_clicked();
}


void MainWindow::on_lineEditLoginPassword_returnPressed()
{
   on_pushButtonLogin_clicked();
}


void MainWindow::on_pushButtonLogin_clicked()
{
   QString id = ui->lineEditLoginName->text().trimmed();
   QString pw = ui->lineEditLoginPassword->text().trimmed();

   QSqlQuery query;

   if (!query.exec("select * from accounts "
                   "where (id = '" + id + "' or email = '" + id + "' or phone = '" + id + "') " +
                   "and password = '" + pw + "'"))
   {
      QMessageBox::critical(this, tr("登录"), tr("数据库异常！"));
      return;
   }
   if (query.next())
   {
      // name
      QString name = query.value("name").toString();
      currentAccount.setName(name);
      ui->labelAccountName->setText(tr("欢迎，") + name);

      // avatar
      QPixmap    avatar;
      QByteArray avaBytes = query.value("avatar").toByteArray();
      if (avaBytes.isEmpty())
      {
         avatar.load(":/icons-shop/dollar.png");
      }
      else
      {
         avatar.loadFromData(avaBytes);
      }
      ui->labelAccountAvatar->setPixmap(avatar.scaled(100, 100));

      // other info
      currentAccount.setLogined(true);
      currentAccount.setEmail(query.value("email").toString());
      currentAccount.setPassword(query.value("password").toString());
      currentAccount.setId(query.value("id").toString());
      currentAccount.setPhone(query.value("phone").toString());

      currentAccount.setAddrList(query.value("addresses").toString().split(";", QString::SkipEmptyParts));
      currentAccount.setPcodeList(query.value("postcodes").toString().split(";", QString::SkipEmptyParts));
      currentAccount.setRecverList(query.value("receivers").toString().split(";", QString::SkipEmptyParts));
      currentAccount.setRecvphList(query.value("recphones").toString().split(";", QString::SkipEmptyParts));

      currentAccount.setCardList(query.value("cards").toString().split(";", QString::SkipEmptyParts));

      // write back
      if (ui->checkBoxRemUid->isChecked())
      {
         query.prepare("update appsettings set lastUid = :cardid");
         query.bindValue(":cardid", id);
         query.exec();
      }

      if (ui->checkBoxRemPwd->isChecked())
      {
         query.prepare("update appsettings set lastPwd = :pwd");
         query.bindValue(":pwd", pw);
         query.exec();
      }

      // show orders
      static QWidget *handle;
      if (handle != nullptr)
      {
         delete handle;
      }
      handle = new QWidget;

      /* 0=id
       * 1=uid
       * 2=state
       * 3=timeCreate
       * 4=timePay
       * 5=address
       * 6=postcode
       * 7=receriver
       * 8=recphone
       * 9=pnames
       * 10=pprices
       * 11=pdiscount
       * 12=pcounts
       */

      orderTableModel = new QSqlTableModel(handle);
      orderTableModel->setTable("accountOrders");
      orderTableModel->select();
      orderTableModel->setFilter("uid = '" + currentAccount.getId() + "'");
      orderTableModel->setHeaderData(2, Qt::Horizontal, tr("订单状态"), Qt::DisplayRole);
      orderTableModel->setHeaderData(3, Qt::Horizontal, tr("订单创建时间"), Qt::DisplayRole);
      orderTableModel->setHeaderData(9, Qt::Horizontal, tr("商品列表"), Qt::DisplayRole);

      ui->tableViewOrderAll->setModel(orderTableModel);
      ui->tableViewOrderAll->hideColumn(0);
      ui->tableViewOrderAll->hideColumn(1);
      ui->tableViewOrderAll->hideColumn(4);
      ui->tableViewOrderAll->hideColumn(5);
      ui->tableViewOrderAll->hideColumn(6);
      ui->tableViewOrderAll->hideColumn(7);
      ui->tableViewOrderAll->hideColumn(8);
      ui->tableViewOrderAll->hideColumn(10);
      ui->tableViewOrderAll->hideColumn(11);
      ui->tableViewOrderAll->hideColumn(12);
      ui->tableViewOrderAll->resizeColumnsToContents();
      ui->tableViewOrderAll->setColumnWidth(9, 500);

      ui->tableViewOrderToPay->setModel(orderTableModel);
      ui->tableViewOrderToPay->hideColumn(0);
      ui->tableViewOrderToPay->hideColumn(1);
      ui->tableViewOrderToPay->hideColumn(4);
      ui->tableViewOrderToPay->hideColumn(5);
      ui->tableViewOrderToPay->hideColumn(6);
      ui->tableViewOrderToPay->hideColumn(7);
      ui->tableViewOrderToPay->hideColumn(8);
      ui->tableViewOrderToPay->hideColumn(10);
      ui->tableViewOrderToPay->hideColumn(11);
      ui->tableViewOrderToPay->hideColumn(12);
      ui->tableViewOrderToPay->resizeColumnsToContents();
      ui->tableViewOrderToPay->setColumnWidth(9, 500);

      ui->tableViewOrderToShip->setModel(orderTableModel);
      ui->tableViewOrderToShip->hideColumn(0);
      ui->tableViewOrderToShip->hideColumn(1);
      ui->tableViewOrderToShip->hideColumn(4);
      ui->tableViewOrderToShip->hideColumn(5);
      ui->tableViewOrderToShip->hideColumn(6);
      ui->tableViewOrderToShip->hideColumn(7);
      ui->tableViewOrderToShip->hideColumn(8);
      ui->tableViewOrderToShip->hideColumn(10);
      ui->tableViewOrderToShip->hideColumn(11);
      ui->tableViewOrderToShip->hideColumn(12);
      ui->tableViewOrderToShip->resizeColumnsToContents();
      ui->tableViewOrderToShip->setColumnWidth(9, 500);

      // switch
      ui->tabWidgetOrder->setCurrentWidget(ui->tabOrderAll);
      ui->stackedWidgetAccount->setCurrentWidget(ui->pageLogined);
      return;
   }

   QMessageBox::warning(this, tr("登录"), tr("登录信息有误！"));
   ui->lineEditLoginName->setFocus();
}


void MainWindow::on_checkBoxRegShowPwd_clicked(bool checked)
{
   if (checked)
   {
      ui->lineEditRegPwd->setEchoMode(QLineEdit::Normal);
   }
   else
   {
      ui->lineEditRegPwd->setEchoMode(QLineEdit::Password);
   }
}


void MainWindow::on_pushButtonRegConfirm_clicked()
{
   QString id    = ui->lineEditRegID->text().trimmed();
   QString email = ui->lineEditRegEmail->text().trimmed();
   QString phone = ui->lineEditRegPhone->text().trimmed();
   QString pwd   = ui->lineEditRegPwd->text().trimmed();

   QString name = ui->lineEditRegName->text().trimmed();
   QString addr = ui->lineEditRegAddr->text().trimmed();
   QString code = ui->lineEditRegPostCode->text().trimmed();

   QSqlQuery query;

   query.exec("select * from accounts where id = '" + id + "' ");
   if (query.next())
   {
      QMessageBox::warning(this, tr("注册"), tr("ID已存在！"));
      ui->lineEditRegID->setFocus();
      return;
   }

   query.exec("select * from accounts where email = '" + email + "' ");
   if (query.next())
   {
      QMessageBox::warning(this, tr("注册"), tr("邮箱已存在！"));
      ui->lineEditRegID->setFocus();
      return;
   }

   query.exec("select * from accounts where phone = '" + phone + "' ");
   if (query.next())
   {
      QMessageBox::warning(this, tr("注册"), tr("手机已存在！"));
      ui->lineEditRegID->setFocus();
      return;
   }

   if (id.length() < 5)
   {
      QMessageBox::warning(this, tr("注册"), tr("ID长度不足！"));
      ui->lineEditRegID->setFocus();
      return;
   }

   QRegularExpression emailReg("^[\\w!#$%&'*+/=?^_`{|}~-]+(?:\\.[\\w!#$%&'*+/=?^_`{|}~-]+)*"
                               "@(?:[\\w](?:[\\w-]*[\\w])?\\.)+[\\w](?:[\\w-]*[\\w])?$");

   if (!email.contains(emailReg))
   {
      QMessageBox::warning(this, tr("注册"), tr("请输入合法的邮箱地址！"));
      ui->lineEditRegEmail->setFocus();
      return;
   }

   QRegularExpression phoneReg("^((13[0-9])"
                               "|(14[5|7])"
                               "|(15([0-3]|[5-9]))"
                               "|(17[6-8])"
                               "|(18([0-3]|[5-9])))" // carrier code
                               "\\d{8}$");           // personal code

   if (!phone.contains(phoneReg))
   {
      QMessageBox::warning(this, tr("注册"), tr("请输入11位合法手机号！"));
      ui->lineEditRegPhone->setFocus();
      return;
   }

   QRegularExpression pwdReg("^(?=.*\\d)(?=.*[a-z])(?=.*[A-Z]).{6,18}$");

   if (!pwd.contains(pwdReg))
   {
      QMessageBox::warning(this, tr("注册"), tr("登录密码6-18位，必须包含大小写字母和数字。"));
      ui->lineEditRegPwd->setFocus();
      return;
   }

   QRegularExpression nameReg("^[\u4e00-\u9fa5]+(·[\u4e00-\u9fa5]+)*$");

   if (!name.contains(nameReg))
   {
      QMessageBox::warning(this, tr("注册"), tr("请输入合法中文姓名！"));
      ui->lineEditRegName->setFocus();
      return;
   }

   if (code.length() != 6)
   {
      QMessageBox::warning(this, tr("注册"), tr("请输入合法邮编！"));
      ui->lineEditRegPostCode->setFocus();
      return;
   }



   query.prepare("insert into"
                 " accounts (id, name, email, phone, password, addresses, postcodes, receivers, recphones)"
                 " values (:id, :name, :email, :phone, :pwd, :addr, :code, :name, :phone)");
   query.bindValue(":id", id);
   query.bindValue(":name", name);
   query.bindValue(":email", email);
   query.bindValue(":phone", phone);
   query.bindValue(":pwd", pwd);
   query.bindValue(":addr", addr);
   query.bindValue(":code", code);

   if (!query.exec())
   {
      QMessageBox::critical(this, tr("注册"), tr("数据库异常，请重试！"));
      return;
   }
   QMessageBox::information(this, tr("注册"), tr("注册成功。"));

   ui->lineEditLoginName->setText(id);
   ui->stackedWidgetAccount->setCurrentWidget(ui->pageLogin);
}


void MainWindow::on_checkBoxRemUid_toggled(bool checked)
{
   QSqlQuery q;

   if (checked)
   {
      if (!q.exec("update appsettings set remUid = 1"))
      {
         qDebug() << "MainWindow::on_checkBoxRemUid_toggled(): update failed";
      }
   }
   else
   {
      if (!q.exec("update appsettings set remUid = 0, lastUid = ''"))
      {
         qDebug() << "MainWindow::on_checkBoxRemUid_toggled(): update failed";
      }
   }
}


void MainWindow::on_checkBoxRemPwd_clicked(bool checked)
{
   QSqlQuery q;

   if (checked)
   {
      QMessageBox::StandardButton btn;
      btn = QMessageBox::question(this, tr("警告")
                                  , tr("记住密码会大大降低本程序的安全性，确定使用吗？"));
      if (btn == QMessageBox::No)
      {
         ui->checkBoxRemPwd->setChecked(false);
         if (!q.exec("update appsettings set remPwd = 0, lastPwd = ''"))
         {
            qDebug() << "MainWindow::on_checkBoxRemPwd_toggled(): update failed";
         }
      }
      else
      {
         ui->checkBoxRemUid->setChecked(true);
         if (!q.exec("update appsettings set remPwd = 1"))
         {
            qDebug() << "MainWindow::on_checkBoxRemPwd_toggled(): update failed";
         }
      }
   }
   else
   {
      if (!q.exec("update appsettings set remPwd = 0, lastPwd = ''"))
      {
         qDebug() << "MainWindow::on_checkBoxRemPwd_toggled(): update failed";
      }
   }
}


void MainWindow::on_pushButtonMyAddresses_clicked()
{
   ui->comboBoxMAAddresses->clear();
   ui->comboBoxMAAddresses->addItems(currentAccount.getAddrList());
   ui->comboBoxMAAddresses->addItem(tr("新增一个地址..."));
   ui->stackedWidgetAccount->setCurrentWidget(ui->pageMyAddresses);
}


void MainWindow::on_pushButtonBackToPageLogined_clicked()
{
   ui->stackedWidgetAccount->setCurrentWidget(ui->pageLogined);
}


void MainWindow::on_comboBoxMAAddresses_currentTextChanged(const QString& arg1)
{
   int idx = currentAccount.getAddrList().indexOf(arg1);

   if (idx == -1)
   {
      ui->lineEditMAAddress->clear();
      ui->lineEditMAPostcode->clear();
      ui->lineEditMARecPhone->clear();
      ui->lineEditMARecver->clear();

      ui->lineEditMAAddress->setFocus();
      return;
   }

   ui->lineEditMAAddress->setText(arg1);
   ui->lineEditMAPostcode->setText(currentAccount.getPcodeList().at(idx));
   ui->lineEditMARecPhone->setText(currentAccount.getRecvphList().at(idx));
   ui->lineEditMARecver->setText(currentAccount.getRecverList().at(idx));
}


void MainWindow::on_pushButtonMAAddAddress_clicked()
{
   QStringList addrList   = currentAccount.getAddrList();
   QStringList pcodeList  = currentAccount.getPcodeList();
   QStringList recverList = currentAccount.getRecverList();
   QStringList recvphList = currentAccount.getRecvphList();

   // duplicate
   if (addrList.indexOf(ui->lineEditMAAddress->text()) != -1)
   {
      QMessageBox::warning(this, tr("新增地址"), tr("地址已存在！"));
      return;
   }

   // postcode
   QString pcode = ui->lineEditMAPostcode->text();
   if (pcode.length() != 6)
   {
      QMessageBox::warning(this, tr("新增地址"), tr("请输入6位邮编！"));
      ui->lineEditMAPostcode->setFocus();
      return;
   }

   // name
   QRegularExpression nameReg("^[\u4e00-\u9fa5]+(·[\u4e00-\u9fa5]+)*$");
   QString            name = ui->lineEditMARecver->text();
   if (!name.contains(nameReg))
   {
      QMessageBox::warning(this, tr("新增地址"), tr("请输入合法中文姓名！"));
      ui->lineEditMARecver->setFocus();
      return;
   }


   // phone
   QString            phone = ui->lineEditMARecPhone->text();
   QRegularExpression phoneReg("^((13[0-9])"
                               "|(14[5|7])"
                               "|(15([0-3]|[5-9]))"
                               "|(17[6-8])"
                               "|(18([0-3]|[5-9])))" // carrier code
                               "\\d{8}$");           // personal code

   if (!phone.contains(phoneReg))
   {
      QMessageBox::warning(this, tr("新增地址"), tr("请输入11位合法手机号！"));
      ui->lineEditMARecPhone->setFocus();
      return;
   }

   addrList.append(ui->lineEditMAAddress->text());
   currentAccount.setAddrList(addrList);

   pcodeList.append(pcode);
   currentAccount.setPcodeList(pcodeList);

   recvphList.append(phone);
   currentAccount.setRecvphList(recvphList);

   recverList.append(name);
   currentAccount.setRecverList(recverList);

   QString addresses = addrList.join(";");
   QString postcodes = pcodeList.join(";");
   QString receivers = recverList.join(";");
   QString recphones = recvphList.join(";");

//   "update accounts "
//                  "set addresses = '" + addresses + "', "
//                  "postcodes = '" + postcodes + "', "
//                  "receivers = '" + receivers + "', "
//                  "recphones = '" + recphones + "' "
//                  "where id = '" + currentAccount.getId() + "'"
   QSqlQuery q;
   q.prepare("UPDATE accounts "
             "SET addresses = :a, "
             "postcodes = :pc, "
             "receivers = :r, "
             "recphones = :rp "
             "WHERE id = :i"); // dont add single quote! bindValue() handles this
   q.bindValue(":a", addresses);
   q.bindValue(":pc", postcodes);
   q.bindValue(":r", receivers);
   q.bindValue(":rp", recphones);
   q.bindValue(":i", currentAccount.getId());
   if (!q.exec())
   {
      qDebug() << "MainWindow::on_pushButtonMAAddAddress_clicked(): addr update failed";
      qDebug() << q.lastError();
      QMessageBox::critical(this, tr("新增地址"), tr("数据库异常！"));
      return;
   }
   QMessageBox::information(this, tr("新增地址"), tr("添加地址成功！"));
   on_pushButtonMyAddresses_clicked();
}


void MainWindow::on_pushButtonMAModifyAddress_clicked()
{
   QStringList addrList   = currentAccount.getAddrList();
   QStringList pcodeList  = currentAccount.getPcodeList();
   QStringList recverList = currentAccount.getRecverList();
   QStringList recvphList = currentAccount.getRecvphList();

   QString addrOld = ui->comboBoxMAAddresses->currentText();
   int     idx     = addrList.indexOf(addrOld);

   if (idx == -1)
   {
      QMessageBox::warning(this, tr("修改地址"), tr("地址不存在！"));
      return;
   }

   QString addrNew = ui->lineEditMAAddress->text();
   if (addrNew.isEmpty())
   {
      QMessageBox::warning(this, tr("修改地址"), tr("地址不能为空！"));
      return;
   }

   QString pcode = ui->lineEditMAPostcode->text();
   if (pcode.length() != 6)
   {
      QMessageBox::warning(this, tr("修改地址"), tr("请输入6位邮编！"));
      ui->lineEditMAPostcode->setFocus();
      return;
   }


   QRegularExpression nameReg("^[\u4e00-\u9fa5]+(·[\u4e00-\u9fa5]+)*$");
   QString            name = ui->lineEditMARecver->text();
   if (!name.contains(nameReg))
   {
      QMessageBox::warning(this, tr("修改地址"), tr("请输入合法中文姓名！"));
      ui->lineEditMARecver->setFocus();
      return;
   }


   QString            phone = ui->lineEditMARecPhone->text();
   QRegularExpression phoneReg("^((13[0-9])"
                               "|(14[5|7])"
                               "|(15([0-3]|[5-9]))"
                               "|(17[6-8])"
                               "|(18([0-3]|[5-9])))" // carrier code
                               "\\d{8}$");           // personal code

   if (!phone.contains(phoneReg))
   {
      QMessageBox::warning(this, tr("修改地址"), tr("请输入11位合法手机号！"));
      ui->lineEditMARecPhone->setFocus();
      return;
   }

   addrList.replace(idx, addrNew);
   currentAccount.setAddrList(addrList);

   pcodeList.replace(idx, pcode);
   currentAccount.setPcodeList(pcodeList);

   recverList.replace(idx, name);
   currentAccount.setRecverList(recverList);

   recvphList.replace(idx, phone);
   currentAccount.setRecvphList(recvphList);

   QString addresses = addrList.join(";");
   QString postcodes = pcodeList.join(";");
   QString receivers = recverList.join(";");
   QString recphones = recvphList.join(";");

   QSqlQuery q;
   q.prepare("UPDATE accounts "
             "SET addresses = :a, "
             "postcodes = :pc, "
             "receivers = :r, "
             "recphones = :rp "
             "WHERE id = :i");  // dont add single quote! bindValue() handles this
   q.bindValue(":a", addresses);
   q.bindValue(":pc", postcodes);
   q.bindValue(":r", receivers);
   q.bindValue(":rp", recphones);
   q.bindValue(":i", currentAccount.getId());
   if (!q.exec())
   {
      qDebug() << "MainWindow::on_pushButtonMAModifyAddress_clicked(): addr update failed";
      qDebug() << q.lastError();
      QMessageBox::critical(this, tr("修改地址"), tr("数据库异常！"));
      return;
   }
   QMessageBox::information(this, tr("修改地址"), tr("修改地址成功！"));
   on_pushButtonMyAddresses_clicked();
}


void MainWindow::on_pushButtonMADeleteAddress_clicked()
{
   QStringList addrList   = currentAccount.getAddrList();
   QStringList pcodeList  = currentAccount.getPcodeList();
   QStringList recverList = currentAccount.getRecverList();
   QStringList recvphList = currentAccount.getRecvphList();

   if (addrList.length() == 1)
   {
      QMessageBox::warning(this, tr("删除地址"), tr("请至少保留一个地址！您可以修改当前地址。"));
      return;
   }

   int idx = addrList.indexOf(ui->lineEditMAAddress->text());

   if (idx == -1)
   {
      QMessageBox::warning(this, tr("删除地址"), tr("地址不存在！"));
      return;
   }

   addrList.removeAt(idx);
   currentAccount.setAddrList(addrList);

   pcodeList.removeAt(idx);
   currentAccount.setPcodeList(pcodeList);

   recverList.removeAt(idx);
   currentAccount.setRecverList(recverList);

   recvphList.removeAt(idx);
   currentAccount.setRecvphList(recvphList);

   QString addresses = addrList.join(";");
   QString postcodes = pcodeList.join(";");
   QString receivers = recverList.join(";");
   QString recphones = recvphList.join(";");

   QSqlQuery q;
   q.prepare("UPDATE accounts "
             "SET addresses = :a, "
             "postcodes = :pc, "
             "receivers = :r, "
             "recphones = :rp "
             "WHERE id = :i");   // dont add single quote! bindValue() handles this
   q.bindValue(":a", addresses);
   q.bindValue(":pc", postcodes);
   q.bindValue(":r", receivers);
   q.bindValue(":rp", recphones);
   q.bindValue(":i", currentAccount.getId());
   if (!q.exec())
   {
      qDebug() << "MainWindow::on_pushButtonMAModifyAddress_clicked(): addr update failed";
      qDebug() << q.lastError();
      QMessageBox::critical(this, tr("删除地址"), tr("数据库异常！"));
      return;
   }
   QMessageBox::information(this, tr("删除地址"), tr("删除地址成功！"));
   on_pushButtonMyAddresses_clicked();
}


void MainWindow::on_pushButtonBackToPageLoginedFromPageOrder_clicked()
{
   on_tabWidgetOrder_tabBarClicked(0);
   ui->stackedWidgetAccount->setCurrentWidget(ui->pageLogined);
}


void MainWindow::on_tabWidgetOrder_tabBarClicked(int index)
{
//   QTabWidget *tab = ui->tabWidgetOrder;

   switch (index)
   {
   case 0:
      // all orders
      orderTableModel->select();
      orderTableModel->setFilter("uid = '" + currentAccount.getId() + "'");
      break;

   case 1:
      // orders to pay
      orderTableModel->select();
      orderTableModel->setFilter("uid = '" + currentAccount.getId() + "' and state = '待支付'");
      break;

   case 2:
      // orders to ship
      orderTableModel->select();
      orderTableModel->setFilter("uid = '" + currentAccount.getId() + "' and state = '待发货'");
      break;

   default:
      break;
   }
}


void MainWindow::on_pushButtonMyCards_clicked()
{
   ui->labelMCPhone->setText(currentAccount.getPhone() + tr("（手机号必须与银行卡手机一致！）"));
   ui->comboBoxMCCards->clear();
   ui->comboBoxMCCards->addItems(currentAccount.getCardList());
   ui->comboBoxMCCards->addItem(tr("添加一张新卡..."));
   ui->stackedWidgetAccount->setCurrentWidget(ui->pageMyCards);
}


void MainWindow::on_comboBoxMCCards_currentTextChanged(const QString& arg1)
{
   int idx = currentAccount.getCardList().indexOf(arg1);

   if (idx == -1)
   {
      ui->lineEditMCCardID->clear();
      ui->lineEditMCPwdPay->clear();

      ui->lineEditMCCardID->setFocus();
      return;
   }

   ui->lineEditMCCardID->setText(arg1);
}


void MainWindow::on_pushButtonMCAddCard_clicked()
{
   QStringList cardList = currentAccount.getCardList();
   QString     newCard  = ui->lineEditMCCardID->text();
   QString     paypwd   = ui->lineEditMCPwdPay->text();

   // duplicate
   if (cardList.indexOf(newCard) != -1)
   {
      QMessageBox::warning(this, tr("新增银行卡"), tr("卡号已存在！"));
      return;
   }
   if (newCard.length() != 19)
   {
      QMessageBox::warning(this, tr("新增银行卡"), tr("卡号长度不足！"));
      return;
   }

   if (paypwd.length() != 6)
   {
      QMessageBox::warning(this, tr("新增银行卡"), tr("支付密码长度不足！"));
      return;
   }

   QString    phone = currentAccount.getPhone();
   BindDialog bd(nullptr, newCard, phone, paypwd);
   if (bd.exec() == QDialog::Accepted)
   {
      cardList.append(newCard);
      currentAccount.setCardList(cardList);
   }
   else
   {
      QMessageBox::warning(this, tr("新增银行卡")
                           , tr("绑卡失败，请检查输入信息！要求卡号、手机、支付密码完全匹配。"));
      return;
   }

   QString   cards = cardList.join(";");
   QSqlQuery q;
   q.prepare("UPDATE accounts "
             "SET cards = :c "
             "WHERE id = :i");  // dont add single quote! bindValue() handles this
   q.bindValue(":c", cards);
   q.bindValue(":i", currentAccount.getId());
   if (!q.exec())
   {
      qDebug() << "MainWindow::on_pushButtonMCAddCard_clicked(): cards update failed";
      qDebug() << q.lastError();
      QMessageBox::critical(this, tr("新增银行卡"), tr("数据库异常！"));
      return;
   }

   QMessageBox::information(this, tr("新增银行卡"), tr("添加卡号成功！"));
   on_pushButtonMyCards_clicked();
}


void MainWindow::on_pushButtonMCDeleteCard_clicked()
{
   QStringList cardList = currentAccount.getCardList();
   QString     cid      = ui->comboBoxMCCards->currentText();
   int         idx      = cardList.indexOf(cid);

   if (idx == -1)
   {
      QMessageBox::warning(this, tr("删除银行卡"), tr("请先在下拉栏选中一张卡！"));
      return;
   }

   if (QMessageBox::No == QMessageBox::question(this, tr("删除银行卡"), tr("您确定要删除吗？")))
   {
      return;
   }

   cardList.removeAt(idx);
   currentAccount.setCardList(cardList);

   QString   cards = cardList.join(";");
   QSqlQuery q;
   q.prepare("UPDATE accounts "
             "SET cards = :c "
             "WHERE id = :i");   // dont add single quote! bindValue() handles this
   q.bindValue(":c", cards);
   q.bindValue(":i", currentAccount.getId());
   if (!q.exec())
   {
      qDebug() << "MainWindow::on_pushButtonMCAddCard_clicked(): cards update failed";
      qDebug() << q.lastError();
      QMessageBox::critical(this, tr("删除银行卡"), tr("数据库异常！"));
      return;
   }

   QMessageBox::information(this, tr("删除银行卡"), tr("删除卡号成功！"));
   on_pushButtonMyCards_clicked();
}


void MainWindow::on_pushButtonBackToPageLoginedFromMC_clicked()
{
   ui->stackedWidgetAccount->setCurrentWidget(ui->pageLogined);
}


void MainWindow::on_pushButtonMCQueryCard_clicked()
{
   QString cid = ui->comboBoxMCCards->currentText();
   QString pp  = ui->lineEditMCPwdPay->text();

   if (currentAccount.getCardList().indexOf(cid) == -1)
   {
      QMessageBox::warning(this, tr("查询银行卡"), tr("请在下拉栏选中一张卡！"));
      return;
   }

   if (pp.length() != 6)
   {
      QMessageBox::warning(this, tr("查询银行卡"), tr("支付密码长度不足！"));
      return;
   }

   QueryDialog qd(nullptr, cid, pp);

   qd.exec();
}


void MainWindow::on_pushButtonRepay_clicked()
{
   QString   oid  = ui->labelOrderID->text();
   QString   cost = ui->labelOrderPriceTotal->text();
   PayDialog pd(nullptr, currentAccount, cost, oaList);

   if (pd.exec() == QDialog::Accepted)
   {
      QSqlQuery q;
      q.exec("update accountOrders "
             " set state = '待发货', timePay = '" + QDateTime::currentDateTime().toString("yyyy-MM-dd-hh:mm:ss") + "' " +
             " where id = '" + oid + "'");
      ui->pushButtonRepay->hide();
   }
   else
   {
      QMessageBox::information(this, tr("支付"), tr("支付未成功！请重新支付。"));
   }
}


void MainWindow::on_tableViewOrderAll_clicked(const QModelIndex& index)
{
   /* 0=id
    * 1=uid
    * 2=state
    * 3=timeCreate
    * 4=timePay
    * 5=address
    * 6=postcode
    * 7=receriver
    * 8=recphone
    * 9=pnames
    * 10=pprices
    * 11=pdiscount
    * 12=pcounts
    */
   QString   oid   = index.sibling(index.row(), 0).data().toString();
   QString   state = index.sibling(index.row(), 2).data().toString();
   QSqlQuery q;

   if (!q.exec("select * from accountOrders where id = '" + oid + "'"))
   {
      qDebug() << "MainWindow::on_tableViewOrderAll_clicked: exec failed";
   }
   if (!q.next())
   {
      qDebug() << "MainWindow::on_tableViewOrderAll_clicked: next failed";
   }

   ui->labelOrderID->setText(oid);
   ui->labelOrderTC->setText(q.value("timeCreate").toString());
   ui->labelOrderTP->setText(q.value("timePay").toString());
   ui->labelOrderAddr->setText(q.value("address").toString());
   ui->labelOrderPcode->setText(q.value("postcode").toString());
   ui->labelOrderRecver->setText(q.value("receiver").toString());
   ui->labelOrderRecvph->setText(q.value("recphone").toString());

   QStringList pnames    = q.value("pnames").toString().split(";");
   QStringList pprices   = q.value("pprices").toString().split(";");
   QStringList pdiscount = q.value("pdiscount").toString().split(";");
   QStringList pcounts   = q.value("pcounts").toString().split(";");
   int         count     = pnames.size();

   /* 0=pname
    * 1=pprice
    * 2=pcount
    * 3=pdiscount
    * 4=total
    */
   QTableWidget *tableOrder = ui->tableWidgetOrder;
   double       total       = 0;
   for (int row = 0, col = 0; row < count; ++row, col = 0)
   {
      tableOrder->setRowCount(row + 1);
      tableOrder->setItem(row, col++, new QTableWidgetItem(pnames.at(row)));
      tableOrder->setItem(row, col++, new QTableWidgetItem(pprices.at(row)));
      tableOrder->setItem(row, col++, new QTableWidgetItem(pdiscount.at(row)));
      tableOrder->setItem(row, col++, new QTableWidgetItem(pcounts.at(row)));
      double totalSingle = pprices.at(row).toDouble()
                           * pdiscount.at(row).toDouble()
                           * pcounts.at(row).toDouble();
      total += totalSingle;
      tableOrder->setItem(row, col++, new QTableWidgetItem(QString::number(totalSingle, 'f', 2)));
   }

   tableOrder->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);
   tableOrder->resizeColumnsToContents();
   tableOrder->setColumnWidth(0, 600);

   ui->labelOrderPriceTotal->setText(tr("总金额：") + QString::number(total, 'f', 2));

   if (state == "待支付")
   {
      ui->pushButtonRepay->show();
   }
   else
   {
      ui->pushButtonRepay->hide();
   }

   ui->stackedWidgetAccount->setCurrentWidget(ui->pageOrder);
}


void MainWindow::on_tableViewOrderToPay_clicked(const QModelIndex& index)
{
   /* 0=id
    * 1=uid
    * 2=state
    * 3=timeCreate
    * 4=timePay
    * 5=address
    * 6=postcode
    * 7=receriver
    * 8=recphone
    * 9=pnames
    * 10=pprices
    * 11=pdiscount
    * 12=pcounts
    */
   QString   oid   = index.sibling(index.row(), 0).data().toString();
   QString   state = index.sibling(index.row(), 2).data().toString();
   QSqlQuery q;

   if (!q.exec("select * from accountOrders where id = '" + oid + "'"))
   {
      qDebug() << "MainWindow::on_tableViewOrderAll_clicked: exec failed";
   }
   if (!q.next())
   {
      qDebug() << "MainWindow::on_tableViewOrderAll_clicked: next failed";
   }

   ui->labelOrderID->setText(oid);
   ui->labelOrderTC->setText(q.value("timeCreate").toString());
   ui->labelOrderTP->setText(q.value("timePay").toString());
   ui->labelOrderAddr->setText(q.value("address").toString());
   ui->labelOrderPcode->setText(q.value("postcode").toString());
   ui->labelOrderRecver->setText(q.value("receiver").toString());
   ui->labelOrderRecvph->setText(q.value("recphone").toString());

   QStringList pnames    = q.value("pnames").toString().split(";");
   QStringList pprices   = q.value("pprices").toString().split(";");
   QStringList pdiscount = q.value("pdiscount").toString().split(";");
   QStringList pcounts   = q.value("pcounts").toString().split(";");
   int         count     = pnames.size();

   /* 0=pname
    * 1=pprice
    * 2=pcount
    * 3=pdiscount
    * 4=total
    */
   QTableWidget *tableOrder = ui->tableWidgetOrder;
   double       total       = 0;
   for (int row = 0, col = 0; row < count; ++row, col = 0)
   {
      tableOrder->setRowCount(row + 1);
      tableOrder->setItem(row, col++, new QTableWidgetItem(pnames.at(row)));
      tableOrder->setItem(row, col++, new QTableWidgetItem(pprices.at(row)));
      tableOrder->setItem(row, col++, new QTableWidgetItem(pdiscount.at(row)));
      tableOrder->setItem(row, col++, new QTableWidgetItem(pcounts.at(row)));
      double totalSingle = pprices.at(row).toDouble()
                           * pdiscount.at(row).toDouble()
                           * pcounts.at(row).toDouble();
      total += totalSingle;
      tableOrder->setItem(row, col++, new QTableWidgetItem(QString::number(totalSingle, 'f', 2)));
   }

   tableOrder->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);
   tableOrder->resizeColumnsToContents();
   tableOrder->setColumnWidth(0, 600);

   ui->labelOrderPriceTotal->setText(tr("总金额：") + QString::number(total, 'f', 2));

   if (state == "待支付")
   {
      ui->pushButtonRepay->show();
   }
   else
   {
      ui->pushButtonRepay->hide();
   }

   ui->stackedWidgetAccount->setCurrentWidget(ui->pageOrder);
}


void MainWindow::on_tableViewOrderToShip_clicked(const QModelIndex& index)
{
   /* 0=id
    * 1=uid
    * 2=state
    * 3=timeCreate
    * 4=timePay
    * 5=address
    * 6=postcode
    * 7=receriver
    * 8=recphone
    * 9=pnames
    * 10=pprices
    * 11=pdiscount
    * 12=pcounts
    */
   QString   oid   = index.sibling(index.row(), 0).data().toString();
   QString   state = index.sibling(index.row(), 2).data().toString();
   QSqlQuery q;

   if (!q.exec("select * from accountOrders where id = '" + oid + "'"))
   {
      qDebug() << "MainWindow::on_tableViewOrderAll_clicked: exec failed";
   }
   if (!q.next())
   {
      qDebug() << "MainWindow::on_tableViewOrderAll_clicked: next failed";
   }

   ui->labelOrderID->setText(oid);
   ui->labelOrderTC->setText(q.value("timeCreate").toString());
   ui->labelOrderTP->setText(q.value("timePay").toString());
   ui->labelOrderAddr->setText(q.value("address").toString());
   ui->labelOrderPcode->setText(q.value("postcode").toString());
   ui->labelOrderRecver->setText(q.value("receiver").toString());
   ui->labelOrderRecvph->setText(q.value("recphone").toString());

   QStringList pnames    = q.value("pnames").toString().split(";");
   QStringList pprices   = q.value("pprices").toString().split(";");
   QStringList pdiscount = q.value("pdiscount").toString().split(";");
   QStringList pcounts   = q.value("pcounts").toString().split(";");
   int         count     = pnames.size();

   /* 0=pname
    * 1=pprice
    * 2=pcount
    * 3=pdiscount
    * 4=total
    */
   QTableWidget *tableOrder = ui->tableWidgetOrder;
   double       total       = 0;
   for (int row = 0, col = 0; row < count; ++row, col = 0)
   {
      tableOrder->setRowCount(row + 1);
      tableOrder->setItem(row, col++, new QTableWidgetItem(pnames.at(row)));
      tableOrder->setItem(row, col++, new QTableWidgetItem(pprices.at(row)));
      tableOrder->setItem(row, col++, new QTableWidgetItem(pdiscount.at(row)));
      tableOrder->setItem(row, col++, new QTableWidgetItem(pcounts.at(row)));
      double totalSingle = pprices.at(row).toDouble()
                           * pdiscount.at(row).toDouble()
                           * pcounts.at(row).toDouble();
      total += totalSingle;
      tableOrder->setItem(row, col++, new QTableWidgetItem(QString::number(totalSingle, 'f', 2)));
   }

   tableOrder->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);
   tableOrder->resizeColumnsToContents();
   tableOrder->setColumnWidth(0, 600);

   ui->labelOrderPriceTotal->setText(tr("总金额：") + QString::number(total, 'f', 2));

   if (state == "待支付")
   {
      ui->pushButtonRepay->show();
   }
   else
   {
      ui->pushButtonRepay->hide();
   }

   ui->stackedWidgetAccount->setCurrentWidget(ui->pageOrder);
}
