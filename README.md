# id3tom3u
Parses ID3 tags in directory and writes album playlists
The idea is to be able to create .m3u playlists based on the metatags present in MP3 files. This program automatically generates .m3u playlists, separated by album and ordered by track number, for all MP3 files present in a directory. A use case for this is for devices or applications that ignore the "track number" field in metatags and instead order tracks alphabetically. I like to listen to my albums in order, and I own one of these devices. I was unable to find any existing utilities that had this function.

This application uses the TagLib library, which is under LGPL license and is available freely from https://taglib.github.io/. 
