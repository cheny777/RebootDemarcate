#include "cfile.h"
#include "string.h"

CCFGFILE::CCFGFILE()
{

}

CCFGFILE::~CCFGFILE()
{

}


bool CCFGFILE::OnSaveString(FILE * fp,char * s)
{
    if(s == NULL)
    {
        return false;
    }

    long size = strlen(s);
    if(size >= 1024)
    {
        return false;
    }

    fprintf(fp,"#%d",size);
    if(size > 0)
    {
        fprintf(fp,"#%c",s[0]);
        for(long i = 1; i < size; i ++)
        {
            fprintf(fp,"%c",s[i]);
        }
        return true;
    }
    else
    {
        fprintf(fp,"#E");
        return true;
    }
}

bool CCFGFILE::OnSaveDouble(FILE * fp,double v)
{
    fprintf(fp,"#%lf",v);
    return true;
}
bool CCFGFILE::OnSaveLong(FILE * fp,long c)
{
    fprintf(fp,"#%d",c);
    return true;
}

bool CCFGFILE::OnSaveBool(FILE * fp,bool b)
{
    if(b)
    {
        fprintf(fp,"#1");
    }
    else
    {
        fprintf(fp,"#0");
    }
    return true;
}

bool CCFGFILE::OnLoadString(FILE * fp,char * s)
{
    long size;
    if(fscanf(fp,"#%d",&size)!=1)
    {
        return false;
    }
    if(size < 0 || size >= 1024)
    {
        return false;
    }
    if( fscanf(fp,"#%c",&s[0])!=1)
    {
        return false;
    }

    if(size == 0)
    {
        s[0] = '\0';
        return true;
    }



    for(long i = 1; i < size; i ++)
    {
        if( fscanf(fp,"%c",&s[i]) != 1)
        {
            return false;
        }
    }
    s[size] = '\0';
    return true;

}
bool CCFGFILE::OnLoadDouble(FILE * fp,double &v)
{
    if(fscanf(fp,"#%lf",&v)!=1)
    {
        return false;
    }
    return true;
}
bool CCFGFILE::OnLoadLong(FILE * fp,long &c)
{
    if(fscanf(fp,"#%d",&c)!=1)
    {
        return false;
    }
    return true;
}


bool CCFGFILE::OnLoadBool(FILE * fp,bool &b)
{
    long c;
    if(fscanf(fp,"#%d",&c)!=1)
    {
        return false;
    }
    if(c == 0)
    {
        b = false;
        return true;
    }
    if(c == 1)
    {
        b = true;
        return true;
    }
    return false;
}

