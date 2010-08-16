/****************************************************************************
**
** This file is part of the CADuntu project, a 2D CAD program
**
** Copyright (C) 2010 R. van Twisk (caduntu@rvt.dds.nl)
** Copyright (C) 2001-2003 RibbonSoft. All rights reserved.
**
**
** This program is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by 
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
** 
** You should have received a copy of the GNU General Public License
** along with this program; if not, write to the Free Software
** Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
**
** This copyright notice MUST APPEAR in all copies of the script!  
**
**********************************************************************/

#ifndef QC_DIALOGFACTORY_H
#define QC_DIALOGFACTORY_H

#include "qg_dialogfactory.h"


/**
 * This is the CADuntu specific implementation of a widget which can create and
 * show dialogs. Some functions cannot be implemented on the
 * caduntu library level and need to be implemented here,
 * on the application level.
 */
class QC_DialogFactory: public QG_DialogFactory {
public:
    QC_DialogFactory(QWidget* parent, QToolBar* ow) :
	  QG_DialogFactory(parent, ow) {}
    virtual ~QC_DialogFactory() {}

    virtual void requestEditBlockWindow(RS_BlockList* blockList = NULL);
    virtual void closeEditBlockWindow(RS_Block* block = NULL);
};

#endif