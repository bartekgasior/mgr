#ifndef BONE_H
#define BONE_H

#include <string>
#include <cstring>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

class Bone
{
public:
    Bone();

    int id;
    char name[30];
    float directions[3];
    float length;
    double axisx, axisy, axisz;
    char axisXYZ[3];
    double dofrx,dofry,dofrz;
    float limits[3][3];

    Bone *child = NULL;
    Bone *sibiling = NULL;

    static Bone* findBoneById(int id, std::vector<Bone> bones);
    static Bone* findBoneByName(char *name, std::vector<Bone> bones);
    static void setChildrenAndSibiling(int parentId, int childBoneId, std::vector<Bone> bones);
    //static void setSibiling(int parentId, Bone *childBone, std::vector<Bone> bones);
    static int getBoneId(char *boneName, std::vector<Bone> bones);
    void init();
};

#endif // BONE_H
