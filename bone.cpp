#include "bone.h"
#include <iostream>
Bone::Bone()
{
    id=0;
    strcpy(name, "");

    directions[0]=1.0;
    directions[1]=1.0;
    directions[2]=1.0;

    length=0.0;

    axisx = axisy = axisz = 0;

    dofrx = dofry = dofrz = 0;

    limits[0][0] = -360.0;
    limits[0][1] = 360.0;
    limits[1][0] = -360.0;
    limits[1][1] = 360.0;
    limits[2][0] = -360.0;
    limits[2][1] = 360.0;
}

Bone* Bone::findBoneById(int id, std::vector<Bone> bones){
    Bone *foundBone = (Bone *)malloc(sizeof(Bone));

    for(unsigned int i=0; i<bones.size(); i++){
        if(id == bones[i].id)
            foundBone = &bones[i];
    }
    return foundBone;
}

Bone* Bone::findBoneByName(char *name, std::vector<Bone> bones){
    Bone *foundBone;

    for(unsigned int i=0; i<bones.size(); i++){
        if(strcmp(name, bones[i].name) == 0){
            foundBone = &bones[i];
            std::cout<<"znaleziono kosc " << foundBone->name << std::endl;
        }

    }
    return foundBone;
}

void Bone::setChildrenAndSibiling(int parentId, int childBoneId, std::vector<Bone> bones){
    Bone *parentBone, *childBone;
    parentBone = findBoneById(parentId, bones);
    childBone = findBoneById(childBoneId, bones);

    if (parentBone->child == NULL)
        parentBone->child = childBone;
    else{
        parentBone=parentBone->child;
        while(parentBone->sibiling != NULL)
            parentBone=parentBone->sibiling;

        parentBone->sibiling = childBone;
    }

}

/*void Bone::setSibiling(int parentId, Bone *childBone, std::vector<Bone> bones){
    Bone *parentBone = findBoneById(parentId, bones);

    if (parentBone->child != NULL){
        parentBone=parentBone->child;
        while(parentBone->sibiling != NULL)
            parentBone=parentBone->sibiling;

        parentBone->sibiling = childBone;
    }
}*/

int Bone::getBoneId(char *boneName, std::vector<Bone> bones){
    int id=0;
    for(unsigned int i=0; i<bones.size(); i++){
        if(strcmp(boneName, bones[i].name) == 0)
            id=bones[i].id;
    }
    return id;
}
