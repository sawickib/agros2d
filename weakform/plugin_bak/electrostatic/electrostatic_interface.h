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

#ifndef WEAKFORM_FACTORY_H
#define WEAKFORM_FACTORY_H

#include <QObject>

#include "util.h"
#include "hermes2d.h"

#include "scene.h"
#include "scenebasic.h"
#include "scenemarker.h"
#include "scenemarkerdialog.h"

#include "hermes2d/localpoint.h"
#include "hermes2d/volumeintegral.h"
#include "hermes2d/surfaceintegral.h"

#include "hermes2d/marker.h"
#include "hermes2d/localpoint.h"
#include "hermes2d/weakform_parser.h"
#include "hermes2d/weakform_interface.h"

class ElectrostaticInterface : public QObject, public WeakFormInterface
{
    Q_OBJECT
    Q_INTERFACES(WeakFormInterface)

public:

    virtual ~ElectrostaticInterface() {}

    inline virtual QString fieldId() { return "electrostatic"; }

    virtual Hermes::Hermes2D::MatrixFormVol<double> *matrixFormVol(const ProblemID problemId, int i, int j,
                                                                  const std::string &area, Hermes::Hermes2D::SymFlag sym,
                                                                  SceneMaterial *materialSource, Material *materialTarget, int offsetI, int offsetJ );    
    virtual Hermes::Hermes2D::VectorFormVol<double> *vectorFormVol(const ProblemID problemId, int i, int j,
                                                                  const std::string &area, SceneMaterial *materialSource, Material *materialTarget, int offsetI, int offsetJ);

    virtual Hermes::Hermes2D::MatrixFormSurf<double> *matrixFormSurf(const ProblemID problemId, int i, int j,
                                                                    const std::string &area, SceneBoundary *boundary, int offsetI, int offsetJ);


    virtual Hermes::Hermes2D::VectorFormSurf<double> *vectorFormSurf(const ProblemID problemId, int i, int j,
                                                                    const std::string &area, SceneBoundary *boundary, int offsetI, int offsetJ);

    virtual Hermes::Hermes2D::ExactSolutionScalar<double> *exactSolution(const ProblemID problemId, int i,Hermes::Hermes2D::Mesh *mesh, Boundary *boundary);
};
#endif