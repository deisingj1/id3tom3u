//
//  main.cpp
//  id3tom3u
//
//  Created by Jesse Deisinger on 6/15/16.
//  Copyright (c) 2016 Jesse Deisinger. All rights reserved.
//

#include <iostream>
#include <taglib/tag.h>
#include <taglib/fileref.h>
#include <dirent.h>
#include <fstream>
#include <vector>
#include <algorithm>
#include <string.h>

struct args {
    const char * ifName;
    const char * ofName;
    const char * append;
};
struct song {
    char * fName;
    TagLib::uint trackNo;
    TagLib::String album;
    bool operator<(const song& rhs) const {
        return std::tie(rhs.album,rhs.trackNo) > std::tie(this->album,this->trackNo);
    }
};
const char * ext = ".mp3";

struct args getArgs(int argc, const char * argv[]) {
    struct args a;
    for(int i = 1; i < argc; i++) {
        if(argv[i][0] == '-'){
            switch(argv[i][1]) {
                    case 'i':
                        a.ifName = argv[i+1];
                        break;
                    case 'o':
                        a.ofName = argv[i+1];
                        break;
                    case 'a':
                        a.append = argv[i+1];
                    default:
                        break;
                }
        }
    }
    return a;
};
bool checkSuffix(const char * sarr, std::string suffix) {
    std::string str(sarr);
    if (suffix.length() > str.length()) {
        return true;
    }
    std::string strsuffix = str.substr(str.length()-4);
    //std::cout << strsuffix << std::endl;
    return strsuffix.compare(ext);
}

std::vector<struct song> getFileNames(const char * dirName) {
    std::vector<struct song> fl;
    struct song s;
    DIR *dp;
    struct dirent *ep;
    dp = opendir(dirName);
    if(dp != NULL) {
        while ((ep = readdir(dp))) {
            if(!checkSuffix(ep->d_name,ext)) {
                s = *new struct song;
                s.fName = new char[1024];
                strcpy(s.fName,ep->d_name);
                char path[1024];
                std::strcpy(path, dirName);
                std::strcat(path,s.fName);
                TagLib::FileRef f(path);
                s.album = f.tag()->album();
                s.trackNo = f.tag()->track();
                fl.push_back(s);
            }
        }
        (void) closedir(dp);
    }
    else {
        perror("Couldn't find directory");
    }
    return fl;
}

void printContents(std::vector<struct song> sl) {
    for (int i = 0; i < sl.size(); i++) {
        struct song s = sl.at(i);
        std::cout << s.fName << " " << s.trackNo <<  " " << s.album << std::endl;
    }
}
void writeLists(std::vector<struct song> sl, const char * append) {
    TagLib::String currentAlb = sl.at(0).album;
    std::string outputFile = currentAlb.to8Bit() + ".m3u";
    std::ofstream output;
    output.open(outputFile);
    for (int i = 0; i < sl.size(); i++) {
        struct song currentSong = sl.at(i);
        if (currentSong.album.operator!=(currentAlb)) {
            currentAlb = currentSong.album;
            std::string outputFile = currentAlb.to8Bit() + ".m3u";
            output.close();
            output.open(outputFile);
        }
        output << append << currentSong.fName << std::endl;
        std::cout << currentSong.fName << " " << currentSong.trackNo <<  " " << currentSong.album << std::endl;
    }
}

int main(int argc, const char * argv[]) {
    struct args a = getArgs(argc, argv);
    std::vector<struct song> songList = getFileNames(a.ifName);
    //printContents(songList);
    std::sort(songList.begin(),songList.end());
    printContents(songList);
    writeLists(songList,a.append);
    return 0;
}
