// -------------------------------------------------------------- 
// glf_render_widget.cpp
// The window contains the rendering content
//
// An OpenGL framework.
//
// All rights reserved by AMD.
//
// Hongwei Li (hongwei.li@amd.com)
// -------------------------------------------------------------- 

#define GLEW_STATIC
#include <GL/glew.h>

#include "glf_render_widget.h"

#include <QDesktopWidget>
#include <QTimer>
#include <QMouseEvent>
#include <QApplication>

#include "../01system/glf_bitmap.h"

GLFRenderWidget::GLFRenderWidget(int width, int height, const QGLFormat& format, QWidget* parent)
        : QGLWidget(format, parent)
{
    setFixedSize(width, height); 
    
    // To receive the keyboard events.
    setFocusPolicy(Qt::StrongFocus);

    _timer = NULL;
}

GLFRenderWidget::~GLFRenderWidget()
{
    delete _timer;
}

void GLFRenderWidget::timeoutSlot()
{
    updateGL();
};

void GLFRenderWidget::initializeGL()
{
    //
    // Initialize the GLEW.
    glewExperimental = GL_TRUE; 
    if (glewInit() != GLEW_OK)
    {
        qCritical("Fail to initialize GLEW");
        return ;
    }
    
    // FIXME: there's an OpenGL error in glewInit(), we need to
    // remove it before entering Kiwi context initialization.
    GLenum error = glGetError();

    // Get the display/drawable resolution.
    QRect rect = QApplication::desktop()->screenGeometry();
    qDebug("Display resolution: %d x %d", rect.width(), rect.height());
    qDebug("Drawable resolution: %d x %d", size().width(), size().height());

    // Get the information of OpenGL context.
    const char* profileStrings[] =
    {
        "No Profile"
        "Core Profile",
        "Compatibility Profile",
    };
    qDebug("OpenGL %d.%d %s", format().minorVersion(), format().majorVersion(),
        profileStrings[format().profile()]);

    qDebug("GL surface created. RGBA=(%d,%d,%d,%d), depth=%d, stencil=%d, multisample=%d", 
            format().redBufferSize(),
            format().greenBufferSize(),
            format().blueBufferSize(),
            format().alphaBufferSize(),
            format().depthBufferSize(),
            format().stencilBufferSize());
    
    _timer = new QTimer(this);
    connect(_timer, SIGNAL(timeout()), this, SLOT(timeoutSlot()));
    
    // FIXME: change the update frequency.
    _timer->start(5);
}

void GLFRenderWidget::paintGL()
{
    glClearColor(1, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void GLFRenderWidget::dumpScreen()
{
    unsigned char* data = new unsigned char [width() * height() * 3];
    glReadBuffer(GL_BACK);
    glReadPixels(0, 0, width(), height(), GL_RGB, GL_UNSIGNED_BYTE, data);

    glf::Bitmap bitmap;
    bitmap.createFromMemory(data, width(), height(), 3);
    bitmap.write("dumpscreen.ppm");

    delete [] data;
}

void GLFRenderWidget::mousePressEvent(QMouseEvent* event)
{
    QGLWidget::mousePressEvent(event);
}

void GLFRenderWidget::mouseMoveEvent(QMouseEvent* event)
{
    if ((event->buttons() & Qt::LeftButton) != 0)
    {
        //Q_ASSERT(_window != KW_NULL);
        //_window->mouseMoveEvent(event->pos().x(), event->pos().y(), 
        //        (event->modifiers() & Qt::ControlModifier) != 0);
    }
    
    QGLWidget::mouseMoveEvent(event);
}

void GLFRenderWidget::mouseReleaseEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton)
    {
        //Q_ASSERT(_window != KW_NULL);
        //_window->mouseReleaseEvent(event->pos().x(), event->pos().y(), 
        //        (event->modifiers() & Qt::ControlModifier) != 0);
    }
    
    QGLWidget::mouseReleaseEvent(event);
}

void GLFRenderWidget::keyPressEvent(QKeyEvent* event)
{
    //kwUint32 state = KW_KEY_DEVICE_STATE_DOWN;
    //if (event->modifiers() & Qt::ShiftModifier)
    //{
    //    state |= KW_KEY_DEVICE_STATE_SHIFT;
    //}
    //if (event->modifiers() & Qt::AltModifier)
    //{
    //    state |= KW_KEY_DEVICE_STATE_ALT;
    //}
    //if (event->modifiers() & Qt::ControlModifier)
    //{
    //    state |= KW_KEY_DEVICE_STATE_CTRL;
    //}
    //
    //Q_ASSERT(_window != KW_NULL);
    //_window->keyPressEvent(event->key(), kwInputNativeGetTranslatedKey(event->key()), state);
    
    QGLWidget::keyPressEvent(event);
}

void GLFRenderWidget::keyReleaseEvent(QKeyEvent* event)
{
    //kwUint32 state = KW_KEY_DEVICE_STATE_UP;
    //if (event->modifiers() & Qt::ShiftModifier)
    //{
    //    state |= KW_KEY_DEVICE_STATE_SHIFT;
    //}
    //if (event->modifiers() & Qt::AltModifier)
    //{
    //    state |= KW_KEY_DEVICE_STATE_ALT;
    //}
    //if (event->modifiers() & Qt::ControlModifier)
    //{
    //    state |= KW_KEY_DEVICE_STATE_CTRL;
    //}

    //Q_ASSERT(_window != KW_NULL);
    //_window->keyReleaseEvent(event->key(), kwInputNativeGetTranslatedKey(event->key()), state);

    QGLWidget::keyReleaseEvent(event);
}

void GLFRenderWidget::resizeEvent(QResizeEvent* event)
{
    //if (_window != KW_NULL)
    //{
    //    QSize s = event->size();
    //    _window->resizeEvent(s.width(), s.height());
    //}
    QGLWidget::resizeEvent(event);
}

void GLFRenderWidget::focusInEvent(QFocusEvent* event)
{
    //Q_ASSERT(_window != KW_NULL);
    //_window->focusGainedEvent();

    QGLWidget::focusInEvent(event);
}

void GLFRenderWidget::focusOutEvent(QFocusEvent* event)
{
    //Q_ASSERT(_window != KW_NULL);
    //_window->focusLostEvent();

    QGLWidget::focusOutEvent(event);
}

