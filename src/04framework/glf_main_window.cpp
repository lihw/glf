// -------------------------------------------------------------- 
// glf_main_window.cpp
// The main window
//
// An OpenGL framework.
//
// All rights reserved by AMD.
//
// Hongwei Li (hongwei.li@amd.com)
// -------------------------------------------------------------- 

#include "glf_main_window.h"

#include "glf_control_window.h"
#include "glf_render_widget.h"
#include "glf_output_window.h"

GLFMainWindow::GLFMainWindow(const GLFConfig& config, QWidget *parent)
{
    setMinimumSize(config.windowWidth, config.windowHeight);
    
    _config = config;

    _pRenderWidget  = NULL;
    _pOutputWindow  = NULL;
    _pControlWindow = NULL;

    // -------------------------------------------------------------- 
    // Create menus
    // -------------------------------------------------------------- 
    QMenuBar* menubar = new QMenuBar;
    setMenuBar(menubar);

    _pDumpAct = new QAction(tr("&Dump Screen"), this);
    _pDumpAct->setStatusTip(tr("Dump back buffer to the local file"));
    connect(_pDumpAct, SIGNAL(triggered()), this, SLOT(onDumpScreen()));

    _pAboutAct = new QAction(tr("&About"), this);
    connect(_pAboutAct, SIGNAL(triggered()), this, SLOT(onAbout()));

    _pFileMenu = menuBar()->addMenu(tr("&File"));
    _pFileMenu->addAction(_pDumpAct);
    _pFileMenu->addSeparator();
    _pFileMenu->addAction(_pAboutAct);
}

void GLFMainWindow::closeEvent(QCloseEvent* event)
{
    // TODO: sure to close?
}

void GLFMainWindow::showEvent(QShowEvent* event)
{
    // -------------------------------------------------------------- 
    // GL rendering widget
    // -------------------------------------------------------------- 
    QGLFormat qglFormat;

    qglFormat.setVersion(4, 3);  
    qglFormat.setProfile(QGLFormat::CoreProfile);
            
    qglFormat.setRedBufferSize(_config.redBits);
    qglFormat.setGreenBufferSize(_config.greenBits);
    qglFormat.setBlueBufferSize(_config.blueBits);
    qglFormat.setAlphaBufferSize(_config.alphaBits);
    qglFormat.setDepthBufferSize(_config.depthBits);
    qglFormat.setStencilBufferSize(_config.stencilBits);

    qglFormat.setDoubleBuffer(true);

    if (_config.multisamples > 1)
    {
        qglFormat.setSampleBuffers(true);
        qglFormat.setSamples(_config.multisamples);
    }
    else
    {
        qglFormat.setSampleBuffers(false);
    }
    
    _pRenderWidget = new GLFRenderWidget(_config.windowWidth, _config.windowHeight, qglFormat, this);
    setCentralWidget(_pRenderWidget);
    
    // -------------------------------------------------------------- 
    // Create windows
    // -------------------------------------------------------------- 
    _pControlWindow = new GLFControlWindow(this);
    _pOutputWindow = new GLFOutputWindow(this);

    if (_config.noOutputWindow)
    {
        _pOutputWindow->setVisible(false);
    }
    else
    {
        _pOutputWindow->show();
    }


    if (_config.noControlWindow)
    {
        _pControlWindow->setVisible(false);
    }
    else
    {
        _pControlWindow->show();
    }

    QMainWindow::showEvent(event);
}
    
    
void GLFMainWindow::onDumpScreen()
{
    Q_ASSERT(_pRenderWidget != NULL);
    _pRenderWidget->dumpScreen();
}

void GLFMainWindow::onAbout()
{
    QMessageBox msgBox;
    msgBox.setText(tr("It is a  application"));
    msgBox.exec();
}

