#ifndef GRAPHPLOT_H
#define GRAPHPLOT_H
#include "stdio.h"
#include <QIODevice>
#include <QMainWindow>

namespace Ui {
class graphplot;
}

class graphplot : public QMainWindow
{
    Q_OBJECT

public:
    explicit graphplot(QWidget *parent = nullptr);
    ~graphplot();

private slots:
    void on_OpenFileButton_clicked();

    void on_SetxLabelButton_clicked();

    void on_SetyLabelButton_clicked();

    void on_PlotFunButton_clicked();

    void on_PlotFileDataButton_clicked();

    void on_ClearGraphButton_clicked();

    void on_SaveGraphButton_clicked();

private:
    Ui::graphplot *ui;
    void OpenFile(FILE *fp,QIODevice::OpenMode mode);
    QVector<double> x0, y0;
    QVector<double> x1, y1;
    QString xlabel;
    QString ylabel;
    int i_graph;

};

#endif // GRAPHPLOT_H
