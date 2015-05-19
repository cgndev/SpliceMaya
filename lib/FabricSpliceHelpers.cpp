#include "FabricSpliceEditorWidget.h" // [pzion 20150519] Must come first because of some stupid macro definition somewhere
#include "FabricSpliceHelpers.h"
#include <DFG/DFGLogWidget.h>
#include <DFG/DFGCombinedWidget.h>

#include <maya/MGlobal.h>

MString gLastLoadedScene;
MString mayaGetLastLoadedScene()
{
  return gLastLoadedScene;
}

MString gModuleFolder;
void initModuleFolder(MString moduleFolder)
{
  gModuleFolder = moduleFolder;
}

MString getModuleFolder()
{
  return gModuleFolder;
}

void mayaLogFunc(const MString & message)
{
  MGlobal::displayInfo(MString("[Splice] ")+message);
  FabricUI::DFG::DFGLogWidget::log(message.asChar());
}

void mayaLogFunc(const char * message, unsigned int length)
{
  mayaLogFunc(MString(message));
}

bool gErrorEnabled = true;
void mayaErrorLogEnable(bool enable)
{
  gErrorEnabled = enable;
}

bool gErrorOccured = false;
void mayaLogErrorFunc(const MString & message)
{
  if(!gErrorEnabled)
    return;
  MGlobal::displayError(MString("[Splice] ")+message);
  FabricUI::DFG::DFGLogWidget::log(message.asChar());
  gErrorOccured = true;
}

void mayaLogErrorFunc(const char * message, unsigned int length)
{
  mayaLogErrorFunc(MString(message));
}

void mayaClearError()
{
  gErrorOccured = false;
}

MStatus mayaErrorOccured()
{
  MStatus result = MS::kSuccess;
  if(gErrorOccured)
    result = MS::kFailure;
  gErrorOccured = false;
  return result;
}

void mayaKLReportFunc(const char * message, unsigned int length)
{
  MGlobal::displayInfo(MString("[KL]: ")+MString(message));
}

void mayaCompilerErrorFunc(unsigned int row, unsigned int col, const char * file, const char * level, const char * desc)
{
  MString line;
  line.set(row);
  MString composed = "[KL Compiler "+MString(level)+"]: line "+line+", op '"+MString(file)+"': "+MString(desc);
  MGlobal::displayInfo(composed);
  FabricSpliceEditorWidget::reportAllCompilerError(row, col, file, level, desc);
  FabricUI::DFG::DFGLogWidget::log(composed.asChar());
}

void mayaKLStatusFunc(const char * topic, unsigned int topicLength,  const char * message, unsigned int messageLength)
{
  MString composed = MString("[KL Status]: ")+MString(message);
  MGlobal::displayInfo(composed);
  FabricUI::DFG::DFGLogWidget::log(composed.asChar());
}

void mayaRefreshFunc()
{
  MGlobal::executeCommandOnIdle("refresh");
}

void mayaSetLastLoadedScene(MString scene)
{
  gLastLoadedScene = scene;
}
