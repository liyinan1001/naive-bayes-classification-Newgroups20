//
//  nb.h
//  naive_bayes
//
//  Created by 李一楠 on 20/4/15.
//  Copyright (c) 2015 yinan. All rights reserved.
//

#ifndef naive_bayes_nb_h
#define naive_bayes_nb_h
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

using namespace std;
class nb{
public:
    nb(char* path);
    void train();
    double test();
private:
    class c;    //c is the label of document, totally 20 labels
    vector<c> cVec;
    char * path;
    int allCount=0;    //number of word in all labels
    int labelCount=0;
    map<string,string> testSet;  //map first is filename, second is file label
    void fileProc(char * filename);
    int testdir(char *path);
    int scanFolder(char *path);
};
class nb::c{
    friend class nb;
private:
    c(string label){
        this->label=label;
    }
    map<string,double> dict;  //dict saves word frequency
    int wordCount=0;        //total volumn of word in the c label
    int dictDim=0;            //dimension of dict, number of kind of word
    double prior=0;           //prior prop of c label,=wordCount/allCount
    //double posterior=0;       //posterior prop of a doc, post(ci)= π[p(wj|ci)]
    string label;
};


#endif