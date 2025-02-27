#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "opencv2/opencv.hpp"
#include "opencv_modules.hpp"
#include "qabstractanimation.h"
#include "QPixmap"
#include "QDebug"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "highgui.h"
#include <stdlib.h>
#include <stdio.h>

QImage cvMat2QImage (const cv::Mat& mat);
cv::Mat img;
cv::Mat greyim;
cv::Mat thresh;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QPixmap pix("ricegrain.jfif");
    img = cv::imread("ricegrain.jfif");
    cv::cvtColor(img,greyim,cv::COLOR_BGR2GRAY,0);
    cv::threshold(greyim,thresh,160,255,cv::THRESH_BINARY_INV);
    int h = ui->picturelabel->height();
    int w = ui->picturelabel->width();
    ui ->picturelabel -> setPixmap(pix.scaled (w,h,Qt::KeepAspectRatio));
}

MainWindow::~MainWindow()
{
    delete ui;
}
QImage cvMat2QImage(const cv::Mat& mat)
{
    // 8-bits unsigned, NO. OF CHANNELS = 1
    if(mat.type() == CV_8UC1)
    {
        QImage image(mat.cols, mat.rows, QImage::Format_Indexed8);
        // Set the color table (used to translate colour indexes to qRgb values)
        image.setColorCount(256);
        for(int i = 0; i < 256; i++)
        {
            image.setColor(i, qRgb(i, i, i));
        }
        // Copy input Mat
        uchar *pSrc = mat.data;
        for(int row = 0; row < mat.rows; row ++)
        {
            uchar *pDest = image.scanLine(row);
            memcpy(pDest, pSrc, mat.cols);
            pSrc += mat.step;
        }
        return image;
    }
    else if(mat.type() == CV_8UC3)
       {
           // Copy input Mat
           const uchar *pSrc = (const uchar*)mat.data;
           // Create QImage with same dimensions as input Mat
           QImage image(pSrc, mat.cols, mat.rows, mat.step, QImage::Format_RGB888);
           return image.rgbSwapped();
       }
       else if(mat.type() == CV_8UC4)
       {
           qDebug() << "CV_8UC4";
           // Copy input Mat
           const uchar *pSrc = (const uchar*)mat.data;
           // Create QImage with same dimensions as input Mat
           QImage image(pSrc, mat.cols, mat.rows, mat.step, QImage::Format_ARGB32);
           return image.copy();
       }
       else
       {
           qDebug() << "ERROR: Mat could not be converted to QImage.";
           return QImage();
       }
   }



void MainWindow::on_erosion_clicked()
{
    cv::Mat modified = thresh.clone();
    int w = ui->picturelabel->width();
    int h = ui->picturelabel->height();
    int erosion_type = 0;
    QString text = ui->element1->text();
    int erosion_elem;
    erosion_elem = text.toInt();
    if( erosion_elem == 0 ){ erosion_type = cv::MORPH_RECT; }
    else if( erosion_elem == 1 ){ erosion_type =cv:: MORPH_CROSS; }
    else if( erosion_elem == 2) { erosion_type =cv:: MORPH_ELLIPSE; }
    /*Mat sharpeningKernel = (Mat_<double>(3,3) << 0, -1, 0, -1, 5, -1, 0, -1, 0);*/
    cv::Mat element = cv::getStructuringElement( erosion_type, cv::Size(10,10), cv::Point(-1,-1));
    cv::erode( thresh, modified, element );
    ui->picturelabel->setPixmap(QPixmap::fromImage(cvMat2QImage(modified)).scaled(w,h,Qt::KeepAspectRatio));
 }

void MainWindow::on_elementslider1_valueChanged(int value)
{
    ui->element1->setText(QString("%1").arg(value));
}



void MainWindow::on_elementslider2_valueChanged(int value)
{
    ui->element2->setText(QString("%1").arg(value));
}

void MainWindow::on_dilation_clicked()
{
    cv::Mat modified = thresh.clone();
    int w = ui->picturelabel->width();
    int h = ui->picturelabel->height();
    int dilation_type = 0;
    QString text = ui->element2->text();
    int dilation_elem;
    dilation_elem = text.toInt();
    if( dilation_elem == 0 ){ dilation_type = cv::MORPH_RECT; }
    else if( dilation_elem == 1 ){ dilation_type =cv:: MORPH_CROSS; }
    else if( dilation_elem == 2) { dilation_type =cv:: MORPH_ELLIPSE; }
    /*Mat sharpeningKernel = (Mat_<double>(3,3) << 0, -1, 0, -1, 5, -1, 0, -1, 0);*/
    cv::Mat element = cv::getStructuringElement( dilation_type, cv::Size(10,10), cv::Point(-1,-1));
    cv::dilate( thresh, modified, element );
    ui->picturelabel->setPixmap(QPixmap::fromImage(cvMat2QImage(modified)).scaled(w,h,Qt::KeepAspectRatio));
}

void MainWindow::on_binary_clicked()
{
    cv::Mat modified = greyim.clone();
    int w = ui->picturelabel->width();
    int h = ui->picturelabel->height();
    cv::threshold(greyim,modified,160,255,cv::THRESH_BINARY_INV);
    ui->picturelabel->setPixmap(QPixmap::fromImage(cvMat2QImage(modified)).scaled(w,h,Qt::KeepAspectRatio));
}

void MainWindow::on_reset_clicked()
{
    ui->elementslider1->setValue(0);
    ui->elementslider2->setValue(0);
    ui->ricenum->setText("num of rice ");
    QPixmap pix("ricegrain.jfif");
    img = cv::imread("ricegrain.jfif");
    int h = ui->picturelabel->height();
    int w = ui->picturelabel->width();
    ui ->picturelabel -> setPixmap(pix.scaled (w,h,Qt::KeepAspectRatio));
    ui->binary->setAutoExclusive(false);
    ui->binary->setChecked(false);
    ui->binary->setAutoExclusive(true);
    ui->erosion->setAutoExclusive(false);
    ui->erosion->setChecked(false);
    ui->erosion->setAutoExclusive(true);
    ui->dilation->setAutoExclusive(false);
    ui->dilation->setChecked(false);
    ui->dilation->setAutoExclusive(true);
    ui->contour->setAutoExclusive(false);
    ui->contour->setChecked(false);
    ui->contour->setAutoExclusive(true);

}





void MainWindow::on_contour_clicked()
{
    cv::Mat modified = thresh.clone();
    int w = ui->picturelabel->width();
    int h = ui->picturelabel->height();
    std::vector<std::vector<cv::Point> > contours;
    std::vector<cv::Vec4i> hierarchy;
    std::vector<std::vector<cv::Point> > bigcontours;
    cv::findContours(modified,contours,hierarchy,CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE,cv::Point(0, 0) );
    cv::drawContours(modified,contours,-1, (0,255,0), 3);
    for (int i=0; i<contours.size(); i++)
    {
            if (contourArea(contours[i]) > 30)
            bigcontours.push_back(contours[i]);
    }
    QString n = QString::number(bigcontours.size());
    ui->ricenum->setText("num of rice "+ n);
    ui->picturelabel->setPixmap(QPixmap::fromImage(cvMat2QImage(modified)).scaled(w,h,Qt::KeepAspectRatio));
}


