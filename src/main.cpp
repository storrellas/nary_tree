#include "mainwindow.h"


// Qt Includes
#include <QApplication>
#include <QCoreApplication>
#include <QDebug>
#include <QDir>
#include <QPlainTextEdit>
#include <QTime>

// Project includes
#include "src/commonparameters.h"
#include "src/connection.h"

MainWindow* pMainWindow = NULL;

/**
 * @brief nicMessageHandler Handles all qDebug
 * @param type Type of message sent
 * @param msg  Message sent
 */
void messageHandler(QtMsgType, const QMessageLogContext&, const QString &msg)
{

    qDebug() << QTime::currentTime().toString("HH:mm:ss:zzz | ") + msg;

    // Output to GUI
    QString lineGUI = QTime::currentTime().toString("HH:mm:ss:zzz | ") + msg;
    if(pMainWindow != NULL){
        pMainWindow->consoleWrite( lineGUI );
    }

}


/*!
 * \brief main Main method
 * \param argc
 * \param argv
 * \return
 */
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow mainWindow;
    mainWindow.show();
    pMainWindow = &mainWindow;


    // Install the message handler for qDebug() calls
    qInstallMessageHandler(messageHandler);

    loggerMacroDebug("First log")


    // Create SQLite connection
    if (!createConnection())
        return 1;

    mainWindow.initialize();

    // Connect signal&slot
    QObject::connect(&mainWindow, SIGNAL(quit()), &a, SLOT(quit()));
    return a.exec();
}
