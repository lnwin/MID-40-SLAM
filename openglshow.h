#ifndef OPENGLSHOW_H
#define OPENGLSHOW_H

#include <QObject>
#include <QtOpenGL>
#include <QOpenGLExtraFunctions>
#include <qopenglfunctions_4_5_core.h>
#include <QOpenGLWidget>
#include <QOpenGLShader>
#include <QOpenGLTexture>
#include <QOpenGLShaderProgram>
#include <localStruct.h>
class openglShow : public QOpenGLWidget,protected QOpenGLFunctions_4_5_Core
{
    Q_OBJECT
public:
     openglShow();
     openglShow(QWidget *parent);
    ~openglShow();
   // void initializeGL() override;
  //  void paintGL() override;
  //  void resizeGL(int w, int h) override;
signals:


public slots:
    void receivePointCloud(cloudData);
};

#endif // OPENGLSHOW_H
