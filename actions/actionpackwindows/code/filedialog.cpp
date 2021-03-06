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

#include "filedialog.h"
#include "code/codetools.h"

#include <QScriptValueIterator>
#include <QUrl>

namespace Code
{
	QScriptValue FileDialog::constructor(QScriptContext *context, QScriptEngine *engine)
	{
		FileDialog *fileDialog = new FileDialog;
		fileDialog->setupConstructorParameters(context, engine, context->argument(0));

		QScriptValueIterator it(context->argument(0));

		while(it.hasNext())
		{
			it.next();
			
			if(it.name() == "acceptMode")
				fileDialog->mFileDialog->setAcceptMode(static_cast<QFileDialog::AcceptMode>(it.value().toInt32()));
			else if(it.name() == "fileMode")
				fileDialog->mFileDialog->setFileMode(static_cast<QFileDialog::FileMode>(it.value().toInt32()));
			else if(it.name() == "viewMode")
				fileDialog->mFileDialog->setViewMode(static_cast<QFileDialog::ViewMode>(it.value().toInt32()));
			else if(it.name() == "selectedFile")
				fileDialog->mFileDialog->selectFile(it.value().toString());
			else if(it.name() == "selectedNameFilter")
				fileDialog->mFileDialog->selectNameFilter(it.value().toString());
			else if(it.name() == "defaultSuffix")
				fileDialog->mFileDialog->setDefaultSuffix(it.value().toString());
			else if(it.name() == "directory")
				fileDialog->mFileDialog->setDirectory(it.value().toString());
			else if(it.name() == "filter")
				fileDialog->mFileDialog->setFilter(static_cast<QDir::Filter>(it.value().toInt32()));
			else if(it.name() == "nameFilter")
				fileDialog->mFileDialog->setNameFilter(it.value().toString());
			else if(it.name() == "nameFilters")
				fileDialog->mFileDialog->setNameFilters(arrayParameterToStringList(it.value()));
			else if(it.name() == "showDirectoriesOnly")
				fileDialog->mFileDialog->setOption(QFileDialog::ShowDirsOnly, it.value().toBool());
			else if(it.name() == "dontResolveSymlinks")
				fileDialog->mFileDialog->setOption(QFileDialog::DontResolveSymlinks, it.value().toBool());
			else if(it.name() == "dontConfirmOverwrite")
				fileDialog->mFileDialog->setOption(QFileDialog::DontConfirmOverwrite, it.value().toBool());
			else if(it.name() == "dontUseNativeDialog")
				fileDialog->mFileDialog->setOption(QFileDialog::DontUseNativeDialog, it.value().toBool());
			else if(it.name() == "readOnly")
				fileDialog->mFileDialog->setOption(QFileDialog::ReadOnly, it.value().toBool());
			else if(it.name() == "hideNameFilterDetails")
				fileDialog->mFileDialog->setOption(QFileDialog::HideNameFilterDetails, it.value().toBool());
			else if(it.name() == "sidebarUrls")
			{
				QList<QUrl> urls;
				
                for(const QString &url: arrayParameterToStringList(it.value()))
					urls.append(QUrl::fromLocalFile(url));
					
				fileDialog->mFileDialog->setSidebarUrls(urls);
			}
			else if(it.name() == "onClosed")
				fileDialog->mOnClosed = it.value();
			else if(it.name() == "onCurrentChanged")
				fileDialog->mOnCurrentChanged = it.value();
			else if(it.name() == "onDirectoryEntered")
				fileDialog->mOnDirectoryEntered = it.value();
			else if(it.name() == "onFileSelected")
				fileDialog->mOnFileSelected = it.value();
			else if(it.name() == "onFilesSelected")
				fileDialog->mOnFilesSelected = it.value();
			else if(it.name() == "onFilterSelected")
				fileDialog->mOnFilterSelected = it.value();
		}

		return CodeClass::constructor(fileDialog, context, engine);
	}
	
	FileDialog::FileDialog()
		: BaseWindow(),
		mFileDialog(new QFileDialog)
	{
        mFileDialog->setWindowFlags(mFileDialog->windowFlags() | Qt::WindowContextHelpButtonHint);

		setWidget(mFileDialog);
		
		connect(mFileDialog, SIGNAL(finished(int)), this, SLOT(finished(int)));
		connect(mFileDialog, SIGNAL(currentChanged(QString)), this, SLOT(currentChanged(QString)));
		connect(mFileDialog, SIGNAL(directoryEntered(QString)), this, SLOT(directoryEntered(QString)));
		connect(mFileDialog, SIGNAL(fileSelected(QString)), this, SLOT(fileSelected(QString)));
		connect(mFileDialog, SIGNAL(filesSelected(QStringList)), this, SLOT(filesSelected(QStringList)));
		connect(mFileDialog, SIGNAL(filterSelected(QString)), this, SLOT(filterSelected(QString)));
	}
	
	FileDialog::~FileDialog()
	{
		delete mFileDialog;
	}
	
	QScriptValue FileDialog::setAcceptMode(AcceptMode acceptMode)
	{
		mFileDialog->setAcceptMode(static_cast<QFileDialog::AcceptMode>(acceptMode));
		
		return thisObject();
	}

	QScriptValue FileDialog::setFileMode(FileMode fileMode)
	{
		mFileDialog->setFileMode(static_cast<QFileDialog::FileMode>(fileMode));
				
		return thisObject();
	}

	QScriptValue FileDialog::setViewMode(ViewMode viewMode)
	{
		mFileDialog->setViewMode(static_cast<QFileDialog::ViewMode>(viewMode));
		
		return thisObject();
	}

	QScriptValue FileDialog::setLabelText(DialogLabel dialogLabel, const QString &text)
	{
		mFileDialog->setLabelText(static_cast<QFileDialog::DialogLabel>(dialogLabel), text);
		
		return thisObject();
	}

	QScriptValue FileDialog::selectFile(const QString &filename)
	{
		mFileDialog->selectFile(filename);
		
		return thisObject();
	}

	QScriptValue FileDialog::selectNameFilter(const QString &filter)
	{
		mFileDialog->selectNameFilter(filter);
		
		return thisObject();
	}

	QScriptValue FileDialog::setDefaultSuffix(const QString &defaultSuffix)
	{
		mFileDialog->setDefaultSuffix(defaultSuffix);
		
		return thisObject();
	}

	QScriptValue FileDialog::setDirectory(const QString &directory)
	{
		mFileDialog->setDirectory(directory);
		
		return thisObject();
	}

	QScriptValue FileDialog::setFilter(Filters filters)
	{
		mFileDialog->setFilter(static_cast<QDir::Filters>(filters));
		
		return thisObject();
	}

	QScriptValue FileDialog::setNameFilter(const QString &nameFilter)
	{
		mFileDialog->setNameFilter(nameFilter);
		
		return thisObject();
	}

	QScriptValue FileDialog::setNameFilters(const QScriptValue &nameFilters)
	{
		mFileDialog->setNameFilters(arrayParameterToStringList(nameFilters));
		
		return thisObject();
	}

	QScriptValue FileDialog::setShowDirectoriesOnly(bool showDirectoriesOnly)
	{
		mFileDialog->setOption(QFileDialog::ShowDirsOnly, showDirectoriesOnly);
		
		return thisObject();
	}

	QScriptValue FileDialog::setDontResolveSymlinks(bool dontResolveSymlinks)
	{
		mFileDialog->setOption(QFileDialog::DontResolveSymlinks, dontResolveSymlinks);
		
		return thisObject();
	}

	QScriptValue FileDialog::setDontConfirmOverwrite(bool dontConfirmOverwrite)
	{
		mFileDialog->setOption(QFileDialog::DontConfirmOverwrite, dontConfirmOverwrite);
		
		return thisObject();
	}

	QScriptValue FileDialog::setDontUseNativeDialog(bool dontUseNativeDialog)
	{
		mFileDialog->setOption(QFileDialog::DontUseNativeDialog, dontUseNativeDialog);
		
		return thisObject();
	}

	QScriptValue FileDialog::setReadOnly(bool readOnly)
	{
		mFileDialog->setOption(QFileDialog::ReadOnly, readOnly);
		
		return thisObject();
	}

	QScriptValue FileDialog::setHideNameFilterDetails(bool hideNameFilterDetails)
	{
		mFileDialog->setOption(QFileDialog::HideNameFilterDetails, hideNameFilterDetails);
		
		return thisObject();
	}

	QScriptValue FileDialog::setSidebarUrls(const QScriptValue &sidebarUrls)
	{
		QList<QUrl> urls;
		
        for(const QString &url: arrayParameterToStringList(sidebarUrls))
			urls.append(QUrl::fromLocalFile(url));
			
		mFileDialog->setSidebarUrls(urls);
		
		return thisObject();
	}

	QString FileDialog::selectedFile() const
	{
		const QStringList &selectedFiles = mFileDialog->selectedFiles();
		
		if(selectedFiles.count() > 0)
			return selectedFiles.at(0);
		
		return QString();
	}
	
	QScriptValue FileDialog::selectedFiles() const
	{
        return qScriptValueFromSequence(engine(), mFileDialog->selectedFiles());
	}
	
	QString FileDialog::selectedNameFilter() const
	{
		return mFileDialog->selectedNameFilter();
	}
	
	QScriptValue FileDialog::show()
	{
		mFileDialog->open();

		return thisObject();
	}

	int FileDialog::showModal()
	{
		return mFileDialog->exec();
	}

	void FileDialog::finished(int result)
	{
		if(mOnClosed.isValid())
			mOnClosed.call(thisObject(), QScriptValueList() << result);
	}

	void FileDialog::currentChanged(const QString &path)
	{
		if(mOnCurrentChanged.isValid())
			mOnCurrentChanged.call(thisObject(), QScriptValueList() << path);
	}

	void FileDialog::directoryEntered(const QString &directory)
	{
		if(mOnDirectoryEntered.isValid())
			mOnDirectoryEntered.call(thisObject(), QScriptValueList() << directory);
	}

	void FileDialog::fileSelected(const QString &file)
	{
		if(mOnFileSelected.isValid())
			mOnFileSelected.call(thisObject(), QScriptValueList() << file);
	}

	void FileDialog::filesSelected(const QStringList &files)
	{
		if(mOnFilesSelected.isValid())
            mOnFilesSelected.call(thisObject(), QScriptValueList() << qScriptValueFromSequence(engine(), files));
	}

	void FileDialog::filterSelected(const QString &filter)
	{
		if(mOnFilterSelected.isValid())
			mOnFilterSelected.call(thisObject(), QScriptValueList() << filter);
	}
}
