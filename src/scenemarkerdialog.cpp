// This file is part of Agros2D.
//
// Agros2D is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 2 of the License, or
// (at your option) any later version.
//
// Agros2D is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Agros2D.  If not, see <http://www.gnu.org/licenses/>.
//
// hp-FEM group (http://hpfem.org/)
// University of Nevada, Reno (UNR) and University of West Bohemia, Pilsen
// Email: agros2d@googlegroups.com, home page: http://hpfem.org/agros2d/

#include "scenemarker.h"
#include "scenemarkerdialog.h"
#include "hermes2d/module.h"
#include "hermes2d/module_agros.h"
#include "hermes2d/field.h"
//#include "hermes2d/problem.h"
#include "scene.h"
#include "scenebasic.h"
#include "sceneedge.h"
#include "scenelabel.h"
#include "gui.h"

SceneBoundary::SceneBoundary(FieldInfo *fieldInfo, std::string name, std::string type,
                             std::map<std::string, Value> values)
    : Boundary(fieldInfo, name, type, values)
{

}

int SceneBoundary::showDialog(QWidget *parent)
{
    SceneBoundaryDialog *dialog = new SceneBoundaryDialog(this, parent);
    if (dialog)
        return dialog->exec();
    else
        QMessageBox::information(QApplication::activeWindow(), QObject::tr(""), QObject::tr("Boundary dialog doesn't exists."));
}

QString SceneBoundary::html()
{
    //TODO
    QString out;
    //    out += "<h4>" + QString::fromStdString(Util::problem()->config()->module()->name) + "</h4>";
    //    out += "<table>";
    
    //    Hermes::Module::BoundaryType *boundary_type = Util::problem()->config()->module()->get_boundary_type(type);
    //    if (boundary_type)
    //        for (Hermes::vector<Hermes::Module::BoundaryTypeVariable *>::iterator it = boundary_type->variables.begin(); it < boundary_type->variables.end(); ++it)
    //        {
    //            Hermes::Module::BoundaryTypeVariable *variable = ((Hermes::Module::BoundaryTypeVariable *) *it);

    //            out += "<tr>";
    ////            out += QString("<td>%1 (%2):</td>").
    ////                    arg(QString::fromStdString(variable->name)).
    ////                    arg(QString::fromStdString(variable->unit));
    //            out += QString("<td>%1</td>").
    //                    arg(values[variable->id].text());
    //            out += "</tr>";
    //        }
    
    //    out += "</table>";
    
    return out;
}

QVariant SceneBoundary::variant()
{
    QVariant v;
    v.setValue(this);
    return v;
}

SceneBoundaryNone::SceneBoundaryNone() : SceneBoundary(NULL, "none")
{

}

// *************************************************************************************************************************************

SceneMaterial::SceneMaterial(FieldInfo *fieldInfo, std::string name,
                             std::map<std::string, Value> values) : Material(fieldInfo, name, values)
{

}

int SceneMaterial::showDialog(QWidget *parent)
{
    SceneMaterialDialog *dialog = new SceneMaterialDialog(this, parent);
    if (dialog)
        return dialog->exec();
    else
        QMessageBox::information(QApplication::activeWindow(), QObject::tr(""), QObject::tr("Material dialog doesn't exists."));

}

QString SceneMaterial::html()
{
    //TODO
    QString out;
    
    //    out += "<h4>" + QString::fromStdString(Util::problem()->config()->module()->name) + "</h4>";
    //    out += "<table>";
    
    //    for (Hermes::vector<Hermes::Module::MaterialTypeVariable *>::iterator it = Util::problem()->config()->module()->material_type_variables.begin();
    //         it < Util::problem()->config()->module()->material_type_variables.end(); ++it )
    //    {
    //        Hermes::Module::MaterialTypeVariable *material = ((Hermes::Module::MaterialTypeVariable *) *it);

    //        out += "<tr>";
    ////        out += QString("<td>%1 (%2)</td>").
    ////                arg(QString::fromStdString(material->name)).
    ////                arg(QString::fromStdString(material->unit));
    //        // FIXME - add value
    //        out += QString("<td>%1</td>").
    //                arg(QString::fromStdString("FIXME"));
    //        out += "</tr>";
    //    }
    //    out += "</table>";
    
    return out;
}

QVariant SceneMaterial::variant()
{
    QVariant v;
    v.setValue(this);
    return v;
}

SceneMaterialNone::SceneMaterialNone() : SceneMaterial(NULL, "none")
{

}

// *************************************************************************************************************************************


SceneFieldWidget::SceneFieldWidget(Hermes::Module::DialogUI *ui, QWidget *parent)
    : QWidget(parent), ui(ui)
{
}

void SceneFieldWidget::createContent()
{
    // widget layout
    layout = new QVBoxLayout();
    setLayout(layout);

    // add custom widget
    addCustomWidget(layout);

    // equation
    equationImage = new QLabel();
    equationImage->setMinimumHeight(fontMetrics().height()*2.2);
    equationImage->setMinimumWidth(250);

    QGridLayout *layoutEquation = new QGridLayout();
    layoutEquation->addWidget(new QLabel(tr("Equation:")), 0, 0);
    layoutEquation->addWidget(equationImage, 0, 1, 1, 1, Qt::AlignRight);
    layout->addLayout(layoutEquation);

    for (std::map<std::string, Hermes::vector<Hermes::Module::DialogUI::Row> >::iterator it = ui->groups.begin(); it != ui->groups.end(); ++it)
    {
        // group layout
        QGridLayout *layoutGroup = new QGridLayout();

        // variables
        Hermes::vector<Hermes::Module::DialogUI::Row> variables = it->second;

        for (Hermes::vector<Hermes::Module::DialogUI::Row>::iterator
             itv = variables.begin(); itv < variables.end(); ++itv)
        {
            Hermes::Module::DialogUI::Row material = *itv;

            // id
            ids.append(QString::fromStdString(material.id));

            // label
            labels.append(new QLabel(QString("%1 (%2):").
                                     arg(QString::fromStdString(material.shortname_html)).
                                     arg(QString::fromStdString(material.unit_html))));
            labels.at(labels.count() - 1)->setToolTip((QString::fromStdString(material.name)));
            labels.at(labels.count() - 1)->setMinimumWidth(100);

            // text edit
            values.append(new ValueLineEdit(this, material.timedep, material.nonlin));
            values.at(values.count() - 1)->setValue(Value(QString::number(material.default_value)));

            int index = layoutGroup->rowCount();
            layoutGroup->addWidget(labels.at(labels.count() - 1), index, 0);
            layoutGroup->addWidget(values.at(values.count() - 1), index, 1);
        }

        // widget layout
        if (it->first == "")
        {
            layout->addLayout(layoutGroup);
        }
        else
        {
            QGroupBox *grpGroup = new QGroupBox(QString::fromStdString(it->first));
            grpGroup->setLayout(layoutGroup);
            layout->addWidget(grpGroup);
        }
    }

    refresh();
}

// *************************************************************************************************************************************

SceneFieldWidgetMaterial::SceneFieldWidgetMaterial(Hermes::Module::DialogUI *ui, SceneMaterial *material, QWidget *parent)
    : SceneFieldWidget(ui, parent), material(material)
{
}

void SceneFieldWidgetMaterial::load()
{
    for (int j = 0; j < ids.count(); j++)
        values.at(j)->setValue(material->getValue(ids.at(j).toStdString()));
}

bool SceneFieldWidgetMaterial::save()
{
    for (int j = 0; j < ids.count(); j++)
        if (values.at(j)->evaluate())
            material->setValue(ids.at(j).toStdString(), values.at(j)->value());
        else
            return false;

    return true;
}

void SceneFieldWidgetMaterial::refresh()
{
    // read equation
    QString fileName = QString(":/equations/%1/%1_%2.png")
            .arg(material->getFieldInfo()->fieldId())
            .arg(analysisTypeToStringKey(material->getFieldInfo()->analysisType()));

    readPixmap(equationImage, fileName);
}

// *************************************************************************************************************************************

SceneFieldWidgetBoundary::SceneFieldWidgetBoundary(Hermes::Module::DialogUI *ui, SceneBoundary *boundary, QWidget *parent)
    : SceneFieldWidget(ui, parent), boundary(boundary)
{
}

void SceneFieldWidgetBoundary::addCustomWidget(QVBoxLayout *layout)
{
    comboBox = new QComboBox(this);
    boundary->getFieldInfo()->module()->fillComboBoxBoundaryCondition(comboBox);
    connect(comboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(doTypeChanged(int)));

    QFormLayout *layoutForm = new QFormLayout();
    layoutForm->addRow(tr("Type:"), comboBox);

    layout->addLayout(layoutForm);
}

void SceneFieldWidgetBoundary::refresh()
{
    // set active marker
    doTypeChanged(comboBox->currentIndex());
}

void SceneFieldWidgetBoundary::doTypeChanged(int index)
{
    setMinimumSize(sizeHint());

    // disable variables
    for (int i = 0; i < ids.count(); i++)
        values.at(i)->setEnabled(false);

    Hermes::Module::BoundaryType *boundary_type = boundary->getFieldInfo()->module()->get_boundary_type(comboBox->itemData(index).toString().toStdString());
    for (Hermes::vector<Hermes::Module::BoundaryTypeVariable *>::iterator it = boundary_type->variables.begin(); it < boundary_type->variables.end(); ++it)
    {
        Hermes::Module::BoundaryTypeVariable *variable = ((Hermes::Module::BoundaryTypeVariable *) *it);

        int i = ids.indexOf(QString::fromStdString(variable->id));

        if (i >= 0)
            values.at(i)->setEnabled(true);
    }

    // read equation
    QString fileName = QString(":/equations/%1/%2.png")
            .arg(boundary->getFieldInfo()->fieldId())
            .arg(comboBox->itemData(index).toString());

    readPixmap(equationImage, fileName);

    // sizehint
    setMinimumSize(sizeHint());
}

void SceneFieldWidgetBoundary::load()
{
    // load type
    comboBox->setCurrentIndex(comboBox->findData(QString::fromStdString(boundary->getType())));

    // load variables
    for (int i = 0; i < ids.count(); i++)
        values.at(i)->setValue(boundary->getValue(ids.at(i).toStdString()));
}

bool SceneFieldWidgetBoundary::save()
{
    // save type
    boundary->setType(comboBox->itemData(comboBox->currentIndex()).toString().toStdString());

    // save variables
    for (int i = 0; i < ids.count(); i++)
        if (values.at(i)->evaluate())
            boundary->setValue(ids.at(i).toStdString(), values.at(i)->value());
        else
            return false;

    return true;
}

// *************************************************************************************************************************************

SceneBoundaryDialog::SceneBoundaryDialog(SceneBoundary *boundary, QWidget *parent)
    : QDialog(parent), boundary(boundary)
{
    setWindowIcon(icon("scene-edgemarker"));
    setWindowTitle(tr("Boundary condition - %1").arg(QString::fromStdString(boundary->getFieldInfo()->module()->name)));

    layout = new QGridLayout();
    txtName = new QLineEdit(this);

    createDialog();

    load();
    setSize();
}

void SceneBoundaryDialog::createDialog()
{
    // dialog buttons
    buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    connect(buttonBox, SIGNAL(accepted()), this, SLOT(doAccept()));
    connect(buttonBox, SIGNAL(rejected()), this, SLOT(doReject()));
    
    layout->addWidget(new QLabel(tr("Name:")), 0, 0);
    layout->addWidget(txtName, 0, 2);
    
    // content
    createContent();
    
    layout->addWidget(buttonBox, 100, 0, 1, 3);
    layout->setRowStretch(99, 1);
    
    txtName->setFocus();
    
    setLayout(layout);
}

void SceneBoundaryDialog::createContent()
{
    fieldWidget = new SceneFieldWidgetBoundary(&boundary->getFieldInfo()->module()->boundary_ui, boundary, this);
    fieldWidget->createContent();
    fieldWidget->setMinimumSize(sizeHint());

    layout->addWidget(fieldWidget, 10, 0, 1, 3);
}

void SceneBoundaryDialog::load()
{
    txtName->setText(QString::fromStdString(boundary->getName()));

    // load variables
    fieldWidget->load();
}

bool SceneBoundaryDialog::save()
{
    // find name duplicities
    foreach (SceneBoundary *boundary, Util::scene()->boundaries->items())
    {
        if (QString::fromStdString(boundary->getName()) == txtName->text())
        {
            if (boundary == boundary)
                continue;

            QMessageBox::warning(this, tr("Boundary marker"), tr("Boundary marker name already exists."));
            return false;
        }
    }
    boundary->setName(txtName->text().toStdString());

    // save variables
    if (!fieldWidget->save())
        return false;

    return true;
}

void SceneBoundaryDialog::setSize()
{
    setMinimumSize(sizeHint());
}

void SceneBoundaryDialog::doAccept()
{
    if (save())
        accept();
}

void SceneBoundaryDialog::doReject()
{
    reject();
}

void SceneBoundaryDialog::evaluated(bool isError)
{
    buttonBox->button(QDialogButtonBox::Ok)->setEnabled(!isError);
}

// *************************************************************************************************************************************

SceneMaterialDialog::SceneMaterialDialog(SceneMaterial *material, QWidget *parent)
    : QDialog(parent), material(material)
{
    setWindowIcon(icon("scene-labelmarker"));
    setWindowTitle(tr("Material - %1").arg(QString::fromStdString(material->getFieldInfo()->module()->name)));

    layout = new QGridLayout();
    txtName = new QLineEdit(this);

    createDialog();

    load();
    setSize();
}

void SceneMaterialDialog::createDialog()
{
    // dialog buttons
    buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    connect(buttonBox, SIGNAL(accepted()), this, SLOT(doAccept()));
    connect(buttonBox, SIGNAL(rejected()), this, SLOT(doReject()));
    
    // name
    layout->addWidget(new QLabel(tr("Name:")), 0, 0, 1, 2);
    layout->addWidget(txtName, 0, 2);
        
    // content
    createContent();
    
    layout->addWidget(buttonBox, 100, 0, 1, 3);
    layout->setRowStretch(99, 1);
    
    txtName->setFocus();
    
    setLayout(layout);
}

void SceneMaterialDialog::createContent()
{
    fieldWidget = new SceneFieldWidgetMaterial(&material->getFieldInfo()->module()->material_ui, material, this);
    fieldWidget->createContent();
    fieldWidget->setMinimumSize(sizeHint());

    layout->addWidget(fieldWidget, 10, 0, 1, 3);
}

void SceneMaterialDialog::load()
{
    txtName->setText(QString::fromStdString(material->getName()));

    // load variables
    fieldWidget->load();
}

bool SceneMaterialDialog::save()
{
    // find name duplicities
    foreach (SceneMaterial *material, Util::scene()->materials->items())
    {
        if (material->getName() == txtName->text().toStdString())
        {
            if (material == material)
                continue;
            
            QMessageBox::warning(this, tr("Material marker"), tr("Material marker name already exists."));
            return false;
        }
    }
    material->setName(txtName->text().toStdString());

    // save variables
    if (!fieldWidget->save())
        return false;

    return true;
}


void SceneMaterialDialog::setSize()
{
    setMinimumSize(sizeHint());
}

void SceneMaterialDialog::doAccept()
{
    if (save())
        accept();
}

void SceneMaterialDialog::doReject()
{
    reject();
}

void SceneMaterialDialog::evaluated(bool isError)
{
    buttonBox->button(QDialogButtonBox::Ok)->setEnabled(!isError);
}
