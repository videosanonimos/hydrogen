/*
 * Hydrogen
 * Copyright(c) 2008-2023 The hydrogen development team [hydrogen-devel@lists.sourceforge.net]
 *
 * http://www.hydrogen-music.org
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY, without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

#include "ColorSelectionButton.h"

#include <QColorDialog>
#include <core/Globals.h>

ColorSelectionButton::ColorSelectionButton( QWidget* pParent, QColor sInitialColor, int nSize )
 : QPushButton( pParent )
 , m_sColor( sInitialColor )
 , m_bMouseOver( false )
{
	setFlat( true );

	if ( nSize != 0 ) {
		QSize size( nSize, nSize );
		setFixedSize( size );
		resize( size );
	}
}

ColorSelectionButton::~ColorSelectionButton() {
}

void ColorSelectionButton::mousePressEvent(QMouseEvent*ev) {

	if ( isEnabled() ) {

		QColor newColor = QColorDialog::getColor( m_sColor, this, tr( "Pick a pattern color" ) );

		if ( m_sColor != newColor && newColor.isValid() ) {
			m_sColor = newColor;
			update();
			emit colorChanged();
		}
	}
}

void ColorSelectionButton::enterEvent(QEvent *ev) {
	UNUSED( ev );
	m_bMouseOver = true;
	update();
}


void ColorSelectionButton::setColor( const QColor& color) {
	m_sColor = color;
	update();
}

void ColorSelectionButton::leaveEvent(QEvent *ev) {
	UNUSED( ev );
	m_bMouseOver = false;
	update();
}

void ColorSelectionButton::paintEvent( QPaintEvent* ev) {
	QPainter painter(this);
	QColor color( m_sColor );
	QColor backgroundColor( "#333" );
	if ( m_bMouseOver ) {
		if ( isEnabled() ) {
			backgroundColor = H2Core::Preferences::get_instance()->getColorTheme()->m_highlightColor;
		} else {
			backgroundColor = H2Core::Preferences::get_instance()->getColorTheme()->m_lightColor;
		}
	}
	
	painter.setPen( backgroundColor );
	painter.drawRect( 0, 0, width() - 1, height() -1 );
	painter.setPen( color );
	painter.fillRect( 3, 3, width() - 6, height() - 6, color );
}
