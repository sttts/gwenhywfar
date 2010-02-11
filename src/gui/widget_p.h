/***************************************************************************
    begin       : Wed Jan 20 2010
    copyright   : (C) 2010 by Martin Preuss
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


#ifndef GWENHYWFAR_GUI_WIDGET_P_H
#define GWENHYWFAR_GUI_WIDGET_P_H

#include "widget_l.h"


struct GWEN_WIDGET {
  GWEN_TREE_ELEMENT(GWEN_WIDGET)

  GWEN_DIALOG *dialog;

  uint32_t flags;
  GWEN_WIDGET_TYPE wtype;

  char *name;

  int columns;
  int rows;
  char *text[GWEN_WIDGET_TEXTCOUNT];

  char *iconFile;
  char *imageFile;

  void *impl_data;

  uint32_t refCount;
};




#endif