#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include <QObject>
#include <QThread>
#include "modules/LZ77_Compressor.h"
#include "modules/LZ77_Decompressor.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_CompressButton_clicked();
    void on_DecompressButton_clicked();
    void gui_update(bool x);

    void on_actionCreator_triggered();

private:
    Ui::MainWindow *ui;
};

class CompressionWorker : public QObject {
    Q_OBJECT

public:
    explicit CompressionWorker(const QString &inputFile, const QString &outputFile, QObject *parent = nullptr)
        : QObject(parent), inputFile(inputFile), outputFile(outputFile) {}

public slots:
    void process() {
        LZ77_COMPRESSOR compressor;
        int result = compressor.compress(inputFile.toStdString(), outputFile.toStdString());
        emit finished(result);
    }

signals:
    void finished(int result);

private:
    QString inputFile;
    QString outputFile;
};

class DecompressionWorker : public QObject {
    Q_OBJECT

public:
    explicit DecompressionWorker(const QString &inputFile, const QString &outputFile, QObject *parent = nullptr)
        : QObject(parent), inputFile(inputFile), outputFile(outputFile) {}

public slots:
    void process() {
        LZ77_DECOMPRESSOR decompressor;
        int result = decompressor.decompress(inputFile.toStdString(), outputFile.toStdString());
        emit finished(result);
    }

signals:
    void finished(int result);

private:
    QString inputFile;
    QString outputFile;
};
#endif // MAINWINDOW_H
