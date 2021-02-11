#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include "parser.hpp"
#include "overloads.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , model(new TableModel(this))
{
    ui->setupUi(this);

    ui->tableView->setModel(model);

    setupActions();
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
    saveToFile(selectFileToSave(), model->table());
}

ParseResult MainWindow::parse(const QString &source)
{
    if (source.size() == 0) {
        return {};
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

    openFile->setShortcut(QKeySequence::Open);
    saveFile->setShortcut(QKeySequence::Save);
    //Ignore argument from signal
    connect(openFile, SIGNAL(triggered(bool)), this, SLOT(openFile()));
    connect(saveFile, SIGNAL(triggered(bool)), this, SLOT(saveToFile()));
}

QString MainWindow::selectFileToOpen()
{
    return QFileDialog::getOpenFileName(this, tr("Open File"),
                                        QDir::currentPath(),
                                        tr("All files (*.*)"));
    /*tr("Headers (*.h *.hpp *.hxx);;"
       "Sources (*.c *.cc *.cpp *.cxx)")*/
}

QString MainWindow::selectFileToSave()
{
    return QFileDialog::getSaveFileName(this, tr("Save to File"),
                                        QDir::currentPath(),
                                        tr("All files (*.*)"));
}

void MainWindow::openFile(const QString &fileName)
{
    QFile file(fileName);
    if (!file.exists()) {
        showError(tr("File %1 not found.").arg(fileName));
        return;
    }
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream stream(&file);
        QString content = stream.readAll();
        if (content.back() != '\n') {
            content.append('\n');
        }
        ParseResult result = parse(content);
        if (result.ok) {
            beginTablePos = result.tableBeginIdx;
            endTablePos = result.tableEndIdx;
            lastLoadedFileName = fileName;

            model->setTable(result.table);
        } else {
            showError(QString::fromStdString(result.output));
        }
    } else {
        showError(tr("Can't read file %1").arg(fileName));
        return;
    }
    file.close();
}

void MainWindow::saveToFile(const QString &fileName, const StringTable &table)
{
    QFile outFile(fileName);
    QFile inFile(lastLoadedFileName);
    if (outFile.open(QIODevice::WriteOnly | QIODevice::Text)){
        if (inFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QTextStream outStream(&outFile);
            QTextStream inStream(&inFile);

            outStream << inStream.read(beginTablePos) << table;
            if (inStream.seek(endTablePos)) {
                outStream << inStream.readAll();
            }
        } else {
            showError(tr("Can't open file %1").arg(lastLoadedFileName));
            return;
        }
    } else {
        showError(tr("Can't save to file %1").arg(fileName));
        return;
    }
    outFile.close();
}
