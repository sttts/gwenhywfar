/***************************************************************************
 $RCSfile$
 -------------------
 cvs         : $Id$
 begin       : Thu Oct 30 2003
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

#ifndef GWENHYFWAR_CSV_H
#define GWENHYFWAR_CSV_H


#include <gwenhyfwar/bufferedio.h>
#include <gwenhyfwar/db.h>

#ifdef __cplusplus
extern "C" {
#endif


int GWEN_CSV_Write(GWEN_BUFFEREDIO *bio,
                   GWEN_DB_NODE *cfg,
                   GWEN_DB_NODE *data);

int GWEN_CSV_Read(GWEN_BUFFEREDIO *bio,
                  GWEN_DB_NODE *cfg,
                  GWEN_DB_NODE *data);


#ifdef __cplusplus
}
#endif


#endif

