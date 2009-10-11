#ifndef HARMONICMAGNETIC_H
#define HARMONICMAGNETIC_H

#include "util.h"
#include "hermes_field.h"

struct HermesHarmonicMagnetic : public HermesField
{
    Q_OBJECT
public:
    HermesHarmonicMagnetic() { physicField = PHYSICFIELD_HARMONIC_MAGNETIC; }

    inline int numberOfSolution() { return 2; }
    bool hasFrequency() { return true; }

    void readEdgeMarkerFromDomElement(QDomElement *element);
    void writeEdgeMarkerToDomElement(QDomElement *element, SceneEdgeMarker *marker);
    void readLabelMarkerFromDomElement(QDomElement *element);
    void writeLabelMarkerToDomElement(QDomElement *element, SceneLabelMarker *marker);

    LocalPointValue *localPointValue(Point point);
    QStringList localPointValueHeader();

    SurfaceIntegralValue *surfaceIntegralValue();
    QStringList surfaceIntegralValueHeader();

    VolumeIntegralValue *volumeIntegralValue();
    QStringList volumeIntegralValueHeader();

    SceneEdgeMarker *newEdgeMarker();
    SceneLabelMarker *newLabelMarker();

    SolutionArray *solve(SolverThread *solverThread);

    inline PhysicFieldVariable contourPhysicFieldVariable() { return PHYSICFIELDVARIABLE_HARMONIC_MAGNETIC_VECTOR_POTENTIAL; }
    inline PhysicFieldVariable scalarPhysicFieldVariable() { return PHYSICFIELDVARIABLE_HARMONIC_MAGNETIC_VECTOR_POTENTIAL; }
    inline PhysicFieldVariableComp scalarPhysicFieldVariableComp() { return PHYSICFIELDVARIABLECOMP_MAGNITUDE; }
    inline PhysicFieldVariable vectorPhysicFieldVariable() { return PHYSICFIELDVARIABLE_HARMONIC_MAGNETIC_FLUX_DENSITY; }

    void fillComboBoxScalarVariable(QComboBox *cmbFieldVariable)
    {
        cmbFieldVariable->addItem(physicFieldVariableString(PHYSICFIELDVARIABLE_HARMONIC_MAGNETIC_VECTOR_POTENTIAL), PHYSICFIELDVARIABLE_HARMONIC_MAGNETIC_VECTOR_POTENTIAL);
        cmbFieldVariable->addItem(physicFieldVariableString(PHYSICFIELDVARIABLE_HARMONIC_MAGNETIC_VECTOR_POTENTIAL_REAL), PHYSICFIELDVARIABLE_HARMONIC_MAGNETIC_VECTOR_POTENTIAL_REAL);
        cmbFieldVariable->addItem(physicFieldVariableString(PHYSICFIELDVARIABLE_HARMONIC_MAGNETIC_VECTOR_POTENTIAL_IMAG), PHYSICFIELDVARIABLE_HARMONIC_MAGNETIC_VECTOR_POTENTIAL_IMAG);
        cmbFieldVariable->addItem(physicFieldVariableString(PHYSICFIELDVARIABLE_HARMONIC_MAGNETIC_FLUX_DENSITY), PHYSICFIELDVARIABLE_HARMONIC_MAGNETIC_FLUX_DENSITY);
        cmbFieldVariable->addItem(physicFieldVariableString(PHYSICFIELDVARIABLE_HARMONIC_MAGNETIC_FLUX_DENSITY_REAL), PHYSICFIELDVARIABLE_HARMONIC_MAGNETIC_FLUX_DENSITY_REAL);
        cmbFieldVariable->addItem(physicFieldVariableString(PHYSICFIELDVARIABLE_HARMONIC_MAGNETIC_FLUX_DENSITY_IMAG), PHYSICFIELDVARIABLE_HARMONIC_MAGNETIC_FLUX_DENSITY_IMAG);
        cmbFieldVariable->addItem(physicFieldVariableString(PHYSICFIELDVARIABLE_HARMONIC_MAGNETIC_MAGNETICFIELD), PHYSICFIELDVARIABLE_HARMONIC_MAGNETIC_MAGNETICFIELD);
        cmbFieldVariable->addItem(physicFieldVariableString(PHYSICFIELDVARIABLE_HARMONIC_MAGNETIC_MAGNETICFIELD_REAL), PHYSICFIELDVARIABLE_HARMONIC_MAGNETIC_MAGNETICFIELD_REAL);
        cmbFieldVariable->addItem(physicFieldVariableString(PHYSICFIELDVARIABLE_HARMONIC_MAGNETIC_MAGNETICFIELD_IMAG), PHYSICFIELDVARIABLE_HARMONIC_MAGNETIC_MAGNETICFIELD_IMAG);
        cmbFieldVariable->addItem(physicFieldVariableString(PHYSICFIELDVARIABLE_HARMONIC_MAGNETIC_CURRENT_DENSITY_TOTAL), PHYSICFIELDVARIABLE_HARMONIC_MAGNETIC_CURRENT_DENSITY_TOTAL);
        cmbFieldVariable->addItem(physicFieldVariableString(PHYSICFIELDVARIABLE_HARMONIC_MAGNETIC_CURRENT_DENSITY_TOTAL_REAL), PHYSICFIELDVARIABLE_HARMONIC_MAGNETIC_CURRENT_DENSITY_TOTAL_REAL);
        cmbFieldVariable->addItem(physicFieldVariableString(PHYSICFIELDVARIABLE_HARMONIC_MAGNETIC_CURRENT_DENSITY_TOTAL_IMAG), PHYSICFIELDVARIABLE_HARMONIC_MAGNETIC_CURRENT_DENSITY_TOTAL_IMAG);
        cmbFieldVariable->addItem(physicFieldVariableString(PHYSICFIELDVARIABLE_HARMONIC_MAGNETIC_CURRENT_DENSITY_INDUCED), PHYSICFIELDVARIABLE_HARMONIC_MAGNETIC_CURRENT_DENSITY_INDUCED);
        cmbFieldVariable->addItem(physicFieldVariableString(PHYSICFIELDVARIABLE_HARMONIC_MAGNETIC_CURRENT_DENSITY_INDUCED_REAL), PHYSICFIELDVARIABLE_HARMONIC_MAGNETIC_CURRENT_DENSITY_INDUCED_REAL);
        cmbFieldVariable->addItem(physicFieldVariableString(PHYSICFIELDVARIABLE_HARMONIC_MAGNETIC_CURRENT_DENSITY_INDUCED_IMAG), PHYSICFIELDVARIABLE_HARMONIC_MAGNETIC_CURRENT_DENSITY_INDUCED_IMAG);
        cmbFieldVariable->addItem(physicFieldVariableString(PHYSICFIELDVARIABLE_HARMONIC_MAGNETIC_POWER_LOSSES), PHYSICFIELDVARIABLE_HARMONIC_MAGNETIC_POWER_LOSSES);
        cmbFieldVariable->addItem(physicFieldVariableString(PHYSICFIELDVARIABLE_HARMONIC_MAGNETIC_ENERGY_DENSITY), PHYSICFIELDVARIABLE_HARMONIC_MAGNETIC_ENERGY_DENSITY);
        cmbFieldVariable->addItem(physicFieldVariableString(PHYSICFIELDVARIABLE_HARMONIC_MAGNETIC_PERMEABILITY), PHYSICFIELDVARIABLE_HARMONIC_MAGNETIC_PERMEABILITY);    }

    void fillComboBoxVectorVariable(QComboBox *cmbFieldVariable)
    {
    }

    void showLocalValue(QTreeWidget *trvWidget, LocalPointValue *localPointValue);
    void showSurfaceIntegralValue(QTreeWidget *trvWidget, SurfaceIntegralValue *surfaceIntegralValue);
    void showVolumeIntegralValue(QTreeWidget *trvWidget, VolumeIntegralValue *volumeIntegralValue);
};

class LocalPointValueHarmonicMagnetic : public LocalPointValue
{
public:
    double current_density_real;
    double current_density_imag;
    double current_density_total_real;
    double current_density_total_imag;
    double current_density_induced_real;
    double current_density_induced_imag;
    double permeability;
    double potential_real;
    double potential_imag;
    Point H_real;
    Point H_imag;
    Point B_real;
    Point B_imag;
    Point FL_real;
    Point FL_imag;
    double pj;
    double wm;

    LocalPointValueHarmonicMagnetic(Point &point);
    double variableValue(PhysicFieldVariable physicFieldVariable, PhysicFieldVariableComp physicFieldVariableComp);
    QStringList variables();
};

class SurfaceIntegralValueHarmonicMagnetic : public SurfaceIntegralValue
{
public:
    SurfaceIntegralValueHarmonicMagnetic();
    QStringList variables();
};

class VolumeIntegralValueHarmonicMagnetic : public VolumeIntegralValue
{
public:
    double currentInducedReal;
    double currentInducedImag;
    double currentTotalReal;
    double currentTotalImag;
    double forceXReal;
    double forceXImag;
    double forceYReal;
    double forceYImag;
    double powerLosses;
    double energy;

    VolumeIntegralValueHarmonicMagnetic();
    QStringList variables();
};

class SceneEdgeHarmonicMagneticMarker : public SceneEdgeMarker
{
public:
    Value value;

    SceneEdgeHarmonicMagneticMarker(const QString &name, PhysicFieldBC type, Value value);

    QString script();
    QMap<QString, QString> data();
    int showDialog(QWidget *parent);
};

class SceneLabelHarmonicMagneticMarker : public SceneLabelMarker
{
public:
    Value permeability;
    Value conductivity;
    Value current_density_real;
    Value current_density_imag;

    SceneLabelHarmonicMagneticMarker(const QString &name, Value current_density_real, Value current_density_imag, Value permeability, Value conductivity);

    QString script();
    QMap<QString, QString> data();
    int showDialog(QWidget *parent);
};

class DSceneEdgeHarmonicMagneticMarker : public DSceneEdgeMarker
{
    Q_OBJECT

public:
    DSceneEdgeHarmonicMagneticMarker(SceneEdgeHarmonicMagneticMarker *edgeHarmonicMagneticMarker, QWidget *parent);
    ~DSceneEdgeHarmonicMagneticMarker();

protected:
    QLayout *createContent();

    void load();
    bool save();

private:
    QComboBox *cmbType;
    SLineEditValue *txtValue;
};

class DSceneLabelHarmonicMagneticMarker : public DSceneLabelMarker
{
    Q_OBJECT

public:
    DSceneLabelHarmonicMagneticMarker(QWidget *parent, SceneLabelHarmonicMagneticMarker *labelHarmonicMagneticMarker);
    ~DSceneLabelHarmonicMagneticMarker();

protected:
    QLayout *createContent();

    void load();
    bool save();

private:
    SLineEditValue *txtPermeability;
    SLineEditValue *txtConductivity;
    SLineEditValue *txtCurrentDensityReal;
    SLineEditValue *txtCurrentDensityImag;
};

#endif // HARMONICMAGNETIC_H
