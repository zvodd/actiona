/*
	Actiona
	Copyright (C) 2008-2015 Jonathan Mercier-Ganady

	Actiona is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	Actiona is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program. If not, see <http://www.gnu.org/licenses/>.

	Contact : jmgr@jmgr.info
*/

#include "nativeeventfilteringapplication.h"
#include "nativeeventfilter.h"

namespace ActionTools
{
	NativeEventFilteringApplication::NativeEventFilteringApplication(const QString &appId, int &argc, char **argv)
		: QtSingleApplication(appId, argc, argv)
	{
    }

    NativeEventFilteringApplication::~NativeEventFilteringApplication()
    {
    }
	
	void NativeEventFilteringApplication::installNativeEventFilter(NativeEventFilter *filter)
	{
		if(!filter)
			return;
		
		mNativeFilters.removeAll(filter);
		mNativeFilters.prepend(filter);
	}

	void NativeEventFilteringApplication::removeNativeEventFilter(NativeEventFilter *filter)
	{
        mNativeFilters.removeAll(filter);
    }

#ifdef Q_OS_LINUX
	bool NativeEventFilteringApplication::x11EventFilter(XEvent *event)
	{
		for(NativeEventFilter *filter: mNativeFilters)
		{
			if(filter && filter->x11EventFilter(event))
				return true;
		}
		
		return QApplication::x11EventFilter(event);
	}
#endif
	
#ifdef Q_OS_WIN
	bool NativeEventFilteringApplication::winEventFilter(MSG *msg, long *result)
	{
		for(NativeEventFilter *filter: mNativeFilters)
		{
			if(filter && filter->winEventFilter(msg, result))
				return true;
		}
		
		return QApplication::winEventFilter(msg, result);
    }
#endif
}
