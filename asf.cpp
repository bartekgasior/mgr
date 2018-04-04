#include "asf.h"

using namespace std;

QVector<Bone> ASF::getASF(){
    QVector<Bone> skeletonBones;
    char string[100], *token, *boneName; //token - poszczegolne elementy dof
    char inputFilename[100];
    Bone *bone;

    int numberOfBones = 0;
    strcpy(inputFilename,"walk.asf");

    ifstream inputFile;
    inputFile.open(inputFilename);
    if(!inputFile.is_open())
        cout << "Błąd otwarcia pliku: " << inputFilename << endl;

    else { //obsluga pliku asf
        cout << "Plik " << inputFilename << " otwarty" << endl;

        //pomiń informacje z nagłówka
        inputFile >> string;

        while (strcmp(string,":units")) {
            inputFile >> string;
        }
        while (strcmp(string,":documentation")) {
            inputFile >> string;
        }

        while (strcmp(string,":root")) {
            inputFile >> string;
        }

        while (strcmp(string,":bonedata")) {
                inputFile >> string;
        }

        cout << "process bones..." << endl;

        //czytaj dane o kościach
        while(inputFile >> string && strcmp(string,":hierarchy")){
            bone = new Bone();
            skeletonBones.push_back(*bone);
            while(inputFile >> string && strcmp(string,"end")){

                if(strcmp(string,"id") == 0){
                    inputFile >> skeletonBones[numberOfBones].id;
                    cout << "Wczytano id " << skeletonBones[numberOfBones].id << endl;
                }

                if(strcmp(string,"name") == 0){
                    inputFile >> skeletonBones[numberOfBones].name;
                    cout << "Wczytano name " << skeletonBones[numberOfBones].name << endl;
                }

                if(strcmp(string,"direction") == 0){
                    inputFile >> skeletonBones[numberOfBones].directions[0];
                    inputFile >> skeletonBones[numberOfBones].directions[1];
                    inputFile >> skeletonBones[numberOfBones].directions[2];
                    cout << "Wczytano direction1 " << skeletonBones[numberOfBones].directions[0] << endl;
                    cout << "Wczytano direction2 " << skeletonBones[numberOfBones].directions[1] << endl;
                    cout << "Wczytano direction3 " << skeletonBones[numberOfBones].directions[2] << endl;
                }

                if(strcmp(string,"length") == 0){
                    inputFile >> skeletonBones[numberOfBones].length;
                    cout << "Wczytano length " << skeletonBones[numberOfBones].length << endl;
                }

                if(strcmp(string,"axis") == 0){
                    inputFile >> skeletonBones[numberOfBones].axisx;
                    inputFile >> skeletonBones[numberOfBones].axisy;
                    inputFile >> skeletonBones[numberOfBones].axisz;
                    inputFile >> skeletonBones[numberOfBones].axisXYZ;

                    cout << "Wczytano axisX " << skeletonBones[numberOfBones].axisx << endl;
                    cout << "Wczytano axisY " << skeletonBones[numberOfBones].axisy << endl;
                    cout << "Wczytano axisZ " << skeletonBones[numberOfBones].axisz << endl;
                    cout << "Wczytano axisXYZ " << skeletonBones[numberOfBones].axisXYZ << endl;
                }

                if(strcmp(string,"dof") == 0){
                    std::string str;
                    char strTmp[100];
                    std::getline(inputFile, str);
                    strcpy(strTmp, str.c_str());

                    token = strtok(strTmp, " ");

                    while(token != NULL){
                        if(strcmp(token, "rx") == 0)
                            skeletonBones[numberOfBones].dofrx = 1.0;
                        else if(strcmp(token, "ry") == 0)
                            skeletonBones[numberOfBones].dofry = 1.0;
                        else if(strcmp(token, "rz") == 0)
                            skeletonBones[numberOfBones].dofrz = 1.0;
                        else
                            printf("Nieznany token %s ", token);

                        token=strtok(NULL, " ");

                    }
                    cout << "dofrx - " <<skeletonBones[numberOfBones].dofrx << endl;
                    cout << "dofry - " <<skeletonBones[numberOfBones].dofry << endl;
                    cout << "dofrz - " <<skeletonBones[numberOfBones].dofrz << endl;
                }

               //if(!strcmp(string,"limits")) (-180.0 , 0.0 , 180.0)

            }
        numberOfBones++;
        }
        //pierwsza pobrana kosc zawsze jest root (????)


        int j;

        //czytaj :hierarchy
        if(strcmp(string, ":hierarchy") == 0){
            //pomin pierwszą linie - begin
            std::string skipLine;
            for (int i=0; i<3;i++){
                std::getline(inputFile, skipLine);
            }

            while(inputFile >> string && !strcmp(string,"end") == 0){
                    std::string str;
                    char strTmp[100];
                    std::getline(inputFile, str);

                    str = string + str;
                    strcpy(strTmp, str.c_str());
                    //cout << strTmp << endl;
                    boneName = strtok(strTmp, " ");

                    j=0;
                    Bone *parent, *sibilingPtr, *childTmp;

                    while(boneName != NULL){
                        if(j==0){
                          // parent = Bone::findBoneByName(boneName, skeletonBones);

                           for(unsigned int i=0; i<skeletonBones.size(); i++){
                               if(strcmp(boneName, skeletonBones[i].name) == 0){
                                   parent  = &skeletonBones[i];
                               }
                           }
                           // parent = parentTmp;
                        }else{
                            if(parent->child != NULL){
                                //cout << parent->name << "  " << parent->child->name << endl;

                               // childTmp = Bone::findBoneByName(boneName, skeletonBones);

                                for(unsigned int i=0; i<skeletonBones.size(); i++){
                                    if(strcmp(boneName, skeletonBones[i].name) == 0){
                                        childTmp = &skeletonBones[i];
                                    }
                                }

                                sibilingPtr = parent->child;
                                while (sibilingPtr->sibiling != NULL)
                                    sibilingPtr = sibilingPtr->sibiling;

                                sibilingPtr->sibiling = childTmp;
                               // cout << "sibiling name - " << parent->child->sibiling->name << endl;
                            } else{
                                //childTmp = Bone::findBoneByName(boneName, skeletonBones);

                                for(unsigned int i=0; i<skeletonBones.size(); i++){
                                    if(strcmp(boneName, skeletonBones[i].name) == 0){
                                        childTmp = &skeletonBones[i];
                                    }

                                }

                                parent->child = childTmp;
                                //cout << "child name - " << parent->child->name << endl;
                            }
                            //cout<< "child name - " << skeletonBones[parent->child->id].name << endl;
                        }
                        boneName=strtok(NULL," ");
                        j++;
                    }
                    cout<< endl;
            }
            //rootBone = &skeletonBones[0];
        }

        inputFile >> string;
        inputFile.close();
    }

    return skeletonBones;
}
