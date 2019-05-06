#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtGui>
#include <QtWidgets>
#include <QMap>
#include <stdio.h>
#include <algorithm>
#include <string>
#include <string.h>
#include <QString>
#include <QDebug>
#include <QChar>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    init();
    ui->setupUi(this);
    connect(ui->showKonw, &QPushButton::pressed, this, &MainWindow::showFile);
    connect(ui->openKonw, &QPushButton::pressed, this, &MainWindow::openFile);
    connect(ui->saveKonw, &QPushButton::pressed, this, &MainWindow::saveFile);
    connect(ui->outKonw, &QPushButton::pressed, this, &MainWindow::savetoFile);

    // setCentralWidget(ui->showK);

    connect(ui->quit, &QPushButton::pressed, this, [=]()
    {
        int ret = QMessageBox::warning(this, "退出", "确认要退出吗？", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
        if(ret == QMessageBox::Yes){    qApp->quit();   }
    }
    );


    setWindowTitle("动物识别系统 v1.0");
    //
    // 菜单栏
    //
    QMenuBar *mBar = menuBar();
    setMenuBar(mBar);
    // 添加菜单
    QMenu *pKnow = mBar->addMenu("知识库");
    QMenu *pReson = mBar->addMenu("推理");
    QMenu *pAbout = mBar->addMenu("关于作者");

    // 知识库
    QAction *pShow = pKnow->addAction("显示知识库");
    connect(pShow, &QAction::triggered, this, &MainWindow::showFile);
    QAction *pOpen = pKnow->addAction("导入知识库");
    connect(pOpen, &QAction::triggered, this, &MainWindow::openFile);
    QAction *pSave = pKnow->addAction("保存知识库");
    connect(pSave, &QAction::triggered, this, &MainWindow::saveFile);
    QAction *pOut = pKnow->addAction(tr("导出知识库"));
    connect(pOut, &QAction::triggered, this, &MainWindow::savetoFile);

    // 推理
    QAction *pForward = pReson->addAction("正向推理");
    QAction *pReverse = pReson->addAction("反向推理");

    connect(pForward, &QAction::triggered, this, &MainWindow::forwardPush);
    connect(ui->Forward, &QPushButton::pressed, this, &MainWindow::forwardPush);
    connect(pReverse, &QAction::triggered, this, &MainWindow::reversePush);
    connect(ui->Reverse, &QPushButton::pressed, this, &MainWindow::reversePush);

    // 关于作者
    QAction *pAuthor = pAbout->addAction("关于");
    QAction *pGit = pAbout->addAction("Github");
    QAction *pBlog = pAbout->addAction("Blog");
    connect(pAuthor, &QAction::triggered, [=](){QMessageBox::about(this, "关于","产生式系统(动物识别系统)\n\n作者:杜宁元\n版本:v1.0\n联系方式:461336274\n");});
    connect(pGit, &QAction::triggered, [=](){openGit();});
    connect(pBlog, &QAction::triggered, [=](){openBlog();});

    QTextEdit *hit = new QTextEdit(this);
    hit->append("<font color=\"#FF0000\">请在左侧文本框内输入对应特征前的序号，使用空格分割。然后点击</font><font color = \"#0000FF\">正向推理</font>");
    hit->append(initShow);

    (ui->ruleInit)->setWidget(hit);

}

void MainWindow::openGit()
{
    QString link = "https://github.com/duny31030";
    QDesktopServices::openUrl(QUrl(link));
}
void MainWindow::openBlog()
{
    QString link = "https://duny31030.top";
    QDesktopServices::openUrl(QUrl(link));
}

void MainWindow::showFile()
{
    if(!savePath.isEmpty())
    {
        QFile file(savePath);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            QMessageBox::warning(this, tr("Read File"), tr("错误，无法打开文件:\n%1 \n请检查目录下是否有对应文件").arg(savePath));
            return;
        }
        QTextStream in(&file);
        (ui->showK)->setText(in.readAll());
        file.close();
    }
    else
    {
        QMessageBox::warning(this, tr("Path"), tr("文件打开过程中出错，请先手动选择导入知识库\n!"));
    }
}

void MainWindow::openFile()
{
    QString path = QFileDialog::getOpenFileName(this, tr("Open File"), ".", tr("Text Files(*.txt)"));
    // qDebug() << path;
    savePath = path;
    if(!path.isEmpty())
    {
        QFile file(path);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            QMessageBox::warning(this, tr("Read File"), tr("错误，无法打开文件:\n%1").arg(path));
            return;
        }
        QTextStream in(&file);
        (ui->showK)->setText(in.readAll());
        file.close();
    }
    else
    {
        QMessageBox::warning(this, tr("Path"), tr("你没有选择任何文件!"));
    }
}

void MainWindow::saveFile()
{
    if(!savePath.isEmpty())
    {
        QFile file(savePath);
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            QMessageBox::warning(this, tr("Write File"), tr("无法写入到目录:\n%1").arg(savePath));
            return;
        }
        QTextStream out(&file);
        out << ui->showK->toPlainText();
        file.close();
    }
    else
    {
        QMessageBox::warning(this, tr("Path"), tr("保存知识库过程中出错，请手动选择导出知识库."));
    }
}

void MainWindow::savetoFile()
{
    QString path = QFileDialog::getSaveFileName(this, tr("Save File"), ".", tr("Text Files(*.txt)"));
    if(!path.isEmpty())
    {
        QFile file(path);
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            QMessageBox::warning(this, tr("Write File"), tr("Cannot open file:\n%1").arg(path));
            return;
        }
        QTextStream out(&file);
        out << ui->showK->toPlainText();
        file.close();
    }
    else
    {
        QMessageBox::warning(this, tr("Path"), tr("You did not select any file."));
    }
}

void MainWindow::forwardPush()
{
    scanStr = (ui->scanRule)->toPlainText();
    init2();
    scan(scanStr);
    reasoning();
    Judge_last();

    (ui->resonResult)->setPlainText(rst);
}

void MainWindow::reversePush()
{

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::init()
{
    m[1] = "有毛发";m[2] = "产奶";m[3] = "有羽毛";m[4] = "不会飞";
    m[5] = "会下蛋";m[6] = "吃肉";m[7] = "有犬齿";m[8] = "有爪";
    m[9] = "眼盯前方";m[10] = "有蹄";m[11] = "反刍";m[12] = "黄褐色";
    m[13] = "有斑点";m[14] = "有黑色条纹";m[15] = "长脖";m[16] = "长腿";
    m[17] = "不会飞";m[18] = "会游泳";m[19] = "黑白二色";m[20] = "善飞";
    m[21] = "哺乳类";m[22] = "鸟类";m[23] = "食肉类";m[24] = "蹄类";
    m[25] = "金钱豹";m[26] = "虎";m[27] = "长颈鹿";m[28] = "斑马";
    m[29] = "鸵鸟";m[30] = "企鹅";m[31] = "信天翁";
}

void MainWindow::init2()
{
    for(int i = 1;i < 32;i++)   m2[i] = 0;
}

void MainWindow::scan(QString str)
{
    std::string st = str.toStdString();
    st += " ";
    int len = st.length();
    int tmp = 0;
    for(int i = 0;i < len;i++)
    {
        if(st[i] == ' ')
        {
            qDebug() << m[tmp];
            if(tmp) m2[tmp] = 1;
            tmp = 0;
        }
        else
        {
            tmp = tmp*10+st[i]-'0';
        }
    }
}

void MainWindow::reasoning()
{
    QString tmp;

    if(!m2[21])
    {
        if(m2[1])
        {
            tmp += (m[1] + "->" + m[21] + "\n");   m2[21] = 1;
        }
        else
        {
            if(m2[2])
            {
                tmp += (m[2] + "->" + m[21] + "\n");    m[21] = 1;
            }
            else
            {
                if(m2[6])
                {
                    tmp += (m[6] + "->" + m[21] + "\n"); m2[21] = 1;
                }
                else
                {
                    if(m2[10])
                    {
                        tmp += (m[6] + "->" + m[21] + "\n");    m2[21] = 1;
                    }
                }
            }
        }
    }

    if(m2[3] && !m2[22])
    {
        tmp += (m[3] + "->" + m[22] + "\n");    m2[22] = 1;
    }
    if(m2[7] && m2[8] && m2[9] && !m2[23])
    {
        tmp += (m[7] + "," + m[8] + "," + m[9] + "->" + m[23] + "\n");  m2[23] = 1;
    }
    if(m2[11] && m2[21] && !m2[24])
    {
        tmp += (m[11] + "," + m[21] + "->" + m[24] + "\n"); m2[24] = 1;
    }
    rst += ("推理过程如下:\n"+ tmp);
}

void MainWindow::Judge_last()
{
    bool flag = 0;
    // 信天翁
    if(m2[20] && m2[22])
    {
        prin(20,22,31); prin2(31); flag = 1;
    }
    // 鸵鸟
    if(m2[4] && m2[22] && m2[15] && m2[16])
    {
        prin(4,22,15,16,29);	prin2(29);	flag = 1;
    }
    // 企鹅
    if(m2[4] && m2[22] && m2[18] && m2[19])
    {
        prin(4,22,18,19,30);	prin2(30);	flag = 1;
    }
    // 金钱豹
    if(m2[23] && m2[12] && m2[21] && m2[13])
    {
        prin(23,12,21,13,25);	prin2(25);	flag = 1;
    }
    // 虎
    if(m2[23] && m2[12] && m2[21] && m2[13])
    {
        prin(23,13,21,14,26);	prin2(26);	flag = 1;
    }
    // 斑马
    if(m2[14] && m2[24])
    {
        prin(14,24,28);	prin2(28);	flag = 1;
    }
    // 长颈鹿
    if(m2[24] && m2[13] && m2[15] && m2[16])
    {
        prin(24,13,15,16,27);	prin2(27);	flag = 1;
    }

    if(!flag)
    {
        rst += "根据所给条件无法判断为何种动物\n";
    }
}

void MainWindow::prin(int a,int b,int c,int d,int e)
{
    if(c == 0)
    {
        rst += (m[a] + "->" + m[b] + "\n");
    }
    else
    {
        if(d == 0)
        {
            rst += (m[a] + "," + m[b] + "->" + m[c] + "\n");
        }
        else
        {
            if(e == 0)
            {
                rst += (m[a] + "," + m[b] + "," + m[c] + "->" + m[d] + "\n");
            }
            else
            {
                rst += (m[a] + "," + m[b] + "," + m[c] + "," + m[d] + "->" + m[e] + "\n");
            }
        }
    }
}

void MainWindow::prin2(int a)
{
    rst += ("所识别动物为" + m[a] + "\n");
}
