/****************************************************************************
**
** This file is part of the LibreCAD project, a 2D CAD program
**
** Copyright (C) 2010 R. van Twisk (librecad@rvt.dds.nl)
** Copyright (C) 2001-2003 RibbonSoft. All rights reserved.
**
**
** This file may be distributed and/or modified under the terms of the
** GNU General Public License version 2 as published by the Free Software
** Foundation and appearing in the file gpl-2.0.txt included in the
** packaging of this file.
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
#include <QContextMenuEvent>
#include <QToolBar>

#include "qg_snaptoolbar.h"
#include "rs_settings.h"

/*
 *  Constructs a QG_CadToolBarSnap as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 */
QG_SnapToolBar::QG_SnapToolBar( const QString & title, QG_ActionHandler* ah, QWidget * parent )
    : QToolBar(title, parent)
,m_iActionCounts(0)
{
    actionHandler=ah;
    actionHandler->setSnapToolBar(this);
    init();
}

/*
 *  Destroys the object and frees any allocated resources
 */
QG_SnapToolBar::~QG_SnapToolBar()
{
    //@Save default snap mode to prefrences.
    //never being called
    saveSnapMode();
    // no need to delete child widgets, Qt does it all for us
}

void QG_SnapToolBar::saveSnapMode()
{
    //@write default snap mode from prefrences.
    unsigned int snapFlags=RS_Snapper::snapModeToInt(getSnaps());
    RS_SETTINGS->beginGroup("/Snap");
    RS_SETTINGS->writeEntry("/SnapMode",QString::number(snapFlags));
    RS_SETTINGS->endGroup();
    // no need to delete child widgets, Qt does it all for us
}

void QG_SnapToolBar::setSnaps ( RS_SnapMode s )
{
    snapFree->setChecked(s.snapFree);
    snapGrid->setChecked(s.snapGrid);
    snapEnd->setChecked(s.snapEndpoint);
    snapOnEntity->setChecked(s.snapOnEntity);
    snapCenter->setChecked(s.snapCenter);
    snapMiddle->setChecked(s.snapMiddle);
    snapDistance->setChecked(s.snapDistance);
    snapIntersection->setChecked(s.snapIntersection);
    restrictHorizontal->setChecked(s.restriction==RS2::RestrictHorizontal ||  s.restriction==RS2::RestrictOrthogonal);
    restrictVertical->setChecked(s.restriction==RS2::RestrictVertical ||  s.restriction==RS2::RestrictOrthogonal);
    restrictOrthogonal->setChecked(s.restriction==RS2::RestrictOrthogonal);
    restrictNothing->setChecked(s.restriction==RS2::RestrictNothing);
}

RS_SnapMode QG_SnapToolBar::getSnaps ( void )
{
    RS_SnapMode s;

    s.snapFree         = snapFree->isChecked();
    s.snapGrid         = snapGrid->isChecked();
    s.snapEndpoint     = snapEnd->isChecked();
    s.snapOnEntity     = snapOnEntity->isChecked();
    s.snapCenter       = snapCenter->isChecked();
    s.snapMiddle       = snapMiddle->isChecked();
    s.snapDistance       = snapDistance->isChecked();
    s.snapIntersection = snapIntersection->isChecked();
    // removed Restrict Othogonal button
    // todo simplify internal restrict rules
    if (restrictHorizontal->isChecked()) {
        if (restrictVertical->isChecked()) {
            s.restriction = RS2::RestrictOrthogonal;
        } else {
            s.restriction = RS2::RestrictHorizontal;
        }
    } else {
        if (restrictVertical->isChecked()) {
            s.restriction = RS2::RestrictVertical;
        } else {
            s.restriction = RS2::RestrictNothing;
        }
    }

    return s;
}

void QG_SnapToolBar::init()
{
    snapFree = new QAction(QIcon(":/extui/snapfree.png"), tr("Free Snap"), this);
    snapFree->setCheckable(true);
    connect(snapFree, SIGNAL(triggered()), this, SLOT(actionTriggered()));
    m_vSnapActions<<snapFree;
    this->addAction(snapFree);
    snapGrid = new QAction(QIcon(":/extui/snapgrid.png"), tr("Snap on grid"), this);
    snapGrid->setCheckable(true);
    connect(snapGrid, SIGNAL(triggered()), this, SLOT(actionTriggered()));
    m_vSnapActions<<snapGrid;
    this->addAction(snapGrid);
    snapEnd = new QAction(QIcon(":/extui/snapendpoint.png"), tr("Snap on Endpoints"), this);
    snapEnd->setCheckable(true);
    connect(snapEnd, SIGNAL(triggered()), this, SLOT(actionTriggered()));
    m_vSnapActions<<snapEnd;
    this->addAction(snapEnd);
    snapOnEntity = new QAction(QIcon(":/extui/snaponentity.png"), tr("Snap on Entity"), this);
    snapOnEntity->setCheckable(true);
    connect(snapOnEntity, SIGNAL(triggered()), this, SLOT(actionTriggered()));
    m_vSnapActions<<snapOnEntity;
    this->addAction(snapOnEntity);
    snapCenter = new QAction(QIcon(":/extui/snapcenter.png"), tr("Snap Center"), this);
    snapCenter->setCheckable(true);
    connect(snapCenter, SIGNAL(triggered()), this, SLOT(actionTriggered()));
    m_vSnapActions<<snapCenter;
    this->addAction(snapCenter);
    snapMiddle = new QAction(QIcon(":/extui/snapmiddle.png"), tr("Snap Middle"), this);
    snapMiddle->setCheckable(true);
    connect(snapMiddle, SIGNAL(triggered()), this, SLOT(actionTriggered()));
    m_vSnapActions<<snapMiddle;
    this->addAction(snapMiddle);
    snapDistance = new QAction(QIcon(":/extui/snapdist.png"), tr("Snap Distance"), this);
    snapDistance ->setCheckable(true);
    connect(snapDistance, SIGNAL(triggered()), this, SLOT(actionTriggered()));
    m_vSnapActions<<snapDistance;
    this->addAction(snapDistance);
    snapIntersection = new QAction(QIcon(":/extui/snapintersection.png"), tr("Snap Intersection"), this);
    snapIntersection->setCheckable(true);
    connect(snapIntersection, SIGNAL(triggered()), this, SLOT(actionTriggered()));
    m_vSnapActions<<snapIntersection;
    this->addAction(snapIntersection);
    m_iActionCounts = m_vSnapActions.size();

    this->addSeparator();

    /*
    restrictOrthoagonal = new QAction(QIcon(":/extui/restrictorthogonal.png"),
                                      "Restrict Orthogonal", this);
    restrictOrthoagonal->setCheckable(true);
    connect(restrictOrthoagonal, SIGNAL(triggered(bool)),
            this, SLOT(restrictOrthoagonalTriggered(bool)));
    connect(restrictOrthoagonal, SIGNAL(triggered()), this, SLOT(actionTriggered()));
    this->addAction(restrictOrthoagonal);
    */
    restrictHorizontal = new QAction(QIcon(":/extui/restricthorizontal.png"),
                                     tr("Restrict Horizontal"), this);
    restrictHorizontal->setCheckable(true);
    connect(restrictHorizontal, SIGNAL(triggered()), this, SLOT(actionTriggered()));
    m_vSnapActions<<restrictHorizontal;
    this->addAction(restrictHorizontal);
    restrictVertical = new QAction(QIcon(":/extui/restrictvertical.png"),
                                   tr("Restrict Vertical"), this);
    restrictVertical->setCheckable(true);
    connect(restrictVertical, SIGNAL(triggered()), this, SLOT(actionTriggered()));
    m_vSnapActions<<restrictVertical;
    this->addAction(restrictVertical);

    restrictOrthogonal = new QAction(QIcon(":/extui/restrictorthogonal.png"),
                                   tr("Restrict Orthogonal"), this);
    restrictOrthogonal->setCheckable(true);
    connect(restrictOrthogonal, SIGNAL(triggered(bool)), this,
            SLOT(slotRestrictOrthogonal(bool)));
    m_vSnapActions<<restrictOrthogonal;

    restrictNothing = new QAction(QIcon(":/extui/restrictnothing.png"),
                                   tr("Restrict Nothing"), this);
    restrictNothing->setCheckable(true);
    connect(restrictNothing, SIGNAL(triggered(bool)), this,
            SLOT(slotRestrictNothing(bool)));
    m_vSnapActions<<restrictNothing;

    this->addSeparator();
    bRelZero = new QAction(QIcon(":/extui/relzeromove.png"), tr("Set relative zero position"), this);
    bRelZero->setCheckable(false);
    connect(bRelZero, SIGNAL(triggered()), actionHandler, SLOT(slotSetRelativeZero()));
    m_vSnapActions<<bRelZero;
    //connect(bRelZero, SIGNAL(triggered()), this, SLOT(slotSetRelativeZero()));
    this->addAction(bRelZero);
    bLockRelZero = new QAction(QIcon(":/extui/relzerolock.png"), tr("Lock relative zero position"), this);
    bLockRelZero->setCheckable(true);
    connect(bLockRelZero, SIGNAL(toggled(bool)),actionHandler, SLOT(slotLockRelativeZero(bool)));
    m_vSnapActions<<bLockRelZero;
    this->addAction(bLockRelZero);
    //restore snapMode from saved preferences
    RS_SETTINGS->beginGroup("/Snap");
    setSnaps(RS_Snapper::intToSnapMode(RS_SETTINGS->readNumEntry("/SnapMode",0)));
    RS_SETTINGS->endGroup();
}

bool QG_SnapToolBar::lockedRelativeZero()
{
    return bLockRelZero->isChecked();
}
void QG_SnapToolBar::setLockedRelativeZero(bool on)
{
    bLockRelZero->setChecked(on);
}
const QVector<QAction*>& QG_SnapToolBar::getActions() const
{
    return m_vSnapActions;
}
void QG_SnapToolBar::setActionHandler(QG_ActionHandler* ah){
    actionHandler=ah;
}

/* Slots */

void QG_SnapToolBar::slotRestrictNothing(bool checked)
{
    if( restrictVertical != NULL) restrictVertical->setChecked(!checked);
    if( restrictHorizontal != NULL) restrictHorizontal->setChecked(!checked);
    if( restrictOrthogonal != NULL) restrictOrthogonal->setChecked(!checked);
    actionTriggered();
}

void QG_SnapToolBar::slotRestrictOrthogonal(bool checked)
{
    if( restrictVertical != NULL) restrictVertical->setChecked(checked);
    if( restrictHorizontal != NULL) restrictHorizontal->setChecked(checked);
    if( restrictNothing != NULL) restrictNothing->setChecked(checked);
    actionTriggered();
}

void QG_SnapToolBar::actionTriggered()
{
    actionHandler->slotSetSnaps(getSnaps());
}

