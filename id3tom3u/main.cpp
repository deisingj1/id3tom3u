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

struct args {
    const char * ifName;
    const char * ofName;
    const char * append;
};

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

int main(int argc, const char * argv[]) {
    struct args a = getArgs(argc, argv);
    DIR *dp;
    struct dirent *ep;
    dp = opendir(a.ifName);
    if(dp != NULL) {
            while ((ep = readdir(dp)))
            puts(ep->d_name);
        (void) closedir(dp);
    }
    else {
        perror("Couldn't find directory");
    }
    
    TagLib::FileRef f("test.mp3");
    TagLib::String artist = f.tag()->album();
    TagLib::uint trackno = f.tag()->track();
    std::cout << artist << " track " << trackno << std::endl;
    return 0;
}
