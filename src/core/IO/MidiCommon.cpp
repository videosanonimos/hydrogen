/*
 * Hydrogen
 * Copyright(c) 2023-2023 The hydrogen development team [hydrogen-devel@lists.sourceforge.net]
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
 * along with this program. If not, see https://www.gnu.org/licenses
 *
 */

#include <core/IO/MidiCommon.h>

namespace H2Core
{

void MidiMessage::clear() {
	m_type = UNKNOWN;
	m_nData1 = -1;
	m_nData2 = -1;
	m_nChannel = -1;
	m_sysexData.clear();
}

void MidiMessage::setType( int nStatusByte ) {

	if ( ( nStatusByte >= 128 ) && ( nStatusByte < 144 ) ) {
		m_nChannel = nStatusByte - 128;
		m_type = MidiMessage::NOTE_OFF;
	}
	else if ( ( nStatusByte >= 144 ) && ( nStatusByte < 160 ) ) {
		m_nChannel = nStatusByte - 144;
		m_type = MidiMessage::NOTE_ON;
	}
	else if ( ( nStatusByte >= 160 ) && ( nStatusByte < 176 ) ) {
		m_nChannel = nStatusByte - 160;
		m_type = MidiMessage::POLYPHONIC_KEY_PRESSURE;
	}
	else if ( ( nStatusByte >= 176 ) && ( nStatusByte < 192 ) ) {
		m_nChannel = nStatusByte - 176;
		m_type = MidiMessage::CONTROL_CHANGE;
	}
	else if ( ( nStatusByte >= 192 ) && ( nStatusByte < 208 ) ) {
		m_nChannel = nStatusByte - 192;
		m_type = MidiMessage::PROGRAM_CHANGE;
	}
	else if ( ( nStatusByte >= 208 ) && ( nStatusByte < 224 ) ) {
		m_nChannel = nStatusByte - 208;
		m_type = MidiMessage::CHANNEL_PRESSURE;
	}
	else if ( ( nStatusByte >= 224 ) && ( nStatusByte < 240 ) ) {
		m_nChannel = nStatusByte - 224;
		m_type = MidiMessage::PITCH_WHEEL;
	}
	// System Common Messages
	else if ( ( nStatusByte == 240 ) ) {
		m_nChannel = nStatusByte - 224;
		m_type = MidiMessage::SYSEX;
	}
	else if ( ( nStatusByte == 241 ) ) {
		m_type = MidiMessage::QUARTER_FRAME;
	}
	else if ( ( nStatusByte == 242 ) ) {
		m_type = MidiMessage::SONG_POS;
	}
	else if ( ( nStatusByte == 243 ) ) {
		m_type = MidiMessage::SONG_SELECT;
	}
	// 244, 245 are undefined/reserved
	else if ( ( nStatusByte == 246 ) ) {
		m_type = MidiMessage::TUNE_REQUEST;
	}
	// 247 indicates the end of a SysEx (240) message
	// System Realtime Messages
	else if ( ( nStatusByte == 248 ) ) {
		m_type = MidiMessage::TIMING_CLOCK;
	}
	// 249 is undefined/reserved
	else if ( ( nStatusByte == 250 ) ) {
		m_type = MidiMessage::START;
	}
	else if ( ( nStatusByte == 251 ) ) {
		m_type = MidiMessage::CONTINUE;
	}
	else if ( ( nStatusByte == 252 ) ) {
		m_type = MidiMessage::STOP;
	}
	// 253 is undefined/reserved
	else if ( ( nStatusByte == 254 ) ) {
		m_type = MidiMessage::ACTIVE_SENSING;
	}
	else if ( ( nStatusByte == 255 ) ) {
		m_type = MidiMessage::RESET;
	}
}

QString MidiMessage::toQString( const QString& sPrefix, bool bShort ) const {

	QString s = Base::sPrintIndention;
	QString sOutput;
	if ( ! bShort ) {
		sOutput = QString( "%1[MidiMessage]\n" ).arg( sPrefix )
			.append( QString( "%1%2m_type: %3\n" )
					 .arg( MidiMessage::TypeToQString( m_type ) ) )
			.append( QString( "%1%2m_nData1: %3\n" )
					 .arg( m_nData1 ) )
			.append( QString( "%1%2m_nData2: %3\n" )
					 .arg( m_nData2 ) )
			.append( QString( "%1%2m_nChannel: %3\n" )
					 .arg( m_nChannel ) )
			.append( QString( "%1%2m_sysexData: [" ) );
		bool bIsFirst = true;
		for ( const auto& dd : m_sysexData ) {
			if ( bIsFirst ) {
				sOutput.append( QString( "%1" ).arg( dd ) );
				bIsFirst = false;
			}
			else {
				sOutput.append( QString( " %1" ).arg( dd ) );
			}
		}
		sOutput.append( "]" );
	}
	else {
		sOutput = QString( "[MidiMessage] " )
			.append( QString( "m_type: %1" ).arg( MidiMessage::TypeToQString( m_type ) ) )
			.append( QString( ", m_nData1: %1" ).arg( m_nData1 ) )
			.append( QString( ", m_nData2: %1" ).arg( m_nData2 ) )
			.append( QString( ", m_nChannel: %1" ).arg( m_nChannel ) )
			.append( QString( ", m_sysexData: [" ) );
		bool bIsFirst = true;
		for ( const auto& dd : m_sysexData ) {
			if ( bIsFirst ) {
				sOutput.append( QString( "%1" ).arg( dd ) );
				bIsFirst = false;
			}
			else {
				sOutput.append( QString( " %1" ).arg( dd ) );
			}
		}
		sOutput.append( "]" );
	}
	
	return sOutput;
}

QString MidiMessage::TypeToQString( MidiMessageType type ) {
	QString sType;
	switch( type ) {
	case MidiMessageType::SYSEX:
		sType = "SYSEX";
		break;
	case MidiMessageType::NOTE_ON:
		sType = "NOTE_ON";
		break;
	case MidiMessageType::NOTE_OFF:
		sType = "NOTE_OFF";
		break;
	case MidiMessageType::POLYPHONIC_KEY_PRESSURE:
		sType = "POLYPHONIC_KEY_PRESSURE";
		break;
	case MidiMessageType::CONTROL_CHANGE:
		sType = "CONTROL_CHANGE";
		break;
	case MidiMessageType::PROGRAM_CHANGE:
		sType = "PROGRAM_CHANGE";
		break;
	case MidiMessageType::CHANNEL_PRESSURE:
		sType = "CHANNEL_PRESSURE";
		break;
	case MidiMessageType::PITCH_WHEEL:
		sType = "PITCH_WHEEL";
		break;
	case MidiMessageType::START:
		sType = "START";
		break;
	case MidiMessageType::CONTINUE:
		sType = "CONTINUE";
		break;
	case MidiMessageType::STOP:
		sType = "STOP";
		break;
	case MidiMessageType::SONG_POS:
		sType = "SONG_POS";
		break;
	case MidiMessageType::QUARTER_FRAME:
		sType = "QUARTER_FRAME";
		break;
	case MidiMessageType::UNKNOWN:
	default:
		sType = "Unknown MIDI message type";
	}

	return std::move( sType );
}

};

