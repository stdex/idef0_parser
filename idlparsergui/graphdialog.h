#ifndef GRAPHDIALOG_H
#define GRAPHDIALOG_H

#include <QDialog>

namespace Ui {
    class GraphDialog;
}

class GraphDialog : public QDialog
{
    Q_OBJECT
    int nDiag;
    int nType;
public:
    explicit GraphDialog(QWidget *parent = 0, int nDiag = 0,int nType = 0);
    ~GraphDialog();

private:
    Ui::GraphDialog *ui;
};

#endif // GRAPHDIALOG_H
