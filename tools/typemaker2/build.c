/***************************************************************************
    begin       : Thu Jul 02 2009
    copyright   : (C) 2009 by Martin Preuss
    email       : martin@libchipcard.de

 ***************************************************************************
 *          Please see toplevel file COPYING for license details           *
 ***************************************************************************/

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#include "typemaker2.h"
#include "tm_typemanager.h"
#include "tm_builder_c.h"

#include <gwenhywfar/debug.h>
#include <gwenhywfar/gwenhywfar.h>
#include <gwenhywfar/pathmanager.h>



int buildFile2(GWEN_DB_NODE *dbArgs, const char *fname) {
  TYPEMAKER2_TYPEMANAGER *tym;
  TYPEMAKER2_BUILDER *tb=NULL;
  TYPEMAKER2_TYPE *ty;
  GWEN_STRINGLIST *sl;
  TYPEMAKER2_TYPE_LIST2 *tlist;
  TYPEMAKER2_TYPE_LIST2_ITERATOR *it;
  const char *s;
  int i;
  int rv;

  tym=Typemaker2_TypeManager_new();

  s=GWEN_DB_GetCharValue(dbArgs, "api", 0, NULL);
  if (s && *s)
    Typemaker2_TypeManager_SetApiDeclaration(tym, s);

  s=GWEN_DB_GetCharValue(dbArgs, "language", 0, "c");
  if (s && *s) {
    Typemaker2_TypeManager_SetLanguage(tym, s);
    if (strcasecmp(s, "c")==0) {
      tb=Typemaker2_Builder_C_new();
      Typemaker2_Builder_SetTypeManager(tb, tym);
    }
    else {
      DBG_ERROR(GWEN_LOGDOMAIN, "Unsupported language [%s]", s);
      return 1;
    }
  }
  else {
    DBG_ERROR(GWEN_LOGDOMAIN, "Missing language specification");
    return 1;
  }

  Typemaker2_Builder_SetSourceFileName(tb, fname);

  for (i=0; i<99; i++) {
    s=GWEN_DB_GetCharValue(dbArgs, "include", i, NULL);
    if (s && *s)
      Typemaker2_TypeManager_AddFolder(tym, s);
    else
      break;
  }

  sl=GWEN_PathManager_GetPaths(GWEN_PM_LIBNAME, GWEN_PM_DATADIR);
  if (sl) {
    GWEN_STRINGLISTENTRY *se;

    se=GWEN_StringList_FirstEntry(sl);
    while(se) {
      s=GWEN_StringListEntry_Data(se);
      if (s) {
	GWEN_BUFFER *xbuf;

	xbuf=GWEN_Buffer_new(0, 256, 0, 1);
	GWEN_Buffer_AppendString(xbuf, s);
	GWEN_Buffer_AppendString(xbuf, "/typemaker2/");
        s=Typemaker2_TypeManager_GetLanguage(tym);
	if (s && *s)
	  GWEN_Buffer_AppendString(xbuf, s);
	Typemaker2_TypeManager_AddFolder(tym, GWEN_Buffer_GetStart(xbuf));
        GWEN_Buffer_free(xbuf);
      }
      se=GWEN_StringListEntry_Next(se);
    }
    GWEN_StringList_free(sl);
  }

  tlist=Typemaker2_Type_List2_new();
  rv=Typemaker2_TypeManager_LoadTypeFile2(tym, fname, tlist);
  if (rv<0) {
    DBG_ERROR(GWEN_LOGDOMAIN, "Unable to load file [%s] (%d)", fname, rv);
    return 2;
  }

  it=Typemaker2_Type_List2_First(tlist);
  if(it) {
    ty=Typemaker2_Type_List2Iterator_Data(it);
    while(ty) {
      /* DEBUG */
#if 0
      Typemaker2_TypeManager_Dump(tym, stderr, 2);
#endif

      /* build */
      rv=Typemaker2_Builder_Build(tb, ty);
      if (rv<0) {
        DBG_ERROR(GWEN_LOGDOMAIN, "here (%d)", rv);
        return 2;
      }

      /* only write typedef files */
      rv=Typemaker2_Builder_WriteFiles(tb, ty, 1);
      if (rv<0) {
        DBG_ERROR(GWEN_LOGDOMAIN, "here (%d)", rv);
        return 2;
      }


      /* handle next type */
      ty=Typemaker2_Type_List2Iterator_Next(it);
    }
    Typemaker2_Type_List2Iterator_free(it);


    s=GWEN_DB_GetCharValue(dbArgs, "publicFile", 0, NULL);
    if (s)
      Typemaker2_Builder_SetFileNamePublic(tb, s);
  
    s=GWEN_DB_GetCharValue(dbArgs, "libraryFile", 0, NULL);
    if (s)
      Typemaker2_Builder_SetFileNameLibrary(tb, s);
  
    s=GWEN_DB_GetCharValue(dbArgs, "protectedFile", 0, NULL);
    if (s)
      Typemaker2_Builder_SetFileNameProtected(tb, s);
  
    s=GWEN_DB_GetCharValue(dbArgs, "privateFile", 0, NULL);
    if (s)
      Typemaker2_Builder_SetFileNamePrivate(tb, s);
  
    s=GWEN_DB_GetCharValue(dbArgs, "codeFile", 0, NULL);
    if (s)
      Typemaker2_Builder_SetFileNameCode(tb, s);

    ty=Typemaker2_Type_List2_GetFront(tlist);
    rv=Typemaker2_Builder_WriteFiles(tb, ty, 0);
    if (rv<0) {
      DBG_ERROR(GWEN_LOGDOMAIN, "here (%d)", rv);
      return 2;
    }

  }
  Typemaker2_Type_List2_free(tlist);


  return 0;
}



int buildFile(GWEN_DB_NODE *dbArgs, const char *fname) {
  TYPEMAKER2_TYPEMANAGER *tym;
  TYPEMAKER2_BUILDER *tb=NULL;
  TYPEMAKER2_TYPE *ty;
  GWEN_STRINGLIST *sl;
  const char *s;
  int i;
  int rv;

  tym=Typemaker2_TypeManager_new();

  s=GWEN_DB_GetCharValue(dbArgs, "api", 0, NULL);
  if (s && *s)
    Typemaker2_TypeManager_SetApiDeclaration(tym, s);

  s=GWEN_DB_GetCharValue(dbArgs, "language", 0, "c");
  if (s && *s) {
    Typemaker2_TypeManager_SetLanguage(tym, s);
    if (strcasecmp(s, "c")==0) {
      tb=Typemaker2_Builder_C_new();
      Typemaker2_Builder_SetTypeManager(tb, tym);
    }
    else {
      DBG_ERROR(GWEN_LOGDOMAIN, "Unsupported language [%s]", s);
      return 1;
    }
  }
  else {
    DBG_ERROR(GWEN_LOGDOMAIN, "Missing language specification");
    return 1;
  }

  Typemaker2_Builder_SetSourceFileName(tb, fname);

  for (i=0; i<99; i++) {
    s=GWEN_DB_GetCharValue(dbArgs, "include", i, NULL);
    if (s && *s)
      Typemaker2_TypeManager_AddFolder(tym, s);
    else
      break;
  }

  sl=GWEN_PathManager_GetPaths(GWEN_PM_LIBNAME, GWEN_PM_DATADIR);
  if (sl) {
    GWEN_STRINGLISTENTRY *se;

    se=GWEN_StringList_FirstEntry(sl);
    while(se) {
      s=GWEN_StringListEntry_Data(se);
      if (s) {
	GWEN_BUFFER *xbuf;

	xbuf=GWEN_Buffer_new(0, 256, 0, 1);
	GWEN_Buffer_AppendString(xbuf, s);
	GWEN_Buffer_AppendString(xbuf, "/typemaker2/");
        s=Typemaker2_TypeManager_GetLanguage(tym);
	if (s && *s)
	  GWEN_Buffer_AppendString(xbuf, s);
	Typemaker2_TypeManager_AddFolder(tym, GWEN_Buffer_GetStart(xbuf));
        GWEN_Buffer_free(xbuf);
      }
      se=GWEN_StringListEntry_Next(se);
    }
    GWEN_StringList_free(sl);
  }

  s=GWEN_DB_GetCharValue(dbArgs, "publicFile", 0, NULL);
  if (s)
    Typemaker2_Builder_SetFileNamePublic(tb, s);

  s=GWEN_DB_GetCharValue(dbArgs, "libraryFile", 0, NULL);
  if (s)
    Typemaker2_Builder_SetFileNameLibrary(tb, s);

  s=GWEN_DB_GetCharValue(dbArgs, "protectedFile", 0, NULL);
  if (s)
    Typemaker2_Builder_SetFileNameProtected(tb, s);

  s=GWEN_DB_GetCharValue(dbArgs, "privateFile", 0, NULL);
  if (s)
    Typemaker2_Builder_SetFileNamePrivate(tb, s);

  s=GWEN_DB_GetCharValue(dbArgs, "codeFile", 0, NULL);
  if (s)
    Typemaker2_Builder_SetFileNameCode(tb, s);

  ty=Typemaker2_TypeManager_LoadTypeFile(tym, fname);
  if (ty==NULL) {
    DBG_ERROR(GWEN_LOGDOMAIN, "Unable to load file [%s]", fname);
    return 2;
  }

  /* DEBUG */
#if 0
  Typemaker2_TypeManager_Dump(tym, stderr, 2);
#endif

  /* build */
  rv=Typemaker2_Builder_Build(tb, ty);
  if (rv<0) {
    DBG_ERROR(GWEN_LOGDOMAIN, "here (%d)", rv);
    return 2;
  }

  rv=Typemaker2_Builder_WriteFiles(tb, ty, 0);
  if (rv<0) {
    DBG_ERROR(GWEN_LOGDOMAIN, "here (%d)", rv);
    return 2;
  }

  return 0;
}



int build(GWEN_DB_NODE *dbArgs) {
  int i;

  for (i=0; i<99; i++) {
    const char *fileName;

    fileName=GWEN_DB_GetCharValue(dbArgs, "params", i, NULL);
    if (fileName) {
      int rv=buildFile2(dbArgs, fileName);
      if (rv != 0) {
	DBG_ERROR(GWEN_LOGDOMAIN, "Error building type from [%s]", fileName);
        return 2;
      }
    }
    else {
      if (i==0) {
	DBG_ERROR(GWEN_LOGDOMAIN, "No input");
	return 1;
      }
    }
  }

  return 0;
}


