/**
 * @file Logging.h
 * @brief Defines logging macros for application-wide logging
 */

#ifndef LOGGING_H
#define LOGGING_H

#include <QDebug>
#include <QFileInfo>
#include <QString>

// Extract filename without path for cleaner logging
#define __FILENAME__ (QFileInfo(__FILE__).fileName().toStdString().c_str())

/**
 * @brief Logging macros with file, line, and function information
 *
 * These macros provide different levels of logging with consistent formatting:
 * - DEBUG:   Development-time debugging information (only in debug builds)
 * - INFO:    General information about program execution
 * - WARNING: Potentially harmful situations
 * - ERROR:   Error conditions but program can continue
 * - FATAL:   Severe errors that prevent program continuation
 */

#ifdef QT_DEBUG
#define DEBUG(msg)                                                      \
    qDebug().nospace() << "[DEBUG] " << __FILENAME__ << ":" << __LINE__ \
                       << " - " << __FUNCTION__ << ": " << msg
#endif

#define INFO(msg)                                                              \
    qInfo().nospace() << "[INFO] " << __FILENAME__ << ":" << __LINE__ << " - " \
                      << __FUNCTION__ << ": " << msg

#define WARNING(msg)                                                        \
    qWarning().nospace() << "[WARNING] " << __FILENAME__ << ":" << __LINE__ \
                         << " - " << __FUNCTION__ << ": " << msg

#define ERROR(msg)                                                         \
    qCritical().nospace() << "[ERROR] " << __FILENAME__ << ":" << __LINE__ \
                          << " - " << __FUNCTION__ << ": " << msg

#define FATAL(msg) \
    qFatal("[FATAL] %s:%d - %s: %s", __FILENAME__, __LINE__, __FUNCTION__, msg)

#endif  // LOGGING_H
