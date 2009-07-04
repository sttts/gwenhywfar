/**********************************************************
 * This file has been automatically created by "typemaker2"
 * from the file "tm_define.xml".
 * Please do not edit this file, all changes will be lost.
 * Better edit the mentioned source file instead.
 **********************************************************/

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#include "tm_define_p.h"

#include <gwenhywfar/misc.h>
#include <gwenhywfar/debug.h>

/* code headers */

/* macro functions */
GWEN_LIST_FUNCTIONS(TYPEMAKER2_DEFINE, Typemaker2_Define)


int Typemaker2_Define_Mode_fromString(const char *p_s) {
  if (p_s && *p_s) {
    if (strcasecmp(p_s, "sequence")==0)
      return Typemaker2_Define_Mode_Sequence;
    else if (strcasecmp(p_s, "bitField")==0)
      return Typemaker2_Define_Mode_BitField;
  }
  return Typemaker2_Define_Mode_Unknown;
}

const char *Typemaker2_Define_Mode_toString(int p_i) {
  switch(p_i) {
    case Typemaker2_Define_Mode_Sequence: return "sequence";
    case Typemaker2_Define_Mode_BitField: return "bitField";
    default: return "unknown";
  }
}

TYPEMAKER2_DEFINE *Typemaker2_Define_new() {
  TYPEMAKER2_DEFINE *p_struct;

  GWEN_NEW_OBJECT(TYPEMAKER2_DEFINE, p_struct)
  GWEN_LIST_INIT(TYPEMAKER2_DEFINE, p_struct)
  /* members */
  p_struct->prefix=NULL;
  p_struct->items=Typemaker2_Item_List_new();
  p_struct->mode=Typemaker2_Define_Mode_Unknown;

  return p_struct;
}

void Typemaker2_Define_free(TYPEMAKER2_DEFINE *p_struct) {
  if (p_struct) {
    GWEN_LIST_FINI(TYPEMAKER2_DEFINE, p_struct)
  /* members */
    free(p_struct->prefix);
    Typemaker2_Item_List_free(p_struct->items);
    
    GWEN_FREE_OBJECT(p_struct);
  }
}

TYPEMAKER2_DEFINE *Typemaker2_Define_dup(const TYPEMAKER2_DEFINE *p_src) {
  TYPEMAKER2_DEFINE *p_struct;

  assert(p_src);
  p_struct=Typemaker2_Define_new();
  /* member "prefix" */
#define TYPEMAKER2_MEMBER_FLAGS_OWN_ON
#define TYPEMAKER2_MEMBER_FLAGS_ATTRIBUTE_ON
  if (p_struct->prefix) {
    free(p_struct->prefix);
    p_struct->prefix=NULL;
  }
  if (p_src->prefix) {
    p_struct->prefix=strdup(p_src->prefix);
  }
#undef TYPEMAKER2_MEMBER_FLAGS_OWN_ON
#undef TYPEMAKER2_MEMBER_FLAGS_ATTRIBUTE_ON

  /* member "items" */
#define TYPEMAKER2_MEMBER_FLAGS_OWN_ON
#define TYPEMAKER2_MEMBER_FLAGS_VOLATILE_ON
  if (p_struct->items) {
    Typemaker2_Item_List_free(p_struct->items);
    p_struct->items=NULL;
  }
  if (p_src->items) {
    { if (p_src->items) { TYPEMAKER2_ITEM_LIST *t; TYPEMAKER2_ITEM *elem; t=Typemaker2_Item_List_new(); elem=Typemaker2_Item_List_First(p_src->items); while(elem) { TYPEMAKER2_ITEM *cpy; cpy=Typemaker2_Item_dup(elem); Typemaker2_Item_List_Add(cpy, t); elem=Typemaker2_Item_List_Next(elem); } p_struct->items=t; } else p_struct->items=NULL; }
  }
#undef TYPEMAKER2_MEMBER_FLAGS_OWN_ON
#undef TYPEMAKER2_MEMBER_FLAGS_VOLATILE_ON

  /* member "mode" */
#define TYPEMAKER2_MEMBER_FLAGS_OWN_ON
#define TYPEMAKER2_MEMBER_FLAGS_ATTRIBUTE_ON
#define TYPEMAKER2_MEMBER_FLAGS_ENUM_ON
  p_struct->mode=p_src->mode;
#undef TYPEMAKER2_MEMBER_FLAGS_OWN_ON
#undef TYPEMAKER2_MEMBER_FLAGS_ATTRIBUTE_ON
#undef TYPEMAKER2_MEMBER_FLAGS_ENUM_ON

  return p_struct;
}

const char *Typemaker2_Define_GetPrefix(const TYPEMAKER2_DEFINE *p_struct) {
#define TYPEMAKER2_MEMBER_FLAGS_OWN_ON
#define TYPEMAKER2_MEMBER_FLAGS_ATTRIBUTE_ON
  assert(p_struct);
  return p_struct->prefix;
#undef TYPEMAKER2_MEMBER_FLAGS_OWN_ON
#undef TYPEMAKER2_MEMBER_FLAGS_ATTRIBUTE_ON
}

TYPEMAKER2_ITEM_LIST *Typemaker2_Define_GetItems(const TYPEMAKER2_DEFINE *p_struct) {
#define TYPEMAKER2_MEMBER_FLAGS_OWN_ON
#define TYPEMAKER2_MEMBER_FLAGS_VOLATILE_ON
  assert(p_struct);
  return p_struct->items;
#undef TYPEMAKER2_MEMBER_FLAGS_OWN_ON
#undef TYPEMAKER2_MEMBER_FLAGS_VOLATILE_ON
}

int Typemaker2_Define_GetMode(const TYPEMAKER2_DEFINE *p_struct) {
#define TYPEMAKER2_MEMBER_FLAGS_OWN_ON
#define TYPEMAKER2_MEMBER_FLAGS_ATTRIBUTE_ON
#define TYPEMAKER2_MEMBER_FLAGS_ENUM_ON
  assert(p_struct);
  return p_struct->mode;
#undef TYPEMAKER2_MEMBER_FLAGS_OWN_ON
#undef TYPEMAKER2_MEMBER_FLAGS_ATTRIBUTE_ON
#undef TYPEMAKER2_MEMBER_FLAGS_ENUM_ON
}

void Typemaker2_Define_SetPrefix(TYPEMAKER2_DEFINE *p_struct, const char *p_src) {
#define TYPEMAKER2_MEMBER_FLAGS_OWN_ON
#define TYPEMAKER2_MEMBER_FLAGS_ATTRIBUTE_ON
  assert(p_struct);
  if (p_struct->prefix) {
    free(p_struct->prefix);
}
  if (p_src) {
    p_struct->prefix=strdup(p_src);
  }
  else {
    p_struct->prefix=NULL;
  }
#undef TYPEMAKER2_MEMBER_FLAGS_OWN_ON
#undef TYPEMAKER2_MEMBER_FLAGS_ATTRIBUTE_ON
}

void Typemaker2_Define_SetItems(TYPEMAKER2_DEFINE *p_struct, const TYPEMAKER2_ITEM_LIST *p_src) {
#define TYPEMAKER2_MEMBER_FLAGS_OWN_ON
#define TYPEMAKER2_MEMBER_FLAGS_VOLATILE_ON
  assert(p_struct);
  if (p_struct->items) {
    Typemaker2_Item_List_free(p_struct->items);
}
  if (p_src) {
    { if (p_src) { TYPEMAKER2_ITEM_LIST *t; TYPEMAKER2_ITEM *elem; t=Typemaker2_Item_List_new(); elem=Typemaker2_Item_List_First(p_src); while(elem) { TYPEMAKER2_ITEM *cpy; cpy=Typemaker2_Item_dup(elem); Typemaker2_Item_List_Add(cpy, t); elem=Typemaker2_Item_List_Next(elem); } p_struct->items=t; } else p_struct->items=NULL; }
  }
  else {
    p_struct->items=Typemaker2_Item_List_new();
  }
#undef TYPEMAKER2_MEMBER_FLAGS_OWN_ON
#undef TYPEMAKER2_MEMBER_FLAGS_VOLATILE_ON
}

void Typemaker2_Define_SetMode(TYPEMAKER2_DEFINE *p_struct, int p_src) {
#define TYPEMAKER2_MEMBER_FLAGS_OWN_ON
#define TYPEMAKER2_MEMBER_FLAGS_ATTRIBUTE_ON
#define TYPEMAKER2_MEMBER_FLAGS_ENUM_ON
  assert(p_struct);
    p_struct->mode=p_src;
#undef TYPEMAKER2_MEMBER_FLAGS_OWN_ON
#undef TYPEMAKER2_MEMBER_FLAGS_ATTRIBUTE_ON
#undef TYPEMAKER2_MEMBER_FLAGS_ENUM_ON
}

void Typemaker2_Define_ReadXml(TYPEMAKER2_DEFINE *p_struct, GWEN_XMLNODE *p_db) {
  assert(p_struct);
  /* member "prefix" */
#define TYPEMAKER2_MEMBER_FLAGS_OWN_ON
#define TYPEMAKER2_MEMBER_FLAGS_ATTRIBUTE_ON
  if (p_struct->prefix) {
    free(p_struct->prefix);
  }
  p_struct->prefix=NULL;
  { const char *s; 
#ifdef TYPEMAKER2_MEMBER_FLAGS_ATTRIBUTE_ON
 s=GWEN_XMLNode_GetProperty(p_db, "prefix", NULL); 
#else
 s=GWEN_XMLNode_GetCharValue(p_db, "prefix", NULL); 
#endif
 if (s) p_struct->prefix=strdup(s); }

#undef TYPEMAKER2_MEMBER_FLAGS_OWN_ON
#undef TYPEMAKER2_MEMBER_FLAGS_ATTRIBUTE_ON
  /* member "items" */
#define TYPEMAKER2_MEMBER_FLAGS_OWN_ON
#define TYPEMAKER2_MEMBER_FLAGS_VOLATILE_ON
  if (p_struct->items) {
    Typemaker2_Item_List_free(p_struct->items);
  }
  p_struct->items=Typemaker2_Item_List_new();
  /* member "items" is volatile, not reading from xml */

#undef TYPEMAKER2_MEMBER_FLAGS_OWN_ON
#undef TYPEMAKER2_MEMBER_FLAGS_VOLATILE_ON
  /* member "mode" */
#define TYPEMAKER2_MEMBER_FLAGS_OWN_ON
#define TYPEMAKER2_MEMBER_FLAGS_ATTRIBUTE_ON
#define TYPEMAKER2_MEMBER_FLAGS_ENUM_ON
  p_struct->mode=Typemaker2_Define_Mode_Unknown;
  
#ifdef TYPEMAKER2_MEMBER_FLAGS_ENUM_ON
 { const char *s; s=GWEN_XMLNode_GetCharValue(p_db, "mode", 0, NULL); if (s) p_struct->mode=Typemaker2_Define_Mode_fromString(s); else p_struct->mode=Typemaker2_Define_Mode_Unknown; } 
#else
 p_struct->mode=GWEN_XMLNode_GetIntValue(p_db, "mode", Typemaker2_Define_Mode_Unknown); 
#endif


#undef TYPEMAKER2_MEMBER_FLAGS_OWN_ON
#undef TYPEMAKER2_MEMBER_FLAGS_ATTRIBUTE_ON
#undef TYPEMAKER2_MEMBER_FLAGS_ENUM_ON
}

void Typemaker2_Define_WriteXml(const TYPEMAKER2_DEFINE *p_struct, GWEN_XMLNODE *p_db) {
  assert(p_struct);
  /* member "prefix" */
#define TYPEMAKER2_MEMBER_FLAGS_OWN_ON
#define TYPEMAKER2_MEMBER_FLAGS_ATTRIBUTE_ON
  if (p_struct->prefix) { 
#ifdef TYPEMAKER2_MEMBER_FLAGS_ATTRIBUTE_ON
 GWEN_XMLNode_SetProperty(p_db, "prefix", p_struct->prefix); 
#else
 GWEN_XMLNode_SetCharValue(p_db, "prefix", p_struct->prefix); 
#endif
 } else { /* TODO: remove element */ }
#undef TYPEMAKER2_MEMBER_FLAGS_OWN_ON
#undef TYPEMAKER2_MEMBER_FLAGS_ATTRIBUTE_ON

  /* member "items" is volatile, not writing to xml */

  /* member "mode" */
#define TYPEMAKER2_MEMBER_FLAGS_OWN_ON
#define TYPEMAKER2_MEMBER_FLAGS_ATTRIBUTE_ON
#define TYPEMAKER2_MEMBER_FLAGS_ENUM_ON
  
#ifdef TYPEMAKER2_MEMBER_FLAGS_ENUM_ON
 GWEN_XMLNode_SetCharValue(p_db, "mode", Typemaker2_Define_Mode_toString((p_struct->mode))); 
#else
 GWEN_XMLNode_SetIntValue(p_db, "mode", (p_struct->mode)); 
#endif

#undef TYPEMAKER2_MEMBER_FLAGS_OWN_ON
#undef TYPEMAKER2_MEMBER_FLAGS_ATTRIBUTE_ON
#undef TYPEMAKER2_MEMBER_FLAGS_ENUM_ON

}

void Typemaker2_Define_toXml(const TYPEMAKER2_DEFINE *p_struct, GWEN_XMLNODE *p_db) {
  Typemaker2_Define_WriteXml(p_struct, p_db);
}

TYPEMAKER2_DEFINE *Typemaker2_Define_fromXml(GWEN_XMLNODE *p_db) {
  TYPEMAKER2_DEFINE *p_struct;
  p_struct=Typemaker2_Define_new();
  Typemaker2_Define_ReadXml(p_struct, p_db);
  return p_struct;
}

