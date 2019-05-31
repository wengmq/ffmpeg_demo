#include "frmmain.h"
#include "ui_frmmain.h"
#include "qffmpeg.h"
#include "rtspthread.h"
#include <QDebug>
frmMain::frmMain(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::frmMain)
{
    ui->setupUi(this);    

    tempWidth=320;
    tempHeight=180;
    video1Max=false;
    video2Max=false;
    video3Max=false;
    all=false;
    ui->labVideo1->installEventFilter(this);
    ui->labVideo2->installEventFilter(this);
    ui->labVideo3->installEventFilter(this);
}

frmMain::~frmMain()
{
    delete ui;
}

//处理用户双击对应通道最大化处理
bool frmMain::eventFilter(QObject *obj, QEvent *event)
{    
    if (event->type()==QEvent::MouseButtonDblClick){
        if (obj==ui->labVideo1){
            if (video1Max){
                tempWidth=320;
                tempHeight=180;
                ui->labVideo2->setVisible(true);
                ui->labVideo3->setVisible(true);
                ui->labImage->setVisible(true);
            }else{
                tempWidth=645;
                tempHeight=370;
                ui->labVideo2->setVisible(false);
                ui->labVideo3->setVisible(false);
                ui->labImage->setVisible(false);
            }
            video1Max=!video1Max;
        }else if (obj==ui->labVideo2){
            if (video2Max){
                tempWidth=320;
                tempHeight=180;
                ui->labVideo1->setVisible(true);
                ui->labVideo3->setVisible(true);
                ui->labImage->setVisible(true);
            }else{
                tempWidth=645;
                tempHeight=370;
                ui->labVideo1->setVisible(false);
                ui->labVideo3->setVisible(false);
                ui->labImage->setVisible(false);
            }
            video2Max=!video2Max;
        }else if (obj==ui->labVideo3){
            if (video3Max){
                tempWidth=320;
                tempHeight=180;
                ui->labVideo1->setVisible(true);
                ui->labVideo2->setVisible(true);
                ui->labImage->setVisible(true);
            }else{
                tempWidth=645;
                tempHeight=370;
                ui->labVideo1->setVisible(false);
                ui->labVideo2->setVisible(false);
                ui->labImage->setVisible(false);
            }
            video3Max=!video3Max;
        }
    }
    return QObject::eventFilter(obj,event);
}

void frmMain::on_btnOpen_clicked()
{    
    QFFmpeg *ffmpeg=new QFFmpeg(this);
    connect(ffmpeg,SIGNAL(GetImage(QImage)),this,SLOT(SetImage(QImage)));
    ffmpeg->SetUrl(ui->txtUrl->text());

    if (ffmpeg->Init()){
        RtspThread *rtsp=new RtspThread(this);
        rtsp->setffmpeg(ffmpeg);
        rtsp->start();
    }
}

void frmMain::on_btnGetImage_clicked()
{
    ui->labImage->clear();
    int index=ui->cboxVideo->currentIndex();
    if (index==0){
        if (ui->labVideo1->pixmap()!=0x0)
            ui->labImage->setPixmap(*ui->labVideo1->pixmap());
    }else if (index==1){
        if (ui->labVideo2->pixmap()!=0x0)
            ui->labImage->setPixmap(*ui->labVideo2->pixmap());
    }else if (index==2){
        if (ui->labVideo3->pixmap()!=0x0)
            ui->labImage->setPixmap(*ui->labVideo3->pixmap());
    }
}

void frmMain::SetImage(const QImage &image)
{
    if (image.height()>0){
        QPixmap pix = QPixmap::fromImage(image.scaled(tempWidth,tempHeight));
        ui->labVideo1->setPixmap(pix);
        if (all){//启用三通道同步
            ui->labVideo2->setPixmap(pix);
            ui->labVideo3->setPixmap(pix);
        }
    }
}

void frmMain::on_ckAll_stateChanged(int arg1)
{
    all=arg1!=0?true:false;
}
