/***************************************************************************
 $RCSfile$
                             -------------------
    cvs         : $Id$
    begin       : Tue Nov 25 2003
    copyright   : (C) 2003 by Martin Preuss
    email       : martin@libchipcard.de

 ***************************************************************************
 *                                                                         *
 *   This library is free software; you can redistribute it and/or         *
 *   modify it under the terms of the GNU Lesser General Public            *
 *   License as published by the Free Software Foundation; either          *
 *   version 2.1 of the License, or (at your option) any later version.    *
 *                                                                         *
 *   This library is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU     *
 *   Lesser General Public License for more details.                       *
 *                                                                         *
 *   You should have received a copy of the GNU Lesser General Public      *
 *   License along with this library; if not, write to the Free Software   *
 *   Foundation, Inc., 59 Temple Place, Suite 330, Boston,                 *
 *   MA  02111-1307  USA                                                   *
 *                                                                         *
 ***************************************************************************/



#ifdef HAVE_CONFIG_H
# include <config.h>
#endif


#include "ipcxmlcmd.h"
#include <gwenhywfar/db.h>
#include <gwenhywfar/debug.h>
#include <gwenhywfar/ipc.h>
#include <gwenhywfar/ipcxmlconnlayer.h>
#include <string.h>
#include <assert.h>
#include <time.h>


static unsigned int gwen_ipcxmlcmd_lastname=0;



int GWEN_IPCXMLCmd_IsSignedBy(GWEN_DB_NODE *n, const char *signer) {
  const char *p;
  unsigned int i;

  for (i=0; ; i++) {
    p=GWEN_DB_GetCharValue(n, "security/signers", i, 0);
    if (!p)
      break;
    if (strcasecmp(p, signer)==0) {
      return 1;
    }
  } /* for */
  return 0;
}



GWEN_ERRORCODE GWEN_IPCXMLCmd_Result_SegResult(GWEN_IPCXMLSERVICE *xs,
                                               GWEN_DB_NODE *db,
                                               unsigned int *result) {
  GWEN_DB_NODE *gr;
  const char *p;
  int code;

  assert(xs);
  assert(db);

  gr=GWEN_DB_GetGroup(db,
                      GWEN_DB_FLAGS_DEFAULT |
                      GWEN_PATH_FLAGS_NAMEMUSTEXIST,
                      "SegResult");
  if (!gr) {
    DBG_ERROR(0, "Not a SegResult");
    return GWEN_Error_new(0,
                          GWEN_ERROR_SEVERITY_ERR,
                          GWEN_Error_FindType(GWEN_IPC_ERROR_TYPE),
                          GWEN_IPC_ERROR_INVALID);
  }

  code=GWEN_DB_GetIntValue(gr, "result/resultcode", 0, -1);
  if (code==-1) {
    DBG_ERROR(0, "No result code in segment result !");
    return GWEN_Error_new(0,
                          GWEN_ERROR_SEVERITY_ERR,
                          GWEN_Error_FindType(GWEN_IPC_ERROR_TYPE),
                          GWEN_IPC_ERROR_BAD_DATA);
  }
  *result=code;

  p=GWEN_DB_GetCharValue(gr, "result/text", 0, 0);
  if (!p) {
    DBG_WARN(0, "No result text in segment result");
  }
  else {
    DBG_INFO(0, "Result: %s", p);
  }

  return 0;
}



GWEN_ERRORCODE GWEN_IPCXMLCmd_Response_SegResult(GWEN_IPCXMLSERVICE *xs,
                                                 unsigned int rqid,
                                                 unsigned int flags,
                                                 unsigned int result,
                                                 const char *text,
                                                 const char *param) {
  GWEN_ERRORCODE err;
  GWEN_DB_NODE *rsp;

  assert(xs);
  assert(text);

  /* build response data */
  rsp=GWEN_DB_Group_new("response");

  GWEN_DB_SetIntValue(rsp,
                      GWEN_DB_FLAGS_DEFAULT,
                      "result/resultcode",
                      result);
  GWEN_DB_SetCharValue(rsp, GWEN_DB_FLAGS_DEFAULT,
                       "result/text", text);
  if (param) {
    GWEN_DB_SetCharValue(rsp, GWEN_DB_FLAGS_DEFAULT,
                         "result/param", param);
  }

  err=GWEN_IPCXMLService_AddResponse(xs, rqid, "SegResult", 0,
                                     rsp, flags);
  if (!GWEN_Error_IsOk(err)) {
    DBG_INFO_ERR(0, err);
    GWEN_DB_Group_free(rsp);
    return err;
  }

  GWEN_DB_Group_free(rsp);
  return 0;

}





GWEN_ERRORCODE GWEN_IPCXMLCmd_PreparePubKey(GWEN_IPCXMLSERVICE *xs,
                                            const GWEN_CRYPTKEY *key,
                                            GWEN_DB_NODE *rsp) {
  GWEN_ERRORCODE err;
  GWEN_DB_NODE *keygr;
  unsigned int binsize;
  const void *binval;
  int isSignKey;

  assert(xs);
  assert(key);
  assert(rsp);

  if (strcasecmp(GWEN_CryptKey_GetKeyName(key), "S")==0)
    isSignKey=1;
  else
    isSignKey=0;

  GWEN_DB_SetCharValue(rsp, GWEN_DB_FLAGS_DEFAULT,
                       "keyname/userid",
                       GWEN_CryptKey_GetOwner(key));
  GWEN_DB_SetCharValue(rsp, GWEN_DB_FLAGS_DEFAULT,
                       "keyname/keytype",
                       GWEN_CryptKey_GetKeyName(key));
  GWEN_DB_SetIntValue(rsp, GWEN_DB_FLAGS_DEFAULT,
                      "keyname/keynum",
                      GWEN_CryptKey_GetNumber(key));
  GWEN_DB_SetIntValue(rsp, GWEN_DB_FLAGS_DEFAULT,
                      "keyname/keyversion",
                      GWEN_CryptKey_GetVersion(key));

  keygr=GWEN_DB_Group_new("key");
  /* get public key data */
  err=GWEN_CryptKey_ToDb(key, keygr, 1);
  if (!GWEN_Error_IsOk(err)) {
    DBG_INFO_ERR(0, err);
    GWEN_DB_Group_free(keygr);
    return GWEN_Error_new(0,
                          GWEN_ERROR_SEVERITY_ERR,
                          GWEN_Error_FindType(GWEN_IPC_ERROR_TYPE),
                          GWEN_IPC_ERROR_GENERIC);
  }

  binval=GWEN_DB_GetBinValue(keygr,
                             "data/n", 0, 0, 0, &binsize);
  if (!binval) {
    DBG_ERROR(0, "No modulus in key");
    GWEN_DB_Group_free(keygr);
    return GWEN_Error_new(0,
                          GWEN_ERROR_SEVERITY_ERR,
                          GWEN_Error_FindType(GWEN_IPC_ERROR_TYPE),
                          GWEN_IPC_ERROR_BAD_DATA);
  }
  GWEN_DB_SetBinValue(rsp,
                      GWEN_DB_FLAGS_DEFAULT,
                      "key/modulus", binval, binsize);

  binval=GWEN_DB_GetBinValue(keygr,
                             "data/e", 0, 0, 0, &binsize);
  if (!binval) {
    DBG_ERROR(0, "No exponent in key");
    GWEN_DB_Group_free(keygr);
    return GWEN_Error_new(0,
                          GWEN_ERROR_SEVERITY_ERR,
                          GWEN_Error_FindType(GWEN_IPC_ERROR_TYPE),
                          GWEN_IPC_ERROR_BAD_DATA);
  }
  GWEN_DB_SetBinValue(rsp,
                      GWEN_DB_FLAGS_DEFAULT,
                      "key/exponent", binval, binsize);
  GWEN_DB_SetIntValue(rsp, GWEN_DB_FLAGS_DEFAULT,
                      "key/purpose",
                      isSignKey?5:6);
  GWEN_DB_Group_free(keygr);

  return 0;
}








/*___________________________________________________________________________
 *AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
 *                                Get Key
 *YYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYY
 */



unsigned int GWEN_IPCXMLCmd_Request_GetPubKey(GWEN_IPCXMLSERVICE *xs,
                                              unsigned int clid,
                                              unsigned int flags,
                                              const GWEN_KEYSPEC *ks) {
  GWEN_DB_NODE *cfg;
  unsigned int rqid;

  cfg=GWEN_DB_Group_new("request");

  GWEN_DB_SetCharValue(cfg, GWEN_DB_FLAGS_DEFAULT,
                       "key/bankcode",
                       GWEN_IPCXMLService_GetServiceCode(xs, clid));
  GWEN_DB_SetCharValue(cfg, GWEN_DB_FLAGS_DEFAULT,
                       "key/userid",
                       GWEN_KeySpec_GetOwner(ks));
  GWEN_DB_SetCharValue(cfg, GWEN_DB_FLAGS_DEFAULT,
                       "key/keytype",
                       GWEN_KeySpec_GetKeyName(ks));
  GWEN_DB_SetIntValue(cfg, GWEN_DB_FLAGS_DEFAULT,
                      "key/keynum",
                      GWEN_KeySpec_GetNumber(ks));
  GWEN_DB_SetIntValue(cfg, GWEN_DB_FLAGS_DEFAULT,
                      "key/keyversion",
                      GWEN_KeySpec_GetVersion(ks));
  rqid=GWEN_IPCXMLService_AddRequest(xs, clid, "GetKey", 0, cfg, flags);
  if (!rqid) {
    DBG_INFO(0, "here");
  }
  return rqid;
}



GWEN_ERRORCODE GWEN_IPCXMLCmd_Handle_GetPubKey(GWEN_IPCXMLSERVICE *xs,
                                               unsigned int rqid,
                                               unsigned int flags,
                                               GWEN_DB_NODE *n) {
  GWEN_ERRORCODE err;
  GWEN_KEYSPEC *ks;
  const GWEN_CRYPTKEY *key;
  const char *s;
  unsigned int connid;
  GWEN_DB_NODE *rsp;
  int wantSignKey;

  assert(xs);
  assert(n);

  connid=GWEN_IPCXMLService_GetRequestConnection(xs, rqid);
  if (!connid) {
    DBG_INFO(0, "Could not get connection for request %d", rqid);
    return GWEN_Error_new(0,
                          GWEN_ERROR_SEVERITY_ERR,
                          GWEN_Error_FindType(GWEN_IPC_ERROR_TYPE),
                          GWEN_IPC_ERROR_REQUEST_NOT_FOUND);
  }
  DBG_INFO(0, "Connection id is %d", connid);

  ks=GWEN_KeySpec_new();

  GWEN_KeySpec_SetOwner(ks, GWEN_DB_GetCharValue(n, "key/userid", 0, ""));
  GWEN_KeySpec_SetNumber(ks, GWEN_DB_GetIntValue(n, "key/keynum", 0, 0));
  GWEN_KeySpec_SetKeyName(ks, GWEN_DB_GetCharValue(n, "key/keytype", 0, ""));
  GWEN_KeySpec_SetVersion(ks,
                          GWEN_DB_GetIntValue(n, "key/keyversion", 0, 0));
  s=GWEN_DB_GetCharValue(n, "key/keytype", 0, 0);
  if (!s) {
    DBG_ERROR(0, "no keytype");
    GWEN_KeySpec_free(ks);
    return GWEN_Error_new(0,
                          GWEN_ERROR_SEVERITY_ERR,
                          GWEN_Error_FindType(GWEN_IPC_ERROR_TYPE),
                          GWEN_IPC_ERROR_BAD_DATA);
  }
  GWEN_KeySpec_SetKeyName(ks, s);

  if (strcasecmp(GWEN_KeySpec_GetKeyName(ks), "S")==0) {
    wantSignKey=1;
    key=GWEN_IPCXMLService_GetSignKey(xs, connid);
  }
  else if (strcasecmp(GWEN_KeySpec_GetKeyName(ks), "V")==0) {
    wantSignKey=0;
    key=GWEN_IPCXMLService_GetCryptKey(xs, connid);
  }
  else {
    DBG_ERROR(0, "bad keytype");
    GWEN_KeySpec_free(ks);
    return GWEN_Error_new(0,
                          GWEN_ERROR_SEVERITY_ERR,
                          GWEN_Error_FindType(GWEN_IPC_ERROR_TYPE),
                          GWEN_IPC_ERROR_BAD_DATA);
  }

  if (!key) {
    DBG_ERROR(0, "No key");
    return GWEN_Error_new(0,
                          GWEN_ERROR_SEVERITY_ERR,
                          GWEN_Error_FindType(GWEN_IPC_ERROR_TYPE),
                          GWEN_IPC_ERROR_GENERIC);
  }
  /* TODO: check key */

  /* build response data */
  rsp=GWEN_DB_Group_new("response");

  GWEN_DB_SetIntValue(rsp,
                      GWEN_DB_FLAGS_DEFAULT,
                      "msgnum",
                      GWEN_DB_GetIntValue(n, "security/msgnum", 0, 0));
  GWEN_DB_SetCharValue(rsp,
                       GWEN_DB_FLAGS_DEFAULT,
                       "exchangecontrol",
                       GWEN_DB_GetCharValue(n, "security/dialogid", 0, "0"));
  GWEN_DB_SetCharValue(rsp, GWEN_DB_FLAGS_DEFAULT,
                       "keyname/bankcode",
                       GWEN_IPCXMLService_GetServiceCode(xs, connid));
  err=GWEN_IPCXMLCmd_PreparePubKey(xs, key, rsp);
  GWEN_KeySpec_free(ks);
  if (!GWEN_Error_IsOk(err)) {
    DBG_INFO_ERR(0, err);
    GWEN_DB_Group_free(rsp);
    return err;
  }

  err=GWEN_IPCXMLService_AddResponse(xs, rqid, "GetKeyResponse", 0,
                                     rsp, flags);
  if (!GWEN_Error_IsOk(err)) {
    DBG_INFO_ERR(0, err);
    GWEN_DB_Group_free(rsp);
    return err;
  }

  GWEN_DB_Group_free(rsp);
  return 0;
}



GWEN_ERRORCODE GWEN_IPCXMLCmd_Result_GetPubKey(GWEN_IPCXMLSERVICE *xs,
                                               unsigned int rqid,
                                               GWEN_CRYPTKEY **pkey) {
  GWEN_DB_NODE *n;
  GWEN_DB_NODE *keygr;
  GWEN_CRYPTKEY *key;
  const void *binval;
  unsigned int binsize;

  n=GWEN_IPCXMLService_GetResponseData(xs, rqid);
  if (!n) {
    return GWEN_Error_new(0,
                          GWEN_ERROR_SEVERITY_ERR,
                          GWEN_Error_FindType(GWEN_IPC_ERROR_TYPE),
                          GWEN_IPC_ERROR_INQUEUE_EMPTY);
  }

  if (strcasecmp(GWEN_DB_GroupName(n), "SegResult")==0) {
    unsigned int rc;
    GWEN_ERRORCODE err;

    err=GWEN_IPCXMLCmd_Result_SegResult(xs, n, &rc);
    if (!GWEN_Error_IsOk(err)) {
      DBG_INFO_ERR(0, err);
    }
    else {
      DBG_INFO(0, "Got an error: %d", rc);
      return GWEN_Error_new(0,
                            GWEN_ERROR_SEVERITY_ERR,
                            GWEN_Error_FindType(GWEN_IPC_ERROR_TYPE),
                            GWEN_IPC_ERROR_GENERIC);
    }
  }

  if (strcasecmp(GWEN_DB_GroupName(n), "GetKeyResponse")!=0) {
    DBG_INFO(0, "Not a GetkeyResponse");
    return GWEN_Error_new(0,
                          GWEN_ERROR_SEVERITY_ERR,
                          GWEN_Error_FindType(GWEN_IPC_ERROR_TYPE),
                          GWEN_IPC_ERROR_UNKNOWN_MSG);
  }

  keygr=GWEN_DB_Group_new("key");

  GWEN_DB_SetCharValue(keygr,
                       GWEN_DB_FLAGS_DEFAULT,
                       "type", "RSA");
  GWEN_DB_SetCharValue(keygr,
                       GWEN_DB_FLAGS_DEFAULT,
                       "name",
                       GWEN_DB_GetCharValue(n,
                                            "GetKeyResponse/keyname/keytype",
                                            0, ""));

  GWEN_DB_SetCharValue(keygr,
                       GWEN_DB_FLAGS_DEFAULT,
                       "owner",
                       GWEN_DB_GetCharValue(n,
                                            "GetKeyResponse/keyname/userid",
                                            0, ""));

  GWEN_DB_SetIntValue(keygr,
                      GWEN_DB_FLAGS_DEFAULT,
                      "number",
                      GWEN_DB_GetIntValue(n,
                                          "GetKeyResponse/keyname/keynum",
                                          0, 0));
  GWEN_DB_SetIntValue(keygr,
                      GWEN_DB_FLAGS_DEFAULT,
                      "version",
                      GWEN_DB_GetIntValue(n,
                                          "GetKeyResponse/keyname/keyversion",
                                          0, 0));
  GWEN_DB_SetIntValue(keygr,
                      GWEN_DB_FLAGS_DEFAULT,
                      "data/public", 1);

  binval=GWEN_DB_GetBinValue(n, "GetKeyResponse/key/modulus", 0,
                             0, 0, &binsize);
  if (!binval) {
    DBG_ERROR(0, "No modulus in key");
    GWEN_DB_Group_free(keygr);
    GWEN_DB_Group_free(n);
    return GWEN_Error_new(0,
                          GWEN_ERROR_SEVERITY_ERR,
                          GWEN_Error_FindType(GWEN_IPC_ERROR_TYPE),
                          GWEN_IPC_ERROR_BAD_DATA);
  }
  GWEN_DB_SetBinValue(keygr,
                      GWEN_DB_FLAGS_DEFAULT,
                      "data/n", binval, binsize);

  binval=GWEN_DB_GetBinValue(n, "GetKeyResponse/key/exponent", 0,
                             0, 0, &binsize);
  if (!binval) {
    DBG_ERROR(0, "No exponent in key");
    GWEN_DB_Group_free(keygr);
    GWEN_DB_Group_free(n);
    return GWEN_Error_new(0,
                          GWEN_ERROR_SEVERITY_ERR,
                          GWEN_Error_FindType(GWEN_IPC_ERROR_TYPE),
                          GWEN_IPC_ERROR_BAD_DATA);
  }
  GWEN_DB_SetBinValue(keygr,
                      GWEN_DB_FLAGS_DEFAULT,
                      "data/e", binval, binsize);

  key=GWEN_CryptKey_FromDb(keygr);
  if (!key) {
    DBG_INFO(0, "here");
    GWEN_DB_Group_free(keygr);
    GWEN_DB_Group_free(n);
    return GWEN_Error_new(0,
                          GWEN_ERROR_SEVERITY_ERR,
                          GWEN_Error_FindType(GWEN_IPC_ERROR_TYPE),
                          GWEN_IPC_ERROR_GENERIC);
  }
  GWEN_DB_Group_free(keygr);

  GWEN_DB_Group_free(n);
  *pkey=key;
  return 0;
}




/*___________________________________________________________________________
 *AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
 *                                Send Key
 *YYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYY
 */


unsigned int GWEN_IPCXMLCmd_Request_SendPubKey(GWEN_IPCXMLSERVICE *xs,
                                               unsigned int connid,
                                               unsigned int flags,
                                               const GWEN_CRYPTKEY *key) {
  GWEN_ERRORCODE err;
  GWEN_DB_NODE *db;
  int isSignKey;
  unsigned int rqid;

  assert(xs);
  assert(key);

  if (strcasecmp(GWEN_CryptKey_GetKeyName(key), "S")==0) {
    isSignKey=1;
  }
  else if (strcasecmp(GWEN_CryptKey_GetKeyName(key), "V")==0) {
    isSignKey=0;
  }
  else {
    DBG_ERROR(0, "bad keytype (%s)", GWEN_CryptKey_GetKeyName(key));
    return 0;
  }

  /* TODO: check key */

  /* build request data */
  db=GWEN_DB_Group_new("request");

  GWEN_DB_SetCharValue(db, GWEN_DB_FLAGS_DEFAULT,
                       "keyname/bankcode",
                       GWEN_IPCXMLService_GetServiceCode(xs, connid));
  err=GWEN_IPCXMLCmd_PreparePubKey(xs, key, db);
  if (!GWEN_Error_IsOk(err)) {
    DBG_INFO_ERR(0, err);
    GWEN_DB_Group_free(db);
    return 0;
  }

  rqid=GWEN_IPCXMLService_AddRequest(xs, connid,
                                     "SendKey", 0,
                                     db, flags);
  if (!rqid) {
    DBG_INFO(0, "here");
    GWEN_DB_Group_free(db);
    return 0;
  }

  GWEN_DB_Group_free(db);
  return rqid;
}



GWEN_ERRORCODE GWEN_IPCXMLCmd_Handle_SendPubKey(GWEN_IPCXMLSERVICE *xs,
                                                unsigned int rqid,
                                                unsigned int flags,
                                                GWEN_DB_NODE *n) {
  GWEN_ERRORCODE err;
  unsigned int connid;
  int isSignKey;
  GWEN_DB_NODE *keygr;
  GWEN_CRYPTKEY *key;
  const void *binval;
  unsigned int binsize;
  GWEN_SECCTX *ctx;

  assert(xs);
  assert(n);

  connid=GWEN_IPCXMLService_GetRequestConnection(xs, rqid);
  if (!connid) {
    DBG_INFO(0, "Could not get connection for request %d", rqid);
    return GWEN_Error_new(0,
                          GWEN_ERROR_SEVERITY_ERR,
                          GWEN_Error_FindType(GWEN_IPC_ERROR_TYPE),
                          GWEN_IPC_ERROR_REQUEST_NOT_FOUND);
  }
  DBG_INFO(0, "Connection id is %d", connid);

  /* get key */
  keygr=GWEN_DB_Group_new("key");
  GWEN_DB_SetCharValue(keygr,
                       GWEN_DB_FLAGS_DEFAULT,
                       "type", "RSA");
  GWEN_DB_SetCharValue(keygr,
                       GWEN_DB_FLAGS_DEFAULT,
                       "name",
                       GWEN_DB_GetCharValue(n, "sendkey/keyname/keytype", 0,
                                            ""));

  GWEN_DB_SetCharValue(keygr,
                       GWEN_DB_FLAGS_DEFAULT,
                       "owner",
                       GWEN_DB_GetCharValue(n, "sendkey/keyname/userid", 0,
                                            ""));

  GWEN_DB_SetIntValue(keygr,
                      GWEN_DB_FLAGS_DEFAULT,
                      "number",
                      GWEN_DB_GetIntValue(n, "sendkey/keyname/keynum", 0,
                                          0));
  GWEN_DB_SetIntValue(keygr,
                      GWEN_DB_FLAGS_DEFAULT,
                      "version",
                      GWEN_DB_GetIntValue(n, "sendkey/keyname/keyversion", 0,
                                          0));
  GWEN_DB_SetIntValue(keygr,
                      GWEN_DB_FLAGS_DEFAULT,
                      "data/public", 1);

  binval=GWEN_DB_GetBinValue(n, "sendkey/key/modulus", 0, 0, 0, &binsize);
  if (!binval) {
    DBG_ERROR(0, "No modulus in key");
    GWEN_DB_Group_free(keygr);
    GWEN_IPCXMLCmd_Response_SegResult(xs, rqid, flags,
                                      9000,
                                      "Bad key data",
                                      0);
    return GWEN_Error_new(0,
                          GWEN_ERROR_SEVERITY_ERR,
                          GWEN_Error_FindType(GWEN_IPC_ERROR_TYPE),
                          GWEN_IPC_ERROR_BAD_DATA);
  }
  GWEN_DB_SetBinValue(keygr,
                      GWEN_DB_FLAGS_DEFAULT,
                      "data/n", binval, binsize);

  binval=GWEN_DB_GetBinValue(n, "sendkey/key/exponent", 0, 0, 0, &binsize);
  if (!binval) {
    DBG_ERROR(0, "No exponent in key");
    GWEN_DB_Group_free(keygr);
    GWEN_IPCXMLCmd_Response_SegResult(xs, rqid, flags,
                                      9000,
                                      "Bad key data",
                                      0);
    return GWEN_Error_new(0,
                          GWEN_ERROR_SEVERITY_ERR,
                          GWEN_Error_FindType(GWEN_IPC_ERROR_TYPE),
                          GWEN_IPC_ERROR_BAD_DATA);
  }
  GWEN_DB_SetBinValue(keygr,
                      GWEN_DB_FLAGS_DEFAULT,
                      "data/e", binval, binsize);

  key=GWEN_CryptKey_FromDb(keygr);
  if (!key) {
    DBG_INFO(0, "here");
    GWEN_DB_Group_free(keygr);
    return GWEN_Error_new(0,
                          GWEN_ERROR_SEVERITY_ERR,
                          GWEN_Error_FindType(GWEN_IPC_ERROR_TYPE),
                          GWEN_IPC_ERROR_GENERIC);
  }
  GWEN_DB_Group_free(keygr);


  if (strcasecmp(GWEN_CryptKey_GetKeyName(key), "S")==0) {
    isSignKey=1;
  }
  else if (strcasecmp(GWEN_CryptKey_GetKeyName(key), "V")==0) {
    isSignKey=0;
  }
  else {
    DBG_ERROR(0, "bad keytype");
    GWEN_CryptKey_free(key);
    GWEN_IPCXMLCmd_Response_SegResult(xs, rqid, flags,
                                      9000,
                                      "Bad key data",
                                      0);
    return GWEN_Error_new(0,
                          GWEN_ERROR_SEVERITY_ERR,
                          GWEN_Error_FindType(GWEN_IPC_ERROR_TYPE),
                          GWEN_IPC_ERROR_BAD_DATA);
  }

  /* get context */
  DBG_INFO(0, "Looking for context %s (%d)",
           GWEN_CryptKey_GetOwner(key), connid);
  err=GWEN_IPCXMLService_GetContext(xs, connid,
                                    GWEN_CryptKey_GetOwner(key),
                                    &ctx);
  if (!GWEN_Error_IsOk(err)) {
    DBG_INFO_ERR(0, err);
    getchar();
    GWEN_CryptKey_free(key);
    GWEN_IPCXMLCmd_Response_SegResult(xs, rqid, flags,
                                      9000,
                                      "Unknown context", 0);
    return err;
  }

  /* set key if possible */
  if (!(GWEN_SecContext_GetFlags(ctx) & GWEN_SECCTX_FLAGS_TEMP)) {
    /* only change keys on temporary context */
    DBG_ERROR(0,
              "Will not change keys on non-temporary context (%s)",
              GWEN_CryptKey_GetOwner(key));
    GWEN_IPCXMLService_ReleaseContext(xs, connid, ctx, 1);
    GWEN_CryptKey_free(key);
    GWEN_IPCXMLCmd_Response_SegResult(xs, rqid, flags,
                                      9000,
                                      "Non-temporary context", 0);
    return GWEN_Error_new(0,
                          GWEN_ERROR_SEVERITY_ERR,
                          GWEN_Error_FindType(GWEN_IPC_ERROR_TYPE),
                          GWEN_IPC_ERROR_GENERIC);
  }
  else {
    const GWEN_CRYPTKEY *oldKey;
    if (isSignKey)
      oldKey=GWEN_IPCXMLSecCtx_GetRemoteSignKey(ctx);
    else
      oldKey=GWEN_IPCXMLSecCtx_GetRemoteCryptKey(ctx);

    if (oldKey &&
        !(GWEN_SecContext_GetFlags(ctx)&GWEN_SECCTX_FLAGS_ALLOW_KEY_CHANGE)){
      /* only change keys on temporary context */
      DBG_ERROR(0, "Will not change existing keys");
      GWEN_IPCXMLService_ReleaseContext(xs, connid, ctx, 1);
      GWEN_CryptKey_free(key);
      GWEN_IPCXMLCmd_Response_SegResult(xs, rqid, flags,
                                        9000,
                                        "Keys already exist", 0);
      return GWEN_Error_new(0,
                            GWEN_ERROR_SEVERITY_ERR,
                            GWEN_Error_FindType(GWEN_IPC_ERROR_TYPE),
                            GWEN_IPC_ERROR_GENERIC);
    }
    if (isSignKey)
      GWEN_IPCXMLSecCtx_SetRemoteSignKey(ctx, key);
    else
      GWEN_IPCXMLSecCtx_SetRemoteCryptKey(ctx, key);
  }

  err=GWEN_IPCXMLService_ReleaseContext(xs, connid, ctx, 0);
  if (!GWEN_Error_IsOk(err)) {
    DBG_WARN(0, "Internal error");
    DBG_INFO_ERR(0, err);
    GWEN_IPCXMLCmd_Response_SegResult(xs, rqid, flags,
                                      9000,
                                      "Key already exists", 0);
    return err;
  }

  err=GWEN_IPCXMLCmd_Response_SegResult(xs, rqid, flags,
                                        10,
                                        "Key accepted", 0);
  if (!GWEN_Error_IsOk(err)) {
    DBG_INFO_ERR(0, err);
    return err;
  }
  DBG_INFO(0, "New key in use");
  return 0;
}






GWEN_ERRORCODE GWEN_IPCXMLCmd_Result_SendPubKey(GWEN_IPCXMLSERVICE *xs,
                                                unsigned int rqid,
                                                unsigned int *result){
  GWEN_DB_NODE *db;
  GWEN_ERRORCODE err;

  db=GWEN_IPCXMLService_GetResponseData(xs, rqid);
  if (!db) {
    return GWEN_Error_new(0,
                          GWEN_ERROR_SEVERITY_WARN,
                          GWEN_Error_FindType(GWEN_IPC_ERROR_TYPE),
                          GWEN_IPC_ERROR_INQUEUE_EMPTY);
  }

  err=GWEN_IPCXMLCmd_Result_SegResult(xs, db, result);
  if (!GWEN_Error_IsOk(err)) {
    DBG_INFO_ERR(0, err);
    GWEN_DB_Group_free(db);
    return err;
  }

  GWEN_DB_Group_free(db);
  return 0;
}





/*___________________________________________________________________________
 *AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
 *                                Open Session
 *YYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYY
 */



unsigned int GWEN_IPCXMLCmd_Request_OpenSession(GWEN_IPCXMLSERVICE *xs,
                                                unsigned int clid,
                                                const char *localName,
                                                unsigned int flags){
  GWEN_DB_NODE *cfg;
  unsigned int rqid;
  unsigned int sflags;

  cfg=GWEN_DB_Group_new("request");

  /* turn on encryption */
  sflags=GWEN_IPCXMLService_GetSecurityFlags(xs, clid);
  if (strcasecmp(localName, "anonymous")==0) {
    sflags&=~GWEN_HBCIMSG_FLAGS_CRYPT;
    sflags&=~GWEN_HBCIMSG_FLAGS_SIGN;
  }
  else
    sflags|=GWEN_HBCIMSG_FLAGS_CRYPT;

  GWEN_IPCXMLService_SetSecurityFlags(xs, clid, sflags);

  if (!localName) {
    localName=GWEN_IPCXMLService_GetLocalName(xs, clid);
    if (!localName) {
      DBG_ERROR(0, "No local name");
      return 0;
    }
  }
  GWEN_DB_SetCharValue(cfg, GWEN_DB_FLAGS_DEFAULT, "id", localName);
  rqid=GWEN_IPCXMLService_AddRequest(xs, clid, "OpenSession", 0, cfg, flags);
  if (!rqid) {
    DBG_INFO(0, "here");
  }
  return rqid;
}



GWEN_ERRORCODE GWEN_IPCXMLCmd_Handle_OpenSession(GWEN_IPCXMLSERVICE *xs,
                                                 unsigned int rqid,
                                                 unsigned int flags,
                                                 GWEN_DB_NODE *n) {
  GWEN_ERRORCODE err;
  unsigned int connid;
  GWEN_DB_NODE *rsp;
  GWEN_DB_NODE *gr;
  const char *p;
  GWEN_SECCTX *sc;
  unsigned int cflags;
  int ctxadded;
  int anon;
  unsigned int sid;
  char buffer[32];
  const char *peerName;
  assert(xs);
  assert(n);

  ctxadded=0;
  anon=0;
  connid=GWEN_IPCXMLService_GetRequestConnection(xs, rqid);
  if (!connid) {
    DBG_INFO(0, "Could not get connection for request %d", rqid);
    return GWEN_Error_new(0,
                          GWEN_ERROR_SEVERITY_ERR,
                          GWEN_Error_FindType(GWEN_IPC_ERROR_TYPE),
                          GWEN_IPC_ERROR_REQUEST_NOT_FOUND);
  }
  DBG_INFO(0, "Connection id is %d", connid);

  cflags=GWEN_IPCXMLService_GetConnectionFlags(xs, connid);
  cflags&=~GWEN_IPCXMLCONNLAYER_FLAGS_VERIFIED;
  if (cflags & GWEN_IPCXMLCONNLAYER_FLAGS_SESSION_OPEN) {
    DBG_ERROR(0, "Session already IS open");
    /* TODO: Maybe close connection diirectly */
    GWEN_IPCXMLCmd_Response_SegResult(xs, rqid, flags,
                                      9000,
                                      "Session already open", 0);
    return GWEN_Error_new(0,
                          GWEN_ERROR_SEVERITY_ERR,
                          GWEN_Error_FindType(GWEN_IPC_ERROR_TYPE),
                          GWEN_IPC_ERROR_INVALID);
  }

  GWEN_IPCXMLService_SetConnectionFlags(xs, connid, cflags);

  gr=GWEN_DB_GetGroup(n,
                      GWEN_DB_FLAGS_DEFAULT |
                      GWEN_PATH_FLAGS_NAMEMUSTEXIST,
                      "OpenSession");
  if (!gr) {
    DBG_ERROR(0, "OpenSession not found");
    GWEN_DB_Dump(n, stderr, 2);
    return GWEN_Error_new(0,
                          GWEN_ERROR_SEVERITY_ERR,
                          GWEN_Error_FindType(GWEN_IPC_ERROR_TYPE),
                          GWEN_IPC_ERROR_UNKNOWN_MSG);
  }

  peerName=GWEN_DB_GetCharValue(gr, "id", 0, 0);
  if (!peerName) {
    DBG_ERROR(0, "No id given");
    /* Send error message */
    GWEN_IPCXMLCmd_Response_SegResult(xs, rqid, flags,
                                      9000,
                                      "Syntax error (no id)", 0);
    return GWEN_Error_new(0,
                          GWEN_ERROR_SEVERITY_ERR,
                          GWEN_Error_FindType(GWEN_IPC_ERROR_TYPE),
                          GWEN_IPC_ERROR_BAD_DATA);
  }

  /* check for encryption (only if non-anonymous) */
  if (strcasecmp(peerName, "anonymous")!=0) {
    p=GWEN_DB_GetCharValue(n, "security/crypter", 0, 0);
    if (!p) {
      DBG_ERROR(0, "Message is not encrypted (it should be)");
      GWEN_IPCXMLCmd_Response_SegResult(xs, rqid, flags,
                                        9000,
                                        "Message not encrypted", 0);
      return GWEN_Error_new(0,
                            GWEN_ERROR_SEVERITY_ERR,
                            GWEN_Error_FindType(GWEN_IPC_ERROR_TYPE),
                            GWEN_IPC_ERROR_BAD_MSG);
    }
  }

  /* check for anonymous access */
  if (strcasecmp(peerName, "anonymous")==0) {
    time_t t;

    t=time(0);
    /* anonymous access, create name for peer */
    sprintf(buffer, "anon%08x-%08x",
            ++gwen_ipcxmlcmd_lastname, (unsigned int)t);
    peerName=buffer;
    DBG_INFO(0, "Created id \"%s\" for anonymous peer", peerName);
    anon=1;
  }

  sid=GWEN_IPCXMLService_AddSession(xs,
                                    GWEN_IPCXMLService_GetLocalName(xs,
                                                                    connid),
                                    peerName);
  if (!sid){
    DBG_WARN(0, "Session for id \"%s\" already in use", peerName);
    GWEN_IPCXMLCmd_Response_SegResult(xs, rqid, flags,
                                      9000,
                                      "Id already in use", 0);
    return GWEN_Error_new(0,
                          GWEN_ERROR_SEVERITY_ERR,
                          GWEN_Error_FindType(GWEN_IPC_ERROR_TYPE),
                          GWEN_IPC_ERROR_GENERIC);
  }
  else {
    DBG_NOTICE(0, "Added session for id \"%s\"", peerName);
  }

  sc=0;
  err=GWEN_IPCXMLService_GetContext(xs, connid, peerName, &sc);
  if (!GWEN_Error_IsOk(err)) {
    if (cflags & GWEN_IPCXMLCONNLAYER_FLAGS_ALLOW_ADDCTX) {
      DBG_NOTICE(0, "Context %s not found, will add it temporarily",
                 peerName);
      sc=GWEN_IPCXMLSecCtx_new(GWEN_IPCXMLService_GetLocalName(xs, connid),
                               peerName);
      GWEN_SecContext_SetFlags(sc, GWEN_SECCTX_FLAGS_TEMP);
      err=GWEN_IPCXMLService_AddContext(xs, connid, sc);
      if (!GWEN_Error_IsOk(err)) {
        DBG_INFO_ERR(0, err);
        /* Send error message */
        GWEN_IPCXMLCmd_Response_SegResult(xs, rqid, flags,
                                          9000,
                                          "Could not add context", 0);
        GWEN_SecContext_free(sc);
        GWEN_IPCXMLService_DelSession(xs, sid);
        return err;
      }
      ctxadded=1;
    }
    else {
      DBG_ERROR(0, "Context unknown, and I'm not allowed to add it");
      /* Send error message */
      GWEN_IPCXMLService_DelSession(xs, sid);
      GWEN_IPCXMLCmd_Response_SegResult(xs, rqid, flags,
                                        9000,
                                        "Unknown id", 0);
      return err;
    }
  }
  else {
    /* release the context again */
    err=GWEN_IPCXMLService_ReleaseContext(xs, connid, sc, 0);
    if (!GWEN_Error_IsOk(err)) {
      DBG_INFO_ERR(0, err);
      /* Send error message */
      GWEN_IPCXMLService_DelSession(xs, sid);
      GWEN_IPCXMLCmd_Response_SegResult(xs, rqid, flags,
                                        9000,
                                        "Internal error", 0);
      return err;
    }
  }

  /* set remote name */
  GWEN_IPCXMLService_SetRemoteName(xs, connid, peerName);

  if (!ctxadded && !anon && GWEN_IPCXMLCmd_IsSignedBy(n, peerName)) {
    /* the other side signed the message, so it is verified */
    cflags|=GWEN_IPCXMLCONNLAYER_FLAGS_VERIFIED;
    GWEN_IPCXMLService_SetConnectionFlags(xs, connid, cflags);
  }

  /* build response data */
  rsp=GWEN_DB_Group_new("response");

  GWEN_DB_SetCharValue(rsp,
                       GWEN_DB_FLAGS_DEFAULT,
                       "id",
                       GWEN_IPCXMLService_GetLocalName(xs, connid));
  if (anon)
    GWEN_DB_SetCharValue(rsp,
                         GWEN_DB_FLAGS_DEFAULT,
                         "peerid",
                         peerName);

  err=GWEN_IPCXMLService_AddResponse(xs, rqid, "OpenSessionResponse", 0,
                                     rsp, flags);
  if (!GWEN_Error_IsOk(err)) {
    DBG_INFO_ERR(0, err);
    GWEN_DB_Group_free(rsp);
    GWEN_IPCXMLService_DelSession(xs, sid);
    return err;
  }

  /* mark session as open */
  GWEN_IPCXMLService_SetSessionId(xs, connid, sid);
  cflags|=GWEN_IPCXMLCONNLAYER_FLAGS_SESSION_OPEN;
  GWEN_IPCXMLService_SetConnectionFlags(xs, connid, cflags);

  GWEN_DB_Group_free(rsp);
  return 0;
}



GWEN_ERRORCODE GWEN_IPCXMLCmd_Result_OpenSession(GWEN_IPCXMLSERVICE *xs,
                                                 unsigned int rqid,
                                                 char *snbuffer,
                                                 unsigned int snsize,
                                                 char *cnbuffer,
                                                 unsigned int cnsize) {
  GWEN_DB_NODE *n;
  const char *p;

  n=GWEN_IPCXMLService_GetResponseData(xs, rqid);
  if (!n) {
    return GWEN_Error_new(0,
                          GWEN_ERROR_SEVERITY_WARN,
                          GWEN_Error_FindType(GWEN_IPC_ERROR_TYPE),
                          GWEN_IPC_ERROR_INQUEUE_EMPTY);
  }

  if (strcasecmp(GWEN_DB_GroupName(n), "SegResult")==0) {
    unsigned int rc;
    GWEN_ERRORCODE err;

    err=GWEN_IPCXMLCmd_Result_SegResult(xs, n, &rc);
    if (!GWEN_Error_IsOk(err)) {
      DBG_INFO_ERR(0, err);
    }
    else {
      DBG_INFO(0, "Got an error: %d", rc);
      return GWEN_Error_new(0,
                            GWEN_ERROR_SEVERITY_ERR,
                            GWEN_Error_FindType(GWEN_IPC_ERROR_TYPE),
                            GWEN_IPC_ERROR_GENERIC);
    }
  }

  if (strcasecmp(GWEN_DB_GroupName(n), "OpenSessionresponse")!=0) {
    DBG_INFO(0, "Not an OpenSessionresponse");
    return GWEN_Error_new(0,
                          GWEN_ERROR_SEVERITY_ERR,
                          GWEN_Error_FindType(GWEN_IPC_ERROR_TYPE),
                          GWEN_IPC_ERROR_UNKNOWN_MSG);
  }

  p=GWEN_DB_GetCharValue(n, "OpenSessionresponse/id", 0, 0);
  if (!p) {
    DBG_WARN(0, "Peer has sent no id");
    if (snbuffer)
      *snbuffer=0;
  }
  else {
    if (snbuffer) {
      if (strlen(p)>=snsize) {
        DBG_ERROR(0, "Buffer too small, will not copy id");
        GWEN_DB_Group_free(n);
        return GWEN_Error_new(0,
                              GWEN_ERROR_SEVERITY_ERR,
                              GWEN_Error_FindType(GWEN_IPC_ERROR_TYPE),
                              GWEN_IPC_ERROR_INVALID);
      }
      memmove(snbuffer, p, strlen(p)+1);
      DBG_NOTICE(0, "Peer is \"%s\"", snbuffer);
    }
  }

  p=GWEN_DB_GetCharValue(n, "OpenSessionresponse/peerid", 0, 0);
  if (!p) {
    DBG_WARN(0, "Peer has sent no new id");
    if (cnbuffer)
      *cnbuffer=0;
  }
  else {
    if (cnbuffer) {
      if (strlen(p)>=cnsize) {
        DBG_ERROR(0, "Buffer too small, will not copy id");
        GWEN_DB_Group_free(n);
        return GWEN_Error_new(0,
                              GWEN_ERROR_SEVERITY_ERR,
                              GWEN_Error_FindType(GWEN_IPC_ERROR_TYPE),
                              GWEN_IPC_ERROR_INVALID);
      }
      memmove(cnbuffer, p, strlen(p)+1);
      DBG_NOTICE(0, "New id is \"%s\"", cnbuffer);
    }
  }

  GWEN_DB_Group_free(n);
  return 0;
}






/*___________________________________________________________________________
 *AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
 *                                Close Session
 *YYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYY
 */











/*___________________________________________________________________________
 *AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
 *                                Secure
 *YYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYY
 */



unsigned int GWEN_IPCXMLCmd_Request_Secure(GWEN_IPCXMLSERVICE *xs,
                                           unsigned int clid,
                                           unsigned int flags) {
  GWEN_DB_NODE *cfg;
  unsigned int rqid;
  unsigned int sflags;

  cfg=GWEN_DB_Group_new("request");

  /* turn on encryption and signing */
  sflags=GWEN_IPCXMLService_GetSecurityFlags(xs, clid);
  sflags|=GWEN_HBCIMSG_FLAGS_SIGN;
  sflags|=GWEN_HBCIMSG_FLAGS_CRYPT;
  GWEN_IPCXMLService_SetSecurityFlags(xs, clid, sflags);

  rqid=GWEN_IPCXMLService_AddRequest(xs, clid, "Secure", 0, cfg, flags);
  if (!rqid) {
    DBG_INFO(0, "here");
  }
  return rqid;
}



GWEN_ERRORCODE GWEN_IPCXMLCmd_Handle_Secure(GWEN_IPCXMLSERVICE *xs,
                                            unsigned int rqid,
                                            unsigned int flags,
                                            GWEN_DB_NODE *n) {
  GWEN_ERRORCODE err;
  unsigned int connid;
  const char *p;
  unsigned int sflags;

  assert(xs);
  assert(n);

  connid=GWEN_IPCXMLService_GetRequestConnection(xs, rqid);
  if (!connid) {
    DBG_INFO(0, "Could not get connection for request %d", rqid);
    return GWEN_Error_new(0,
                          GWEN_ERROR_SEVERITY_ERR,
                          GWEN_Error_FindType(GWEN_IPC_ERROR_TYPE),
                          GWEN_IPC_ERROR_REQUEST_NOT_FOUND);
  }
  DBG_INFO(0, "Connection id is %d", connid);

  /* check for encryption */
  p=GWEN_DB_GetCharValue(n, "security/crypter", 0, 0);
  if (!p) {
    DBG_ERROR(0, "Message is not encrypted (it should be)");
    GWEN_IPCXMLCmd_Response_SegResult(xs, rqid, flags,
                                      9000,
                                      "Message not encrypted", 0);
    return GWEN_Error_new(0,
                          GWEN_ERROR_SEVERITY_ERR,
                          GWEN_Error_FindType(GWEN_IPC_ERROR_TYPE),
                          GWEN_IPC_ERROR_BAD_MSG);
  }

  /* check for signer */
  p=GWEN_IPCXMLService_GetRemoteName(xs, connid);
  if (!GWEN_IPCXMLCmd_IsSignedBy(n, p)) {
    DBG_ERROR(0, "Message is not signed (it should be)");
    GWEN_IPCXMLCmd_Response_SegResult(xs, rqid, flags,
                                      9000,
                                      "Message not signed", 0);
    return GWEN_Error_new(0,
                          GWEN_ERROR_SEVERITY_ERR,
                          GWEN_Error_FindType(GWEN_IPC_ERROR_TYPE),
                          GWEN_IPC_ERROR_BAD_MSG);
  }

  /* turn on encryption and signing */
  sflags=GWEN_IPCXMLService_GetSecurityFlags(xs, connid);
  sflags|=GWEN_HBCIMSG_FLAGS_SIGN;
  sflags|=GWEN_HBCIMSG_FLAGS_CRYPT;
  GWEN_IPCXMLService_SetSecurityFlags(xs, connid, sflags);

  err=GWEN_IPCXMLCmd_Response_SegResult(xs, rqid, flags,
                                        10,
                                        "Channel secured", 0);
  if (!GWEN_Error_IsOk(err)) {
    DBG_INFO_ERR(0, err);
    return err;
  }

  return 0;
}



GWEN_ERRORCODE GWEN_IPCXMLCmd_Result_Secure(GWEN_IPCXMLSERVICE *xs,
                                            unsigned int rqid,
                                            unsigned int *result){
  GWEN_DB_NODE *db;
  GWEN_ERRORCODE err;

  db=GWEN_IPCXMLService_GetResponseData(xs, rqid);
  if (!db) {
    return GWEN_Error_new(0,
                          GWEN_ERROR_SEVERITY_WARN,
                          GWEN_Error_FindType(GWEN_IPC_ERROR_TYPE),
                          GWEN_IPC_ERROR_INQUEUE_EMPTY);
  }

  err=GWEN_IPCXMLCmd_Result_SegResult(xs, db, result);
  if (!GWEN_Error_IsOk(err)) {
    DBG_INFO_ERR(0, err);
    GWEN_DB_Group_free(db);
    return err;
  }

  GWEN_DB_Group_free(db);
  return 0;
}




