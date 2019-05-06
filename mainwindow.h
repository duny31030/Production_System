#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTextEdit>


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void init();
    void init2();
    void scan(QString str);
    void reasoning();
    void Judge_last();
    void prin(int a,int b,int c = 0,int d = 0,int e = 0);
    void prin2(int a);
    QMap<int,QString> m;
    QMap<int,bool> m2;

private:
    Ui::MainWindow *ui;
    QString savePath = ":/txt/1.txt";
    QString rst = "开始推理\n";
    QString scanStr = "";
    QString initShow = "1:有毛发  2:产奶  3:有羽毛  4:不会飞  5:会下蛋  6:吃肉  7:有犬齿  8:有爪  9:眼盯前方  10:有蹄  11:反刍  12:黄褐色  13:有斑点  14:有黑色条纹  "
                       "15:长脖  16:长腿  17:不会飞  18:会游泳  19:黑白二色  20:善飞  21:哺乳类 22:鸟类  23:食肉类  24:蹄类\n";

private slots:
    void openGit();
    void openBlog();
    void showFile();
    void openFile();
    void saveFile();
    void savetoFile();
    void forwardPush();
    void reversePush();
};

#endif // MAINWINDOW_H
