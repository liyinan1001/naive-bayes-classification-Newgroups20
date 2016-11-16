//
//  nb.h
//  naive_bayes
//
//  Created by 李一楠 on 20/4/15.
//  Copyright (c) 2015 yinan. All rights reserved.
//

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include<map>
#include<set>
#include<vector>
#include<fstream>
#include<iostream>
#include<sstream>
#include<string>
#include<random>
#include<math.h>

#include "nb.h"

using namespace std;

nb::nb(char * path){
    this->path=path;
}
void nb::train(){   //train dataset in path directory
    
    if(access(this->path,F_OK)==0&&testdir(this->path))
    {
        printf("is directory\n");
        scanFolder(this->path);
    }
    else printf("%s not exist\n",this->path);
}
int nb::scanFolder(char *path)
{
    DIR *db;
    char filename[128];
    struct dirent *p;
    db=opendir(path);
    if(db==NULL)return 0;
    memset(filename,0,128);
    while ((p=readdir(db)))
    {
        if((strcmp(p->d_name,".DS_Store")==0)||(strcmp(p->d_name,".")==0)||(strcmp(p->d_name,"..")==0))  //.DS_Store is system file in folders
            continue;
        else
        {
            sprintf(filename,"%s/%s",path,p->d_name);
            if(testdir(filename))   //if scannned is a folder, whose name is a label
            {
                string labelname(filename);
                //cout<<labelname;
                nb::c itLabel(labelname);
                this->cVec.push_back(itLabel);
                scanFolder(filename);
                
            }
            else{       //scanned is a file
                fileProc(filename);
            }
        }
        memset(filename,0,64);
    }
    closedir(db);
    return 0;
}

int nb::testdir(char *path)
{
    struct stat buf;
    if(lstat(path,&buf)<0)
    {
        return 0;
    }
    if(S_ISDIR(buf.st_mode))
    {
        return 1; //directory
    }
    return 0;
}

void nb::fileProc(char * filename){
    random_device rd;
    //cout<<rd();
    double rdnum=double((rd()-random_device::min()))/(random_device::max()-random_device::min());
    string sfilename(filename);
    //cout<<endl<<"rd is "<<rdnum<<endl;
    if(rdnum<=0.9){    //90% files are trainset and the others are test set
        string word;
        ifstream inFile(sfilename);
        while(inFile>>word){
            auto vecIter=this->cVec.rbegin();
            if(vecIter->dict.find(word)==vecIter->dict.end()){  //if the word hasn't been inserted
                vecIter->dict[word]=1;
            }
            else{
                vecIter->dict[word]++;
            }
        }
        inFile.close();
    }
    else{
        this->testSet[sfilename]=this->cVec.back().label;   //insert pair(string filename, string label)
    }
}
double nb::test(){
    //first caculate p(ci) and p(vj|ci)
    int correctCount=0;   //number of file with correct test result
    for(auto vecIter=this->cVec.begin();vecIter!=this->cVec.end();vecIter++){
        for(auto dictIter=vecIter->dict.begin();dictIter!=vecIter->dict.end();dictIter++){
            vecIter->wordCount+=dictIter->second;
        }
        vecIter->dictDim=vecIter->dict.size();
        this->allCount+=vecIter->wordCount;
    }
    this->labelCount=this->cVec.size();
    
    for(auto vecIter=this->cVec.begin();vecIter!=this->cVec.end();vecIter++){
        vecIter->prior=(double(vecIter->wordCount)/this->allCount);
    }
    for(auto setIter=this->testSet.begin();setIter!=this->testSet.end();setIter++){
        ifstream testFile(setIter->first);
        string word;
        double* posterior=new double[this->labelCount];
        for(int i=0;i<this->labelCount;i++){
            posterior[i]=0;
        }
        while(testFile>>word){
            for(int i=0;i<this->labelCount;i++){
                posterior[i]+=log((this->cVec[i].dict[word]+1)/(this->cVec[i].wordCount+this->cVec[i].dictDim)); //use laplace smoothing to caculate p(vj|ci). if word doesn't exist in dict, dict[word]==0.
               // cout<<endl<<"posterior is "<<posterior[i]<<endl;
            }
        }
        for(int i=0;i<this->labelCount;i++){
            posterior[i]+=log(this->cVec[i].prior);
        }
        double maxpost=posterior[0]; //initialize maxpost
        int maxlabel=0;
        for(int i=0;i<this->labelCount;i++){
            if(maxpost<posterior[i]) { maxpost=posterior[i]; maxlabel=i; }  //find the largest posterior in all labels
        }
       // cout<<endl<<"test file label "<<setIter->second<<" most prop label "<<this->cVec[maxlabel].label<<endl;
        if(setIter->second==this->cVec[maxlabel].label) correctCount++;
        delete [] posterior;
        testFile.close();
    }
    int testCount=this->testSet.size();
    double correctRatio=double(correctCount)/testCount;  //final correctRatio of the test
    long dictsize=0;
    for(int i=0;i<20;i++)
    {
        cout<<"dict size "<<this->cVec[i].dictDim<<endl;
        
        dictsize+=this->cVec[i].dictDim;
    }
    cout<<"dictSize ave: "<<dictsize<<endl;
    cout<<endl<<"correct ratio is "<<correctRatio<<endl;
    //first calculate
    return correctRatio;
}


    
