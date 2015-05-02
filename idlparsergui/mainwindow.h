#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStandardItemModel>
#include <QDomNode>
#include "edge.h"
#include <QModelIndex>

class EKit;
class EDiagram;
namespace Ui {
    class MainWindow;
}

struct nDouble{
    int nDiag;
    int nType;
    QString nameDoc;
};

class MainWindow : public QMainWindow
{
    Q_OBJECT
    EKit *ekit;
    QString filename;
    QList<edge*> edges;
    QList<nDouble> searchList;
    bool isResult;
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void searchItems();
    void saveXml();
    void showGraph(nDouble nFile);
    void exportModelToXml(const QString& fileName);
    void importXmlToModel(const QString& fileName);
    void traverseNode(const QDomNode & node);

private:
    Ui::MainWindow *ui;

private slots:
    void on_tableWidget_clicked(QModelIndex index);
    void on_btnBrowse_clicked();
    void on_btnClear_clicked();
    void on_btnAdd_clicked();
    void on_btnExit_clicked();
    void on_btnParse_clicked();
    void open();
    void save();
};

#endif // MAINWINDOW_H
