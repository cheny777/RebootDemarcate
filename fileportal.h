#ifndef FILEPORTAL_H
#define FILEPORTAL_H
extern "C" __declspec(dllexport) bool SendFile (unsigned char ip1,unsigned char ip2,unsigned char ip3,unsigned char ip4,char * local_file,char * remote_file);
extern "C" __declspec(dllexport) bool RecvFile (unsigned char ip1,unsigned char ip2,unsigned char ip3,unsigned char ip4,char * local_file,char * remote_file);

#endif // FILEPORTAL_H
