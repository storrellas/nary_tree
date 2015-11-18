#ifndef COMMONPARAMETERS_H
#define COMMONPARAMETERS_H

#include <QDebug>

// This is used for debug
#define loggerMacroDebug(line) qDebug() << __FILE__ << " | " << __LINE__ << " | " << __FUNCTION__ << " | " << line;

#endif // COMMONPARAMETERS_H
