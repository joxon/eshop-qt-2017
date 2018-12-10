#include "account.h"

Account::Account()
{
}


QString Account::getId() const
{
   return id;
}


void Account::setId(const QString& value)
{
   id = value;
}


QString Account::getName() const
{
   return name;
}


void Account::setName(const QString& value)
{
   name = value;
}


QString Account::getEmail() const
{
   return email;
}


void Account::setEmail(const QString& value)
{
   email = value;
}


QString Account::getPhone() const
{
   return phone;
}


void Account::setPhone(const QString& value)
{
   phone = value;
}


QString Account::getPassword() const
{
   return password;
}


void Account::setPassword(const QString& value)
{
   password = value;
}


QStringList Account::getAddrList() const
{
   return addrList;
}


void Account::setAddrList(const QStringList& value)
{
   addrList = value;
}


QStringList Account::getPcodeList() const
{
   return pcodeList;
}


void Account::setPcodeList(const QStringList& value)
{
   pcodeList = value;
}


QStringList Account::getCardList() const
{
   return cardIDList;
}


void Account::setCardList(const QStringList& value)
{
   cardIDList = value;
}


QStringList Account::getRecverList() const
{
   return recverList;
}


void Account::setRecverList(const QStringList& value)
{
   recverList = value;
}


QStringList Account::getRecvphList() const
{
   return recvphList;
}


void Account::setRecvphList(const QStringList& value)
{
   recvphList = value;
}


bool Account::isLogined() const
{
   return logined;
}


void Account::setLogined(bool value)
{
   logined = value;
}
