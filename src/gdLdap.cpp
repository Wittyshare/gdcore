#include <stdio.h>
#include <iconv.h>
#include <iostream>

#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/replace.hpp>

#include "gdLdap.h"

gdCLdapFilter::gdCLdapFilter()
  : pServer(0)
{ }

gdCLdapFilter::gdCLdapFilter(gdCLdapFilter* pOld)
{
  if ( pOld ) {
    pServer               = pOld->pServer;
    strInitialSearchBase  = pOld->strInitialSearchBase;
    strSearchBase         = pOld->strSearchBase;
    strDisplayName        = pOld->strDisplayName;
    strFilter             = pOld->strFilter;
    nScope                = pOld->nScope;
  } else {
    pServer = 0;
  }
};

gdCLdapServer::gdCLdapServer()
  : bOIDObjectOnly(true), bHasSubordinates(false), SchemaStatus(SchemaStatus_NotLoaded), m_nErr(0), m_pLd(0), lPort(389)
{ }

gdCLdapServer::~gdCLdapServer()
{
  DisConnect();
}

bool gdCLdapServer::TryServer()
{
  LDAP*                 pLd;
  //  pLd = ldap_init(strServer, lPort);
  pLd = ldap_init(strServer.c_str(), lPort);
  if ( !pLd ) {
    SetError(CLdapErrInit);
    return false;
  }
  pLd = ldap_open(strServer.c_str(), lPort);
  if ( !pLd ) {
    SetError(CLdapErrOpen);
    return false;
  }
  ldap_unbind(pLd);
  SetError(CLdapErrOK);
  return true;
}

bool gdCLdapServer::Connect()
{
  int                   res;
  int                   version = LDAP_VERSION3;
  if ( m_pLd ) return true;
  m_pLd = ldap_init(strServer.c_str(), lPort);
  if ( !m_pLd ) {
    fprintf(stderr, "Cancel initialize ldap\n");
    SetError(CLdapErrInit);
    return false;
  }
  res = ldap_set_option(m_pLd, LDAP_OPT_PROTOCOL_VERSION, &version);
  if ( res != LDAP_OPT_SUCCESS ) {
    m_pLd = 0;
    fprintf(stderr, "%s\n", ldap_err2string(res));
    SetError(CLdapErrSetProto);
    return false;
  }
  res = ldap_bind_s(m_pLd, strLogon.c_str(), strPassword.c_str(), LDAP_AUTH_SIMPLE);
  if ( res != LDAP_SUCCESS ) {
    fprintf(stderr, "%s\n", ldap_err2string(res));
    DisConnect();
    SetError(CLdapErrBind);
    return false;
  }
  return true;
}

void gdCLdapServer::DisConnect()
{
  if ( m_pLd ) {
    ldap_unbind(m_pLd);
    m_pLd = 0;
  }
}

bool gdCLdapServer::GetNamingContexts(gdCLdapEntries& pEntries)
// Start Search = "" et attribut = "namingContexts" retourne touts les StartSearch
// ldapsearch -x -s base  -LLL "objectclass=*" namingContexts
// dn:
// namingContexts: o=theCompany,c=be
{
  gdCLdapFilter             cFilter;
  cFilter.pServer         = this;
  cFilter.nScope          = LDAP_SCOPE_BASE;
  cFilter.strFilter       = "(objectClass=*)";
  gdCLdapAttributes    cQuery;
  cQuery.push_back(new gdCLdapAttribute("namingContexts"));
  gdCLdap cLdap;
  if ( !cLdap.GetInfo(cFilter, cQuery, pEntries) ) return false;
  return true;
}

bool gdCLdapServer::LoadSchema()
{
  if ( SchemaStatus == SchemaStatus_Loaded ) return true;
  int                    ret;
  const char*            pErr;
  gdCLdapEntries         cEntries;
  if ( !GetSubSchema(cEntries) ) return false;
  for (size_t nEntries = 0; nEntries < cEntries.size(); ++nEntries)
    for (size_t nAttrs = 0; nAttrs   < cEntries[nEntries]->Attrs.size(); ++nAttrs) {
      std::string& refAttr = cEntries[nEntries]->Attrs[nAttrs]->Attr;
      for (size_t nVals = 0; nVals < cEntries[nEntries]->Attrs[nAttrs]->Values.size(); nVals++) {
        gdCLdapBerVal* refBerVal = cEntries[nEntries]->Attrs[nAttrs]->Values[nVals];
        std::string& refVal = refBerVal->ToStr();
        if ( boost::iequals("objectClasses", refAttr) ) { // equal
          Schema.arOC.push_back(ldap_str2objectclass(refBerVal->GetVal(), &ret, &pErr, LDAP_SCHEMA_ALLOW_NO_OID | LDAP_SCHEMA_ALLOW_QUOTED));
          continue;
        }
        if ( boost::iequals("attributeTypes", refAttr) ) {
          Schema.arAT.push_back(ldap_str2attributetype(refBerVal->GetVal(), &ret, &pErr, LDAP_SCHEMA_ALLOW_NO_OID | LDAP_SCHEMA_ALLOW_QUOTED));
          continue;
        }
        if ( boost::iequals("matchingRules", refAttr) ) {
          Schema.arMR.push_back(ldap_str2matchingrule(refBerVal->GetVal(), &ret, &pErr, LDAP_SCHEMA_ALLOW_NO_OID | LDAP_SCHEMA_ALLOW_QUOTED));
          continue;
        }
        if ( boost::iequals("ldapSyntaxes", refAttr) ) {
          Schema.arSyntax.push_back(ldap_str2syntax(refBerVal->GetVal(), &ret, &pErr, LDAP_SCHEMA_ALLOW_NO_OID | LDAP_SCHEMA_ALLOW_QUOTED));
          continue;
        }
      }
    }
  SchemaStatus = SchemaStatus_Loaded;
  return true;
}

/*Page*/
bool gdCLdapServer::GetSubSchema(gdCLdapEntries& cEntries)
{
  // OpenLDAP : retourne : objectClasses: (...
  //            Manuel   : ldapsearch -x -s base -b "cn=Subschema" "(objectclass=*)" objectClasses attributeTypes matchingRules ldapSyntaxes
  //                ou   : ldapsearch -x -s base -b "cn=Subschema" "(objectclass=*)" +
  //                ou   : ldapsearch -x -s base -b "cn=Subschema" "(objectclass=*)" * +
  std::string                strSubSchemaSubEntry;
  if ( !GetSubSchemaSubEntry(strSubSchemaSubEntry) ) return false;
  gdCLdapFilter           cFilter;
  cFilter.pServer              = this;
  cFilter.nScope               = LDAP_SCOPE_BASE;
  cFilter.strFilter            = "objectClass=*";
  cFilter.strInitialSearchBase = strSubSchemaSubEntry;
  gdCLdapAttributes       cQuery;
  cQuery.push_back(new gdCLdapAttribute("objectClasses"));
  cQuery.push_back(new gdCLdapAttribute("attributeTypes"));
  cQuery.push_back(new gdCLdapAttribute("matchingRules"));
  cQuery.push_back(new gdCLdapAttribute("ldapSyntaxes"));
  gdCLdap   cLdap;
  return cLdap.GetInfo(cFilter, cQuery, cEntries);
}

/*Page*/
bool gdCLdapServer::GetSubSchemaSubEntry(std::string& strSsSe)
{
  // OpenLDAP : retourne : subschemaSubentry: cn=Subschema
  //            Manuel   : ldapsearch -x -s base "(objectclass=*)" subschemaSubentry
  gdCLdapEntries         cEntries;
  gdCLdapFilter          cFilter;
  cFilter.pServer   = this;
  cFilter.nScope    = LDAP_SCOPE_BASE;
  cFilter.strFilter = "objectClass=*";
  gdCLdapAttributes      cQuery;
  cQuery.push_back(new gdCLdapAttribute("subschemaSubentry"));
  gdCLdap     cLdap;
  if ( !cLdap.GetInfo(cFilter, cQuery, cEntries) ) return false;
  strSsSe = cEntries[0]->Attrs[0]->Values[0]->ToStr();
  return true;
}
bool gdCLdapServer::GetEntry(const std::string& theDN, gdCLdapEntry& pEntry)
{
  // ldapsearch -x -b theDN -s base "(objectClass=*)" "*" "+"
  gdCLdapEntries         cEntries;
  gdCLdapFilter          cFilter;
  cFilter.pServer              = this;
  cFilter.strInitialSearchBase = theDN;
  cFilter.nScope               = LDAP_SCOPE_BASE;
  cFilter.strFilter     = "objectClass=*";
  gdCLdapAttributes      cQuery;
  cQuery.push_back(new gdCLdapAttribute("*"));
  cQuery.push_back(new gdCLdapAttribute("+"));
  gdCLdap     cLdap;
  if ( !cLdap.GetInfo(cFilter, cQuery, cEntries) ) return false;
  pEntry = *cEntries[0];
  return true;
}

bool gdCLdapServer::LoadClassesInfo(gdCLdapEntries& cEntries, bool force)
{
  for (size_t nEntries = 0; nEntries < cEntries.size(); ++nEntries)
    if ( !LoadClassesInfo(*cEntries[nEntries], force) ) return false;
  return true;
}

bool gdCLdapServer::LoadClassesInfo(gdCLdapEntry& cEntry, bool force)
{
  if ( cEntry.KnowAttr )
    if ( !force ) return true;
    else {
      delete cEntry.KnowAttr;
      cEntry.KnowAttr = 0;
    }
  cEntry.KnowAttr = new gdCLdapClassInfo();
  if ( !cEntry.KnowAttr ) return false;
  int ret = 0;
  for (unsigned a = 0; a < cEntry.Attrs.size(); ++a)
    if ( boost::iequals("objectClass", cEntry.Attrs[a]->Attr) )
      for (size_t v = 0; v < cEntry.Attrs[a]->Values.size(); ++v )
        ret = LoadClassInfo(cEntry.Attrs[a]->Values[v]->ToStr(), cEntry);
  if ( !force )
    if (!ret ) {
      delete cEntry.KnowAttr;
      cEntry.KnowAttr = 0;
    }
  return ret ? true : false;
}

bool gdCLdapServer::LoadClassInfo(const std::string& ocName, gdCLdapEntry& cEntry)
{
  LDAPObjectClass*        pObjC  = NULL;
  // Recherche de la classe
  for (unsigned c = 0; c < Schema.arOC.size(); ++c)
    if ( boost::iequals(ocName, Schema.arOC[c]->oc_names[0]) ) {
      pObjC = Schema.arOC[c];
      break;
    }
  if ( !pObjC ) return false;
  gdCLdapClassItemInfo*  cOcInfo;
  if ( pObjC->oc_at_oids_must )
    for (int must = 0; pObjC->oc_at_oids_must[must]; ++must) {
      // verifier si l'attribut existe deja
      bool bMatch = false;
      for (size_t a = 0; a < cEntry.KnowAttr->Must.size(); ++a)
        if ( cEntry.KnowAttr->Must[a]->Attr == pObjC->oc_at_oids_must[must] ) {
          bMatch = true;
          break;
        }
      if ( bMatch ) continue;
      // Sinon on le crée
      cOcInfo = new gdCLdapClassItemInfo();
      cOcInfo->ocOwner = ocName;
      cOcInfo->Attr    = pObjC->oc_at_oids_must[must];
      cEntry.KnowAttr->Must.push_back(cOcInfo);
    }
  if ( pObjC->oc_at_oids_may )
    for (int may = 0; pObjC->oc_at_oids_may[may]; ++may) {
      // verifier si l'attribut existe deja
      bool bMatch = false;
      for (size_t a = 0; a < cEntry.KnowAttr->May.size(); ++a)
        if ( cEntry.KnowAttr->May[a]->Attr == pObjC->oc_at_oids_may[may] ) {
          bMatch = true;
          break;
        }
      if ( bMatch ) continue;
      // Sinon on le crée
      cOcInfo = new gdCLdapClassItemInfo();
      cOcInfo->ocOwner = ocName;
      cOcInfo->Attr    = pObjC->oc_at_oids_may[may];
      cEntry.KnowAttr->May.push_back(cOcInfo);
    }
  // Rechercher oc_sup_oids qui donne les classes parentes
  // Et réentrance de la fonction
  if ( pObjC->oc_sup_oids )
    for (int sup = 0; pObjC->oc_sup_oids[sup]; ++sup)
      LoadClassInfo(pObjC->oc_sup_oids[sup], cEntry);
  return true;
}

bool gdCLdapServer::GetAttributeSyntaxDesc(const std::string& pOID, std::string& pDesc)
{
  LDAPSyntax*        pObjSyntax  = NULL;
  pDesc = "";
  // Recherche de l'OID
  for (unsigned s = 0; s < Schema.arSyntax.size(); ++s) {
    if ( boost::iequals(pOID, Schema.arSyntax[s]->syn_oid) ) {
      pObjSyntax = Schema.arSyntax[s];
      break;
    }
  }
  if ( !pObjSyntax ) return false;
  if ( pObjSyntax->syn_desc )
    pDesc = pObjSyntax->syn_desc;
  return true;
}

bool gdCLdapServer::GetAttribute(const std::string& pAttr, LDAPAttributeType** pObjAttr)
{
  if ( !pObjAttr ) return false;
  *pObjAttr = 0;
  // Recherche de l'attribut
  for (unsigned a = 0; a < Schema.arAT.size(); ++a)
    for ( unsigned i = 0; Schema.arAT[a]->at_names[i]; ++i )
      if ( boost::iequals(pAttr, Schema.arAT[a]->at_names[i]) ) {
        *pObjAttr = Schema.arAT[a];
        return true;
      }
  return false;
}

bool gdCLdapServer::GetAttributeOID(const std::string& pAttr, std::string& pOID, LDAPAttributeType** pObjAttr)
{
  LDAPAttributeType*        pObjAT  = NULL;
  if ( pObjAttr ) pObjAT = *pObjAttr;
  pOID = "";
  if ( !pObjAT )
    if ( ! GetAttribute(pAttr, &pObjAT) ) return false;
  if ( pObjAT->at_syntax_oid ) {
    pOID = pObjAT->at_syntax_oid;
    return true;
  }
  if ( pObjAT->at_sup_oid )
    if ( GetAttributeOID(pObjAT->at_sup_oid, pOID) ) return true;
  return false;
}

bool gdCLdapServer::GetAttributeType(const std::string& pAttr, std::string& pType, LDAPAttributeType** pObjAttr)
{
  std::string           pOID;
  std::string           pNewAttr = pAttr;
  LDAPAttributeType* pObjAT   = NULL;
  if ( pObjAttr ) pObjAT = *pObjAttr;
  pType = "";
  if ( !GetAttributeOID(pNewAttr, pOID, pObjAttr) )
    GetAttributeOID(pAttr, pOID, pObjAttr); // on ne sait jamais :))
  if ( !GetAttributeSyntaxDesc(pOID, pType) ) pType = pOID;
  if ( pType.empty() )
    if ( pObjAT )
      if ( pObjAT->at_usage == LDAP_SCHEMA_DIRECTORY_OPERATION )
        pType = "Operationnal";
  // BUG OPENLDAP entryUUID et entryCSN n'ont pas d'OID
  if ( pType.empty() )
    pType = "Unknown";
  return true;
}

bool gdCLdapServer::HasUseBinary(LDAPAttributeType* pObjAT)
{
  if ( pObjAT ) {
    std::string sDesc(pObjAT->at_desc);
    if ( sDesc.find(";binary") == std::string::npos ) return true;
  }
  return false;
}

bool gdCLdapServer::IsOperationnal(std::string& pAttr)
{
  LDAPAttributeType* pType;
  if ( !GetAttribute(pAttr, &pType) ) return false;
  if ( pType->at_usage == LDAP_SCHEMA_DIRECTORY_OPERATION ) return true;
  return false;
}

bool gdCLdapServer::IsMustAttribute(gdCLdapEntry& cEntry, const std::string& pAttr)
{
  if ( SchemaStatus != SchemaStatus_Loaded )
    if ( !LoadSchema() ) return false;
  if ( !cEntry.KnowAttr )
    if ( !LoadClassesInfo(cEntry) ) return false;
  for (unsigned int a = 0; a < cEntry.KnowAttr->Must.size(); ++a)
    if ( boost::iequals(pAttr, cEntry.KnowAttr->Must[a]->Attr ) ) return true;
  return false;
}

bool gdCLdapServer::GetParentClass(gdCLdapEntry& cEntry, const std::string& pAttr, std::string& pClass)
{
  pClass = "";
  if ( SchemaStatus != SchemaStatus_Loaded )
    if ( !LoadSchema() ) return false;
  if ( !cEntry.KnowAttr )
    if ( !LoadClassesInfo(cEntry) ) return false;
  for (unsigned int a = 0; a < cEntry.KnowAttr->Must.size(); ++a)
    if ( boost::iequals(pAttr, cEntry.KnowAttr->Must[a]->Attr) ) {
      pClass = cEntry.KnowAttr->Must[a]->ocOwner;
      return true;
    }
  for (unsigned int a = 0; a < cEntry.KnowAttr->May.size(); ++a)
    if ( boost::iequals(pAttr, cEntry.KnowAttr->May[a]->Attr) ) {
      pClass = cEntry.KnowAttr->May[a]->ocOwner;
      return true;
    }
  pClass = "Directory Operation";
  return false;
}

bool gdCLdapServer::UpdateAttribute(gdCLdapEntry& cEntry, gdCLdapAttribute& pAttr)
{
  LDAP*                 pLd;
  LDAPMod               Att1;
  LDAPMod*              mods[2];
  berval*               vals[pAttr.Values.size() + 1];
  int                   v;
  int                   v2;
  LDAPAttributeType*    pAt;
  if ( !Connect() ) return false;
  pLd = GetHandle();
  if ( pAttr.bNewAttribute )
    Att1.mod_op   = LDAP_MOD_ADD | LDAP_MOD_BVALUES;
  else
    Att1.mod_op   = LDAP_MOD_REPLACE | LDAP_MOD_BVALUES;
  std::string att = pAttr.Attr;
  GetAttribute(pAttr.Attr, &pAt);
  if ( HasUseBinary(pAt) )
    att += ";binary";
  Att1.mod_type = (char*) att.c_str();
  mods[0] = &Att1;
  mods[1] = NULL;
  for (v = 0; v < pAttr.Values.size() + 1; ++v)
    vals[v] = NULL;
  for (v = v2 = 0; v < pAttr.Values.size(); ++v) {
    if ( !pAttr.Values[v]->GetVal() ) continue;
    berval* val = new berval;
    val->bv_val = pAttr.Values[v]->GetVal();
    val->bv_len = pAttr.Values[v]->GetLenght();
    vals[v2] = val;
    ++v2;
  }
  Att1.mod_bvalues = vals;
  int res = ldap_modify_s(pLd, cEntry.Dn.c_str(), mods);
  if ( res != LDAP_SUCCESS ) {
    fprintf(stderr, "%s\n", ldap_err2string(res));
    SetError(CLdapErrModify);
    return false;
  }
  for (v = 0; v < pAttr.Values.size(); ++v)
    delete vals[v];
  return true;
}

bool gdCLdapServer::UpdateEntry(gdCLdapEntry& cEntry, bool bNewEntry)
{
  LDAP*                 pLd = 0;
  size_t                nbAttr = (int) cEntry.Attrs.size();
  LDAPMod*              Att1 = (LDAPMod*)  malloc(sizeof(LDAPMod)  * nbAttr);
  LDAPMod**             mods = (LDAPMod**)  malloc(sizeof(LDAPMod*) * (nbAttr + 1));
  std::string*          att = new std::string [nbAttr];
  size_t                a, a2;
  size_t                v, v2;
  LDAPAttributeType*    pAt;
  berval**              vals = 0;
  char**                strvals = 0;
  int                   res;
  bool                  bBin = false;
  for (a = a2 = 0; a < nbAttr; ++a) {
    mods[a] = 0;
    gdCLdapAttribute& pAttr = *cEntry.Attrs[a];
    std::string       strType;
    pAt = 0;
    GetAttribute(pAttr.Attr, &pAt);
    GetAttributeType(pAttr.Attr, strType, &pAt);
    if (
      strType == "Certificate" ||
      strType == "JPEG"
    )
      bBin = true;
    else bBin = false;
    att[a] = pAttr.Attr;
    if ( HasUseBinary(pAt) )
      att[a] += ";binary";
    Att1[a].mod_type    = strdup(att[a].c_str());
    Att1[a].mod_op      = 0;
    Att1[a].mod_bvalues = 0;
    Att1[a].mod_values  = 0;
    bool                  bUpd = false;
    if ( bBin ) vals    = (berval**) malloc(sizeof(berval*) * (pAttr.Values.size() + 1));
    else       strvals = (char**)   malloc(sizeof(char*)   * (pAttr.Values.size() + 1));
    for (v = 0; v < pAttr.Values.size(); ++v) {
      if ( pAttr.Values[v]->IsModified() ) bUpd = true;
      if ( bBin ) vals[v]    = 0;
      else       strvals[v] = 0;
    }
    if ( bBin ) vals[v]    = 0;
    else       strvals[v] = 0;
    if ( !bNewEntry && !bUpd ) continue;
    for (v = v2 = 0; v < pAttr.Values.size(); ++v) {
      if ( !pAttr.Values[v]->GetLenght() ) continue;
      if ( bBin ) {
        berval* val = (berval*) malloc(sizeof(berval));
        val->bv_val = pAttr.Values[v]->GetVal();
        val->bv_len = pAttr.Values[v]->GetLenght();
        vals[v2] = val;
      } else
        strvals[v2] = (char*) pAttr.Values[v]->GetVal();
      ++v2;
    }
    if ( !v2 )
      Att1[a].mod_op      = LDAP_MOD_DELETE;
    else {
      pAttr.bNewAttribute ? Att1[a].mod_op = LDAP_MOD_ADD : Att1[a].mod_op = LDAP_MOD_REPLACE;
      if ( bBin ) Att1[a].mod_bvalues = vals;
      else       Att1[a].mod_values  = strvals;
    }
    if ( bBin )
      Att1[a].mod_op |= LDAP_MOD_BVALUES;
    mods[a2] = &Att1[a];
    ++a2;
  }
  mods[a2] = 0;
  fprintf(stderr, "gdCLdapServer::UpdateEntry() : a2 = %ld\n", a2);
  res = LDAP_SUCCESS;
  if ( a2 )
    if ( Connect() ) {
      fprintf(stderr, "gdCLdapServer::UpdateEntry() : Connected\n");
      pLd = GetHandle();
      if ( pLd ) {
        if ( bNewEntry ) {
          res = ldap_add_s(pLd, (const char*) cEntry.Dn.c_str(), mods);
          fprintf(stderr, "ldap_add_s : %s\n", ldap_err2string(res));
        } else {
          res = ldap_modify_s(pLd, (const char*) cEntry.Dn.c_str(), mods);
          fprintf(stderr, "ldap_modify_s : %s\n", ldap_err2string(res));
        }
        if ( res != LDAP_SUCCESS )
          fprintf(stderr, "%s\n", ldap_err2string(res));
      }
    } else
      fprintf(stderr, "Cannot Connect%s\n", ldap_err2string(res));
  for (a = 0; a < nbAttr; ++a) {
    gdCLdapAttribute& pAttr = *cEntry.Attrs[a];
    if ( (Att1[a].mod_op & LDAP_MOD_BVALUES) ) bBin = true;
    else                                      bBin = false;
    if ( res == LDAP_SUCCESS )
      for (v = 0; v < pAttr.Values.size(); ++v)
        if ( pAttr.Values[v]->IsModified() )
          pAttr.Values[v]->DBUpdated();
    if ( bBin )
      if ( Att1[a].mod_bvalues )
        for (v = 0; v < pAttr.Values.size(); ++v)
          if ( Att1[a].mod_bvalues[v] )
            free(Att1[a].mod_bvalues[v]);
    /* ATTENTION  mod_bvalues et mod_values = UNION */
    if ( bBin ) free(Att1[a].mod_bvalues);
    else       free(Att1[a].mod_values);
    free(Att1[a].mod_type);
  }
  delete [] att;
  free(mods);
  free(Att1);
  if ( !a2 ) {
    SetError(CLdapErrNoModification);
    return false;
  }
  if ( res != LDAP_SUCCESS ) {
    SetError(CLdapErrModify);
    return false;
  }
  return true;
}

char* gdCLdap::GetRdn(char* pDn)
{
  /* pDn est obtenu via ldap_get_dn, il faut faire un free de rdn */
  LDAPDN       ava;
  char*         rdn;
  if ( !strlen(pDn) ) return pDn;
  ldap_str2dn(pDn, &ava, LDAP_DN_PRETTY);
  rdn = strdup(ava[0][0][0].la_value.bv_val);
  ldap_dnfree(ava);
  return rdn;
}

/*Page*/
#define PARENT_AUTO      1

char* gdCLdap::GetParentDN(const char* theDN)
{
  /* theDN est obtenu via ldap_get_dn */
  LDAPDN*                     pDn;
  char*                       NewDn;
  int                         iRDN;
#if (PARENT_AUTO == 1)
  /* Version Auto */
  ldap_str2dn(theDN, pDn, LDAP_DN_PRETTY);
  for (iRDN = 0; pDn[ 0 ][ iRDN ]; ++iRDN ) {
    if ( !iRDN ) ldap_rdnfree( pDn[ 0 ][ 0 ] );
    if ( pDn[ 0 ][ iRDN + 1 ] )
      pDn[ 0 ][ iRDN ] = pDn[ 0 ][ iRDN + 1];
    else if ( iRDN > 0 ) pDn[ 0 ][ iRDN ] = NULL;
  }
  ldap_dn2str(*pDn, &NewDn, LDAP_DN_FORMAT_LDAPV3);
#else
  /* Version Manuelle */
  LDAPRDN*                    pRdn;
  LDAPAVA*                    pAva;
  int                         iAVA;
  int                         szNewDn = 0;
  char*                       pProgress;
  //  Calcul de la taille du nouveau DN
  for (iRDN = STARTRDN; pDn[ 0 ][ iRDN ]; ++iRDN ) {
    pRdn = pDn[ 0 ][ iRDN ];
    for (iAVA = 0; pRdn[ 0 ][ iAVA ]; ++iAVA ) {
      if ( iRDN > STARTRDN )
        ++szNewDn;
      pAva = pRdn[ 0 ][ iAVA ];
      szNewDn += pAva->la_attr.bv_len;
      ++szNewDn;
      szNewDn += pAva->la_value.bv_len;
    }
  }
  // Creation de la zone memoire du nouveau DN
  ++szNewDn;
  NewDn = (char*) malloc(szNewDn);
  if ( !NewDn ) return NewDn;
  memset(NewDn, 0, szNewDn);
  pProgress = NewDn;
  // construction de nouveau DN
  for (iRDN = STARTRDN; pDn[ 0 ][ iRDN ]; ++iRDN ) {
    pRdn = pDn[ 0 ][ iRDN ];
    for (iAVA = 0; pRdn[ 0 ][ iAVA ]; ++iAVA ) {
      if ( iRDN > STARTRDN ) {
        *pProgress = ',';
        ++pProgress;
      }
      pAva = pRdn[ 0 ][ iAVA ];
      memcpy(pProgress, pAva->la_attr.bv_val, pAva->la_attr.bv_len);
      pProgress += pAva->la_attr.bv_len;
      *pProgress = '=';
      ++pProgress;
      memcpy(pProgress, pAva->la_value.bv_val, pAva->la_value.bv_len);
      pProgress += pAva->la_value.bv_len;
    }
  }
#endif
  ldap_dnfree(*pDn);
  return NewDn;
}

/*Page*/
bool gdCLdap::CheckHasSubordinatesAttr(gdCLdapFilter* pFilter)
// ldapsearch -LLL -b "your base" -s base "(objectClass=*)" +
{
  if ( pFilter->pServer->bHasSubordinates ) return true; // Is set to false by default
  gdCLdapEntries                   cEntries;
  gdCLdapFilter                    cFilter;
  cFilter.pServer                = pFilter->pServer;
  cFilter.strInitialSearchBase   = pFilter->strInitialSearchBase;
  cFilter.nScope                 = LDAP_SCOPE_BASE;
  cFilter.strFilter              = "(objectClass=*)";
  gdCLdapAttributes   cQuery;
  cQuery.push_back(new gdCLdapAttribute("+"));
  if ( !GetInfo(cFilter, cQuery, cEntries) ) return false;
  for ( int i = 0; i < (int) cEntries.size(); ++i)
    for ( int a = 0; a < (int) cEntries[i]->Attrs.size(); ++a)
      if ( cEntries[i]->Attrs[a]->Attr == "hasSubordinates")
        cEntries[i]->Attrs[a]->Values[0]->ToStr() == "TRUE" ? pFilter->pServer->bHasSubordinates = true :
            pFilter->pServer->bHasSubordinates = false;
  return true;
}

void gdCLdapServer::SetError(int nErr)
{
  m_nErr = nErr;
  if ( nErr ) fprintf(stderr, "set error : %d\n", nErr);
}

void gdCLdap::SetError(int nErr)
{
  m_nErr = nErr;
  if ( m_bDebug && m_nErr ) fprintf(stderr, "gdCLdap::SetError : %d\n", nErr);
}

bool gdCLdap::GetInfo(gdCLdapFilter& cFilter, std::string* spFilter, gdCLdapAttributes& witchAttributes, gdCLdapEntries& cEntries, bool bCountEntries)
{
  LDAP*                 pLd;
  char*                 filter;
  LDAPMessage*          pEntries;
  LDAPMessage*          pSubEntries;
  LDAPMessage*          pEntry;
  char*                 pAttrCountChild[2] = {const_cast<char*>("1.1"), 0}; // Normalement const char* mais ldap_search prend un char*, cela donne un warning a la compilation
  char*                 pAttr;
  BerElement*           pBerEl;
  int                   res;
  int                   cpt;
  char*                 pDN;
  char**                pTmp;
  //  char**                pRes;
  berval**              bv;
  int                   szpQuery;
  gdCLdapEntry*         Entry;
  gdCLdapAttribute*     Attribute;
  gdCLdapAttributes     Attributes;
  LDAPControl**         serverctrls = NULL;
  LDAPControl**         clntctrls   = NULL;
  bool                  HasChildren;
  // Creation du controle manage dsa it
  LDAPControl           ct;
  LDAPControl*          manageDsaIt[2] = { NULL, NULL } ;
  ct.ldctl_oid           = const_cast<char*>(LDAP_CONTROL_MANAGEDSAIT);
  ct.ldctl_value.bv_val  = NULL;
  ct.ldctl_value.bv_len  = 0;
  ct.ldctl_iscritical    = 1;
  manageDsaIt[0] = &ct;
  if ( !cFilter.pServer->Connect() ) {
    if ( m_bDebug ) fprintf(stderr, "gdCLdap::GetInfo : Cannot connect to the server !\n");
    return false;
  }
  pLd = cFilter.pServer->GetHandle();
  // Creation de la table attributs que le serveur doit retourner
  szpQuery = sizeof(char*) * (witchAttributes.size() + 1);
  pTmp = (char**) malloc(szpQuery);
  if ( !pTmp ) {
    SetError(CLdapErrNotEnoughMem);
    if ( m_bDebug ) fprintf(stderr, "gdCLdap::GetInfo : Cannot allow memory for attributes. !\n");
    return false;
  }
  memset(pTmp, 0, szpQuery);
  int nbTmp;
  for (res = 0, nbTmp = 0; res < (int) witchAttributes.size(); ++res) {
    std::string attr = witchAttributes[res]->Attr;
    if ( attr[0] == '_' )
      attr.erase(0, 1);
    Attributes.push_back(new gdCLdapAttribute(attr));
    pTmp[nbTmp++] = strdup(attr.c_str());
  }
  if ( spFilter ) filter = strdup(spFilter->c_str());
  else           filter = strdup(cFilter.strFilter.c_str());
  std::string tempBase = (cFilter.strSearchBase.size() > 0 ? cFilter.strSearchBase : cFilter.strInitialSearchBase);
  res = ldap_search_ext_s(pLd, (const char*) tempBase.c_str(), cFilter.nScope, filter, &pTmp[0], 0,
                          m_bManageDSAIT ? manageDsaIt : serverctrls,
                          clntctrls, NULL, 0, (LDAPMessage**)&pEntries);
  if ( m_bDebug ) fprintf(stderr, "gdCLdap::GetInfo : ldap_search_s, base=<%s> scope=<%d>, filter=<%s>, attrs=", tempBase.c_str(),  cFilter.nScope, filter);
  for (nbTmp = 0; nbTmp < (int) Attributes.size(); ++nbTmp)
    if ( m_bDebug ) fprintf(stderr, "%s ", pTmp[nbTmp]);
  if ( m_bDebug ) fprintf(stderr, "\n");
  free(filter);
  if ( res != LDAP_SUCCESS ) {
    SetError(CLdapErrSearch);
    if ( m_bDebug ) fprintf(stderr, "gdCLdap::GetInfo : Cannot ldap_search_s. !\n");
    return false;
  }
  for (res = 0, nbTmp = 0; res < (int) Attributes.size(); ++res)
    free(pTmp[nbTmp++]);
  free(pTmp);
  pTmp = 0;
  res = ldap_count_entries(pLd, pEntries);
  if ( !res ) {
    ldap_msgfree(pEntries);
    SetError(CLdapErrNoEntries);
    if ( m_bDebug ) fprintf(stderr, "gdCLdap::GetInfo : Cannot count entries. !\n");
    return false;
  }
  cEntries.clear();
  //  cEntries.Alloc(res);
  pEntry = ldap_first_entry(pLd, pEntries);
  while ( pEntry ) {
    pDN =  ldap_get_dn(pLd, pEntry);
    HasChildren = false;
    if ( bCountEntries ) {
      res = ldap_search_ext_s(pLd, pDN, LDAP_SCOPE_ONELEVEL, "objectclass=*", pAttrCountChild, 1,
                              (LDAPControl**) NULL, (LDAPControl**) NULL, NULL, 0, (LDAPMessage**)&pSubEntries);
      if ( res != LDAP_SUCCESS ) {
        free(pDN);
        ldap_msgfree(pEntries);
        SetError(CLdapErrSearch);
        if ( m_bDebug ) fprintf(stderr, "gdCLdap::GetInfo : Cannot ldap_search_s on a entry. !\n");
        return false;
      }
      if ( ldap_count_entries(pLd, pSubEntries) ) HasChildren = true;
      ldap_msgfree(pSubEntries);
    }
    Entry = new gdCLdapEntry();
    Entry->Dn = pDN;
    Entry->HasChildren = HasChildren;
    for (res = 0; res < (int) Attributes.size(); ++res) {
      if ( Attributes[res]->Attr == "dn" ) {
        Attribute = new gdCLdapAttribute(Attributes[res]->Attr);
        Attribute->Values.push_back(new gdCLdapBerVal(Entry->Dn.size(), (const char*) Entry->Dn.c_str()));
        Entry->Attrs.push_back(Attribute);
      }
      if ( Attributes[res]->Attr == "rdn" ) {
        char*   pRdn;
        Attribute = new gdCLdapAttribute(Attributes[res]->Attr);
        Entry->Attrs.push_back(Attribute);
        pRdn = GetRdn(pDN);
        Attribute->Values.push_back(new gdCLdapBerVal(strlen(pRdn), pRdn));
        free(pRdn);
      }
    }
    pAttr = ldap_first_attribute(pLd, pEntry, &pBerEl);
    while ( pAttr ) {
      // Differences possibles du traitement en fonction du nombre d'entrées/attributs
      for (res = 0; res < (int) Attributes.size(); ++res) {
        if ( (Attributes[res]->Attr == pAttr) || (Attributes[res]->Attr == "*") || (Attributes[res]->Attr == "+") ) {
          std::string pAttrNew = pAttr;
          boost::algorithm::replace_all(pAttrNew, ";binary", "");
          Attribute = new gdCLdapAttribute(pAttrNew);
          bv = ldap_get_values_len(pLd, pEntry, pAttr);
          if ( bv ) {
            for (cpt = 0; cpt < ldap_count_values_len(bv); ++cpt) {
              gdCLdapBerVal* curBV = new gdCLdapBerVal(bv[cpt]->bv_len, bv[cpt]->bv_val);
              // Gestion des acl par valeur ex. parsearch overlay
              if ( bv[cpt]->bv_len >= 7 )
                if ( !strncmp(bv[cpt]->bv_val, "acl:ro ", 7) )
                  curBV->setACL(gdCLdapBerVal::aclRO);
              boost::algorithm::replace_first(pAttrNew, "acl:ro ", "");
              boost::algorithm::replace_first(pAttrNew, "acl:rw ", "");
              Attribute->Values.push_back(curBV);
            }
            ldap_value_free_len(bv);
            Entry->Attrs.push_back(Attribute);
          }
          break;
        }
      }
      // Fin # possibles
      /* pAttr appartient à pBerEl : pas de free */
      pAttr = ldap_next_attribute(pLd, pEntry, pBerEl);
    }
    ber_free(pBerEl, 0); // Verifier le paramètres 2
    /* pEntry appartient à pEntries : pas de free */
    //  Preparation de la recherche des parents
    std::string strSearchBase = Entry->Dn;
    if ( !m_bOnQueryLdap && m_bUseParentInfos ) {
      while (1) {
        if ( m_bDebug ) fprintf(stderr, "gdCLdap::GetInfo : Start process for parent(s) attribute(s). !\n");
        size_t found;
        found =  tempBase.find(cFilter.strInitialSearchBase);
        if ( found == std::string::npos ) break;
        if ( m_sParentLimit.size() )
          if ( strSearchBase == m_sParentLimit ) break;
        //          cFilter2.strSearchBase = GetParentDN(Entry->Dn.c_str());
        strSearchBase.erase(0, strSearchBase.find_first_of(",") + 1);
        gdCLdapFilter cFilter2;
        cFilter2.pServer = cFilter.pServer;
        cFilter2.nScope = LDAP_SCOPE_BASE;
        cFilter2.strInitialSearchBase = cFilter.strInitialSearchBase;
        cFilter2.strFilter = "(objectClass=*)";
        cFilter2.strSearchBase = strSearchBase;
        tempBase = strSearchBase;
        gdCLdapAttributes cAttrs;
        for (int nAtt = 0; nAtt < (int) Attributes.size(); ++nAtt) {
          if ( witchAttributes[nAtt]->Attr[0] != '_' ) continue;
          bool bMatch = false;
          for (int nAtt2 = 0; nAtt2 < (int) Entry->Attrs.size(); ++nAtt2)
            if ( Attributes[nAtt]->Attr ==  Entry->Attrs[nAtt2]->Attr ) bMatch = true;
          if ( !bMatch )
            cAttrs.push_back(new gdCLdapAttribute(witchAttributes[nAtt]->Attr));
        }
        if ( cAttrs.size() > 0 ) {
          if ( m_bDebug ) fprintf(stderr, "gdCLdap::GetInfo : Match attribute(s) requiring query on parent(s). !\n");
          gdCLdapEntries cEntrs;
          m_bOnQueryLdap = true;
          bool bRet = GetInfo(cFilter2, cAttrs, cEntrs);
          m_bOnQueryLdap = false;
          for (int i = 0; i < cAttrs.size() ; ++i )
            delete cAttrs[i];
          if ( bRet )
            for (int i = 0; i < cEntrs.size(); ++i)
              for (int i2 = 0; i2 < cEntrs[i]->Attrs.size(); ++i2 ) {
                Attribute = new gdCLdapAttribute(*cEntrs[i]->Attrs[i2]);
                if ( m_bDebug ) fprintf(stderr, "gdCLdap::GetInfo : Return attribute(s) %s. !\n", Attribute->Attr.c_str());
                Attribute->bNotOnEntry = true;
                Entry->Attrs.push_back(Attribute);
              }
        }
      }// end while
    }  // end if
    cEntries.push_back(Entry);
    free(pDN);
    pEntry = ldap_next_entry(pLd, pEntry);
  }
  //  ldap_msgfree(pEntries);
  SetError(CLdapErrOK);
  return true;
}

gdCLdapBerVal::gdCLdapBerVal(size_t len, const char* val)
{
  m_acl = aclRW;
  m_bstr = false;
  m_len = len;
  m_val = 0;
  m_bUpdated = false;
  m_oldval = 0;
  m_oldlen = 0;
  if ( m_len ) {
    m_val = new char[len + 1];
    memcpy(m_val, val, len);
    m_val[len] = 0;
  }
}

gdCLdapBerVal::gdCLdapBerVal(const gdCLdapBerVal& bervalSource)
{
  m_acl                 = bervalSource.m_acl;
  m_len                 = bervalSource.m_len;
  m_val                 = bervalSource.m_val;
  m_oldlen              = bervalSource.m_oldlen;
  m_oldval              = bervalSource.m_oldval;
  m_bstr                = bervalSource.m_bstr;
  m_bUpdated            = bervalSource.m_bUpdated;
  m_str                 = bervalSource.m_str;
}

void gdCLdapBerVal::setACL(int acl)
{
  m_acl = acl;
}

bool gdCLdapBerVal::isModifiable()
{
  if ( m_acl == aclRW ) return true;
  return false;
}

void gdCLdapBerVal::Update(size_t len, const char* val)
{
  if ( !isModifiable() ) return;
  if ( !m_bUpdated ) {
    m_oldval = m_val;
    m_oldlen = m_len;
    m_bUpdated = true;
  } else {
    if ( m_len )
      delete [] m_val;
  }
  if ( m_bstr ) m_str.empty();
  m_bstr = false;
  m_len = len;
  m_val = 0;
  if ( m_len ) {
    m_val = new char[len + 1];
    memcpy(m_val, val, len);
    m_val[len] = 0;
  }
}

void gdCLdapBerVal::CancelUpdate()
{
  if ( !isModifiable() ) return;
  if ( m_bUpdated ) {
    if ( m_len )
      delete [] m_val;
    m_val = m_oldval;
    m_len = m_oldlen;
    m_bUpdated = false;
    m_oldval = 0;
    m_oldlen = 0;
  }
}

std::string& gdCLdapBerVal::ToStr()
{
  if ( m_len ) {
    if ( !m_bstr ) {
      m_str = m_val;
      m_bstr = true;
    }
  }
  return m_str;
}

void gdCLdapBerVal::DBUpdated()
{
  if ( m_oldlen )
    delete [] m_oldval;
  m_bUpdated = false;
  m_oldval = 0;
  m_oldlen = 0;
}

void gdCLdapBerVal::decodeUTF8(char* utf8)
{
  char        cstrVal[2048];
  char*       u2 = cstrVal;
  size_t      szUTF8  = strlen(utf8);
  size_t      szVal   = sizeof(cstrVal);
  iconv_t     hIconv = iconv_open ("ISO-8859-15", "UTF-8");
  if ( hIconv == ((iconv_t) - 1) ) {
    fprintf(stderr, "gdCLdapBerVal::decodeUTF8 : open error !\n");
    return;
  }
  size_t      sz     = iconv(hIconv, &utf8, &szUTF8, &u2, &szVal);
  if ( sz == ((size_t) - 1) ) {
    fprintf(stderr, "gdCLdapBerVal::decodeUTF8 : decode error !\n");
    return;
  }
  sz = sizeof(cstrVal) - szVal;
  iconv_close(hIconv);
  Update(sz, cstrVal);
}

void gdCLdapBerVal::encodeUTF8(char* cstrVal)
{
  char        utf8[2048];
  char*       u2 = utf8;
  size_t      szVal  = strlen(cstrVal);
  size_t      szUTF8 = sizeof(utf8);
  iconv_t     hIconv = iconv_open ("UTF-8", "ISO-8859-15");
  if ( hIconv == ((iconv_t) - 1) ) {
    fprintf(stderr, "gdCLdapBerVal::encodeUTF8 : open error !\n");
    return;
  }
  size_t      sz     = iconv(hIconv, &cstrVal, &szVal, &u2, &szUTF8);
  if ( sz == ((size_t) - 1) ) {
    fprintf(stderr, "gdCLdapBerVal::encodeUTF8 : encode error !\n");
    return;
  }
  sz = sizeof(utf8) - szUTF8;
  iconv_close(hIconv);
  Update(sz, utf8);
}

gdCLdapAttribute::gdCLdapAttribute(const gdCLdapAttribute& attributeSource)
{
  Attr            = attributeSource.Attr;
  Values          = attributeSource.Values;
  bNewAttribute   = attributeSource.bNewAttribute;
  bNotOnEntry     = attributeSource.bNotOnEntry;
}

void gdCLdapAttribute::addValue(const std::string& sVal)
{
  Values.push_back(new gdCLdapBerVal(sVal.size(), (const char*) sVal.c_str()));
}

