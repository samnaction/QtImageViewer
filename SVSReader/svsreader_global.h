#ifndef SVSREADER_GLOBAL_H
#define SVSREADER_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(SVSREADER_LIBRARY)
#  define SVSREADERSHARED_EXPORT Q_DECL_EXPORT
#else
#  define SVSREADERSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // SVSREADER_GLOBAL_H
