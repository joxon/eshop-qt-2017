#include "product.h"

Product::Product()
{
}


QString Product::getId() const
{
   return id;
}


void Product::setId(const QString& value)
{
   id = value;
}


QString Product::getPname() const
{
   return pname;
}


void Product::setPname(const QString& value)
{
   pname = value;
}


QString Product::getType() const
{
   return type;
}


void Product::setType(const QString& value)
{
   type = value;
}


double Product::getTypeDiscount() const
{
   return typeDiscount;
}


void Product::setTypeDiscount(double value)
{
   typeDiscount = value;
}


QString Product::getDescription() const
{
   return description;
}


void Product::setDescription(const QString& value)
{
   description = value;
}


int Product::getStock() const
{
   return stock;
}


void Product::setStock(int value)
{
   stock = value;
}


double Product::getPriceOri() const
{
   return priceOri;
}


void Product::setPriceOri(double value)
{
   priceOri = value;
}


QPixmap Product::getImage() const
{
   return image;
}


void Product::setImage(const QPixmap& value)
{
   image = value;
}


QString ProductType::getName() const
{
   return name;
}


void ProductType::setName(const QString& value)
{
   name = value;
}


QString ProductType::getPrefix() const
{
   return prefix;
}


void ProductType::setPrefix(const QString& value)
{
   prefix = value;
}


unsigned ProductType::getCount() const
{
   return count;
}


void ProductType::setCount(const unsigned& value)
{
   count = value;
}


double ProductType::getDiscount() const
{
   return discount;
}


void ProductType::setDiscount(double value)
{
   discount = value;
}


double ProductInCart::getPrice()
{
   return getPriceOri() * getTypeDiscount();
}


unsigned ProductInCart::getCount() const
{
   return count;
}


void ProductInCart::setCount(const unsigned& value)
{
   count = value;
}


double Book::getPrice()
{
   return getPriceOri() * getTypeDiscount();
}


QString Book::getPages() const
{
   return pages;
}


void Book::setPages(const QString& value)
{
   pages = value;
}


QString Book::getName() const
{
   return name;
}


void Book::setName(const QString& value)
{
   name = value;
}


QString Book::getIsbn() const
{
   return isbn;
}


void Book::setIsbn(const QString& value)
{
   isbn = value;
}


QString Book::getAuthor() const
{
   return author;
}


void Book::setAuthor(const QString& value)
{
   author = value;
}


QString Book::getPuber() const
{
   return puber;
}


void Book::setPuber(const QString& value)
{
   puber = value;
}


QString Book::getPubver() const
{
   return pubver;
}


void Book::setPubver(const QString& value)
{
   pubver = value;
}


double Clothes::getPrice()
{
   return getPriceOri() * getTypeDiscount();
}


QString Clothes::getTexture() const
{
   return texture;
}


void Clothes::setTexture(const QString& value)
{
   texture = value;
}


QString Clothes::getSize() const
{
   return size;
}


void Clothes::setSize(const QString& value)
{
   size = value;
}


QString Clothes::getColor() const
{
   return color;
}


void Clothes::setColor(const QString& value)
{
   color = value;
}


QString Clothes::getBrand() const
{
   return brand;
}


void Clothes::setBrand(const QString& value)
{
   brand = value;
}


double Food::getPrice()
{
   return getPriceOri() * getTypeDiscount();
}


QString Food::getName() const
{
   return name;
}


void Food::setName(const QString& value)
{
   name = value;
}


QString Food::getBrand() const
{
   return brand;
}


void Food::setBrand(const QString& value)
{
   brand = value;
}


QString Food::getFlavor() const
{
   return flavor;
}


void Food::setFlavor(const QString& value)
{
   flavor = value;
}


QString Food::getPeriod() const
{
   return period;
}


void Food::setPeriod(const QString& value)
{
   period = value;
}


QString Food::getNetwgt() const
{
   return netwgt;
}


void Food::setNetwgt(const QString& value)
{
   netwgt = value;
}
