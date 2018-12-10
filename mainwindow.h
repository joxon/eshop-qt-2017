#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSql>
#include <QCloseEvent>

#include "product.h"
#include "account.h"
#include "categorytablemodel.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
   Q_OBJECT

public:
   MainWindow(QWidget *parent, QApplication& app);
   ~MainWindow();

protected:
   void closeEvent(QCloseEvent *ce);

private slots:
   void removeSingleProductFromCart(int row);
   void adjustSingleProductCountInCart(int row);
   void selectSingleProductInCart(int row);
   void changeBanner();

   void on_actionAccount_triggered();
   void on_actionExit_triggered();
   void on_actionCategory_triggered();
   void on_actionCart_triggered();
   void on_actionAboutQt_triggered();

   void on_actionHome_triggered();
   void on_actionBlueMD_triggered();
   void on_actionBlue_triggered();
   void on_actionDarkVS_triggered();
   void on_actionDark_triggered();
   void on_actionGray_triggered();

   void on_pushButtonRegister_clicked();
   void on_pushButtonLogin_clicked();
   void on_pushButtonRegCancel_clicked();
   void on_lineEditHomeSearch_returnPressed();
   void on_pushButtonHomeSearch_clicked();
   void on_lineEditCatSearch_returnPressed();
   void on_pushButtonCatSearch_clicked();
   void on_pushButtonLogout_clicked();
   void on_pushButtonCatBack_clicked();
   void on_pushButtonViewClothes_clicked();
   void on_pushButtonViewBooks_clicked();
   void on_pushButtonViewFoods_clicked();
   void on_pushButtonPBuyNow_clicked();
   void on_pushButtonPAddToCart_clicked();
   void on_pushButtonCartDelete_clicked();
   void on_checkBoxCartSelAll_toggled(bool checked);
   void on_lineEditCatSearch_textEdited(const QString& arg1);
   void on_pushButtonRegConfirm_clicked();
   void on_lineEditLoginName_returnPressed();
   void on_lineEditLoginPassword_returnPressed();
   void on_checkBoxRemUid_toggled(bool checked);
   void on_checkBoxRemPwd_clicked(bool checked);
   void on_pushButtonMyAddresses_clicked();
   void on_pushButtonBackToPageLogined_clicked();
   void on_comboBoxMAAddresses_currentTextChanged(const QString& arg1);
   void on_pushButtonMAAddAddress_clicked();
   void on_pushButtonMAModifyAddress_clicked();
   void on_pushButtonMADeleteAddress_clicked();
   void on_checkBoxRegShowPwd_clicked(bool checked);
   void on_pushButtonBackToCart_clicked();
   void on_pushButtonCartCheckout_clicked();
   void on_pushButtonCOPay_clicked();
   void on_pushButtonBackToPageLoginedFromPageOrder_clicked();
   void on_pushButtonMyCards_clicked();
   void on_comboBoxMCCards_currentTextChanged(const QString& arg1);
   void on_pushButtonMCAddCard_clicked();
   void on_pushButtonMCDeleteCard_clicked();
   void on_pushButtonBackToPageLoginedFromMC_clicked();
   void on_pushButtonRepay_clicked();
   void on_pushButtonMCQueryCard_clicked();

   void on_tabWidgetOrder_tabBarClicked(int index);
   void on_tableViewCategory_clicked(const QModelIndex& index);
   void on_tableWidgetCart_cellClicked(int row, int column);
   void on_tableViewOrderToPay_clicked(const QModelIndex& index);
   void on_tableViewOrderToShip_clicked(const QModelIndex& index);
   void on_tableViewOrderAll_clicked(const QModelIndex& index);

   void on_actionTaobao_triggered();

private:
   Ui::MainWindow *ui;
   QApplication& rApp;

   QSqlDatabase database;
   CategoryTableModel *categoryTableModel;
   QSqlTableModel *orderTableModel;

   QVector<ProductType> typeList;
   QVector<ProductInCart> cartList;
   QVector<ProductInCart> checkoutList;
   QVector<OffAbove> oaList;

   double cartPriceTotal = 0;
   Account currentAccount;

   bool openDB(const QString dbname);
   void closeDB();
};

#endif // MAINWINDOW_H
