/*
 * Hydrogen
 * Copyright(c) 2002-2008 by Alex >Comix< Cominu [comix@users.sourceforge.net]
 * Copyright(c) 2008-2021 The hydrogen development team [hydrogen-devel@lists.sourceforge.net]
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

#include "PluginTest.h"

#include <unistd.h>

#include <core/Basics/Drumkit.h>
#include <core/Basics/Pattern.h>
#include <core/Basics/Instrument.h>
#include <core/Basics/InstrumentList.h>
#include <core/Basics/InstrumentLayer.h>
#include <core/Basics/InstrumentComponent.h>
#include <core/Basics/Sample.h>
#include <core/Basics/Playlist.h>

#include <core/Helpers/Filesystem.h>
#include <core/Helpers/Xml.h>
#include "TestHelper.h"

CPPUNIT_TEST_SUITE_REGISTRATION( PluginTest );

void PluginTest::testCompiledWindowsLadspaBundle()
{
#ifdef WIN32
  std::vector<H2Core::LadspaFXInfo*> pluginList =
    H2Core::Effects::get_instance()->getPluginList();

  qDebug() << "number of plugins: " << pluginList.size();

  // Number of plugins installed with Hydrogen on Windows
  CPPUNIT_ASSERT( pluginList.size() == 31 );
#endif
}
