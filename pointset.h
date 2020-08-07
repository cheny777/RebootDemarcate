#ifndef _POINTSET_H_
#define _POINTSET_H_
#include "LyDG.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"


enum PoinType
{
    PotT_BEGIN = 0,
    PotT_1d = 1,
    PotT_2d = 2,
    PotT_3d = 3,
    PotT_4d = 4,
    PotT_5d = 5,
    PotT_6d = 6,
    PotT_7d = 7,
    PotT_8d = 8,
    PotT_9d = 9,
    PotT_10d = 10,
    PotT_11d = 11,
    PotT_12d = 12,
    PotT_13d = 13,
    PotT_14d = 14,
    PotT_15d = 15,
    PotT_16d = 16,
    PotT_17d = 17,
    PotT_18d = 18,
    PotT_19d = 19,
    PotT_20d = 20,
    PotT_21d = 21,
    PotT_22d = 22,
    PotT_FINISH = 23,
};

class CPointSet
{
public:
    CPointSet();
    virtual ~CPointSet();
    bool OnInit(PoinType ptype);
    bool OnEmpty();
    PoinType OnGetType();
    long OnGetPointNum();
    double * OnGetPoint(long pos);
    bool OnSetPoint(double point_value[]);
    bool OnSave(char * path);
    bool OnLoad(char * path);
    bool OnLoadFast(char * path);
    bool OnSave(FILE * fp);
    bool OnLoad(FILE * fp);
    bool OnSetTag(char * tag);
    char * OnGetTag();
    bool OnCheckTag(char * c);
protected:
    PoinType m_PointType;
    CLyDG    m_Points;
    char     m_Tag[256];
};

#endif
