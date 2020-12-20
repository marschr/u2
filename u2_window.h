#include <QWidget>

#include <QOpenGLWidget>
#include <QOpenGLFunctions>

#include "location.h"

// container window for onroad NVG UI
class GLWindow : public QOpenGLWidget, protected QOpenGLFunctions {
  Q_OBJECT

    public:
        using QOpenGLWidget::QOpenGLWidget;
        explicit GLWindow(QWidget *parent = 0);
        void wake();
        ~GLWindow();
};

class U2Window : public QWidget
{
    Q_OBJECT

    public:
        U2Window();
        ~U2Window();

        GLWindow *glWindow;

    private slots:
        void recvMsg();
    
    private:
        Location *location;
};