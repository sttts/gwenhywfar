/* This file is auto-generated from "ssl_cipher.xml" by the typemaker
   tool of Gwenhywfar. 
   Do not edit this file -- all changes will be lost! */
#ifndef SSL_CIPHER_H
#define SSL_CIPHER_H

/** @page P_GWEN_SSLCIPHER_PUBLIC GWEN_SslCipher (public)
This page describes the properties of GWEN_SSLCIPHER
@anchor GWEN_SSLCIPHER_Name
<h3>Name</h3>
<p>
</p>
<p>
Set this property with @ref GWEN_SslCipher_SetName, 
get it with @ref GWEN_SslCipher_GetName
</p>

@anchor GWEN_SSLCIPHER_Version
<h3>Version</h3>
<p>
</p>
<p>
Set this property with @ref GWEN_SslCipher_SetVersion, 
get it with @ref GWEN_SslCipher_GetVersion
</p>

@anchor GWEN_SSLCIPHER_Description
<h3>Description</h3>
<p>
</p>
<p>
Set this property with @ref GWEN_SslCipher_SetDescription, 
get it with @ref GWEN_SslCipher_GetDescription
</p>

@anchor GWEN_SSLCIPHER_Bits
<h3>Bits</h3>
<p>
</p>
<p>
Set this property with @ref GWEN_SslCipher_SetBits, 
get it with @ref GWEN_SslCipher_GetBits
</p>

*/
#ifdef __cplusplus
extern "C" {
#endif

typedef struct GWEN_SSLCIPHER GWEN_SSLCIPHER;

#ifdef __cplusplus
} /* __cplusplus */
#endif

#include <gwenhywfar/db.h>
#include <gwenhywfar/misc.h>
#include <gwenhywfar/list2.h>

#ifdef __cplusplus
extern "C" {
#endif


GWEN_LIST_FUNCTION_LIB_DEFS(GWEN_SSLCIPHER, GWEN_SslCipher, GWENHYWFAR_API)
GWEN_SSLCIPHER_LIST *GWEN_SslCipher_List_dup(const GWEN_SSLCIPHER_LIST *stl);

GWEN_LIST2_FUNCTION_LIB_DEFS(GWEN_SSLCIPHER, GWEN_SslCipher, GWENHYWFAR_API)

/** Destroys all objects stored in the given LIST2 and the list itself
*/
GWENHYWFAR_API void GWEN_SslCipher_List2_freeAll(GWEN_SSLCIPHER_LIST2 *stl);
/** Creates a deep copy of the given LIST2.
*/
GWENHYWFAR_API GWEN_SSLCIPHER_LIST2 *GWEN_SslCipher_List2_dup(const GWEN_SSLCIPHER_LIST2 *stl);

/** Creates a new object.
*/
GWENHYWFAR_API GWEN_SSLCIPHER *GWEN_SslCipher_new();
/** Destroys the given object.
*/
GWENHYWFAR_API void GWEN_SslCipher_free(GWEN_SSLCIPHER *st);
/** Increments the usage counter of the given object, so an additional free() is needed to destroy the object.
*/
GWENHYWFAR_API void GWEN_SslCipher_Attach(GWEN_SSLCIPHER *st);
/** Creates and returns a deep copy of thegiven object.
*/
GWENHYWFAR_API GWEN_SSLCIPHER *GWEN_SslCipher_dup(const GWEN_SSLCIPHER*st);
/** Creates an object from the data in the given GWEN_DB_NODE
*/
GWENHYWFAR_API GWEN_SSLCIPHER *GWEN_SslCipher_fromDb(GWEN_DB_NODE *db);
/** Stores an object in the given GWEN_DB_NODE
*/
GWENHYWFAR_API int GWEN_SslCipher_toDb(const GWEN_SSLCIPHER*st, GWEN_DB_NODE *db);
/** Returns 0 if this object has not been modified, !=0 otherwise
*/
GWENHYWFAR_API int GWEN_SslCipher_IsModified(const GWEN_SSLCIPHER *st);
/** Sets the modified state of the given object
*/
GWENHYWFAR_API void GWEN_SslCipher_SetModified(GWEN_SSLCIPHER *st, int i);

/**
* Returns the property @ref GWEN_SSLCIPHER_Name
*/
GWENHYWFAR_API const char *GWEN_SslCipher_GetName(const GWEN_SSLCIPHER *el);
/**
* Set the property @ref GWEN_SSLCIPHER_Name
*/
GWENHYWFAR_API void GWEN_SslCipher_SetName(GWEN_SSLCIPHER *el, const char *d);

/**
* Returns the property @ref GWEN_SSLCIPHER_Version
*/
GWENHYWFAR_API const char *GWEN_SslCipher_GetVersion(const GWEN_SSLCIPHER *el);
/**
* Set the property @ref GWEN_SSLCIPHER_Version
*/
GWENHYWFAR_API void GWEN_SslCipher_SetVersion(GWEN_SSLCIPHER *el, const char *d);

/**
* Returns the property @ref GWEN_SSLCIPHER_Description
*/
GWENHYWFAR_API const char *GWEN_SslCipher_GetDescription(const GWEN_SSLCIPHER *el);
/**
* Set the property @ref GWEN_SSLCIPHER_Description
*/
GWENHYWFAR_API void GWEN_SslCipher_SetDescription(GWEN_SSLCIPHER *el, const char *d);

/**
* Returns the property @ref GWEN_SSLCIPHER_Bits
*/
GWENHYWFAR_API int GWEN_SslCipher_GetBits(const GWEN_SSLCIPHER *el);
/**
* Set the property @ref GWEN_SSLCIPHER_Bits
*/
GWENHYWFAR_API void GWEN_SslCipher_SetBits(GWEN_SSLCIPHER *el, int d);


#ifdef __cplusplus
} /* __cplusplus */
#endif


#endif /* SSL_CIPHER_H */