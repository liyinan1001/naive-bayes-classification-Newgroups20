//this naive_bayes program only runs on unix like system, because of the folder scanner function

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include<map>
#include<set>
#include<fstream>
#include "nb.h"
using namespace std;
int main(int argc,char **argv)  
{
   // ofstream ofs_ratio("/Users/liyinan/Documents/machineLearning/project1/with_stop_no_stem.txt");
  ofstream ofs_ratio("/Users/liyinan/Documents/machineLearning/project1/with_stop_stemmed");
   //ofstream ofs_ratio("/Users/liyinan/Documents/machineLearning/project1/no_stop_stemmed.txt");

    double sumRatio=0;
    for(int i=0;i<10;i++){    //do classification for 10 times and caculate average correct ratio
      // nb mynb("/Users/liyinan/Documents/machineLearning/project1/Newgroups20");

      nb mynb("/Users/liyinan/Documents/machineLearning/project1/Newgroups20_stemmed");
       // nb mynb("/Users/liyinan/Documents/machineLearning/project1/Newgroups20_noStopWord_stemmed");
        mynb.train();
        double ratio=mynb.test();
        sumRatio+=ratio;
        ofs_ratio<<ratio<<" ";
        //cout<<"ratio is "<<sumRatio/(i+1)<<endl;
    }
    ofs_ratio<<endl;
    double finalRatio=sumRatio/10;
    ofs_ratio<<"final ratio: "<<finalRatio;
    ofs_ratio.close();
    cout<<endl<<"final ratio is "<<finalRatio<<endl;
    //final ratio is 0.889404  with nostop_stemmed
    //final ratio is 0.884508 with stemmed
}