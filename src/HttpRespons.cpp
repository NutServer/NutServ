#include "../includes/HttpRespons.hpp"
#include "../includes/HttpRequest.hpp"

// The "Assembly Line" starts here

//  root       path
// ./www  /index.html
// ./www  index.html
// ./www  /index.html // done
std::map<std::string, std::string> HttpRespons::_mimeMap;

// HttpRespons::HttpRespons(Request &req, Config &conf){
//     struct stat fileInfo;
//     std::string autoIndexBody;
//     std::string testPath;
//     std::string root = conf.getroot();
//     std::string path = req.getpath();

//     if((!root.empty() && root[root.size() - 1] != '/') && (!path.empty() &&
//     path[0] == '/')) // normal case
//         _filePath = root + path;
//     else if((!root.empty() && root[root.size() - 1] == '/') && (!path.empty()
//     && path[0] == '/')){ //
//        root = root.substr(0, root.size() - 1);
//         _filePath = root + path;
//     }
//     else if((!root.empty() && root[root.size() - 1] != '/') && (!path.empty()
//     && path[0] != '/'))
//         _filePath = root + "/" + path;

//     if (stat(_filePath.c_str(), &fileInfo) == 0) {
//         if (S_ISDIR(fileInfo.st_mode)) {
//             std::vector<std::string> indexes = conf.getIndex();
//             bool found = false;

//             for(size_t i = 0; i < indexes.size(); i++){
//                 struct stat indexSate;
//                 testPath = _filePath + ( _filePath[_filePath.size() - 1] ==
//                 '/' ? "" : "/" ) + indexes[i]; if(stat(testPath.c_str(),
//                 &indexSate) == 0){
//                     _statusCode = 200;
//                     found = true;
//                     break ;
//                 }
//             }
//             if(!found){
//                 if (conf.getAutoindex() == true) {
//                     _statusCode = 200;
//                     DIR *dir;
//                     if((dir = opendir(_filePath.c_str()))){
//                         struct dirent* entry;
//                         autoIndexBody = "<html><body><h1>Index of " +
//                         req.getpath() + "</h1><hr>";

//                         while ((entry = readdir(dir)) != NULL) {
//                             std::string name = entry->d_name;

//                             // 1. Ignore the current directory "."
//                             if (name == ".") continue;
//                             // 2. Format the name: If it's a directory,
//                             adding a '/' helps the user
//                             // (Note: You'd need a stat() check here to know
//                             if it's a directory) struct stat dirinfo;
//                             std::string namepath = _filePath + (
//                             _filePath[_filePath.size() - 1] == '/' ? "" : "/"
//                             ) + name; if(stat(namepath.c_str(), &dirinfo) ==
//                             0){
//                                 if (S_ISDIR(dirinfo.st_mode)) {
//                                     name += "/"; // Now it displays as
//                                     "images/" instead of "images"
//                                 }
//                             }
//                             // 3. Create the Link
//                             // The href tells the browser where to go; the
//                             text between <a> and </a> is what the user
//                             clicks. autoIndexBody += "<a href=\"" + name +
//                             "\">" + name + "</a><br>";
//                         }
//                         autoIndexBody += "<hr></body></html>";
//                         _body.assign(autoIndexBody.begin(),
//                         autoIndexBody.end());
//                     }
//                     else
//                         _statusCode = 404;
//                 } else {
//                     _statusCode = 404; // Or 403
//                 }
//             }

//         } else {
//             // It's a regular file. We are good to go!
//             _statusCode = 200;
//             std::ifstream file(_filePath.c_str(), std::ios::binary);
//             if (file.is_open()) {
//                 // 3. Prepare the vector using the size from your 'fileInfo'
//                 struct _body.resize(fileInfo.st_size);
//                 // 4. Read into the vector's memory
//                 file.read(&_body[0], fileInfo.st_size);
//                 file.close();
//             } else {
//                 _statusCode = 403; // File exists but we can't open it
//                 (Permissions)
//             }
//         }
//     }
//     else {
//         // File doesn't exist on the drive
//         _statusCode = 404;
//     }
// }

void HttpRespons::mimefillingmap()
{

    const char *keys[]   = { "html", "css", "js", "jpeg", "png", "txt" };
    const char *values[] = {
        "text/html",  "text/css",  "application/javascript",
        "image/jpeg", "image/png", "text/plain" };

    for ( int index = 0; index < 6; index++ ) {
        _mimeMap[ keys[ index ] ] = values[ index ];
    }
}

// Combines headers + body
// std::string HttpRespons::getFullResponse(){

//}

HttpRespons::~HttpRespons() {}
