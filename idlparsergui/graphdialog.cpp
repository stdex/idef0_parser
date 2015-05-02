#include "graphdialog.h"
#include "ui_graphdialog.h"
#include "graphwidget.h"

GraphDialog::GraphDialog(QWidget *parent, int nDiag,int nType) :
    QDialog(parent),
    ui(new Ui::GraphDialog)
{
    ui->setupUi(this);
    this->nDiag = nDiag;
    this->nType = nType;
    GraphWidget *widget = new GraphWidget(this);
    widget->setGeometry(QRect(7, 7, 750, 295));
    widget->updateTree(nDiag,nType);
}

GraphDialog::~GraphDialog()
{
    delete ui;
}
