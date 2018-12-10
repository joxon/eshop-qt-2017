#ifndef PRODUCT_H
#define PRODUCT_H

#include <QString>
#include <QPixmap>

typedef struct OffAbove
{
   QString off;
   QString above;
} OffAbove;

class ProductType
{
public:
   ProductType() {}
   QString getName() const;
   void setName(const QString& value);

   QString getPrefix() const;
   void setPrefix(const QString& value);

   unsigned getCount() const;
   void setCount(const unsigned& value);

   double getDiscount() const;
   void setDiscount(double value);

private:
   QString name;
   QString prefix;
   unsigned count;
   double discount;
};

class Product
{
public:
   Product();
   virtual double getPrice() = 0;

   QString getId() const;
   void setId(const QString& value);

   QString getPname() const;
   void setPname(const QString& value);

   QString getType() const;
   void setType(const QString& value);

   double getTypeDiscount() const;
   void setTypeDiscount(double value);

   QString getDescription() const;
   void setDescription(const QString& value);

   int getStock() const;
   void setStock(int value);

   double getPriceOri() const;
   void setPriceOri(double value);

   QPixmap getImage() const;
   void setImage(const QPixmap& value);

private:
   QPixmap image;
   QString id;
   QString pname;
   QString type;
   double typeDiscount;
   QString description;
   int stock;
   double priceOri;
};

class ProductInCart : public Product
{
public:
   double getPrice();

   unsigned getCount() const;
   void setCount(const unsigned& value);

private:
   unsigned count = 0;
};

class Book : public Product
{
public:
   Book() {}
   double getPrice();

   QString getPages() const;
   void setPages(const QString& value);

   QString getName() const;
   void setName(const QString& value);

   QString getIsbn() const;
   void setIsbn(const QString& value);

   QString getAuthor() const;
   void setAuthor(const QString& value);

   QString getPuber() const;
   void setPuber(const QString& value);

   QString getPubver() const;
   void setPubver(const QString& value);

private:
   QString name;
   QString pages;
   QString isbn;
   QString author;
   QString puber;
   QString pubver;
};

class Clothes : public Product
{
public:
   Clothes() {}
   double getPrice();

   QString getTexture() const;
   void setTexture(const QString& value);

   QString getSize() const;
   void setSize(const QString& value);

   QString getColor() const;
   void setColor(const QString& value);

   QString getBrand() const;
   void setBrand(const QString& value);

private:
   QString texture;
   QString size;
   QString color;
   QString brand;
};

class Food : public Product
{
public:
   Food() {}
   double getPrice();

   QString getName() const;
   void setName(const QString& value);

   QString getBrand() const;
   void setBrand(const QString& value);

   QString getFlavor() const;
   void setFlavor(const QString& value);

   QString getPeriod() const;
   void setPeriod(const QString& value);

   QString getNetwgt() const;
   void setNetwgt(const QString& value);

private:
   QString name;
   QString brand;
   QString flavor;
   QString period;
   QString netwgt;
};

#endif // PRODUCT_H
