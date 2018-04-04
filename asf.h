#ifndef ASF_H
#define ASF_H

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "bone.h"

#include <QVector>
#include <iostream>
#include <fstream>
#include <cstring>

class ASF
{
public:
    static QVector<Bone> getASF();
    Bone *rootBone;
};

#endif // ASF_H
