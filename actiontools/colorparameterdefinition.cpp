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

#include "colorparameterdefinition.h"
#include "subparameter.h"
#include "coloredit.h"
#include "actioninstance.h"

namespace ActionTools
{
    ColorParameterDefinition::ColorParameterDefinition(const Name &name, QObject *parent)
        : ParameterDefinition(name, parent),
		mColorEdit(0)
	{
	}

	void ColorParameterDefinition::buildEditors(Script *script, QWidget *parent)
	{
		ParameterDefinition::buildEditors(script, parent);

		mColorEdit = new ColorEdit(parent);

		addEditor(mColorEdit);
	}

	void ColorParameterDefinition::load(const ActionInstance *actionInstance)
	{
		mColorEdit->setFromSubParameter(actionInstance->subParameter(name().original(), "value"));
	}

	void ColorParameterDefinition::save(ActionInstance *actionInstance)
	{
		actionInstance->setSubParameter(name().original(), "value", mColorEdit->isCode(), mColorEdit->text());
	}
}
