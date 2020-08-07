#ifndef _CFGFILE_H_
#define _CFGFILE_H_
#include "stdio.h"
#include "stdlib.h"
class CCFGFILE
{
public:
    virtual ~CCFGFILE();
    static bool OnSaveString(FILE * fp,char * s);
    static bool OnSaveDouble(FILE * fp,double v);
    static bool OnSaveLong(FILE * fp,long c);
    static bool OnSaveBool(FILE * fp,bool b);
    static bool OnLoadString(FILE * fp,char * s);
    static bool OnLoadDouble(FILE * fp,double &v);
    static bool OnLoadLong(FILE * fp,long &c);
    static bool OnLoadBool(FILE * fp,bool &b);
protected:
    CCFGFILE();
};

#endif
