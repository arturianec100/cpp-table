#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include "parser.hpp"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::showError(const QString &message)
{
    qDebug() << message;
    QMessageBox::information(this, tr("Message"), message);
}

void MainWindow::openFile()
{
    openFile(selectFileToOpen());
}

void MainWindow::saveToFile()
{
    saveToFile(selectFileToSave(), ui->fileContentEdit->toPlainText());
}

ParseResult MainWindow::parse(const QString &source)
{
    if (source.size() == 0) {
        return;
    }
    const char* begin = qPrintable(source);
    const char* end = begin + source.size();

    ParseResult result = parse_source(begin, end);

    //if not empty show error dialog
    //QString output = QString::fromStdString(result.output);
    return result;
}

void MainWindow::setupActions()
{
    QAction *openFile = ui->toolBar->addAction(style()->standardIcon(QStyle::SP_FileIcon),
                           tr("Open File"));
    QAction *saveFile = ui->toolBar->addAction(style()->standardIcon(QStyle::SP_DriveFDIcon),
                           tr("Save File"));
    QAction *parse = ui->toolBar->addAction(style()->standardIcon(QStyle::SP_ArrowRight),
                           tr("Parse"));

    openFile->setShortcut(QKeySequence::Open);
    saveFile->setShortcut(QKeySequence::Save);
    parse->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_Enter));
    //Ignore argument from signal
    connect(openFile, SIGNAL(triggered(bool)), this, SLOT(openFile()));
    connect(saveFile, SIGNAL(triggered(bool)), this, SLOT(saveToFile()));
    connect(parse, SIGNAL(triggered(bool)), this, SLOT(parse()));
}

QString MainWindow::selectFileToOpen()
{
    return QFileDialog::getOpenFileName(this, tr("Open File"),
                                        QDir::currentPath(),
                                        tr("All files (*.*)"));
}

QString MainWindow::selectFileToSave()
{
    return QFileDialog::getSaveFileName(this, tr("Save to File"),
                                        QDir::currentPath(),
                                        tr("Headers (*.h *.hpp *.hxx);;"
                                           "Sources (*.c *.cc *.cpp *.cxx)"));
}

void MainWindow::openFile(const QString &fileName)
{
    QFile file(fileName);
    if (!file.exists()) {
        showError(tr("File %1 not found.").arg(fileName));
        return;
    }
    QString content;
    ui->fileContentEdit->clear();
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream stream(&file);
        stream >> content;
        if (content.back() != '\n') {
            content.append('\n');
        }
        ui->fileContentEdit->appendPlainText(content);
    } else {
        showError(tr("Can't read file %1").arg(fileName));
        return;
    }
    file.close();
}

void MainWindow::saveToFile(const QString &fileName, const QString &content)
{
    QFile file(fileName);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)){
        QTextStream stream(&file);
        stream << content;
    } else {
        showError(tr("Can't write to file %1").arg(fileName));
        return;
    }
    file.close();
}
