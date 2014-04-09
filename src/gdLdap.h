#if !defined GD_LDAP__
#define GD_LDAP__          1

#define LDAP_DEPRECATED 1

#include "ldap.h"
#include "ldap_schema.h"

#include <string>
#include <vector>

typedef enum {
  CLdapErrOK,                  /* OK                                            */
  CLdapErrUnkUser = 301,       /* Unknow user                                   */
  CLdapErrInit,                /* Init Ldap error                               */
  CLdapErrOpen,                /* Open a connection with the ldap server failed */
  CLdapErrSetProto,            /* Cannot set option protocol                    */
  CLdapErrBind,                /* Bind a connection with the ldap server failed */
  CLdapErrSearch,              /* Ldap search failed                            */
  CLdapErrNoEntries,           /* Ldap search return no entries                 */
  CLdapErrTooMuchEntries,      /* Ldap search return too much entries           */
  CLdapErrNotEnoughMem,        /* No enought memory !                           */
  CLdapErrBadServerItem,       /* Bad Server Item                               */
  CLdapErrBadFilterItem,       /* Bad Filter Item                               */
  CLdapErrModify,              /* Ldap modify failed                            */
  CLdapErrNoModification,      /* No modification available                     */

  CLdapErrLast
} CLdapErr;

typedef enum {
  SchemaStatus_NotLoaded,
  SchemaStatus_Loaded,
  SchemaStatus_Error
} TSchemaStatus;

typedef std::vector<LDAPObjectClass*>   gdCLdapObjectClasses;
typedef std::vector<LDAPAttributeType*> gdCLdapAttributeTypes;
typedef std::vector<LDAPMatchingRule*>  gdCLdapMatchingRules;
typedef std::vector<LDAPSyntax*>        gdCLdapSyntaxes;

class gdCLdapSchema {
public :
  gdCLdapObjectClasses   arOC;
  gdCLdapAttributeTypes  arAT;
  gdCLdapMatchingRules   arMR;
  gdCLdapSyntaxes        arSyntax;
};

class gdCLdapClassItemInfo {
public :
  std::string      ocOwner; // ObjectClass owner of this attribute
  std::string      Attr;
};

typedef std::vector<gdCLdapClassItemInfo*> gdCLdapClassItemsInfo;

// Classe contenant tous les attributs de l'objectClass <Name>
class gdCLdapClassInfo {
public :
  gdCLdapClassItemsInfo      Must;
  gdCLdapClassItemsInfo      May;
};

typedef std::vector<gdCLdapClassInfo*> gdCLdapClassesInfo;

enum {
  gdLDAPBERVALTYPE_START,
  // LH = NO
  gdLDAPBERVALTYPE_ACIITEM,
  gdLDAPBERVALTYPE_AUDIO,
  gdLDAPBERVALTYPE_CERTIFICATE,
  gdLDAPBERVALTYPE_CERTIFICATELIST,
  gdLDAPBERVALTYPE_CERTIFICATEPAIR,
  gdLDAPBERVALTYPE_FAX,
  gdLDAPBERVALTYPE_JPEG,
  gdLDAPBERVALTYPE_SUPPORTEDALGORITHM,
  gdLDAPBERVALTYPE_END
};

class gdCLdapBerVal {
public:
  enum acl { aclRW = 0, aclRO = 1 };

public:
  gdCLdapBerVal() : m_len(0), m_val(0), m_oldlen(0), m_oldval(0), m_bstr(false), m_bUpdated(false) {};
  gdCLdapBerVal(size_t len, const char* val);
  gdCLdapBerVal(const gdCLdapBerVal& bervalSource);
  ~gdCLdapBerVal() {
    if ( m_val ) /*delete [] m_val;*/ m_val = 0;
  };

  void                  Update(size_t len, const char* val);
  void                  CancelUpdate();
  void                  encodeUTF8(char* strbase);
  void                  decodeUTF8(char* utf8);
  std::string&          ToStr();
  size_t                GetLenght()     {
    return m_len;
  };
  char*                 GetVal()        {
    return m_val;
  };
  size_t                GetOldLenght()  {
    return m_oldlen;
  };
  char*                 GetOldVal()     {
    return m_oldval;
  };
  bool                  IsModified()    {
    return m_bUpdated;
  };
  void                  DBUpdated();
  void                  setACL(int acl);
  bool                  isModifiable();

protected:
  size_t                 m_len;
  char*                  m_val;
  size_t                 m_oldlen;
  char*                  m_oldval;
  bool                   m_bstr;
  bool                   m_bUpdated;
  std::string            m_str;
  int                    m_acl;
};

typedef std::vector<gdCLdapBerVal*> gdCLdapBerVals;

class gdCLdapAttribute {
public :
  gdCLdapAttribute(const std::string& strAttr) : Attr(strAttr), bNewAttribute(false), bNotOnEntry(false) {};
  gdCLdapAttribute(const gdCLdapAttribute& attributeSource);
  void                   addValue(const std::string& sVal);

  std::string            Attr;
  gdCLdapBerVals         Values;
  bool                   bNewAttribute;
  bool                   bNotOnEntry;
};

typedef std::vector<gdCLdapAttribute*> gdCLdapAttributes;

class gdCLdapEntry {
public :
  gdCLdapEntry() : HasChildren(0), m_bUpdated(0), KnowAttr(0) {}
  ~gdCLdapEntry()               {
    if ( KnowAttr ) delete KnowAttr;
  }
  void                  Clear()                      {
    Attrs.clear();
    delete KnowAttr;
    Dn.clear();
  }

  std::string            Dn;
  gdCLdapAttributes      Attrs;
  bool                   HasChildren;
  bool                   m_bUpdated;
  gdCLdapClassInfo*      KnowAttr;
};

typedef std::vector<gdCLdapEntry*> gdCLdapEntries;

class gdCLdapServer;

class gdCLdapFilter {
public :
  gdCLdapFilter();
  gdCLdapFilter(gdCLdapFilter* pOld);

  gdCLdapServer*       pServer;
  std::string          strInitialSearchBase;
  std::string          strSearchBase;
  std::string          strDisplayName;
  std::string          strFilter;
  int                  nScope;
};

class gdCLdapServer {
public :
  gdCLdapServer();
  ~gdCLdapServer();
  int                  GetLastError()  {
    return m_nErr;
  };
  bool                 TryServer();
  bool                 Connect();
  void                 DisConnect();
  LDAP*                GetHandle() {
    return m_pLd;
  };
  bool                 GetNamingContexts(gdCLdapEntries& pEntries);
  bool                 LoadSchema();
  bool                 GetSubSchema(gdCLdapEntries& cEntries);
  bool                 GetSubSchemaSubEntry(std::string& strSsSe);
  bool                 GetEntry(const std::string& theDN, gdCLdapEntry& pEntry);
  bool                 LoadClassesInfo(gdCLdapEntries& cEntries, bool force = false);
  bool                 LoadClassesInfo(gdCLdapEntry& cEntry, bool force = false);
  bool                 LoadClassInfo(const std::string& ocName, gdCLdapEntry& cEntry);
  bool                 GetAttributeSyntaxDesc(const std::string& pOID, std::string& pDesc);
  bool                 GetAttribute(const std::string& pAttr, LDAPAttributeType** pObjAttr = 0);
  bool                 GetAttributeOID(const std::string& pAttr, std::string& pOID, LDAPAttributeType** pObjAttr = 0);
  bool                 GetAttributeType(const std::string& pAttr, std::string& pType, LDAPAttributeType** pObjAttr = 0);
  bool                 HasUseBinary(LDAPAttributeType* pObjAT);
  bool                 IsOperationnal(std::string& pAttr);
  bool                 IsMustAttribute(gdCLdapEntry& cEntry, const std::string& pAttr);
  bool                 GetParentClass(gdCLdapEntry& cEntry, const std::string& pAttr, std::string& pClass);
  bool                 UpdateAttribute(gdCLdapEntry& cEntry, gdCLdapAttribute& pAttr);
  bool                 UpdateEntry(gdCLdapEntry& cEntry, bool bNewEntry = false);

  std::string          strServer;
  std::string          strDisplayName;
  std::string          strLogon;
  std::string          strPassword;
  long                 lPort;
  bool                 bOIDObjectOnly;
  bool                 bHasSubordinates;

  gdCLdapSchema        Schema;
  TSchemaStatus        SchemaStatus;
  gdCLdapClassesInfo   cInfo;

private :
  void                SetError(int nErr);
  LDAP*               m_pLd;
  int                 m_nErr;
};

class gdCLdap {
public:
  gdCLdap() : m_nErr(0), m_bDebug(0), m_bOnQueryLdap(false), m_bUseParentInfos(false), m_bManageDSAIT(false) {};
  int                  GetLastError()        {
    return m_nErr;
  };

  void                 setDebugging(bool bDebug)                         {
    m_bDebug = bDebug;
  };
  void                 setUseParentInformations(bool bParentInfos)       {
    m_bUseParentInfos = bParentInfos;
  };
  void                 setParentLimit(std::string sParentLimit)          {
    m_sParentLimit = sParentLimit;
  };
  void                 setManageDSAIT(bool bManageDSAIT)                 {
    m_bManageDSAIT = bManageDSAIT;
  };
  bool                 CheckHasSubordinatesAttr(gdCLdapFilter* pFilter);
  /*
     bool GetObjectClasses(gdlLdapServer* pServer,  CBSLdapEntries* pEntries);
     bool CheckUser(gdlLdapServer* pServer, char* pUser, gdlLdapResAttr* pQuery, gdlLdapResEntry** pTblEntry);
     bool CheckID(gdlLdapServer* pServer, char* pID, gdlLdapResAttr* pQuery, gdlLdapResEntry** pTblEntry);
     bool GetUserID(gdlLdapServer* pServer, char* pUser, char* pID);
     bool GetEmailByUID(gdlLdapServer* pServer, char* pID, char* pEMail);
     bool GetTree(gdlLdapServer* pServer, gdlLdapResAttr* pQuery, gdlLdapResEntry** pTblEntry);

  */
  char*                 GetRdn(char* pDn);
  char*                 GetParentDN(const char* theDN);
  bool                  GetInfo(gdCLdapFilter& cFilter, std::string* spFilter, gdCLdapAttributes& witchAttributes, gdCLdapEntries& cEntries, bool bCountEntries = false);
  bool                  GetInfo(gdCLdapFilter& cFilter, gdCLdapAttributes& witchAttributes, gdCLdapEntries& cEntries, bool bCountEntries = false) {
    return GetInfo(cFilter, 0, witchAttributes, cEntries, bCountEntries);
  };

private :

  void                  SetError(int nErr);

  int                   m_nErr;
  bool                  m_bOnQueryLdap;
  bool                  m_bDebug;
  bool                  m_bUseParentInfos;
  bool                  m_bManageDSAIT;
  std::string           m_sParentLimit;
};

#endif // GD_LDAP__

