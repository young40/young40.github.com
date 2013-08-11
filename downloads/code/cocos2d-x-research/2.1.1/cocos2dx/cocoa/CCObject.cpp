CCObject* CCCopying::copyWithZone(CCZone *pZone)
{//CCObject的派生类需要调用copy()则必须重写该函数
    CC_UNUSED_PARAM(pZone);
    CCAssert(0, "not implement");//不重写会报错.
    return 0;
}

CCObject::CCObject(void)
:m_uAutoReleaseCount(0)
,m_uReference(1) // 引用计数默认为1
,m_nLuaID(0)
{
    static unsigned int uObjectCount = 0;
    m_uID = ++uObjectCount;//脚本语言相关, 不去管他.
}

CCObject::~CCObject(void)
{//析构函数
    if (m_uAutoReleaseCount > 0)
    {//自动引用计数则从管理池中删除
        CCPoolManager::sharedPoolManager()->removeObject(this);
    }
    if (m_nLuaID)//脚本语言相关.
    {
        CCScriptEngineManager::sharedManager()->getScriptEngine()->removeScriptObjectByCCObject(this);
    }
    else
    {//看起来脚本语言支持被深度嵌入, 如果能设置个宏直接去掉其支持多好.
        CCScriptEngineProtocol* pEngine = CCScriptEngineManager::sharedManager()->getScriptEngine();
        if (pEngine != NULL && pEngine->getScriptType() == kScriptTypeJavascript)
        {
            pEngine->removeScriptObjectByCCObject(this);
        }
    }
}
// copy实质上是对copyWithZone的调用.
CCObject* CCObject::copy()
{
    return copyWithZone(0);
}

void CCObject::release(void)
{
    CCAssert(m_uReference > 0, "reference count should greater than 0");
    --m_uReference;//引用计数自减

    if (m_uReference == 0)
    {//如果引用计数为0, 调用release会立即释放内存.
        delete this;
    }
}

void CCObject::retain(void)
{
    CCAssert(m_uReference > 0, "reference count should greater than 0");
    ++m_uReference;//引用计数自增.
}

CCObject* CCObject::autorelease(void)
{//autorelease将对象加入自动内存管理池.
    CCPoolManager::sharedPoolManager()->addObject(this);
    return this;
}

bool CCObject::isSingleReference(void)
{//判断对象是否仅被引用了一次
    return m_uReference == 1;
}

unsigned int CCObject::retainCount(void)
{//返回引用次数
    return m_uReference;
}

bool CCObject::isEqual(const CCObject *pObject)
{//判断对象是否为同一个对象.
    return this == pObject;
}
