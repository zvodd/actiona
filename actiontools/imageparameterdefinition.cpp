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

#include "imageparameterdefinition.h"
#include "screenshotpushbutton.h"
#include "screenshotwizard.h"

namespace ActionTools
{
    ImageParameterDefinition::ImageParameterDefinition(const Name &name, QObject *parent)
      : FileParameterDefinition(name, parent),
        mScript(0),
        mParent(0)
    {
    }

    void ImageParameterDefinition::buildEditors(Script *script, QWidget *parent)
    {
        FileParameterDefinition::buildEditors(script, parent);

        mScript = script;// Is this really safe?
        mParent = parent;

        ScreenshotPushButton *screenshotPushButton = new ScreenshotPushButton(parent);
        connect(screenshotPushButton, SIGNAL(clicked()), this, SLOT(onCaptureClicked()));

        addEditor(screenshotPushButton);
    }

    void ImageParameterDefinition::onCaptureClicked()
    {
        ScreenshotWizard screenshotWizard(mScript, false, mParent);

        screenshotWizard.setWindowFlags(screenshotWizard.windowFlags() | Qt::WindowContextHelpButtonHint);

        if(screenshotWizard.exec() == QDialog::Accepted)
        {
            switch(screenshotWizard.saveTarget())
            {
            case ScreenshotWizard::ResourceSaveTarget:
                fileEdit()->setText("$" + screenshotWizard.resourceOrFilename());
                break;
            case ScreenshotWizard::FileSaveTarget:
                fileEdit()->setText(screenshotWizard.resourceOrFilename());
                break;
            default:
                return;
            }
        }
    }
}
