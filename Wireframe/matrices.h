#ifndef MATRICES_H
#define MATRICES_H


#include <QGenericMatrix>
typedef QGenericMatrix<4, 4, float> Matrix4x4;
typedef QGenericMatrix<2, 4, float> Matrix4x2;
typedef QGenericMatrix<2, 1, float> Matrix1x2;
typedef QGenericMatrix<4, 1, float> Matrix1x4;
typedef QGenericMatrix<1, 4, float> Matrix4x1;

#include <QVector3D>
typedef QVector3D Vector3;


#endif // MATRICES_H
