#include "pointset.h"

CPointSet::CPointSet()
{
    m_PointType = PotT_BEGIN;
    memset(m_Tag,0,256);
}

CPointSet::~CPointSet()
{

}

bool CPointSet::OnInit(PoinType ptype)
{
    if(ptype <= PotT_BEGIN || ptype >= PotT_FINISH )
    {
        return false;
    }

    if( m_Points.OnInit( ptype * sizeof(double)) == true)
    {
        m_PointType = ptype;
        return true;
    }

    memset(m_Tag,0,256);

    return false;
}

PoinType CPointSet::OnGetType()
{
    return m_PointType;
}

bool CPointSet::OnEmpty()
{
    if(m_PointType == PotT_BEGIN)
    {
        return false;
    }
    m_Points.OnEmpty();

    if( m_Points.OnInit( m_PointType * sizeof(double)) == true)
    {
        return true;
    }

    memset(m_Tag,0,256);

    return false;

}

bool CPointSet::OnSetTag(char * tag)
{
    if(tag == NULL)
    {
        return false;
    }
    long tlen = strlen(tag);
    if(tlen<=0 || tlen >=256)
    {
        return false;
    }

    bool ok_tag = true;
    long i;
    for(i = 0; i < tlen && ok_tag == true; i ++)
    {
        ok_tag = false;
        if(tag[i]>='a' && tag[i]<='z')
        {
            ok_tag = true;
        }

        if(tag[i]>='A' && tag[i]<='Z')
        {
            ok_tag = true;
        }

        if(i != 0)
        {
            if(tag[i]>='0' && tag[i]<='9')
            {
                ok_tag = true;
            }
        }

        if(tag[i]=='_' || tag[i] =='#')
        {
            ok_tag = true;

        }
    }

    if(ok_tag == false)
    {
        return false;
    }

    memset(m_Tag,0,256);

    for( i = 0; i < tlen; i ++)
    {
        m_Tag[i]= tag[i];
    }
    return true;
}

char * CPointSet::OnGetTag()
{
    if(strlen(m_Tag) == 0)
    {
        return NULL;
    }
    return m_Tag;
}

bool CPointSet::OnCheckTag(char * c)
{
    long len = strlen(m_Tag);
    if(len == 0)
    {
        return false;
    }
    if(c == NULL)
    {
        return false;
    }
    long len2 = strlen(c);
    if(len != len2)
    {
        return false;
    }

    for(long i = 0; i < len; i ++)
    {
        if(m_Tag[i] != c[i])
        {
            return false;
        }
    }
    return true;
}


long CPointSet::OnGetPointNum()
{
    return m_Points.OnGetNum();
}


double * CPointSet::OnGetPoint(long pos)
{
    return (double *)m_Points.OnGet(pos);
}

bool CPointSet::OnSetPoint(double point_value[])
{
    if(m_PointType == PotT_BEGIN)
    {
        return false;
    }
    return m_Points.OnAdd(point_value);
}

bool CPointSet::OnSave(FILE * fp)
{
    if(m_PointType == PotT_BEGIN)
    {
        return false;
    }
    long num = OnGetPointNum();


    char r = 0x0a;
    if(strlen(m_Tag) == 0)
    {
        fprintf(fp,"PoinType=%d@%ld%c",m_PointType,num,r);
    }
    else
    {
        fprintf(fp,"PoinType=%d@%ldT%d%s%c",m_PointType,num,strlen(m_Tag),m_Tag,r);
    }

    double * tmp;
    for(long i = 0; i < num ; i ++)
    {
        tmp = OnGetPoint(i);
        for(long j = 0; j < m_PointType ; j ++)
        {
            if(j < 3)
            {
                fprintf(fp,"%c %lf$",'X'+j,tmp[j]);
            }
            if(j >= 3 && j < 6)
            {
                fprintf(fp,"%c %lf$",'A'+j-3,tmp[j]);
            }
            if(j >= 6 && j < 9)
            {
                fprintf(fp,"%c %lf$",'U'+j-6,tmp[j]);
            }
            if(j >= 9 && j < 12)
            {
                fprintf(fp,"%c %lf$",'O'+j-9,tmp[j]);
            }
            if(j >= 12 && j < 15)
            {
                fprintf(fp,"%c %lf$",'I'+j-12,tmp[j]);
            }

            if(j >= 15 && j < 18)
            {
                fprintf(fp,"%c %lf$",'L'+j-15,tmp[j]);
            }

            if(j >= 18 && j < 21)
            {
                fprintf(fp,"%c %lf$",'R'+j-18,tmp[j]);
            }

            if(j >= 21)
            {
                fprintf(fp,"O %lf$",tmp[j]);
            }
        }
        fprintf(fp,"%c",r);
    }
    return true;
}

bool CPointSet::OnSave(char * path)
{
    if(m_PointType == PotT_BEGIN)
    {
        return false;
    }
    long num = OnGetPointNum();
    if(num <= 0)
    {
        return false;
    }

    if(path == NULL)
    {
        return false;
    }
    FILE * fp = fopen(path,"wb");
    if(fp == NULL)
    {
        return false;
    }
    fflush(fp);

    char r = 0x0a;
    if(strlen(m_Tag) == 0)
    {
        fprintf(fp,"PoinType=%d@%ld%c",m_PointType,num,r);
    }
    else
    {
        fprintf(fp,"PoinType=%d@%ldT%d%s%c",m_PointType,num,strlen(m_Tag),m_Tag,r);
    }

    double * tmp;
    for(long i = 0; i < num ; i ++)
    {
        tmp = OnGetPoint(i);
        for(long j = 0; j < m_PointType ; j ++)
        {
            if(j < 3)
            {
                fprintf(fp,"%c %lf$",'X'+j,tmp[j]);
            }
            if(j >= 3 && j < 6)
            {
                fprintf(fp,"%c %lf$",'A'+j-3,tmp[j]);
            }
            if(j >= 6 && j < 9)
            {
                fprintf(fp,"%c %lf$",'U'+j-6,tmp[j]);
            }
            if(j >= 9 && j < 12)
            {
                fprintf(fp,"%c %lf$",'O'+j-9,tmp[j]);
            }
            if(j >= 12 && j < 15)
            {
                fprintf(fp,"%c %lf$",'I'+j-12,tmp[j]);
            }

            if(j >= 15 && j < 18)
            {
                fprintf(fp,"%c %lf$",'L'+j-15,tmp[j]);
            }

            if(j >= 18 && j < 21)
            {
                fprintf(fp,"%c %lf$",'R'+j-18,tmp[j]);
            }

            if(j >= 21)
            {
                fprintf(fp,"O %lf$",tmp[j]);
            }
        }
        fprintf(fp,"%c",r);
    }
    fclose(fp);
    return true;
}

bool CPointSet::OnLoad(FILE * fp)
{
    if(m_PointType == PotT_BEGIN)
    {
        return false;
    }


    char tag_buf[256];
    memset(tag_buf,0,256);

    char r = 0x0a;
    long lt;
    long num;
    if( fscanf(fp,"PoinType=%ld@%ld%c",&lt,&num,&r)!=3)
    {
        fclose(fp);
        return false;
    }

    if(r == 'T')
    {
        long tlen;
        if( fscanf(fp,"%ld",&tlen) != 1)
        {
            fclose(fp);
            return false;
        }
        if(tlen <=0 || tlen >256)
        {
            fclose(fp);
            return false;
        }


        for(long i = 0; i < tlen; i ++)
        {
            if(fscanf(fp,"%c",&tag_buf[i]) != 1)
            {
                fclose(fp);
                return false;
            }
        }
        if(fscanf(fp,"%c",&r) != 1)
        {
            fclose(fp);
            return false;
        }
        if(r != 0x0a)
        {
            fclose(fp);
            return false;
        }
    }


    if(lt != m_PointType)
    {
        return false;
    }

    if(num < 0)
    {
        return false;
    }

    if(strlen(tag_buf)!=0)
    {
        if(OnSetTag(tag_buf) == false)
        {
            fclose(fp);
            return false;
        }
    }


    if( m_Points.OnInit(m_PointType * sizeof(double)) == false)
    {
        fclose(fp);
        return false;
    }
    double tmp[100];
    char c;
    for(long i = 0; i < num ; i ++)
    {
        for(long j = 0; j < m_PointType ; j ++)
        {
            if( fscanf(fp,"%c %lf$",&c,&tmp[j])!=2)
            {
                return false;
            };
        }
        fscanf(fp,"%c",&r);
        if(r != 0x0a)
        {
            return false;
        }
        OnSetPoint(tmp);
    }
    return true;
}

bool CPointSet::OnLoadFast(char * path)
{
    if(path == NULL)
    {
        return false;
    }
    FILE * fp = fopen(path,"rb");
    if(fp == NULL)
    {
        return false;
    }

    char tag_buf[256];
    memset(tag_buf,0,256);
    memset(m_Tag,0,256);


    char r = 0x0a;
    long lt;
    long num;
    if( fscanf(fp,"PoinType=%ld@%ld%c",&lt,&num,&r)!=3)
    {
        fclose(fp);
        return false;
    }

    if(r == 'T')
    {
        long tlen;
        if( fscanf(fp,"%ld",&tlen) != 1)
        {
            fclose(fp);
            return false;
        }
        if(tlen <=0 || tlen >=256)
        {
            fclose(fp);
            return false;
        }


        for(long i = 0; i < tlen; i ++)
        {
            if(fscanf(fp,"%c",&tag_buf[i]) != 1)
            {
                fclose(fp);
                return false;
            }
        }
        if(fscanf(fp,"%c",&r) != 1)
        {
            fclose(fp);
            return false;
        }
        if(r != 0x0a)
        {
            fclose(fp);
            return false;
        }
    }

    if( OnInit(PoinType(lt)) == false)
    {
        fclose(fp);
        return false;
    }

    if(strlen(tag_buf)!=0)
    {
        if(OnSetTag(tag_buf) == false)
        {
            fclose(fp);
            return false;
        }
    }

    if(num <= 0)
    {
        fclose(fp);
        return false;
    }

    if( m_Points.OnInit(m_PointType * sizeof(double)) == false)
    {
        fclose(fp);
        return false;
    }
    double tmp[100];
    char c;
    for(long i = 0; i < num ; i ++)
    {
        for(long j = 0; j < m_PointType ; j ++)
        {
            if( fscanf(fp,"%c %lf$",&c,&tmp[j])!=2)
            {
                return false;
            }
        }
        fscanf(fp,"%c",&r);
        if(r != 0x0a)
        {
            fclose(fp);
            return false;
        }
        OnSetPoint(tmp);
    }
    fclose(fp);
    return true;
}

bool CPointSet::OnLoad(char * path)
{
    if(m_PointType == PotT_BEGIN)
    {
        return false;
    }
    if(path == NULL)
    {
        return false;
    }
    FILE * fp = fopen(path,"rb");
    if(fp == NULL)
    {
        return false;
    }

    char tag_buf[256];
    memset(tag_buf,0,256);

    char r = 0x0a;
    long lt;
    long num;
    if( fscanf(fp,"PoinType=%ld@%ld%c",&lt,&num,&r)!=3)
    {
        fclose(fp);
        return false;
    }

    if(r == 'T')
    {
        long tlen;
        if( fscanf(fp,"%ld",&tlen) != 1)
        {
            fclose(fp);
            return false;
        }
        if(tlen <=0 || tlen >=256)
        {
            fclose(fp);
            return false;
        }

        for(long i = 0; i < tlen; i ++)
        {
            if(fscanf(fp,"%c",&tag_buf[i]) != 1)
            {
                fclose(fp);
                return false;
            }
        }
        if(fscanf(fp,"%c",&r) != 1)
        {
            fclose(fp);
            return false;
        }
        if(r != 0x0a)
        {
            fclose(fp);
            return false;
        }
    }

    if(lt != m_PointType)
    {
        fclose(fp);
        return false;
    }
    if(num <= 0)
    {
        fclose(fp);
        return false;
    }

    if(strlen(tag_buf)!=0)
    {
        if(OnSetTag(tag_buf) == false)
        {
            fclose(fp);
            return false;
        }
    }
    if( m_Points.OnInit(m_PointType * sizeof(double)) == false)
    {
        fclose(fp);
        return false;
    }

    double tmp[100];
    char c;
    for(long i = 0; i < num ; i ++)
    {
        for(long j = 0; j < m_PointType ; j ++)
        {
            if( fscanf(fp,"%c %lf$",&c,&tmp[j])!=2)
            {
                return false;
            }
        }
        fscanf(fp,"%c",&r);
        if(r != 0x0a)
        {
            fclose(fp);
            return false;
        }
        OnSetPoint(tmp);
    }
    fclose(fp);
    return true;
}
