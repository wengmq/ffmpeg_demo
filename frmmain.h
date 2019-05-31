#ifndef FRMMAIN_H
#define FRMMAIN_H

#include <QWidget>

namespace Ui {
class frmMain;
}

class frmMain : public QWidget
{
    Q_OBJECT

public:
    explicit frmMain(QWidget *parent = 0);
    ~frmMain();

private slots:
    void SetImage(const QImage &image);
    void on_btnOpen_clicked();
    void on_btnGetImage_clicked();

    void on_ckAll_stateChanged(int arg1);

protected:
    bool eventFilter(QObject *obj, QEvent *event);

private:
    Ui::frmMain *ui;

    int tempWidth;
    int tempHeight;
    bool video1Max;
    bool video2Max;
    bool video3Max;
    bool all;    
};

#endif // FRMMAIN_H
