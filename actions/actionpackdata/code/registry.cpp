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

#include "registry.h"

#include <QtEndian>
#include <QDebug>

#ifdef Q_OS_WIN
#include <strsafe.h>
#endif

namespace Code
{
	QScriptValue Registry::constructor(QScriptContext *context, QScriptEngine *engine)
	{
		return CodeClass::constructor(new Registry, context, engine);
	}
	
	Registry::Registry()
		: CodeClass()
	#ifdef Q_OS_WIN
		, mHKey(0)
		, mRootKey(ClassesRoot)
	#endif
	{
	}
	
	Registry::~Registry()
	{
	#ifdef Q_OS_WIN
		RegCloseKey(mHKey);
#endif
    }
	
	QScriptValue Registry::openKey(Key key, const QString &subKey)
	{
	#ifdef Q_OS_WIN
		HKEY hKey = enumToKey(key);
	
		if(RegOpenKeyEx(hKey, subKey.toStdWString().c_str(), 0, KEY_ALL_ACCESS, &mHKey) != ERROR_SUCCESS)
			throwError("OpenKeyError", tr("Unable to open the key"));
		else
		{
			mRootKey = key;
			mSubKey = subKey;
		}
	#else
		Q_UNUSED(key)
		Q_UNUSED(subKey)
	#endif
		return thisObject();
	}
	
	QScriptValue Registry::createKey(Key key, const QString &subKey)
	{
	#ifdef Q_OS_WIN
		HKEY hKey = enumToKey(key);
	
		if(RegCreateKeyEx(hKey, subKey.toStdWString().c_str(), 0, 0, 0, KEY_ALL_ACCESS, 0, &mHKey, 0) != ERROR_SUCCESS)
			throwError("CreateKeyError", tr("Unable to create the key"));
		else
		{
			mRootKey = key;
			mSubKey = subKey;
		}
	#else
		Q_UNUSED(key)
		Q_UNUSED(subKey)
	#endif
		return thisObject();
	}
	
	QScriptValue Registry::setValue(const QString &value, const QVariant &data) const
	{
	#ifdef Q_OS_WIN
		std::wstring wideValue = value.toStdWString();
	
		switch(data.type())
		{
		case QVariant::Int:
		case QVariant::UInt:
			{
				int intData = data.toInt();
				if(RegSetValueEx(mHKey, wideValue.c_str(), 0, REG_DWORD, reinterpret_cast<LPBYTE>(&intData), sizeof(int)) != ERROR_SUCCESS)
					throwError("SetValueError", tr("Cannot set the value data"));
			}
			break;
		case QVariant::LongLong:
		case QVariant::ULongLong:
			{
				long long intData = data.toLongLong();
				if(RegSetValueEx(mHKey, wideValue.c_str(), 0, REG_QWORD, reinterpret_cast<LPBYTE>(&intData), sizeof(long long)) != ERROR_SUCCESS)
					throwError("SetValueError", tr("Cannot set the value data"));
			}
			break;
		case QVariant::StringList:
			{
				const QStringList &stringList = data.toStringList();
				std::wstring wideData;
	
                for(const QString &string: stringList)
				{
					wideData += string.toStdWString();
					wideData += L'\0';
				}
	
				if(RegSetValueEx(mHKey, wideValue.c_str(), 0, REG_MULTI_SZ, reinterpret_cast<LPBYTE>(const_cast<wchar_t*>(wideData.c_str())), static_cast<DWORD>(wideData.size() * sizeof(wchar_t))) != ERROR_SUCCESS)
					throwError("SetValueError", tr("Cannot set the value data"));
			}
			break;
		case QVariant::ByteArray:
			{
				QByteArray byteArray = data.toByteArray();
				if(RegSetValueEx(mHKey, wideValue.c_str(), 0, REG_BINARY, reinterpret_cast<LPBYTE>(byteArray.data()), static_cast<DWORD>(byteArray.size())) != ERROR_SUCCESS)
					throwError("SetValueError", tr("Cannot set the value data"));
			}
			break;
		default:
			{
				if(data.type() == QVariant::String || data.canConvert(QVariant::String))
				{
					std::wstring wideData = data.toString().toStdWString();
					if(RegSetValueEx(mHKey, wideValue.c_str(), 0, REG_SZ, reinterpret_cast<LPBYTE>(const_cast<wchar_t*>(wideData.c_str())), static_cast<DWORD>(wideData.size() * sizeof(wchar_t))) != ERROR_SUCCESS)
						throwError("SetValueError", tr("Cannot set the value data"));
				}
				else
					throwError("SetValueError", tr("Cannot set the value data"));
			}
			break;
		}
	#else
		Q_UNUSED(value)
		Q_UNUSED(data)
	#endif
		return thisObject();
	}
	
	QVariant Registry::value(const QString &value) const
	{
	#ifdef Q_OS_WIN
		DWORD size;
		DWORD type;
		std::wstring wideValue = value.toStdWString();
		if(RegQueryValueEx(mHKey, wideValue.c_str(), 0, &type, 0, &size) != ERROR_SUCCESS)
		{
			throwError("FindValueError", tr("Cannot find the value to read"));
			return QVariant();
		}
	
		switch(type)
		{
		case REG_DWORD:
		case REG_DWORD_BIG_ENDIAN:
			{
				qint32 value;
				if(RegQueryValueEx(mHKey, wideValue.c_str(), 0, 0, reinterpret_cast<LPBYTE>(&value), &size) != ERROR_SUCCESS)
				{
					throwError("FindValueError", tr("Cannot find the value to read"));
					return QVariant();
				}
	
				if(type == REG_DWORD_BIG_ENDIAN)
					value = qFromBigEndian(value);
	
				return value;
			}
			break;
		case REG_SZ:
		case REG_EXPAND_SZ:
		case REG_LINK:
		case REG_MULTI_SZ:
			{
				wchar_t *buffer = new wchar_t[size];
				if(RegQueryValueEx(mHKey, wideValue.c_str(), 0, 0, reinterpret_cast<LPBYTE>(buffer), &size) != ERROR_SUCCESS)
				{
					throwError("FindValueError", tr("Cannot find the value to read"));
					return QVariant();
				}
	
				if(type == REG_MULTI_SZ)
				{
					QStringList stringList = QString::fromWCharArray(buffer, size / 2).split(QChar(L'\0'), QString::SkipEmptyParts);
					delete [] buffer;
	
					if(stringList.last().isEmpty())
						stringList.removeLast();
	
					return stringList;
				}
				else
				{
					QString back = QString::fromWCharArray(buffer, size / 2);
					delete [] buffer;
	
					return back;
	
				}
			}
			break;
		case REG_BINARY:
			{
				char *buffer = new char[size];
				if(RegQueryValueEx(mHKey, wideValue.c_str(), 0, 0, reinterpret_cast<LPBYTE>(buffer), &size) != ERROR_SUCCESS)
				{
					throwError("FindValueError", tr("Cannot find the value to read"));
					return QVariant();
				}
	
				QByteArray back = QByteArray::fromRawData(buffer, size);
				delete [] buffer;
	
				return back;
			}
			break;
		case REG_QWORD:
			{
				qint64 value;
				if(RegQueryValueEx(mHKey, wideValue.c_str(), 0, 0, reinterpret_cast<LPBYTE>(&value), &size) != ERROR_SUCCESS)
				{
					throwError("FindValueError", tr("Cannot find the value to read"));
					return QVariant();
				}
	
				return value;
			}
			break;
		case REG_NONE:
		default:
			throwError("InvalidValueError", tr("Invalid value type"));
			return QVariant();
			break;
		}
	#else
		Q_UNUSED(value)
		
		return QVariant();
	#endif
	}
	
	QStringList Registry::valueNames() const
	{
	#ifdef Q_OS_WIN
		int index = 0;
		DWORD valueCount;
		DWORD maxValueNameLength;
	
		if(RegQueryInfoKey(mHKey, 0, 0, 0, 0, 0, 0, &valueCount, &maxValueNameLength, 0, 0, 0) != ERROR_SUCCESS)
		{
			throwError("InvalidKeyError", tr("Unable to query informations about this key"));
			return QStringList();
		}
	
		if(valueCount == 0 || maxValueNameLength == 0)
			return QStringList();
	
		wchar_t *valueName = new wchar_t[maxValueNameLength + 1];
		int result;
		QStringList back;
	
		for(;;++index)
		{
			DWORD valueNameSize = maxValueNameLength + 1;
	
			result = RegEnumValue(mHKey, index, valueName, &valueNameSize, 0, 0, 0, 0);
			if(result == ERROR_NO_MORE_ITEMS)
				break;
	
			if(valueNameSize == 0)
				continue;//Skip the default value
	
			back.append(QString::fromWCharArray(valueName, valueNameSize));
		}
	
		delete [] valueName;
	
		return back;
	#else
		return QStringList();
	#endif
	}
	
	QStringList Registry::keys() const
	{
	#ifdef Q_OS_WIN
		int index = 0;
		DWORD subKeyCount;
		DWORD maxSubKeyNameLength;
	
		if(RegQueryInfoKey(mHKey, 0, 0, 0, &subKeyCount, &maxSubKeyNameLength, 0, 0, 0, 0, 0, 0) != ERROR_SUCCESS)
		{
			throwError("InvalidKeyError", tr("Unable to query informations about this key"));
			return QStringList();
		}
	
		if(subKeyCount == 0 || maxSubKeyNameLength == 0)
			return QStringList();
	
		wchar_t *subKeyName = new wchar_t[maxSubKeyNameLength + 1];
		int result;
		QStringList back;
	
		for(;;++index)
		{
			DWORD subKeyNameSize = maxSubKeyNameLength + 1;
	
			result = RegEnumKeyEx(mHKey, index, subKeyName, &subKeyNameSize, 0, 0, 0, 0);
			if(result == ERROR_NO_MORE_ITEMS)
				break;
	
			back.append(QString::fromWCharArray(subKeyName, subKeyNameSize));
		}
	
		delete [] subKeyName;
	
		return back;
	#else
		return QStringList();
	#endif
	}
	
	QScriptValue Registry::deleteValue(const QString &value) const
	{
	#ifdef Q_OS_WIN
		if(RegDeleteValue(mHKey, value.toStdWString().c_str()) != ERROR_SUCCESS)
			throwError("InvalidKeyError", tr("Unable to delete the key"));
	#else
		Q_UNUSED(value)
	#endif
		return thisObject();
	}
	
	#ifdef Q_OS_WIN
	BOOL RegDelnodeRecurse (HKEY hKeyRoot, LPTSTR lpSubKey)
	{
		LPTSTR lpEnd;
		LONG lResult;
		DWORD dwSize;
		WCHAR szName[MAX_PATH];
		HKEY hKey;
		FILETIME ftWrite;
	
		// First, see if we can delete the key without having
		// to recurse.
	
		lResult = RegDeleteKey(hKeyRoot, lpSubKey);
	
		if (lResult == ERROR_SUCCESS)
			return TRUE;
	
		lResult = RegOpenKeyEx (hKeyRoot, lpSubKey, 0, KEY_READ, &hKey);
	
		if (lResult != ERROR_SUCCESS)
		{
			if (lResult == ERROR_FILE_NOT_FOUND) {
				printf("Key not found.\n");
				return TRUE;
			}
			else {
				printf("Error opening key.\n");
				return FALSE;
			}
		}
	
		// Check for an ending slash and add one if it is missing.
	
		lpEnd = lpSubKey + lstrlen(lpSubKey);
	
		if (*(lpEnd - 1) !=  TEXT('\\'))
		{
			*lpEnd =  TEXT('\\');
			lpEnd++;
			*lpEnd =  TEXT('\0');
		}
	
		// Enumerate the keys
	
		dwSize = MAX_PATH;
		lResult = RegEnumKeyEx(hKey, 0, szName, &dwSize, NULL,
							   NULL, NULL, &ftWrite);
	
		if (lResult == ERROR_SUCCESS)
		{
			do {
	
				StringCchCopy (lpEnd, MAX_PATH*2, szName);
	
				if (!RegDelnodeRecurse(hKeyRoot, lpSubKey)) {
					break;
				}
	
				dwSize = MAX_PATH;
	
				lResult = RegEnumKeyEx(hKey, 0, szName, &dwSize, NULL,
									   NULL, NULL, &ftWrite);
	
			} while (lResult == ERROR_SUCCESS);
		}
	
		lpEnd--;
		*lpEnd = TEXT('\0');
	
		RegCloseKey (hKey);
	
		// Try again to delete the key.
	
		lResult = RegDeleteKey(hKeyRoot, lpSubKey);
	
		if (lResult == ERROR_SUCCESS)
			return TRUE;
	
		return FALSE;
	}
	
	BOOL RegDelnode (HKEY hKeyRoot, LPCTSTR lpSubKey)
	{
		WCHAR szDelKey[MAX_PATH*2];
	
		StringCchCopy (szDelKey, MAX_PATH*2, lpSubKey);
		return RegDelnodeRecurse(hKeyRoot, szDelKey);
	
	}
	#endif
	
	QScriptValue Registry::deleteKey(Key key, const QString &subKey) const
	{
	#ifdef Q_OS_WIN
		HKEY hKey = enumToKey(key);
	
		if(!RegDelnode(hKey, subKey.toStdWString().c_str()))
			throwError("InvalidKeyError", tr("Unable to delete the key"));
	#else
		Q_UNUSED(key)
		Q_UNUSED(subKey)
	#endif
		return thisObject();
	}
	
	QScriptValue Registry::deleteKey() const
	{
	#ifdef Q_OS_WIN
		RegCloseKey(mHKey);
	
		deleteKey(mRootKey, mSubKey);
	#endif
		return thisObject();
	}
	
	QScriptValue Registry::closeKey() const
	{
	#ifdef Q_OS_WIN
		RegCloseKey(mHKey);
	#endif
		return thisObject();
	}
	
	#ifdef Q_OS_WIN
	HKEY Registry::enumToKey(Key key) const
	{
		switch(key)
		{
		case ClassesRoot:
			return HKEY_CLASSES_ROOT;
		case CurrentConfig:
			return HKEY_CURRENT_CONFIG;
		case CurrentUser:
			return HKEY_CURRENT_USER;
		case Users:
			return HKEY_USERS;
		case LocalMachine:
			return HKEY_LOCAL_MACHINE;
		default:
			return HKEY_CURRENT_USER;
		}
	}
	#endif
}
