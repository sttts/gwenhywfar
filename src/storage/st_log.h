/* This file is auto-generated from "st_log.xml" by the typemaker
   tool of Gwenhywfar. 
   Do not edit this file -- all changes will be lost! */
#ifndef ST_LOG_H
#define ST_LOG_H

/** @page P_GWEN_STO_LOG_PUBLIC GWEN_StoLog (public)
This page describes the properties of GWEN_STO_LOG
@anchor GWEN_STO_LOG_UserName
<h3>UserName</h3>
<p>
Client name</p>
<p>
Set this property with @ref GWEN_StoLog_SetUserName, 
get it with @ref GWEN_StoLog_GetUserName
</p>

@anchor GWEN_STO_LOG_LogAction
<h3>LogAction</h3>
<p>
Reason for the log entry.</p>
<p>
Set this property with @ref GWEN_StoLog_SetLogAction, 
get it with @ref GWEN_StoLog_GetLogAction
</p>

@anchor GWEN_STO_LOG_TypeBaseName
<h3>TypeBaseName</h3>
<p>
Name of the basetype.</p>
<p>
Set this property with @ref GWEN_StoLog_SetTypeBaseName, 
get it with @ref GWEN_StoLog_GetTypeBaseName
</p>

@anchor GWEN_STO_LOG_TypeName
<h3>TypeName</h3>
<p>
Type name</p>
<p>
Set this property with @ref GWEN_StoLog_SetTypeName, 
get it with @ref GWEN_StoLog_GetTypeName
</p>

@anchor GWEN_STO_LOG_ObjectId
<h3>ObjectId</h3>
<p>
Object id</p>
<p>
Set this property with @ref GWEN_StoLog_SetObjectId, 
get it with @ref GWEN_StoLog_GetObjectId
</p>

@anchor GWEN_STO_LOG_Param1
<h3>Param1</h3>
<p>
Parameter 1.</p>
<p>
Set this property with @ref GWEN_StoLog_SetParam1, 
get it with @ref GWEN_StoLog_GetParam1
</p>

@anchor GWEN_STO_LOG_Param2
<h3>Param2</h3>
<p>
Parameter 2.</p>
<p>
Set this property with @ref GWEN_StoLog_SetParam2, 
get it with @ref GWEN_StoLog_GetParam2
</p>

@anchor GWEN_STO_LOG_Param3
<h3>Param3</h3>
<p>
Parameter 3.</p>
<p>
Set this property with @ref GWEN_StoLog_SetParam3, 
get it with @ref GWEN_StoLog_GetParam3
</p>

*/
#ifdef __cplusplus
extern "C" {
#endif

typedef struct GWEN_STO_LOG GWEN_STO_LOG;

#ifdef __cplusplus
} /* __cplusplus */
#endif

#include <gwenhywfar/db.h>
#include <gwenhywfar/misc.h>
/* headers */
#include <gwenhywfar/gwenhywfarapi.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
  GWEN_StoLog_ActionUnknown=-1,
  /** No action  */
  GWEN_StoLog_ActionNone=0,
  /**   */
  GWEN_StoLog_ActionTypeCreate,
  /**   */
  GWEN_StoLog_ActionTypeDelete,
  /**   */
  GWEN_StoLog_ActionTypeAddVar,
  /**   */
  GWEN_StoLog_ActionObjectCreate,
  /**   */
  GWEN_StoLog_ActionObjectDelete,
  /**   */
  GWEN_StoLog_ActionObjectSetValue,
  /**   */
  GWEN_StoLog_ActionObjectIncRef,
  /**   */
  GWEN_StoLog_ActionObjectDecRef,
  /**   */
  GWEN_StoLog_ActionBeginEdit,
  /**   */
  GWEN_StoLog_ActionEndEdit,
  /**   */
  GWEN_StoLog_ActionBeginSession,
  /**   */
  GWEN_StoLog_ActionEndSession
} GWEN_STO_LOG_ACTION;

GWENHYWFAR_API GWEN_STO_LOG_ACTION GWEN_StoLog_Action_fromString(const char *s);
GWENHYWFAR_API const char *GWEN_StoLog_Action_toString(GWEN_STO_LOG_ACTION v);


GWEN_LIST_FUNCTION_LIB_DEFS(GWEN_STO_LOG, GWEN_StoLog, GWENHYWFAR_API)
GWENHYWFAR_API GWEN_STO_LOG_LIST *GWEN_StoLog_List_dup(const GWEN_STO_LOG_LIST *stl);

/** Creates a new object.
*/
GWENHYWFAR_API GWEN_STO_LOG *GWEN_StoLog_new();
/** Creates an object from the data in the given GWEN_DB_NODE
*/
GWENHYWFAR_API GWEN_STO_LOG *GWEN_StoLog_fromDb(GWEN_DB_NODE *db);
/** Creates and returns a deep copy of thegiven object.
*/
GWENHYWFAR_API GWEN_STO_LOG *GWEN_StoLog_dup(const GWEN_STO_LOG*st);
/** Destroys the given object.
*/
GWENHYWFAR_API void GWEN_StoLog_free(GWEN_STO_LOG *st);
/** Increments the usage counter of the given object, so an additional free() is needed to destroy the object.
*/
GWENHYWFAR_API void GWEN_StoLog_Attach(GWEN_STO_LOG *st);
/** Reads data from a GWEN_DB.
*/
GWENHYWFAR_API int GWEN_StoLog_ReadDb(GWEN_STO_LOG *st, GWEN_DB_NODE *db);
/** Stores an object in the given GWEN_DB_NODE
*/
GWENHYWFAR_API int GWEN_StoLog_toDb(const GWEN_STO_LOG*st, GWEN_DB_NODE *db);
/** Returns 0 if this object has not been modified, !=0 otherwise
*/
GWENHYWFAR_API int GWEN_StoLog_IsModified(const GWEN_STO_LOG *st);
/** Sets the modified state of the given object
*/
GWENHYWFAR_API void GWEN_StoLog_SetModified(GWEN_STO_LOG *st, int i);


/**
* Returns the property @ref GWEN_STO_LOG_UserName
*/
GWENHYWFAR_API const char *GWEN_StoLog_GetUserName(const GWEN_STO_LOG *el);
/**
* Set the property @ref GWEN_STO_LOG_UserName
*/
GWENHYWFAR_API void GWEN_StoLog_SetUserName(GWEN_STO_LOG *el, const char *d);

/**
* Returns the property @ref GWEN_STO_LOG_LogAction
*/
GWENHYWFAR_API GWEN_STO_LOG_ACTION GWEN_StoLog_GetLogAction(const GWEN_STO_LOG *el);
/**
* Set the property @ref GWEN_STO_LOG_LogAction
*/
GWENHYWFAR_API void GWEN_StoLog_SetLogAction(GWEN_STO_LOG *el, GWEN_STO_LOG_ACTION d);

/**
* Returns the property @ref GWEN_STO_LOG_TypeBaseName
*/
GWENHYWFAR_API const char *GWEN_StoLog_GetTypeBaseName(const GWEN_STO_LOG *el);
/**
* Set the property @ref GWEN_STO_LOG_TypeBaseName
*/
GWENHYWFAR_API void GWEN_StoLog_SetTypeBaseName(GWEN_STO_LOG *el, const char *d);

/**
* Returns the property @ref GWEN_STO_LOG_TypeName
*/
GWENHYWFAR_API const char *GWEN_StoLog_GetTypeName(const GWEN_STO_LOG *el);
/**
* Set the property @ref GWEN_STO_LOG_TypeName
*/
GWENHYWFAR_API void GWEN_StoLog_SetTypeName(GWEN_STO_LOG *el, const char *d);

/**
* Returns the property @ref GWEN_STO_LOG_ObjectId
*/
GWENHYWFAR_API GWEN_TYPE_UINT32 GWEN_StoLog_GetObjectId(const GWEN_STO_LOG *el);
/**
* Set the property @ref GWEN_STO_LOG_ObjectId
*/
GWENHYWFAR_API void GWEN_StoLog_SetObjectId(GWEN_STO_LOG *el, GWEN_TYPE_UINT32 d);

/**
* Returns the property @ref GWEN_STO_LOG_Param1
*/
GWENHYWFAR_API const char *GWEN_StoLog_GetParam1(const GWEN_STO_LOG *el);
/**
* Set the property @ref GWEN_STO_LOG_Param1
*/
GWENHYWFAR_API void GWEN_StoLog_SetParam1(GWEN_STO_LOG *el, const char *d);

/**
* Returns the property @ref GWEN_STO_LOG_Param2
*/
GWENHYWFAR_API const char *GWEN_StoLog_GetParam2(const GWEN_STO_LOG *el);
/**
* Set the property @ref GWEN_STO_LOG_Param2
*/
GWENHYWFAR_API void GWEN_StoLog_SetParam2(GWEN_STO_LOG *el, const char *d);

/**
* Returns the property @ref GWEN_STO_LOG_Param3
*/
GWENHYWFAR_API const char *GWEN_StoLog_GetParam3(const GWEN_STO_LOG *el);
/**
* Set the property @ref GWEN_STO_LOG_Param3
*/
GWENHYWFAR_API void GWEN_StoLog_SetParam3(GWEN_STO_LOG *el, const char *d);


#ifdef __cplusplus
} /* __cplusplus */
#endif


#endif /* ST_LOG_H */
