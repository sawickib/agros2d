#include "solutionstore.h"
#include "field.h"
#include "block.h"

const int notFoundSoFar = -999;

void SolutionStore::clearAll()
{
    m_multiSolutions.clear();
}

void SolutionStore::clearOne(FieldSolutionID solutionID)
{
    m_multiSolutions.remove(solutionID);
}

SolutionArray<double> SolutionStore::solution(FieldSolutionID solutionID, int component)
{
    return multiSolution(solutionID).component(component);
}

MultiSolutionArray<double> SolutionStore::multiSolution(FieldSolutionID solutionID)
{
    if(m_multiSolutions.contains(solutionID))
        return m_multiSolutions[solutionID];

    return MultiSolutionArray<double>();
}

bool SolutionStore::contains(FieldSolutionID solutionID)
{
    return m_multiSolutions.contains(solutionID);
}

MultiSolutionArray<double> SolutionStore::multiSolution(BlockSolutionID solutionID)
{
    MultiSolutionArray<double> msa;
    foreach(Field *field, solutionID.group->m_fields)
    {
        msa.append(multiSolution(solutionID.fieldSolutionID(field->fieldInfo())));
    }

    return msa;
}

void SolutionStore::saveSolution(FieldSolutionID solutionID,  MultiSolutionArray<double> multiSolution)
{
    assert(!m_multiSolutions.contains(solutionID));
    replaceSolution(solutionID, multiSolution);
}

void SolutionStore::removeSolution(FieldSolutionID solutionID)
{
    assert(m_multiSolutions.contains(solutionID));
    m_multiSolutions.remove(solutionID);
}

void SolutionStore::replaceSolution(FieldSolutionID solutionID,  MultiSolutionArray<double> multiSolution)
{
    cout << "$$$$$$$$  Saving solution " << solutionID << ", now solutions: " << m_multiSolutions.size() << ", time " << multiSolution.component(0).time << endl;
    assert(solutionID.timeStep >= 0);
    assert(solutionID.adaptivityStep >= 0);
    m_multiSolutions[solutionID] = multiSolution;
}

void SolutionStore::saveSolution(BlockSolutionID solutionID, MultiSolutionArray<double> multiSolution)
{
    foreach(Field* field, solutionID.group->m_fields)
    {
        FieldSolutionID fieldSID = solutionID.fieldSolutionID(field->fieldInfo());
        MultiSolutionArray<double> fieldMultiSolution = multiSolution.fieldPart(solutionID.group, field->fieldInfo());
        saveSolution(fieldSID, fieldMultiSolution);
    }
}

void SolutionStore::replaceSolution(BlockSolutionID solutionID, MultiSolutionArray<double> multiSolution)
{
    foreach(Field* field, solutionID.group->m_fields)
    {
        FieldSolutionID fieldSID = solutionID.fieldSolutionID(field->fieldInfo());
        MultiSolutionArray<double> fieldMultiSolution = multiSolution.fieldPart(solutionID.group, field->fieldInfo());
        replaceSolution(fieldSID, fieldMultiSolution);
    }
}

void SolutionStore::removeSolution(BlockSolutionID solutionID)
{
    foreach(Field* field, solutionID.group->m_fields)
    {
        FieldSolutionID fieldSID = solutionID.fieldSolutionID(field->fieldInfo());
        removeSolution(fieldSID);
    }
}

int SolutionStore::lastTimeStep(FieldInfo *fieldInfo, SolutionType solutionType)
{
    int timeStep = notFoundSoFar;
    foreach(FieldSolutionID sid, m_multiSolutions.keys())
    {
        if((sid.group == fieldInfo) && (sid.solutionType == solutionType) && (sid.timeStep > timeStep))
            timeStep = sid.timeStep;
    }

    return timeStep;
}

int SolutionStore::lastTimeStep(Block *block, SolutionType solutionType)
{
    int timeStep = lastTimeStep(block->m_fields.at(0)->fieldInfo(), solutionType);

    foreach(Field* field, block->m_fields)
    {
        assert(lastTimeStep(field->fieldInfo(), solutionType) == timeStep);
    }

    return timeStep;
}

double SolutionStore::lastTime(FieldInfo *fieldInfo)
{
    int timeStep = lastTimeStep(fieldInfo, SolutionType_Normal);
    double time = notFoundSoFar;

    foreach(FieldSolutionID id, m_multiSolutions.keys())
    {
        if((id.group == fieldInfo) && (id.timeStep == timeStep) && (id.exists()))
        {
            if(time == notFoundSoFar)
                time = m_multiSolutions[id].component(0).time;
            else
                assert(time == m_multiSolutions[id].component(0).time);
        }
    }
    assert(time != notFoundSoFar);
    return time;
}

double SolutionStore::lastTime(Block *block)
{
    double time = lastTime(block->m_fields.at(0)->fieldInfo());

    foreach(Field* field, block->m_fields)
    {
        assert(lastTime(field->fieldInfo()) == time);
    }

    return time;

}

int SolutionStore::lastAdaptiveStep(FieldInfo *fieldInfo, SolutionType solutionType, int timeStep)
{
    if(timeStep == -1)
        timeStep = lastTimeStep(fieldInfo, solutionType);

    int adaptiveStep = notFoundSoFar;
    foreach(FieldSolutionID sid, m_multiSolutions.keys())
    {
        if((sid.group == fieldInfo) && (sid.solutionType == solutionType) && (sid.timeStep == timeStep) && (sid.adaptivityStep > adaptiveStep))
            adaptiveStep = sid.adaptivityStep;
    }

    return adaptiveStep;
}

int SolutionStore::lastAdaptiveStep(Block *block, SolutionType solutionType, int timeStep)
{
    int adaptiveStep = lastAdaptiveStep(block->m_fields.at(0)->fieldInfo(), solutionType, timeStep);

    foreach(Field* field, block->m_fields)
    {
        assert(lastAdaptiveStep(field->fieldInfo(), solutionType, timeStep) == adaptiveStep);
    }

    return adaptiveStep;
}

FieldSolutionID SolutionStore::lastTimeAndAdaptiveSolution(FieldInfo *fieldInfo, SolutionType solutionType)
{
    FieldSolutionID solutionID;
    if(solutionType == SolutionType_Finer) {
        FieldSolutionID solutionIDNormal = lastTimeAndAdaptiveSolution(fieldInfo, SolutionType_Normal);
        FieldSolutionID solutionIDReference = lastTimeAndAdaptiveSolution(fieldInfo, SolutionType_Reference);
        if((solutionIDNormal.timeStep > solutionIDReference.timeStep) ||
                (solutionIDNormal.adaptivityStep > solutionIDReference.adaptivityStep))
        {
            solutionID = solutionIDNormal;
        }
        else
        {
            solutionID = solutionIDReference;
        }
    }
    else
    {
        solutionID.group = fieldInfo;
        solutionID.adaptivityStep = lastAdaptiveStep(fieldInfo, solutionType);
        solutionID.timeStep = lastTimeStep(fieldInfo, solutionType);
        solutionID.solutionType = solutionType;
    }

    return solutionID;
}

BlockSolutionID SolutionStore::lastTimeAndAdaptiveSolution(Block *block, SolutionType solutionType)
{
    FieldSolutionID fsid = lastTimeAndAdaptiveSolution(block->m_fields.at(0)->fieldInfo(), solutionType);
    BlockSolutionID bsid = fsid.blockSolutionID(block);


    foreach(Field* field, block->m_fields)
    {
        assert(bsid == lastTimeAndAdaptiveSolution(field->fieldInfo(), solutionType).blockSolutionID(block));
    }

    return bsid;
}

QList<double> SolutionStore::timeLevels(FieldInfo *fieldInfo)
{
    QList<double> list;

    foreach(FieldSolutionID fsid, m_multiSolutions.keys())
    {
        if(fsid.group == fieldInfo)
        {
            double time = m_multiSolutions[fsid].component(0).time;
            if(!list.contains(time))
                list.push_back(time);
        }
    }

    return list;
}
