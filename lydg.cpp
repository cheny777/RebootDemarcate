#include "lydg.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"

CLyDG::CLyDG()
{
    m_pBase = NULL;
    m_BaseNum = 0;
    m_BaseSize = 0;
    m_ItemNum = 0;
    m_ItemSize = 0;
    m_InitTag = false;
}

CLyDG::~CLyDG()
{
    OnEmpty();
}

bool CLyDG::OnExChange(long pos1,long pos2)
{
    if(m_InitTag == false)
    {
        return false;
    }
    if(pos1 < 0 || pos1 >= m_ItemNum || pos2 < 0 || pos2 >= m_ItemNum || pos1 == pos2)
    {
        return false;
    }

    void * a = OnGet(pos1);
    void * b = OnGet(pos2);
    if(a == NULL || b == NULL)
    {
        return false;
    }

    char * c = (char *)malloc(m_ItemSize);
    memcpy(c,a,m_ItemSize);
    memcpy(a,b,m_ItemSize);
    memcpy(b,c,m_ItemSize);
    free(c);
    return true;
}


bool CLyDG::OnDelete(long pos)
{
    if(m_InitTag == false)
    {
        return false;
    }
    if(pos < 0 || pos >= m_ItemNum)
    {
        return false;
    }
    CLyDG t;
    t.OnInit(m_ItemSize);
    long i;
    for( i = 0; i < m_ItemNum ; i ++)
    {
        if(i != pos)
        {
            t.OnAdd(OnGet(i));
        }
    }
    OnEmpty();
    OnInit(t.m_ItemSize);
    for( i = 0; i < t.m_ItemNum ; i ++)
    {
        OnAdd(t.OnGet(i));
    }
    return true;
}

void CLyDG::OnEmpty()
{
    if(m_pBase != NULL)
    {
        for(long i = 0; i < m_BaseSize ; i ++)
        {
            if(m_pBase[i]!=NULL)
            {
                free(m_pBase[i]);
                m_pBase[i] = NULL;
            }
        }
        free(m_pBase);
    }
    m_pBase = NULL;
    m_BaseNum = 0;
    m_BaseSize = 0;
    m_ItemSize = 0;
    m_ItemNum = 0;
    m_InitTag = false;
}

bool CLyDG::OnInit(long item_size)
{
    OnEmpty();
    if(item_size<=0)
    {
        return false;
    }
    m_pBase = (void **)malloc(sizeof(void *) * BLOCKSIZE);
    if(m_pBase == NULL)
    {
        return false;
    }
    for(long i = 0; i < BLOCKSIZE; i ++)
    {
        m_pBase[i] = NULL;
    }
    m_BaseSize = BLOCKSIZE;
    m_BaseNum = 0;
    m_ItemSize = item_size;
    m_ItemNum = 0;
    m_InitTag = true;
    return true;
}


bool CLyDG::EXPadBase()
{
    if(m_InitTag == false)
    {
        return false;
    }
    long tmp = m_BaseSize * 2;
    long i;
    void ** new_base = (void **)malloc(sizeof(void *) * tmp);
    if(new_base == NULL)
    {
        return false;
    }
    for( i = 0; i < tmp; i ++)
    {
        new_base[i] = NULL;
    }
    for( i = 0; i < m_BaseSize ; i ++)
    {
        new_base[i] = m_pBase[i];
    }
    free(m_pBase);
    m_pBase = new_base;
    m_BaseSize = tmp;
    return true;
}


bool CLyDG::EXPadBlock()
{
    if(m_InitTag == false)
    {
        return false;
    }
    if(m_BaseNum + 1 >= m_BaseSize)
    {
        if(EXPadBase()== false)
        {
            return false;
        }
    }
    m_pBase[m_BaseNum] = malloc(BLOCKSIZE * m_ItemSize);
    if(m_pBase[m_BaseNum] == NULL)
    {
        return false;
    }
    m_BaseNum ++;
    return true;
}

long CLyDG::OnGetNum()
{
    if(m_InitTag == false)
    {
        return 0;
    }
    return m_ItemNum;
}

void * CLyDG::OnGet(long serial)
{
    if(m_InitTag == false || serial <0 || serial >= m_ItemNum)
    {
        return NULL;
    }
    long b_serial = serial / BLOCKSIZE;
    long i_serial = serial % BLOCKSIZE;
    if(b_serial >= m_BaseNum )
    {
        return NULL;
    }
    unsigned char * p1 = (unsigned char * )m_pBase[b_serial];
    p1 = p1 + m_ItemSize * i_serial;
    return (void *)p1;
}

bool CLyDG::OnAdd(void * data)
{
    if(m_InitTag == false || data == NULL)
    {
        return false;
    }
    long cap = m_BaseNum * BLOCKSIZE;
    long tmp = m_ItemNum;
    if(tmp +2 >= cap)
    {
        if(EXPadBlock()==false)
        {
            return false;
        }
    }

    long b_serial = tmp / BLOCKSIZE;
    long i_serial = tmp % BLOCKSIZE;
    if(b_serial >= m_BaseNum )
    {
        return false;
    }
    unsigned char * p1 = (unsigned char * )m_pBase[b_serial];
    p1 = p1 + m_ItemSize * i_serial;
    memcpy((void *)p1,data,m_ItemSize);
    m_ItemNum ++;
    return true;
}


