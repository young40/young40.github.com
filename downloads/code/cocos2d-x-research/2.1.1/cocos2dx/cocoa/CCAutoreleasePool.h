// 声明CCAutoreleasePool类
class CC_DLL CCAutoreleasePool : public CCObject
{//注意这个m_pCurReleasePool是一个私有成员. 居然没有加标号.
    CCArray*    m_pManagedObjectArray;    
public:
    CCAutoreleasePool(void);
    ~CCAutoreleasePool(void);

    void addObject(CCObject *pObject);
    void removeObject(CCObject *pObject);

    void clear();
};

class CC_DLL CCPoolManager
{//下面这三个成员变量依然是私有的.
    CCArray*    m_pReleasePoolStack;    
    CCAutoreleasePool*                    m_pCurReleasePool;

    CCAutoreleasePool* getCurReleasePool();
public:
    CCPoolManager();
    ~CCPoolManager();
    void finalize();
    void push();
    void pop();

    void removeObject(CCObject* pObject);
    void addObject(CCObject* pObject);

    static CCPoolManager* sharedPoolManager();
    static void purgePoolManager();
    //友元
    friend class CCAutoreleasePool;
};
