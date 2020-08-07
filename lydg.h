#ifndef _LYDG_H_
#define _LYDG_H_

#define BLOCKSIZE 100

class CLyDG
{
public:
    CLyDG();
    virtual ~CLyDG();
    bool OnDelete(long pos);
    bool OnExChange(long pos1,long pos2);
    bool OnInit(long item_size);
    void OnEmpty();
    bool OnAdd(void * data);
    long OnGetNum();
    void * OnGet(long serial);
protected:
    bool EXPadBase();
    bool EXPadBlock();
    void ** m_pBase;
    long m_BaseNum;
    long m_BaseSize;
    long m_ItemSize;
    long m_ItemNum;
    bool m_InitTag;
};

#endif
