static CCPoolManager* s_pPoolManager = NULL;// 用作单例模式的静态变量.

CCAutoreleasePool::CCAutoreleasePool(void)
{
    m_pManagedObjectArray = new CCArray();
    m_pManagedObjectArray->init();
}

CCAutoreleasePool::~CCAutoreleasePool(void)
{
    CC_SAFE_DELETE(m_pManagedObjectArray);
}

void CCAutoreleasePool::addObject(CCObject* pObject)
{
    m_pManagedObjectArray->addObject(pObject);
    // CCAutoreleasePool是CCObject的友元类, 可以直接访问CCObject的protect成员变量.
    CCAssert(pObject->m_uReference > 1, "reference count should be greater than 1");
    ++(pObject->m_uAutoReleaseCount);
    pObject->release(); // no ref count, in this case autorelease pool added.
}
// 下面这个遍历似乎是为了应对被多次加入pool
void CCAutoreleasePool::removeObject(CCObject* pObject)
{
    for (unsigned int i = 0; i < pObject->m_uAutoReleaseCount; ++i)
    {
        m_pManagedObjectArray->removeObject(pObject, false);
    }
}
// 清空池
void CCAutoreleasePool::clear()
{
    if(m_pManagedObjectArray->count() > 0)
    {
        //CCAutoreleasePool* pReleasePool;
#ifdef _DEBUG
        int nIndex = m_pManagedObjectArray->count() - 1;
#endif

        CCObject* pObj = NULL;
        CCARRAY_FOREACH_REVERSE(m_pManagedObjectArray, pObj)//反序遍历
        {
            // 这里还是令人费解, 为何会使用break, 而不是使用continue完成遍历????
            if(!pObj)
                break;

            --(pObj->m_uAutoReleaseCount);
            //(*it)->release();
            //delete (*it);
#ifdef _DEBUG
            nIndex--;
#endif
        }

        m_pManagedObjectArray->removeAllObjects();
    }
}

// CCPoolManager的单例
CCPoolManager* CCPoolManager::sharedPoolManager()
{
    if (s_pPoolManager == NULL)
    {
        s_pPoolManager = new CCPoolManager();
    }
    return s_pPoolManager;
}

// 销毁当前manager
void CCPoolManager::purgePoolManager()
{
    CC_SAFE_DELETE(s_pPoolManager);
}
//构造函数
CCPoolManager::CCPoolManager()
{
    m_pReleasePoolStack = new CCArray();    
    m_pReleasePoolStack->init();
    m_pCurReleasePool = 0;
}
// 析构函数
CCPoolManager::~CCPoolManager()
{
     finalize();

     // 这个注释也让人费解. 为什么只移除第一个, 而不是遍历????
     // we only release the last autorelease pool here 
    m_pCurReleasePool = 0;
     m_pReleasePoolStack->removeObjectAtIndex(0);
 
     CC_SAFE_DELETE(m_pReleasePoolStack);
}

void CCPoolManager::finalize()
{
    if(m_pReleasePoolStack->count() > 0)
    {
        //CCAutoreleasePool* pReleasePool;
        CCObject* pObj = NULL;
        CCARRAY_FOREACH(m_pReleasePoolStack, pObj)
        {
            // 这里有点令人不解, 为什么遍历还没有完成就break了??? 
            // 或许是因为pop push的方式不会在中间产生为NULL的池.
            if(!pObj)
                break;
            CCAutoreleasePool* pPool = (CCAutoreleasePool*)pObj;
            pPool->clear();
        }
    }
}
//创建一个新池, 注意其中pPool的引用计数的变化.
void CCPoolManager::push()
{
    CCAutoreleasePool* pPool = new CCAutoreleasePool();       //ref = 1
    m_pCurReleasePool = pPool;

    m_pReleasePoolStack->addObject(pPool);                   //ref = 2

    pPool->release();                                       //ref = 1
}
//销毁当前池
void CCPoolManager::pop()
{
    if (! m_pCurReleasePool)
    {
        return;
    }

     int nCount = m_pReleasePoolStack->count();

    m_pCurReleasePool->clear();
 
      if(nCount > 1)
      {//注意这里removeObjectAtIndex会将当前池的引用技术减1, 从而销毁当前池.
        m_pReleasePoolStack->removeObjectAtIndex(nCount-1);

//         if(nCount > 1)
//         {
//             m_pCurReleasePool = m_pReleasePoolStack->objectAtIndex(nCount - 2);
//             return;
//         }
        m_pCurReleasePool = (CCAutoreleasePool*)m_pReleasePoolStack->objectAtIndex(nCount - 2);
    }
//如果没有意外, 这里会最少保留一个池不会被销毁. 但最后一个池还是会被清空..
    /*m_pCurReleasePool = NULL;*/
}
// 从当前池中移除对象.
void CCPoolManager::removeObject(CCObject* pObject)
{
    CCAssert(m_pCurReleasePool, "current auto release pool should not be null");

    m_pCurReleasePool->removeObject(pObject);
}
//添加一个对象到当前池
void CCPoolManager::addObject(CCObject* pObject)
{
    getCurReleasePool()->addObject(pObject);
}

//形成单例模式
CCAutoreleasePool* CCPoolManager::getCurReleasePool()
{
    if(!m_pCurReleasePool)
    {
        push();
    }

    CCAssert(m_pCurReleasePool, "current auto release pool should not be null");

    return m_pCurReleasePool;
}
