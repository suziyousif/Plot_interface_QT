#include "graphplot.h"
#include "ui_graphplot.h"
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include "stdio.h"
#include "tinyexpr.h"

graphplot::graphplot(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::graphplot)
{
    ui->setupUi(this);
    i_graph = 0;
}

graphplot::~graphplot()
{
    delete ui;
}

void graphplot::on_OpenFileButton_clicked()
{
    QString filename = ui->lineEdit_Filname->text();
    QFile file(filename);
    if(!file.open(QIODevice::ReadOnly)){
        QMessageBox::information(this, "info", file.errorString());
    }else {
        QTextStream in(&file);
        ui->textBrowser->setText(in.readAll());
        in.seek(0);
        QString line = in.readLine();
        QStringList parts = line.split(',');
        this->xlabel = parts[0];
        this->ylabel = parts[1];
        line = in.readLine();
        //clear old data
        x0.clear();
        y0.clear();
        while (!line.isNull()) {
            parts = line.split(',');
            x0.push_back(parts[0].toDouble());
            y0.push_back(parts[1].toDouble());
            line = in.readLine();
        }
    }
}

void graphplot::on_PlotFileDataButton_clicked()
{

    int graphItem=ui->customPlot->graphCount();
    if(graphItem!=0)
       i_graph = graphItem;
    else {
        i_graph =0;
    }
    ui->customPlot->addGraph();
    ui->customPlot->graph(i_graph)->setPen(QPen(Qt::blue)); // line color blue for first graph
    ui->customPlot->graph(i_graph)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, 5));

    if(ui->radioButton_Col1_yAxis->isChecked()){
        ui->customPlot->graph(i_graph)->setData(this->y0, this->x0);
        ui->customPlot->xAxis->setLabel(this->ylabel);
        ui->customPlot->yAxis->setLabel(this->xlabel);
    }else{
        ui->customPlot->graph(i_graph)->setData(this->x0, this->y0);
        ui->customPlot->xAxis->setLabel(this->xlabel);
        ui->customPlot->yAxis->setLabel(this->ylabel);
    }

    ui->customPlot->graph(i_graph)->rescaleAxes();
    ui->customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
    ui->customPlot->replot();

}

void graphplot::on_PlotFunButton_clicked()
{
    if(!((ui->lineEdit_Function->text()).isEmpty()) && !((ui->lineEdit_xInterval->text()).isEmpty())){
        double x=0;
        te_variable vars[] = {{"x", &x}};

        int err;
        // Compile the expression with variables.
        te_expr *expr = te_compile((ui->lineEdit_Function->text()).toUtf8(), vars, 1, &err);
        //Determine X interval
        QString line = ui->lineEdit_xInterval->text();
        QStringList parts = line.split(':');
        ui->customPlot->xAxis->setRange(parts[0].toDouble(), parts[1].toDouble());
        //clear old data
        x1.clear();
        y1.clear();
        if (expr) {
            if(parts.size()<3){
                for(x=parts[0].toDouble(); x<(parts[1].toDouble()); x++){
                    y1.push_back(te_eval(expr));
                    x1.push_back(x);
                }
            }else { //if user determined a step
                for(x=parts[0].toDouble(); x<(parts[2].toDouble()); x=(x+(parts[1].toDouble()))){
                    y1.push_back(te_eval(expr));
                    x1.push_back(x);
                 }
            }
            int graphItem=ui->customPlot->graphCount();
            if(graphItem!=0)
               i_graph = graphItem;
            else {
                i_graph =0;
            }

            ui->customPlot->addGraph();
            ui->customPlot->graph(i_graph)->setPen(QPen(Qt::red));
            ui->customPlot->graph(i_graph)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, 5));
            ui->customPlot->graph(i_graph)->setData(this->x1, this->y1);
            ui->customPlot->graph(i_graph)->rescaleAxes();
            ui->customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
            ui->customPlot->replot();
        }
        else{
            QMessageBox::critical(this, "invalid Function","try again", QMessageBox::Ok);
        }
        te_free(expr);
    }
    else
        QMessageBox::critical(this, "invalid Plot","you need to insert a function/an interval!", QMessageBox::Retry);
}

void graphplot::on_SetxLabelButton_clicked()
{
    ui->customPlot->xAxis->setLabel(ui->lineEdit_xLabel->text());
    ui->customPlot->replot();
}

void graphplot::on_SetyLabelButton_clicked()
{
    ui->customPlot->yAxis->setLabel(ui->lineEdit_yLabel->text());
    ui->customPlot->replot();
}

void graphplot::on_ClearGraphButton_clicked()
{
    ui->customPlot->clearGraphs();
    ui->customPlot->replot();
}

void graphplot::on_SaveGraphButton_clicked()
{
    QString outputDir = ui->lineEdit_LocalSaveFig->text();
    QFile file(outputDir);

     if (!file.open(QIODevice::WriteOnly|QFile::WriteOnly))
          {
              QMessageBox::warning(this,"Could not create Project File",
                                         QObject::tr( "\n Could not create Project File on disk"));


          }

    bool response = ui->customPlot->saveJpg( outputDir,  0, 0, 1.0, -1  );
    if(response)
       QMessageBox::information(this, "info", "Figure saved!");
    else {
       QMessageBox::warning(this, "info", "Could not save the figure");
    }
}
