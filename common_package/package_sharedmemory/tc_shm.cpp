#include "tc_shm.h"
#include <cassert>
#include <errno.h>

namespace taf
{

TC_Shm::TC_Shm(size_t iShmSize, key_t iKey, bool bOwner)
{
    init(iShmSize, iKey, bOwner);
}

TC_Shm::~TC_Shm()
{
    if(_bOwner)
    {
        detach();
    }
}

void TC_Shm::init(size_t iShmSize, key_t iKey, bool bOwner)
{
    assert(_pshm == NULL);

    _bOwner     = bOwner;

    //ע??_bCreate?ĸ?ֵλ??:??֤???߳???һ????????ʱ??Ҳ??????????
    //??ͼ????
    if ((_shemID = shmget(iKey, iShmSize, IPC_CREAT | IPC_EXCL | 0666)) < 0)
    {
        _bCreate = false;
        //?п??????Ѿ?????ͬ????key_shm,????ͼ��??
        if ((_shemID = shmget(iKey, iShmSize, 0666)) < 0)
        {
            throw TC_Shm_Exception("[TC_Shm::init()] shmget error", errno);
        }
    }
    else
    {
        _bCreate    = true;
    }

    //try to access shm
    if ((_pshm = shmat(_shemID, NULL, 0)) == (char *) -1)
    {
        throw TC_Shm_Exception("[TC_Shm::init()] shmat error", errno);
    }

    _shmSize = iShmSize;
    _shmKey = iKey;
}

int TC_Shm::detach()
{
    int iRetCode = 0;
    if(_pshm != NULL)
    {
        iRetCode = shmdt(_pshm);

        _pshm = NULL;
    }

    return iRetCode;
}

int TC_Shm::del()
{
    int iRetCode = 0;
    if(_pshm != NULL)
    {
        iRetCode = shmctl(_shemID, IPC_RMID, 0);

        _pshm = NULL;
    }

    return iRetCode;
}

}
