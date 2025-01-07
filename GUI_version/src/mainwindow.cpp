#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->label->setVisible(false);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_CompressButton_clicked()
{
    QMessageBox::information(this, tr("SELECT"), tr("Select a file to compress."));
    QString inputFilePath = QFileDialog::getOpenFileName(this, tr("Select a file to compress"), "", tr("All files (*.*)"));

    if (inputFilePath.isEmpty()) {
        QMessageBox::warning(this, tr("Error"), tr("No file selected!"));
        return;
    }
    QFile inputFile(inputFilePath);
    if (!inputFile.open(QIODevice::ReadOnly)) {
        QMessageBox::warning(this, tr("Error"), tr("Failed to open input file for reading."));
        return;
    }
    qint64 inputFileSize = inputFile.size();
    inputFile.close();
    double inputFileSizeKB = inputFileSize / 1024.0;

    QMessageBox::information(this, tr("SELECT"), tr("Select the location and name of the output file."));
    QString outputFilePath = QFileDialog::getSaveFileName(this, tr("Select the location and name of the output file"), "", tr("All files (*.*)"));

    if (outputFilePath.isEmpty()) {
        QMessageBox::warning(this, tr("Error"), tr("No output file selected!"));
        return;
    }

    QThread *thread = new QThread(this);
    CompressionWorker *worker = new CompressionWorker(inputFilePath, outputFilePath);

    worker->moveToThread(thread);

    connect(thread, &QThread::started, worker, &CompressionWorker::process);
    connect(worker, &CompressionWorker::finished, this, [=](int result) {
        thread->quit();
        thread->wait();
        worker->deleteLater();
        thread->deleteLater();
        gui_update(false);

        if (result == 0) {
            QFile outputFile(outputFilePath);
            if (!outputFile.open(QIODevice::ReadOnly)) {
                QMessageBox::critical(this, tr("Error"), tr("Failed to open output file for reading."));
                return;
            }
            qint64 outputFileSize = outputFile.size();
            outputFile.close();
            double outputFileSizeKB = outputFileSize / 1024.0;
            QString successMessage = tr("The file has been successfully compressed!\n");
            successMessage += tr("Original size: %1 KB\n").arg(inputFileSizeKB, 0, 'f', 2);
            successMessage += tr("Compressed size: %1 KB").arg(outputFileSizeKB, 0, 'f', 2);

            QMessageBox::information(this, tr("Success"), successMessage);
        } else {
            QMessageBox::critical(this, tr("Error"), tr("An error occurred during file compression."));
        }
    });

    gui_update(true);
    thread->start();
}



void MainWindow::on_DecompressButton_clicked()
{
    QMessageBox::information(this, tr("SELECT"), tr("Select a file to decompress."));
    QString inputFilePath = QFileDialog::getOpenFileName(this, tr("Select a file to decompress"), "", tr("All files (*.*)"));

    if (inputFilePath.isEmpty()) {
        QMessageBox::warning(this, tr("Error"), tr("No file selected!"));
        return;
    }

    QMessageBox::information(this, tr("SELECT"), tr("Select the location and name of the output file."));
    QString outputFilePath = QFileDialog::getSaveFileName(this, tr("Select the location and name of the output file"), "", tr("All files (*.*)"));

    if (outputFilePath.isEmpty()) {
        QMessageBox::warning(this, tr("Error"), tr("No output file selected!"));
        return;
    }

    QThread *thread = new QThread(this);
    DecompressionWorker *worker = new DecompressionWorker(inputFilePath, outputFilePath);

    worker->moveToThread(thread);

    connect(thread, &QThread::started, worker, &DecompressionWorker::process);
    connect(worker, &DecompressionWorker::finished, this, [=](int result) {
        thread->quit();
        thread->wait();
        worker->deleteLater();
        thread->deleteLater();
        gui_update(false);
        if (result == 0) {
            QMessageBox::information(this, tr("Success"), tr("The file has been successfully decompressed!"));
        } else if (result == 3) {
            QMessageBox::warning(this, tr("Invalid File"), tr("The selected file is not a valid file for decompression."));
        } else {
            QMessageBox::critical(this, tr("Error"), tr("An error occurred during file decompression."));
        }

    });

    gui_update(true);
    thread->start();
}
void MainWindow::gui_update(bool x){
    ui->label->setVisible(x);
    ui->CompressButton->setVisible(!x);
    ui->CompressButton->setEnabled(!x);
    ui->DecompressButton->setVisible(!x);
    ui->DecompressButton->setEnabled(!x);

}


void MainWindow::on_actionCreator_triggered()
{
    QMessageBox::about(this, "Autor", "Wiktor Lewicki, viksolve@gmail.com");
}

