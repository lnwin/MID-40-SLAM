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
#include <gl/GLU.h>
#include <math3d.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <mutex>
class openglShow : public QOpenGLWidget,protected QOpenGLFunctions_4_5_Core
{
    Q_OBJECT
public:
     openglShow();
     openglShow(QWidget *parent);

    ~openglShow();
    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int w, int h) override;
signals:


public slots:

    void receivePointCloud(QList<float>,QList<float>,QList<float>,QList<float>);

private:
    int  viewport[4];
    void drawball(float cx,float cy,float cz,float radius,float M,float N);
protected:
      void  mousePressEvent(QMouseEvent *event);
      void  mouseReleaseEvent(QMouseEvent *event);
      void  mouseMoveEvent(QMouseEvent *event);
      void  wheelEvent(QWheelEvent*event);
      void  mouseDoubleClickEvent(QMouseEvent*event);
};

#endif // OPENGLSHOW_H
