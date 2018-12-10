#ifndef ACCOUNT_H
#define ACCOUNT_H

#include <QString>
#include <QStringList>

typedef struct Address
{
   QString address;
   QString postcode;
   QString receiver;
   QString recphone;
} Address;

typedef struct Order
{
   QString id;
   QString state;
   QString timeCreate;
   QString timePay;
   Address address;
   QString pnames;
   QString pprices;
   QString pdiscount;
   QString pcounts;
} Order;

class Account
{
public:
   Account();

   QString getId() const;
   void setId(const QString& value);

   QString getName() const;
   void setName(const QString& value);

   QString getEmail() const;
   void setEmail(const QString& value);

   QString getPhone() const;
   void setPhone(const QString& value);

   QString getPassword() const;
   void setPassword(const QString& value);

   QStringList getAddrList() const;
   void setAddrList(const QStringList& value);

   QStringList getPcodeList() const;
   void setPcodeList(const QStringList& value);

   QStringList getCardList() const;
   void setCardList(const QStringList& value);

   QStringList getRecverList() const;
   void setRecverList(const QStringList& value);

   QStringList getRecvphList() const;
   void setRecvphList(const QStringList& value);

   bool isLogined() const;
   void setLogined(bool value);

private:
   bool logined = false;

   QString id;
   QString name;
   QString email;
   QString phone;
   QString password;

//   QVector<Address> addresses;

   QStringList addrList;
   QStringList pcodeList;
   QStringList recverList;
   QStringList recvphList;

   QStringList cardIDList;
};

#endif // ACCOUNT_H
