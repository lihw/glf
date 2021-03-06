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

#include "../01system/glf_image.h"
#include "glf_application.h"
#include "glf_abstract_renderer.h"

GLFRenderWidget::GLFRenderWidget(int width, int height, const QGLFormat& format, QWidget* parent)
        : QGLWidget(format, parent)
{
    setFixedSize(width, height); 
    
    // To receive the keyboard events.
    setFocusPolicy(Qt::StrongFocus);

    _renderer = glfCreateRenderer();
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
        "No Profile",
        "Core Profile",
        "Compatibility Profile",
    };
    qDebug("OpenGL %d.%d %s", format().majorVersion(), format().minorVersion(),
        profileStrings[format().profile()]);

    qDebug("GL surface created. RGBA=(%d,%d,%d,%d), depth=%d, stencil=%d, multisample=%d", 
            format().redBufferSize(),
            format().greenBufferSize(),
            format().blueBufferSize(),
            format().alphaBufferSize(),
            format().depthBufferSize(),
            format().stencilBufferSize(),
            format().samples());
    
    _timer = new QTimer(this);
    connect(_timer, SIGNAL(timeout()), this, SLOT(timeoutSlot()));
    
    // FIXME: change the update frequency.
    _timer->start(5);

    // Initialize the viewport size
    glViewport(0, 0, width(), height());


    // Initialize the user renderer
    if (!_renderer->initialize())
    {
        delete _renderer;
        _renderer = NULL;
    }
}

void GLFRenderWidget::paintGL()
{
    if (_renderer != NULL)
    {
        _renderer->render();
    }
    else
    {
        // TODO: show an image that telss there is no
        // available renderer.
        glClearColor(1, 0, 0, 1);
        glClear(GL_COLOR_BUFFER_BIT);
    }
}

void GLFRenderWidget::dumpScreen()
{
    unsigned char* data = new unsigned char [width() * height() * 3];
    glReadBuffer(GL_BACK);
    glReadPixels(0, 0, width(), height(), GL_RGB, GL_UNSIGNED_BYTE, data);

    static int index = 1;
    QString imageName = QString("dumpscreen_%1.png").arg(index++);
    glf::Image image;
    image.createFromMemory(data, width(), height(), 3);
    image.flipVertical();
    image.write(imageName.toStdString().c_str());

    delete [] data;
}

void GLFRenderWidget::mousePressEvent(QMouseEvent* event)
{
    if (_renderer != NULL)
    {
        int buttons = 0;
        if ((event->buttons() & Qt::LeftButton) != 0) 
        {
            buttons |= GLFAbstractRenderer::MOUSE_BUTTON1;
        }
        if ((event->buttons() & Qt::MiddleButton) != 0) 
        {
            buttons |= GLFAbstractRenderer::MOUSE_BUTTON2;
        }
        if ((event->buttons() & Qt::RightButton) != 0) 
        {
            buttons |= GLFAbstractRenderer::MOUSE_BUTTON3;
        }

        int modifiers = 0;
        if ((event->modifiers() & Qt::ControlModifier))
        {
            modifiers |= GLFAbstractRenderer::KEYBOARD_CTRL;
        }
        if ((event->modifiers() & Qt::AltModifier))
        {
            modifiers |= GLFAbstractRenderer::KEYBOARD_ALT;
        }
        if ((event->modifiers() & Qt::ShiftModifier))
        {
            modifiers |= GLFAbstractRenderer::KEYBOARD_SHIFT;
        }

        _renderer->onMouseButtonDown(event->pos().x(), event->pos().y(), 
                buttons, modifiers);
    }
    
    QGLWidget::mousePressEvent(event);
}

void GLFRenderWidget::mouseMoveEvent(QMouseEvent* event)
{
    if (_renderer != NULL)
    {
        int buttons = 0;
        if ((event->buttons() & Qt::LeftButton) != 0) 
        {
            buttons |= GLFAbstractRenderer::MOUSE_BUTTON1;
        }
        if ((event->buttons() & Qt::MiddleButton) != 0) 
        {
            buttons |= GLFAbstractRenderer::MOUSE_BUTTON2;
        }
        if ((event->buttons() & Qt::RightButton) != 0) 
        {
            buttons |= GLFAbstractRenderer::MOUSE_BUTTON3;
        }

        int modifiers = 0;
        if ((event->modifiers() & Qt::ControlModifier))
        {
            modifiers |= GLFAbstractRenderer::KEYBOARD_CTRL;
        }
        if ((event->modifiers() & Qt::AltModifier))
        {
            modifiers |= GLFAbstractRenderer::KEYBOARD_ALT;
        }
        if ((event->modifiers() & Qt::ShiftModifier))
        {
            modifiers |= GLFAbstractRenderer::KEYBOARD_SHIFT;
        }

        _renderer->onMouseMove(event->pos().x(), event->pos().y(), 
                buttons, modifiers);
    }
    
    QGLWidget::mouseMoveEvent(event);
}

void GLFRenderWidget::mouseReleaseEvent(QMouseEvent* event)
{
    if (_renderer != NULL)
    {
        int buttons = 0;
        if ((event->buttons() & Qt::LeftButton) != 0) 
        {
            buttons |= GLFAbstractRenderer::MOUSE_BUTTON1;
        }
        if ((event->buttons() & Qt::MiddleButton) != 0) 
        {
            buttons |= GLFAbstractRenderer::MOUSE_BUTTON2;
        }
        if ((event->buttons() & Qt::RightButton) != 0) 
        {
            buttons |= GLFAbstractRenderer::MOUSE_BUTTON3;
        }

        int modifiers = 0;
        if ((event->modifiers() & Qt::ControlModifier))
        {
            modifiers |= GLFAbstractRenderer::KEYBOARD_CTRL;
        }
        if ((event->modifiers() & Qt::AltModifier))
        {
            modifiers |= GLFAbstractRenderer::KEYBOARD_ALT;
        }
        if ((event->modifiers() & Qt::ShiftModifier))
        {
            modifiers |= GLFAbstractRenderer::KEYBOARD_SHIFT;
        }

        _renderer->onMouseButtonUp(event->pos().x(), event->pos().y(), 
                buttons, modifiers);
    }
    
    QGLWidget::mouseReleaseEvent(event);
}

void GLFRenderWidget::wheelEvent(QWheelEvent* event)
{
    int numDegrees = event->delta() / 8;
    if (_renderer != NULL)
    {
        _renderer->onMouseWheel(numDegrees);
    }

    QGLWidget::wheelEvent(event);
}

void GLFRenderWidget::keyPressEvent(QKeyEvent* event)
{
    if (_renderer != NULL)
    {
        int modifiers = 0;
        if ((event->modifiers() & Qt::ControlModifier))
        {
            modifiers |= GLFAbstractRenderer::KEYBOARD_CTRL;
        }
        if ((event->modifiers() & Qt::AltModifier))
        {
            modifiers |= GLFAbstractRenderer::KEYBOARD_ALT;
        }
        if ((event->modifiers() & Qt::ShiftModifier))
        {
            modifiers |= GLFAbstractRenderer::KEYBOARD_SHIFT;
        }

        _renderer->onKeyDown(event->key(), modifiers);
    }
    
    QGLWidget::keyPressEvent(event);
}

void GLFRenderWidget::keyReleaseEvent(QKeyEvent* event)
{
    if (_renderer != NULL)
    {
        int modifiers = 0;
        if ((event->modifiers() & Qt::ControlModifier))
        {
            modifiers |= GLFAbstractRenderer::KEYBOARD_CTRL;
        }
        if ((event->modifiers() & Qt::AltModifier))
        {
            modifiers |= GLFAbstractRenderer::KEYBOARD_ALT;
        }
        if ((event->modifiers() & Qt::ShiftModifier))
        {
            modifiers |= GLFAbstractRenderer::KEYBOARD_SHIFT;
        }

        _renderer->onKeyUp(event->key(), modifiers);
    }

    QGLWidget::keyReleaseEvent(event);
}

void GLFRenderWidget::resizeEvent(QResizeEvent* event)
{
    if (_renderer != NULL)
    {
        QSize s = event->size();
        _renderer->onResized(s.width(), s.height());
    }
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

