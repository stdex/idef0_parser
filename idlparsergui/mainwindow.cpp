#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include "idlparser.h"
#include "ekit.h"
#include <QTextStream>
#include "QMessageBox"
#include <QDesktopWidget>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>
#include <QBuffer>
#include <QDebug>
#include <QStandardItemModel>
#include <QDomNode>
#include <QMenuBar>
#include <QVBoxLayout>
#include "graphdialog.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //Центрирование экрана
    QRect scr = QApplication::desktop()->screenGeometry();
    move( scr.center() - rect().center() );
    QStringList headerList;
    headerList<<tr("Название документа");
    headerList<<tr("Свойство");
    headerList<<tr("Результат");
    //order warning!!!
    ui->tableWidget->setColumnCount(3);
    ui->tableWidget->setRowCount(0);
    ui->tableWidget->setHorizontalHeaderLabels(headerList);
    //order warning!!!
    //
    ui->tableWidget->setColumnWidth(0,200);
    ui->tableWidget->setColumnWidth(1,200);
    searchList.clear();
    isResult = false;

    QMenu *fileMenu;
    QAction *saveAct;
    QAction *openAct;
    QMenuBar *menuBar = new QMenuBar(this);

    saveAct = new QAction(tr("&Сохранить"), this);
    saveAct->setStatusTip(tr("Сохранить в файл"));
    openAct = new QAction(tr("&Октрыть..."), this);
    openAct->setStatusTip(tr("Открыть существующий файл"));
    fileMenu = menuBar->addMenu(tr("Файл"));
    fileMenu->addAction(openAct);
    fileMenu->addAction(saveAct);


    connect(saveAct, SIGNAL(triggered()), this, SLOT(save()));
    connect(openAct, SIGNAL(triggered()), this, SLOT(open()));

//    importXmlToModel("documents.xml");
}

void MainWindow::open() {
    QString FileLoading = QFileDialog::getOpenFileName(this,"Открыть","","Файл XML(*.xml)");
    if(FileLoading != "") {
        on_btnClear_clicked();
        importXmlToModel(FileLoading);
    }
}

void MainWindow::save() {
    QString FileSaving = QFileDialog::getSaveFileName(this,"Сохранить","","Файл XML(*.xml)");
    if(FileSaving != "") {
        exportModelToXml(FileSaving);
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_btnParse_clicked()
{
    QMessageBox msg;
    if(filename.length()==0)
    {
       msg.setText("Не выбран файл");
       msg.exec();
       return;
    }
    QFile file1(filename);
    file1.open(QFile::ReadOnly | QFile::Text);

    QString path2 = QDir::tempPath() + "/file.idl";
    QFile file2(path2);
    file2.open(QFile::WriteOnly | QFile::Text);

    QTextStream in(&file1);
    QTextStream out(&file2);
    QString allString = in.readAll();
    QStringList myList = allString.split("\n");
    QString line;
    QString tmpLine = "";
    for(int i=0; i<myList.size(); i++ )
    {
        line = myList.at(i);
        if(line.indexOf(" ;")!=-1)
        {
            out<<line<<"\n";
        }
        else
        {
            tmpLine += line;
            while((line.indexOf(" ;")==-1)&&(i<myList.size()-1))
            {
                i++;
                line = myList.at(i);
                tmpLine.replace("\n", " ");
                tmpLine += line;
            }
            out<<tmpLine<<"\n";
            tmpLine = "";
        }

    }
    file1.close();
    file2.close();

    IdlParser *parser = new IdlParser(path2);
    parser->parse();
    ekit = parser->getEkit();
    edges = parser->getEdges();
    searchItems();
    saveXml();
    return;
}

void MainWindow::on_btnExit_clicked()
{
    qApp->exit();
}


void MainWindow::on_btnAdd_clicked()
{
    QMessageBox msg;
    if(ui->findName->text().length()==0)
    {
        msg.setText(tr("Не указано название документа"));
        msg.exec();
        return;
    }
    if(ui->findProperty->text().length()==0)
    {
        msg.setText(tr("Не указано свойство документа"));
        msg.exec();
        return;
    }
    ui->tableWidget->setRowCount(ui->tableWidget->rowCount()+1);
    QTableWidgetItem *qtablewidgetitem1 = new QTableWidgetItem(ui->findName->text());
    ui->tableWidget->setItem(ui->tableWidget->rowCount()-1,0,qtablewidgetitem1);
    QTableWidgetItem *qtablewidgetitem2 = new QTableWidgetItem(ui->findProperty->text());
    ui->tableWidget->setItem(ui->tableWidget->rowCount()-1,1,qtablewidgetitem2);

//    exportModelToXml("documents.xml");

}

void MainWindow::on_btnClear_clicked()
{
    while (ui->tableWidget->rowCount() > 0)
    {
        ui->tableWidget->removeRow(0);
    }
    isResult = false;
}

void MainWindow::on_btnBrowse_clicked()
{
    filename = QFileDialog::getOpenFileName(this);
    QFile file(filename);
    if (!file.open(QFile::ReadOnly | QFile::Text)) return;
    file.close();
    ui->txtFile->setText(filename);
}

void MainWindow::searchItems()
{
    searchList.clear();
    int nDiag = 0;
    for(int i =0; i < ui->tableWidget->rowCount();i++)
    {
        nDouble item;
        item.nDiag = -1;
        item.nType = -1;
        searchList<<item;
    }
    for(int i =0; i < ui->tableWidget->rowCount();i++)
    {
        QString searchDocument = ui->tableWidget->item(i,0)->text();
        QString searchLabel = ui->tableWidget->item(i,1)->text();
        nDiag = 0;
        foreach(EDiagram *item, ekit->getModel()->getDiagrams())
        {
            foreach(EArrowSeg *item2, item->getArrowsegs())
            {
                if((item2->getLabel()!=NULL)&&(item2->getLabel().indexOf(searchDocument)!=-1))
                {
                    foreach(edge *e1, edges)
                    {
                        if((e1->getNDiag()==nDiag)&&(e1->getType()->name.indexOf(searchLabel)!=-1))
                        {
                            nDouble item;
                            item.nDiag = nDiag;
                            item.nType = e1->getType()->id;
                            item.nameDoc = searchDocument;
                            searchList[i]=item;
                            break;
                        }
                    }
                    break;
                }
            }
            nDiag++;
        }
        nDouble item1;
        item1.nDiag = -1;
        item1.nType = -1;
        if((searchList[i].nDiag==item1.nDiag)&&(searchList[i].nType==item1.nType))
        {
            QTableWidgetItem *qtablewidgetitem1 = new QTableWidgetItem(tr("Не найден"));
            ui->tableWidget->setItem(i,2,qtablewidgetitem1);
        }
        else
        {
            QTableWidgetItem *qtablewidgetitem1 = new QTableWidgetItem(tr("Найден"));
            ui->tableWidget->setItem(i,2,qtablewidgetitem1);
        }
    }
    isResult = true;
}

void MainWindow::saveXml()
{
     //По searchlist строим xml файлы
    foreach(nDouble nd1, searchList)
    {
        if((nd1.nDiag==-1)&&(nd1.nType==-1))continue;
        if(ekit->getModel()->getDiagram(nd1.nDiag)->getBoxes().count()>0)
        {
            QXmlStreamWriter print_Xml;
            QBuffer buffer;
            QFile file(qApp->applicationDirPath() +"/file" + QString::number(nd1.nDiag)+ QString::number(nd1.nType)+".xml");
            if (buffer.open(QIODevice::WriteOnly))
            {
                print_Xml.setDevice(&buffer);
                print_Xml.setAutoFormatting(true);
                print_Xml.writeStartDocument();

                print_Xml.writeStartElement("top");
                print_Xml.writeStartElement("edges");
                print_Xml.writeAttribute("name",nd1.nameDoc);
                foreach(edge *ed1, edges)
                {
                    if((ed1->getNDiag()==nd1.nDiag)&&(ed1->getType()->id==nd1.nType))
                    {
                        for(int j = 0; j < ed1->getEnds().count();j++)
                        {
                            print_Xml.writeStartElement("edge");
                            print_Xml.writeStartElement("id");
                            //Здесь мы исходя из особенности файла idl не id вводить будет а точку старта которая всегда начинается с 1!!!
                            print_Xml.writeCharacters(/*ed1->getId()*/QString::number(ed1->getStart()));
                            print_Xml.writeEndElement();
                            print_Xml.writeStartElement("title");
                            print_Xml.writeCharacters(ed1->getName());
                            print_Xml.writeEndElement();
                            print_Xml.writeStartElement("goto");
                            int *num = ed1->getEnd(j);
                            print_Xml.writeCharacters(QString::number(*num));
                            print_Xml.writeEndElement();
                            print_Xml.writeEndElement();
                        }
                        if(ed1->getEnds().count()==0)
                        {
                            print_Xml.writeStartElement("edge");
                            print_Xml.writeStartElement("id");
                            //Здесь мы исходя из особенности файла idl не id вводить будет а точку старта которая всегда начинается с 1!!!
                            print_Xml.writeCharacters(/*ed1->getId()*/QString::number(ed1->getStart()));
                            print_Xml.writeEndElement();
                            print_Xml.writeStartElement("title");
                            print_Xml.writeCharacters(ed1->getName());
                            print_Xml.writeEndElement();
                            print_Xml.writeStartElement("goto");
                            print_Xml.writeCharacters("END");
                            print_Xml.writeEndElement();
                            print_Xml.writeEndElement();
                        }
                    }
                }

                print_Xml.writeEndElement();
                print_Xml.writeEndElement();
                print_Xml.writeEndDocument();
                buffer.close();
            }

            if (file.open(QIODevice::WriteOnly))
            {
                    QByteArray arr = buffer.buffer();
                    QTextStream stdOut(&file);
                    stdOut << QString(arr);
                    file.close();
            }
        }
    }
}

void MainWindow::on_tableWidget_clicked(QModelIndex index)
{
    if(index.column()!=2)return;
    else
    {
        if(isResult==true)
        {
            showGraph(searchList.at(index.row()));
        }
    }
}

void MainWindow::showGraph(nDouble nFile)
{
    GraphDialog dlg(this,nFile.nDiag,nFile.nType);
    dlg.exec();
}


void MainWindow::exportModelToXml(const QString& fileName)
{
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly)) {
        QMessageBox::critical(this, "", tr("Ошибка сохранения в файл.\n%1").arg(file.errorString()));
        return;
    }

    QXmlStreamWriter stream(&file);
    stream.setAutoFormatting(true);
    stream.writeStartDocument();
    stream.writeStartElement("table");

    for (int i = 0, rowCount = ui->tableWidget->model()->rowCount(); i < rowCount; i++) {
        stream.writeStartElement("row");

        for (int j = 0, columnCount = ui->tableWidget->model()->columnCount(); j < columnCount-1; j++) {
            const QVariant& value = ui->tableWidget->model()->index(i, j).data(Qt::DisplayRole);
            QString columnName;

            if(j==0) {
                columnName = "docname";
            }
            else if(j==1) {
                columnName = "property";
            }
            //qDebug() << columnName;
            stream.writeTextElement(columnName,value.toString());
            //stream.writeTextElement(ui->tableWidget->model()->headerData(j, Qt::Horizontal, Qt::EditRole).toString(),value.toString());
        }
        stream.writeEndElement();
    }

    stream.writeEndElement();
    stream.writeEndDocument();
    file.close();
}

void MainWindow::importXmlToModel(const QString& fileName) {
    /*
    QString FileSaving = QFileDialog::getSaveFileName(this,"Сохранить","c:/","Файл CSV(*.csv)");
    QFile file(FileSaving);
    if (FileSaving.isEmpty()){
        file.close();
    } else {
    if(!file.open(QIODevice::WriteOnly|QIODevice::Text)){
        QMessageBox::StandardButton Save;
        Save=QMessageBox::critical(this,"Ошибка","Ошибка сохранения",QMessageBox::Ok);
        return;
    }
    }
    QTextStream save(&file);

    QString FileLoading = QFileDialog::getOpenFileName(this,"Открыть","c:/","Файл CSV(*.csv)");
    QFile file(FileLoading);
    */

    QDomDocument domDoc;
    QDomElement  domElement;
    QFile file(fileName);

    if( file.exists() ) {

        if (!file.open(QIODevice::ReadOnly)){
            qDebug()<< "ERROR"<< endl;
            return;
        }

        if ( domDoc.setContent ( &file ) )
        {
            domElement = domDoc.documentElement();
            traverseNode ( domElement );
        }
        file.close();
        //QStandardItemModel *model = new QStandardItemModel();
        //preOrder(doc.firstChild(), model);
    }

}


void MainWindow::traverseNode(const QDomNode & node)
{
   //----------------------------------
   QDomNode    domNode;
   QDomElement domElement;
   QDomText    dom_text;
   //----------------------------------

   domNode = node.firstChild();
   while( ! domNode.isNull())
   {
       if ( domNode.isElement())
       {
          domElement = domNode.toElement();

          if(!domElement.isNull())
          {
              if(domElement.tagName() == "docname") {
                  ui->tableWidget->setRowCount(ui->tableWidget->rowCount()+1);
                  QTableWidgetItem *qtablewidgetitem1 = new QTableWidgetItem(domElement.text());
                  ui->tableWidget->setItem(ui->tableWidget->rowCount()-1,0,qtablewidgetitem1);
              }
              else if(domElement.tagName() == "property") {
                  QTableWidgetItem *qtablewidgetitem2 = new QTableWidgetItem(domElement.text());
                  ui->tableWidget->setItem(ui->tableWidget->rowCount()-1,1,qtablewidgetitem2);
              }
          }
       }
       traverseNode(domNode);

       domNode = domNode.nextSibling();
    }
}
/*
void MainWindow::insertFather(QString name){
    item = new QStandardItem(name);
    item->setEditable(false);
}

void MainWindow::insertChildren(QString name){
    QStandardItem *subItem = new QStandardItem( name );
    subItem->setCheckable(true);
    subItem->setEditable(false);
    item->appendRow(subItem);
}

void MainWindow::setItem(QStandardItemModel *model){
    model->setItem(nRow, item);
    nRow++;
}

void MainWindow::preOrder(QDomNode dom, QStandardItemModel *model){
    if(!dom.isNull()){
        string aux = dom.nodeName().toStdString();
        string name = "docname";
        string value = "property";
        if(dom.isText()){
            aux = dom.parentNode().nodeName().toStdString();
            if(strcmp(aux.c_str(),name.c_str())==0){
                cout << dom.nodeValue().toStdString() << endl;
                insertFather(dom.nodeValue());
                setItem(model);
            }
            if(strcmp(aux.c_str(),value.c_str())==0){
                cout << "\t" << dom.nodeValue().toStdString() << endl;
                insertChildren(dom.nodeValue());
            }
        }else{
            preOrder(dom.firstChild(), model);
            preOrder(dom.nextSibling(), model);
        }

    }
}
*/
