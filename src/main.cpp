#include "mainwindow.h"

#include <QApplication>
#include <QSurfaceFormat>
#include "thirdparty/QFontIcon/qfonticon.h"

#include "config.h"

#ifdef QT_DEBUG

void
DebugMessageOutput (QtMsgType type, const QMessageLogContext &context,
                   const QString &msg)
{
    QByteArray localMsg = msg.toLocal8Bit ();
    switch (type)
    {
    case QtDebugMsg:
        fprintf (stderr, "Debug: %s (%s:%u, %s)\n", localMsg.constData (),
                context.file, context.line, context.function);
        break;
    case QtInfoMsg:
        fprintf (stderr, "Info: %s (%s:%u, %s)\n", localMsg.constData (),
                context.file, context.line, context.function);
        break;
    case QtWarningMsg:
        fprintf (stderr, "Warning: %s (%s:%u, %s)\n", localMsg.constData (),
                context.file, context.line, context.function);
        break;
    case QtCriticalMsg:
        fprintf (stderr, "Critical: %s (%s:%u, %s)\n", localMsg.constData (),
                context.file, context.line, context.function);
        break;
    case QtFatalMsg:
        fprintf (stderr, "Fatal: %s (%s:%u, %s)\n", localMsg.constData (),
                context.file, context.line, context.function);
        abort ();
    }
}

#endif

int main(int argc, char *argv[])
{
    Q_INIT_RESOURCE(application);
#ifdef QT_DEBUG
    qInstallMessageHandler (DebugMessageOutput);
#endif
    // Set OpenGL to version 3.3 - Core Profile
    QSurfaceFormat format;
    format.setDepthBufferSize(24);
    format.setVersion(3, 3);
    format.setProfile(QSurfaceFormat::CoreProfile);

#if CONFIG_MULTISAMPLE_ENABLE
    format.setSamples(CONFIG_MULTISAMPLE_NUM_SAMPLES);
#endif

#ifdef QT_DEBUG
    format.setOption(QSurfaceFormat::DebugContext);
#endif

    QSurfaceFormat::setDefaultFormat(format);

    QCoreApplication::setOrganizationName("MGField");
    QCoreApplication::setOrganizationDomain("github.com/Abdul-AZ/mgfield");
    QCoreApplication::setApplicationName("MGField App");

    QApplication a(argc, argv);
    QFontIcon::addFont(":/res/foundation-icons.ttf");

    MainWindow w;

    w.show();
    return a.exec();
}
