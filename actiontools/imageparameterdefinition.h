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

#ifndef IMAGEPARAMETERDEFINITION_H
#define IMAGEPARAMETERDEFINITION_H

#include "fileparameterdefinition.h"
#include "actiontools_global.h"

namespace ActionTools
{
    class ACTIONTOOLSSHARED_EXPORT ImageParameterDefinition : public FileParameterDefinition
    {
        Q_OBJECT

    public:
        ImageParameterDefinition(const Name &name, QObject *parent);

        void buildEditors(Script *script, QWidget *parent);

    private slots:
        void onCaptureClicked();

    private:
        Script *mScript;
        QWidget *mParent;

        Q_DISABLE_COPY(ImageParameterDefinition)
    };
}

#endif // IMAGEPARAMETERDEFINITION_H
