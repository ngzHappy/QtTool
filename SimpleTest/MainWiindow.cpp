#include "MainWiindow.hpp"
#include "TextEdit.hpp"
#include <QHBoxLayout>

MainWiindow::MainWiindow(QWidget *parent)
    : P(parent)
{
    QHBoxLayout * layout_=new QHBoxLayout;
    this->setLayout(layout_);
    layout_->setMargin(0);
    layout_->setSpacing(0);
    layout_->addWidget(new TextEdit);
}

MainWiindow::~MainWiindow()
{

}
