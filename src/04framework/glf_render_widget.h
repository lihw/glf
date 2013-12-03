// -------------------------------------------------------------- 
// glf_render_widget.h
// The window contains the rendering content
//
// An OpenGL framework.
//
// All rights reserved by AMD.
//
// Hongwei Li (hongwei.li@amd.com)
// -------------------------------------------------------------- 


#ifndef GLF_RENDER_WIDGET_H
#define GLF_RENDER_WIDGET_H

#include <QGLWidget>

class GLFAbstractRenderer;

class GLFRenderWidget : public QGLWidget
{
    Q_OBJECT

public:
    GLFRenderWidget(int width, int height, const QGLFormat& format, QWidget* parent);
    ~GLFRenderWidget();

    void dumpScreen();

public slots:
    void timeoutSlot();

protected:
    // Initialize OpenGL
    virtual void initializeGL();

    // Callback for paint event to draw the surface. It is triggered
    // every 5 ms.
    virtual void paintGL();

    virtual void mousePressEvent(QMouseEvent* event);
    virtual void mouseMoveEvent(QMouseEvent* event);
    virtual void mouseReleaseEvent(QMouseEvent* event);
    virtual void wheelEvent(QWheelEvent* event);

    virtual void keyPressEvent(QKeyEvent* event);
    virtual void keyReleaseEvent(QKeyEvent* event);
    
    // Resize events.
    virtual void resizeEvent(QResizeEvent* event);

    // Focus in/out events.
    virtual void focusInEvent(QFocusEvent* event);
    virtual void focusOutEvent(QFocusEvent* event);

private:
    QTimer* _timer;
    GLFAbstractRenderer* _renderer;
};


#endif // !GLF_RENDER_WIDGET_H
