#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include "parser.hpp"
#include "overloads.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::MainWindow),
      dock(new QDockWidget(tr("Table Tools"), this)),
      model(new TableModel(this)),
      tools(new TableTools(model, dock))
{
    ui->setupUi(this);

    setupFont();
    setupActions();
    setupTableView();
    setupTableTools();
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

std::unique_ptr<ParseResult> MainWindow::parse(const QString &source)
{
    if (source.size() == 0) {
        return {};
    }
    const char* begin = qPrintable(source);
    const char* end = begin + source.size();

    std::unique_ptr<ParseResult> result = parse_source(begin, end);

    //if not empty show error dialog
    //QString output = QString::fromStdString(result.output);
    return result;
}

void MainWindow::setupFont()
{
    QFont font = dock->font();
    font.setPointSize(14);
    dock->setFont(font);
}

void MainWindow::setupActions()
{
    QAction *openFile = ui->toolBar->addAction(style()->standardIcon(QStyle::SP_DirIcon),
                           tr("Open File"));
    QAction *saveFile = ui->toolBar->addAction(style()->standardIcon(QStyle::SP_DriveFDIcon),
                           tr("Save File"));

    openFile->setShortcut(QKeySequence::Open);
    saveFile->setShortcut(QKeySequence::Save);
    //Ignore argument from signal
    connect(openFile, SIGNAL(triggered(bool)), this, SLOT(openFile()));
    connect(saveFile, SIGNAL(triggered(bool)), this, SLOT(saveToFile()));
}

void MainWindow::setupTableView()
{
    ui->tableView->setModel(model);

    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableView->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectItems);
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);
}

void MainWindow::setupTableTools()
{
    tools->setView(ui->tableView);

    dock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    dock->setFeatures(dock->features() & ~QDockWidget::DockWidgetClosable);
    dock->setWidget(tools);
    addDockWidget(Qt::RightDockWidgetArea, dock);
    //Show after loading table
    dock->hide();
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
        std::unique_ptr<ParseResult> result = parse(content);
        if (result->ok) {
            beginTablePos = result->tableBeginIdx;
            endTablePos = result->tableEndIdx;
            lastLoadedFileName = fileName;

            model->setTable(result->table);
            dock->show();
        } else {
            QString errorStr = result->output.size() == 0
                    ? tr("Table not found in file %1").arg(fileName)
                    : tr("File parsing error:\n%1").arg(result->output);
            showError(errorStr);
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
