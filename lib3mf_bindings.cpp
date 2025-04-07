#include <iostream>
#include <vector>
#include <string>
#include <emscripten.h>
#include <emscripten/bind.h>
#include "Cpp/lib3mf_implicit.hpp"

using namespace emscripten;
using namespace Lib3MF;

EMSCRIPTEN_BINDINGS(lib3mf) {
    // Enums

    enum_<ePropertyType>("ePropertyType")
        .value("NoPropertyType", ePropertyType::NoPropertyType)
        .value("BaseMaterial", ePropertyType::BaseMaterial)
        .value("TexCoord", ePropertyType::TexCoord)
        .value("Colors", ePropertyType::Colors)
        .value("Composite", ePropertyType::Composite)
        .value("Multi", ePropertyType::Multi);

    enum_<eSlicesMeshResolution>("eSlicesMeshResolution")
        .value("Fullres", eSlicesMeshResolution::Fullres)
        .value("Lowres", eSlicesMeshResolution::Lowres);

    enum_<eModelUnit>("eModelUnit")
        .value("MicroMeter", eModelUnit::MicroMeter)
        .value("MilliMeter", eModelUnit::MilliMeter)
        .value("CentiMeter", eModelUnit::CentiMeter)
        .value("Inch", eModelUnit::Inch)
        .value("Foot", eModelUnit::Foot)
        .value("Meter", eModelUnit::Meter);

    enum_<eObjectType>("eObjectType")
        .value("Other", eObjectType::Other)
        .value("Model", eObjectType::Model)
        .value("Support", eObjectType::Support)
        .value("SolidSupport", eObjectType::SolidSupport);

    enum_<eTextureType>("eTextureType")
        .value("Unknown", eTextureType::Unknown)
        .value("PNG", eTextureType::PNG)
        .value("JPEG", eTextureType::JPEG);

    enum_<eTextureTileStyle>("eTextureTileStyle")
        .value("Wrap", eTextureTileStyle::Wrap)
        .value("Mirror", eTextureTileStyle::Mirror)
        .value("Clamp", eTextureTileStyle::Clamp)
        .value("NoTileStyle", eTextureTileStyle::NoTileStyle);

    enum_<eTextureFilter>("eTextureFilter")
        .value("Auto", eTextureFilter::Auto)
        .value("Linear", eTextureFilter::Linear)
        .value("Nearest", eTextureFilter::Nearest);

    enum_<eBeamLatticeCapMode>("eBeamLatticeCapMode")
        .value("Sphere", eBeamLatticeCapMode::Sphere)
        .value("HemiSphere", eBeamLatticeCapMode::HemiSphere)
        .value("Butt", eBeamLatticeCapMode::Butt);

    enum_<eBeamLatticeClipMode>("eBeamLatticeClipMode")
        .value("NoClipMode", eBeamLatticeClipMode::NoClipMode)
        .value("Inside", eBeamLatticeClipMode::Inside)
        .value("Outside", eBeamLatticeClipMode::Outside);

    enum_<eBeamLatticeBallMode>("eBeamLatticeBallMode")
        .value("BeamLatticeBallModeNone", eBeamLatticeBallMode::BeamLatticeBallModeNone)
        .value("Mixed", eBeamLatticeBallMode::Mixed)
        .value("All", eBeamLatticeBallMode::All);

    enum_<eProgressIdentifier>("eProgressIdentifier")
        .value("QUERYCANCELED", eProgressIdentifier::QUERYCANCELED)
        .value("DONE", eProgressIdentifier::DONE)
        .value("CLEANUP", eProgressIdentifier::CLEANUP)
        .value("READSTREAM", eProgressIdentifier::READSTREAM)
        .value("EXTRACTOPCPACKAGE", eProgressIdentifier::EXTRACTOPCPACKAGE)
        .value("READNONROOTMODELS", eProgressIdentifier::READNONROOTMODELS)
        .value("READROOTMODEL", eProgressIdentifier::READROOTMODEL)
        .value("READRESOURCES", eProgressIdentifier::READRESOURCES)
        .value("READMESH", eProgressIdentifier::READMESH)
        .value("READSLICES", eProgressIdentifier::READSLICES)
        .value("READBUILD", eProgressIdentifier::READBUILD)
        .value("READCUSTOMATTACHMENT", eProgressIdentifier::READCUSTOMATTACHMENT)
        .value("READTEXTURETACHMENTS", eProgressIdentifier::READTEXTURETACHMENTS)
        .value("CREATEOPCPACKAGE", eProgressIdentifier::CREATEOPCPACKAGE)
        .value("WRITEMODELSTOSTREAM", eProgressIdentifier::WRITEMODELSTOSTREAM)
        .value("WRITEROOTMODEL", eProgressIdentifier::WRITEROOTMODEL)
        .value("WRITENONROOTMODELS", eProgressIdentifier::WRITENONROOTMODELS)
        .value("WRITEATTACHMENTS", eProgressIdentifier::WRITEATTACHMENTS)
        .value("WRITECONTENTTYPES", eProgressIdentifier::WRITECONTENTTYPES)
        .value("WRITENOBJECTS", eProgressIdentifier::WRITENOBJECTS)
        .value("WRITENODES", eProgressIdentifier::WRITENODES)
        .value("WRITETRIANGLES", eProgressIdentifier::WRITETRIANGLES)
        .value("WRITESLICES", eProgressIdentifier::WRITESLICES)
        .value("WRITEKEYSTORE", eProgressIdentifier::WRITEKEYSTORE);

    enum_<eBlendMethod>("eBlendMethod")
        .value("NoBlendMethod", eBlendMethod::NoBlendMethod)
        .value("Mix", eBlendMethod::Mix)
        .value("Multiply", eBlendMethod::Multiply);

    enum_<eChannelName>("eChannelName")
        .value("Red", eChannelName::Red)
        .value("Green", eChannelName::Green)
        .value("Blue", eChannelName::Blue)
        .value("Alpha", eChannelName::Alpha);

    enum_<eCompositionMethod>("eCompositionMethod")
        .value("WeightedSum", eCompositionMethod::WeightedSum)
        .value("Multiply", eCompositionMethod::Multiply)
        .value("Min", eCompositionMethod::Min)
        .value("Max", eCompositionMethod::Max)
        .value("Mask", eCompositionMethod::Mask);

    enum_<eCompositionSpace>("eCompositionSpace")
        .value("Raw", eCompositionSpace::Raw)
        .value("LinearColor", eCompositionSpace::LinearColor);

    enum_<eImplicitNodeType>("eImplicitNodeType")
        .value("Addition", eImplicitNodeType::Addition)
        .value("Subtraction", eImplicitNodeType::Subtraction)
        .value("Multiplication", eImplicitNodeType::Multiplication)
        .value("Division", eImplicitNodeType::Division)
        .value("Constant", eImplicitNodeType::Constant)
        .value("ConstVec", eImplicitNodeType::ConstVec)
        .value("ConstMat", eImplicitNodeType::ConstMat)
        .value("ComposeVector", eImplicitNodeType::ComposeVector)
        .value("DecomposeVector", eImplicitNodeType::DecomposeVector)
        .value("ComposeMatrix", eImplicitNodeType::ComposeMatrix)
        .value("MatrixFromColumns", eImplicitNodeType::MatrixFromColumns)
        .value("MatrixFromRows", eImplicitNodeType::MatrixFromRows)
        .value("Dot", eImplicitNodeType::Dot)
        .value("Cross", eImplicitNodeType::Cross)
        .value("MatVecMultiplication", eImplicitNodeType::MatVecMultiplication)
        .value("Transpose", eImplicitNodeType::Transpose)
        .value("Inverse", eImplicitNodeType::Inverse)
        .value("Sinus", eImplicitNodeType::Sinus)
        .value("Cosinus", eImplicitNodeType::Cosinus)
        .value("Tan", eImplicitNodeType::Tan)
        .value("ArcSin", eImplicitNodeType::ArcSin)
        .value("ArcCos", eImplicitNodeType::ArcCos)
        .value("ArcTan", eImplicitNodeType::ArcTan)
        .value("ArcTan2", eImplicitNodeType::ArcTan2)
        .value("Min", eImplicitNodeType::Min)
        .value("Max", eImplicitNodeType::Max)
        .value("Abs", eImplicitNodeType::Abs)
        .value("Fmod", eImplicitNodeType::Fmod)
        .value("Pow", eImplicitNodeType::Pow)
        .value("Sqrt", eImplicitNodeType::Sqrt)
        .value("Exp", eImplicitNodeType::Exp)
        .value("Log", eImplicitNodeType::Log)
        .value("Log2", eImplicitNodeType::Log2)
        .value("Log10", eImplicitNodeType::Log10)
        .value("Select", eImplicitNodeType::Select)
        .value("Clamp", eImplicitNodeType::Clamp)
        .value("Sinh", eImplicitNodeType::Sinh)
        .value("Cosh", eImplicitNodeType::Cosh)
        .value("Tanh", eImplicitNodeType::Tanh)
        .value("Round", eImplicitNodeType::Round)
        .value("Ceil", eImplicitNodeType::Ceil)
        .value("Floor", eImplicitNodeType::Floor)
        .value("Sign", eImplicitNodeType::Sign)
        .value("Fract", eImplicitNodeType::Fract)
        .value("FunctionCall", eImplicitNodeType::FunctionCall)
        .value("Mesh", eImplicitNodeType::Mesh)
        .value("Length", eImplicitNodeType::Length)
        .value("ConstResourceID", eImplicitNodeType::ConstResourceID)
        .value("VectorFromScalar", eImplicitNodeType::VectorFromScalar)
        .value("UnsignedMesh", eImplicitNodeType::UnsignedMesh)
        .value("Mod", eImplicitNodeType::Mod);

    enum_<eImplicitPortType>("eImplicitPortType")
        .value("Scalar", eImplicitPortType::Scalar)
        .value("Vector", eImplicitPortType::Vector)
        .value("Matrix", eImplicitPortType::Matrix)
        .value("ResourceID", eImplicitPortType::ResourceID);

    enum_<eImplicitNodeConfiguration>("eImplicitNodeConfiguration")
        .value("Default", eImplicitNodeConfiguration::Default)
        .value("ScalarToScalar", eImplicitNodeConfiguration::ScalarToScalar)
        .value("VectorToVector", eImplicitNodeConfiguration::VectorToVector)
        .value("MatrixToMatrix", eImplicitNodeConfiguration::MatrixToMatrix);

    enum_<eEncryptionAlgorithm>("eEncryptionAlgorithm")
        .value("AES256_GCM", eEncryptionAlgorithm::AES256_GCM);

    enum_<eWrappingAlgorithm>("eWrappingAlgorithm")
        .value("RSA_OAEP", eWrappingAlgorithm::RSA_OAEP);

    enum_<eMgfAlgorithm>("eMgfAlgorithm")
        .value("MGF1_SHA1", eMgfAlgorithm::MGF1_SHA1)
        .value("MGF1_SHA224", eMgfAlgorithm::MGF1_SHA224)
        .value("MGF1_SHA256", eMgfAlgorithm::MGF1_SHA256)
        .value("MGF1_SHA384", eMgfAlgorithm::MGF1_SHA384)
        .value("MGF1_SHA512", eMgfAlgorithm::MGF1_SHA512);

    enum_<eDigestMethod>("eDigestMethod")
        .value("SHA1", eDigestMethod::SHA1)
        .value("SHA256", eDigestMethod::SHA256);

    enum_<eCompression>("eCompression")
        .value("NoCompression", eCompression::NoCompression)
        .value("Deflate", eCompression::Deflate);

    // Structs

    value_object<sTriangle> ("sTriangle")
        .field("Indices", &sTriangle::m_Indices);

    value_object<sTriangleProperties> ("sTriangleProperties")
        .field("ResourceID", &sTriangleProperties::m_ResourceID)
        .field("PropertyIDs", &sTriangleProperties::m_PropertyIDs);

    value_object<sPosition> ("sPosition")
        .field("Coordinates", &sPosition::m_Coordinates);

    value_object<sPosition2D> ("sPosition2D")
        .field("Coordinates", &sPosition2D::m_Coordinates);

    value_object<sCompositeConstituent> ("sCompositeConstituent")
        .field("PropertyID", &sCompositeConstituent::m_PropertyID)
        .field("MixingRatio", &sCompositeConstituent::m_MixingRatio);

    value_object<sMultiPropertyLayer> ("sMultiPropertyLayer")
        .field("ResourceID", &sMultiPropertyLayer::m_ResourceID)
        .field("TheBlendMethod", &sMultiPropertyLayer::m_TheBlendMethod);

    value_object<sTex2Coord> ("sTex2Coord")
        .field("U", &sTex2Coord::m_U)
        .field("V", &sTex2Coord::m_V);

    value_object<sTransform> ("sTransform")
        .field("Fields", &sTransform::m_Fields);

    value_object<sBox> ("sBox")
        .field("MinCoordinate", &sBox::m_MinCoordinate)
        .field("MaxCoordinate", &sBox::m_MaxCoordinate);

    value_object<sColor> ("sColor")
        .field("Red", &sColor::m_Red)
        .field("Green", &sColor::m_Green)
        .field("Blue", &sColor::m_Blue)
        .field("Alpha", &sColor::m_Alpha);

    value_object<sBeam> ("sBeam")
        .field("Indices", &sBeam::m_Indices)
        .field("Radii", &sBeam::m_Radii)
        .field("CapModes", &sBeam::m_CapModes);

    value_object<sBall> ("sBall")
        .field("Index", &sBall::m_Index)
        .field("Radius", &sBall::m_Radius);

    value_object<sVector> ("sVector")
        .field("Coordinates", &sVector::m_Coordinates);

    value_object<sMatrix4x4> ("sMatrix4x4")
        .field("Field", &sMatrix4x4::m_Field);

    // Value arrays for struct fields that are arrays
    

    // Classes

    class_<CBase> ("CBase")
        .function("ClassTypeId", &CBase::ClassTypeId)
    ;

    class_<CWriter> ("CWriter")
        .function("WriteToFile", &CWriter::WriteToFile)
        .function("GetStreamSize", &CWriter::GetStreamSize)
        // .function("WriteToBuffer", &CWriter::WriteToBuffer)
        // .function("WriteToCallback", &CWriter::WriteToCallback)
        // .function("SetProgressCallback", &CWriter::SetProgressCallback)
        .function("GetDecimalPrecision", &CWriter::GetDecimalPrecision)
        .function("SetDecimalPrecision", &CWriter::SetDecimalPrecision)
        .function("SetStrictModeActive", &CWriter::SetStrictModeActive)
        .function("GetStrictModeActive", &CWriter::GetStrictModeActive)
        // .function("GetWarning", &CWriter::GetWarning)
        .function("GetWarningCount", &CWriter::GetWarningCount)
        // .function("AddKeyWrappingCallback", &CWriter::AddKeyWrappingCallback)
        // .function("SetContentEncryptionCallback", &CWriter::SetContentEncryptionCallback)
    ;

    class_<CReader> ("CReader")
        .function("ReadFromFile", &CReader::ReadFromFile)
        .function("ReadFromBuffer", &CReader::ReadFromBuffer)
        // .function("ReadFromCallback", &CReader::ReadFromCallback)
        // .function("SetProgressCallback", &CReader::SetProgressCallback)
        .function("AddRelationToRead", &CReader::AddRelationToRead)
        .function("RemoveRelationToRead", &CReader::RemoveRelationToRead)
        .function("SetStrictModeActive", &CReader::SetStrictModeActive)
        .function("GetStrictModeActive", &CReader::GetStrictModeActive)
        // .function("GetWarning", &CReader::GetWarning)
        .function("GetWarningCount", &CReader::GetWarningCount)
        // .function("AddKeyWrappingCallback", &CReader::AddKeyWrappingCallback)
        // .function("SetContentEncryptionCallback", &CReader::SetContentEncryptionCallback)
    ;

    class_<CPackagePart> ("CPackagePart")
        .function("GetPath", &CPackagePart::GetPath)
        .function("SetPath", &CPackagePart::SetPath)
    ;

    class_<CResource> ("CResource")
        .function("GetResourceID", &CResource::GetResourceID)
        .function("GetUniqueResourceID", &CResource::GetUniqueResourceID)
        .function("PackagePart", &CResource::PackagePart)
        .function("SetPackagePart", &CResource::SetPackagePart)
        .function("GetModelResourceID", &CResource::GetModelResourceID)
    ;

    class_<CResourceIterator> ("CResourceIterator")
        .function("MoveNext", &CResourceIterator::MoveNext)
        .function("MovePrevious", &CResourceIterator::MovePrevious)
        .function("GetCurrent", &CResourceIterator::GetCurrent)
        .function("Clone", &CResourceIterator::Clone)
        .function("Count", &CResourceIterator::Count)
    ;

    class_<CSliceStackIterator> ("CSliceStackIterator")
        .function("GetCurrentSliceStack", &CSliceStackIterator::GetCurrentSliceStack)
    ;

    class_<CObjectIterator> ("CObjectIterator")
        .function("GetCurrentObject", &CObjectIterator::GetCurrentObject)
    ;

    class_<CMeshObjectIterator> ("CMeshObjectIterator")
        .function("GetCurrentMeshObject", &CMeshObjectIterator::GetCurrentMeshObject)
    ;

    class_<CComponentsObjectIterator> ("CComponentsObjectIterator")
        .function("GetCurrentComponentsObject", &CComponentsObjectIterator::GetCurrentComponentsObject)
    ;

    class_<CTexture2DIterator> ("CTexture2DIterator")
        .function("GetCurrentTexture2D", &CTexture2DIterator::GetCurrentTexture2D)
    ;

    class_<CBaseMaterialGroupIterator> ("CBaseMaterialGroupIterator")
        .function("GetCurrentBaseMaterialGroup", &CBaseMaterialGroupIterator::GetCurrentBaseMaterialGroup)
    ;

    class_<CColorGroupIterator> ("CColorGroupIterator")
        .function("GetCurrentColorGroup", &CColorGroupIterator::GetCurrentColorGroup)
    ;

    class_<CTexture2DGroupIterator> ("CTexture2DGroupIterator")
        .function("GetCurrentTexture2DGroup", &CTexture2DGroupIterator::GetCurrentTexture2DGroup)
    ;

    class_<CCompositeMaterialsIterator> ("CCompositeMaterialsIterator")
        .function("GetCurrentCompositeMaterials", &CCompositeMaterialsIterator::GetCurrentCompositeMaterials)
    ;

    class_<CMultiPropertyGroupIterator> ("CMultiPropertyGroupIterator")
        .function("GetCurrentMultiPropertyGroup", &CMultiPropertyGroupIterator::GetCurrentMultiPropertyGroup)
    ;

    class_<CImage3DIterator> ("CImage3DIterator")
        .function("GetCurrentImage3D", &CImage3DIterator::GetCurrentImage3D)
    ;

    class_<CFunctionIterator> ("CFunctionIterator")
        .function("GetCurrentFunction", &CFunctionIterator::GetCurrentFunction)
    ;

    class_<CLevelSetIterator> ("CLevelSetIterator")
        .function("GetCurrentLevelSet", &CLevelSetIterator::GetCurrentLevelSet)
    ;

    class_<CMetaData> ("CMetaData")
        .function("GetNameSpace", &CMetaData::GetNameSpace)
        .function("SetNameSpace", &CMetaData::SetNameSpace)
        .function("GetName", &CMetaData::GetName)
        .function("SetName", &CMetaData::SetName)
        .function("GetKey", &CMetaData::GetKey)
        .function("GetMustPreserve", &CMetaData::GetMustPreserve)
        .function("SetMustPreserve", &CMetaData::SetMustPreserve)
        .function("GetType", &CMetaData::GetType)
        .function("SetType", &CMetaData::SetType)
        .function("GetValue", &CMetaData::GetValue)
        .function("SetValue", &CMetaData::SetValue)
    ;

    class_<CMetaDataGroup> ("CMetaDataGroup")
        .function("GetMetaDataCount", &CMetaDataGroup::GetMetaDataCount)
        .function("GetMetaData", &CMetaDataGroup::GetMetaData)
        .function("GetMetaDataByKey", &CMetaDataGroup::GetMetaDataByKey)
        .function("RemoveMetaDataByIndex", &CMetaDataGroup::RemoveMetaDataByIndex)
        .function("RemoveMetaData", &CMetaDataGroup::RemoveMetaData)
        .function("AddMetaData", &CMetaDataGroup::AddMetaData)
    ;

    class_<CTriangleSet> ("CTriangleSet")
        .function("SetName", &CTriangleSet::SetName)
        .function("GetName", &CTriangleSet::GetName)
        .function("SetIdentifier", &CTriangleSet::SetIdentifier)
        .function("GetIdentifier", &CTriangleSet::GetIdentifier)
        .function("AddTriangle", &CTriangleSet::AddTriangle)
        .function("RemoveTriangle", &CTriangleSet::RemoveTriangle)
        .function("Clear", &CTriangleSet::Clear)
        .function("SetTriangleList", &CTriangleSet::SetTriangleList)
        // .function("GetTriangleList", &CTriangleSet::GetTriangleList)
        .function("AddTriangleList", &CTriangleSet::AddTriangleList)
        .function("Merge", &CTriangleSet::Merge)
        .function("DeleteSet", &CTriangleSet::DeleteSet)
        .function("Duplicate", &CTriangleSet::Duplicate)
    ;

    class_<CObject> ("CObject")
        .function("GetType", &CObject::GetType)
        .function("SetType", &CObject::SetType)
        .function("GetName", &CObject::GetName)
        .function("SetName", &CObject::SetName)
        .function("GetPartNumber", &CObject::GetPartNumber)
        .function("SetPartNumber", &CObject::SetPartNumber)
        .function("IsMeshObject", &CObject::IsMeshObject)
        .function("IsComponentsObject", &CObject::IsComponentsObject)
        .function("IsLevelSetObject", &CObject::IsLevelSetObject)
        .function("IsValid", &CObject::IsValid)
        .function("SetAttachmentAsThumbnail", &CObject::SetAttachmentAsThumbnail)
        .function("GetThumbnailAttachment", &CObject::GetThumbnailAttachment)
        .function("ClearThumbnailAttachment", &CObject::ClearThumbnailAttachment)
        .function("GetOutbox", &CObject::GetOutbox)
        // .function("GetUUID", &CObject::GetUUID)
        .function("SetUUID", &CObject::SetUUID)
        .function("GetMetaDataGroup", &CObject::GetMetaDataGroup)
        .function("SetSlicesMeshResolution", &CObject::SetSlicesMeshResolution)
        .function("GetSlicesMeshResolution", &CObject::GetSlicesMeshResolution)
        .function("HasSlices", &CObject::HasSlices)
        .function("ClearSliceStack", &CObject::ClearSliceStack)
        .function("GetSliceStack", &CObject::GetSliceStack)
        .function("AssignSliceStack", &CObject::AssignSliceStack)
    ;

    class_<CMeshObject> ("CMeshObject")
        .function("GetVertexCount", &CMeshObject::GetVertexCount)
        .function("GetTriangleCount", &CMeshObject::GetTriangleCount)
        .function("GetVertex", &CMeshObject::GetVertex)
        .function("SetVertex", &CMeshObject::SetVertex)
        .function("AddVertex", &CMeshObject::AddVertex)
        // .function("GetVertices", &CMeshObject::GetVertices)
        .function("GetTriangle", &CMeshObject::GetTriangle)
        .function("SetTriangle", &CMeshObject::SetTriangle)
        .function("AddTriangle", &CMeshObject::AddTriangle)
        // .function("GetTriangleIndices", &CMeshObject::GetTriangleIndices)
        .function("SetObjectLevelProperty", &CMeshObject::SetObjectLevelProperty)
        // .function("GetObjectLevelProperty", &CMeshObject::GetObjectLevelProperty)
        .function("SetTriangleProperties", &CMeshObject::SetTriangleProperties)
        // .function("GetTriangleProperties", &CMeshObject::GetTriangleProperties)
        .function("SetAllTriangleProperties", &CMeshObject::SetAllTriangleProperties)
        // .function("GetAllTriangleProperties", &CMeshObject::GetAllTriangleProperties)
        .function("ClearAllProperties", &CMeshObject::ClearAllProperties)
        .function("SetGeometry", &CMeshObject::SetGeometry)
        .function("IsManifoldAndOriented", &CMeshObject::IsManifoldAndOriented)
        .function("BeamLattice", &CMeshObject::BeamLattice)
        .function("GetVolumeData", &CMeshObject::GetVolumeData)
        .function("SetVolumeData", &CMeshObject::SetVolumeData)
        .function("AddTriangleSet", &CMeshObject::AddTriangleSet)
        .function("HasTriangleSet", &CMeshObject::HasTriangleSet)
        .function("FindTriangleSet", &CMeshObject::FindTriangleSet)
        .function("GetTriangleSetCount", &CMeshObject::GetTriangleSetCount)
        .function("GetTriangleSet", &CMeshObject::GetTriangleSet)
    ;

    class_<CLevelSet> ("CLevelSet")
        .function("GetFunction", &CLevelSet::GetFunction)
        .function("SetFunction", &CLevelSet::SetFunction)
        .function("GetTransform", &CLevelSet::GetTransform)
        .function("SetTransform", &CLevelSet::SetTransform)
        .function("GetChannelName", &CLevelSet::GetChannelName)
        .function("SetChannelName", &CLevelSet::SetChannelName)
        .function("SetMinFeatureSize", &CLevelSet::SetMinFeatureSize)
        .function("GetMinFeatureSize", &CLevelSet::GetMinFeatureSize)
        .function("SetFallBackValue", &CLevelSet::SetFallBackValue)
        .function("GetFallBackValue", &CLevelSet::GetFallBackValue)
        .function("SetMeshBBoxOnly", &CLevelSet::SetMeshBBoxOnly)
        .function("GetMeshBBoxOnly", &CLevelSet::GetMeshBBoxOnly)
        .function("SetMesh", &CLevelSet::SetMesh)
        .function("GetMesh", &CLevelSet::GetMesh)
        .function("GetVolumeData", &CLevelSet::GetVolumeData)
        .function("SetVolumeData", &CLevelSet::SetVolumeData)
    ;

    class_<CBeamLattice> ("CBeamLattice")
        .function("GetMinLength", &CBeamLattice::GetMinLength)
        .function("SetMinLength", &CBeamLattice::SetMinLength)
        // .function("GetClipping", &CBeamLattice::GetClipping)
        .function("SetClipping", &CBeamLattice::SetClipping)
        // .function("GetRepresentation", &CBeamLattice::GetRepresentation)
        .function("SetRepresentation", &CBeamLattice::SetRepresentation)
        // .function("GetBallOptions", &CBeamLattice::GetBallOptions)
        .function("SetBallOptions", &CBeamLattice::SetBallOptions)
        .function("GetBeamCount", &CBeamLattice::GetBeamCount)
        .function("GetBeam", &CBeamLattice::GetBeam)
        .function("AddBeam", &CBeamLattice::AddBeam)
        .function("SetBeam", &CBeamLattice::SetBeam)
        .function("SetBeams", &CBeamLattice::SetBeams)
        // .function("GetBeams", &CBeamLattice::GetBeams)
        .function("GetBallCount", &CBeamLattice::GetBallCount)
        .function("GetBall", &CBeamLattice::GetBall)
        .function("AddBall", &CBeamLattice::AddBall)
        .function("SetBall", &CBeamLattice::SetBall)
        .function("SetBalls", &CBeamLattice::SetBalls)
        // .function("GetBalls", &CBeamLattice::GetBalls)
        .function("GetBeamSetCount", &CBeamLattice::GetBeamSetCount)
        .function("AddBeamSet", &CBeamLattice::AddBeamSet)
        .function("GetBeamSet", &CBeamLattice::GetBeamSet)
    ;

    class_<CFunctionReference> ("CFunctionReference")
        .function("GetFunctionResourceID", &CFunctionReference::GetFunctionResourceID)
        .function("SetFunctionResourceID", &CFunctionReference::SetFunctionResourceID)
        .function("GetTransform", &CFunctionReference::GetTransform)
        .function("SetTransform", &CFunctionReference::SetTransform)
        .function("GetChannelName", &CFunctionReference::GetChannelName)
        .function("SetChannelName", &CFunctionReference::SetChannelName)
        .function("SetMinFeatureSize", &CFunctionReference::SetMinFeatureSize)
        .function("GetMinFeatureSize", &CFunctionReference::GetMinFeatureSize)
        .function("SetFallBackValue", &CFunctionReference::SetFallBackValue)
        .function("GetFallBackValue", &CFunctionReference::GetFallBackValue)
    ;

    class_<CVolumeDataColor> ("CVolumeDataColor")
    ;

    class_<CMaterialMapping> ("CMaterialMapping")
    ;

    class_<CVolumeDataComposite> ("CVolumeDataComposite")
        .function("GetBaseMaterialGroup", &CVolumeDataComposite::GetBaseMaterialGroup)
        .function("SetBaseMaterialGroup", &CVolumeDataComposite::SetBaseMaterialGroup)
        .function("GetMaterialMappingCount", &CVolumeDataComposite::GetMaterialMappingCount)
        .function("GetMaterialMapping", &CVolumeDataComposite::GetMaterialMapping)
        .function("AddMaterialMapping", &CVolumeDataComposite::AddMaterialMapping)
        .function("RemoveMaterialMapping", &CVolumeDataComposite::RemoveMaterialMapping)
    ;

    class_<CVolumeDataProperty> ("CVolumeDataProperty")
        .function("GetName", &CVolumeDataProperty::GetName)
        .function("SetIsRequired", &CVolumeDataProperty::SetIsRequired)
        .function("IsRequired", &CVolumeDataProperty::IsRequired)
    ;

    class_<CVolumeData> ("CVolumeData")
        .function("GetComposite", &CVolumeData::GetComposite)
        .function("CreateNewComposite", &CVolumeData::CreateNewComposite)
        .function("RemoveComposite", &CVolumeData::RemoveComposite)
        .function("GetColor", &CVolumeData::GetColor)
        .function("CreateNewColor", &CVolumeData::CreateNewColor)
        .function("RemoveColor", &CVolumeData::RemoveColor)
        .function("GetPropertyCount", &CVolumeData::GetPropertyCount)
        .function("GetProperty", &CVolumeData::GetProperty)
        .function("AddPropertyFromFunction", &CVolumeData::AddPropertyFromFunction)
        .function("RemoveProperty", &CVolumeData::RemoveProperty)
    ;

    class_<CComponent> ("CComponent")
        .function("GetObjectResource", &CComponent::GetObjectResource)
        .function("GetObjectResourceID", &CComponent::GetObjectResourceID)
        // .function("GetUUID", &CComponent::GetUUID)
        .function("SetUUID", &CComponent::SetUUID)
        .function("HasTransform", &CComponent::HasTransform)
        .function("GetTransform", &CComponent::GetTransform)
        .function("SetTransform", &CComponent::SetTransform)
    ;

    class_<CComponentsObject> ("CComponentsObject")
        .function("AddComponent", &CComponentsObject::AddComponent)
        .function("GetComponent", &CComponentsObject::GetComponent)
        .function("GetComponentCount", &CComponentsObject::GetComponentCount)
    ;

    class_<CBeamSet> ("CBeamSet")
        .function("SetName", &CBeamSet::SetName)
        .function("GetName", &CBeamSet::GetName)
        .function("SetIdentifier", &CBeamSet::SetIdentifier)
        .function("GetIdentifier", &CBeamSet::GetIdentifier)
        .function("GetReferenceCount", &CBeamSet::GetReferenceCount)
        .function("SetReferences", &CBeamSet::SetReferences)
        // .function("GetReferences", &CBeamSet::GetReferences)
        .function("GetBallReferenceCount", &CBeamSet::GetBallReferenceCount)
        .function("SetBallReferences", &CBeamSet::SetBallReferences)
        // .function("GetBallReferences", &CBeamSet::GetBallReferences)
    ;

    class_<CBaseMaterialGroup> ("CBaseMaterialGroup")
        .function("GetCount", &CBaseMaterialGroup::GetCount)
        // .function("GetAllPropertyIDs", &CBaseMaterialGroup::GetAllPropertyIDs)
        .function("AddMaterial", &CBaseMaterialGroup::AddMaterial)
        .function("RemoveMaterial", &CBaseMaterialGroup::RemoveMaterial)
        .function("GetName", &CBaseMaterialGroup::GetName)
        .function("SetName", &CBaseMaterialGroup::SetName)
        .function("SetDisplayColor", &CBaseMaterialGroup::SetDisplayColor)
        .function("GetDisplayColor", &CBaseMaterialGroup::GetDisplayColor)
    ;

    class_<CColorGroup> ("CColorGroup")
        .function("GetCount", &CColorGroup::GetCount)
        // .function("GetAllPropertyIDs", &CColorGroup::GetAllPropertyIDs)
        .function("AddColor", &CColorGroup::AddColor)
        .function("RemoveColor", &CColorGroup::RemoveColor)
        .function("SetColor", &CColorGroup::SetColor)
        .function("GetColor", &CColorGroup::GetColor)
    ;

    class_<CTexture2DGroup> ("CTexture2DGroup")
        .function("GetCount", &CTexture2DGroup::GetCount)
        // .function("GetAllPropertyIDs", &CTexture2DGroup::GetAllPropertyIDs)
        .function("AddTex2Coord", &CTexture2DGroup::AddTex2Coord)
        .function("GetTex2Coord", &CTexture2DGroup::GetTex2Coord)
        .function("RemoveTex2Coord", &CTexture2DGroup::RemoveTex2Coord)
        .function("GetTexture2D", &CTexture2DGroup::GetTexture2D)
    ;

    class_<CCompositeMaterials> ("CCompositeMaterials")
        .function("GetCount", &CCompositeMaterials::GetCount)
        // .function("GetAllPropertyIDs", &CCompositeMaterials::GetAllPropertyIDs)
        .function("GetBaseMaterialGroup", &CCompositeMaterials::GetBaseMaterialGroup)
        .function("AddComposite", &CCompositeMaterials::AddComposite)
        .function("RemoveComposite", &CCompositeMaterials::RemoveComposite)
        // .function("GetComposite", &CCompositeMaterials::GetComposite)
    ;

    class_<CMultiPropertyGroup> ("CMultiPropertyGroup")
        .function("GetCount", &CMultiPropertyGroup::GetCount)
        // .function("GetAllPropertyIDs", &CMultiPropertyGroup::GetAllPropertyIDs)
        .function("AddMultiProperty", &CMultiPropertyGroup::AddMultiProperty)
        .function("SetMultiProperty", &CMultiPropertyGroup::SetMultiProperty)
        // .function("GetMultiProperty", &CMultiPropertyGroup::GetMultiProperty)
        .function("RemoveMultiProperty", &CMultiPropertyGroup::RemoveMultiProperty)
        .function("GetLayerCount", &CMultiPropertyGroup::GetLayerCount)
        .function("AddLayer", &CMultiPropertyGroup::AddLayer)
        .function("GetLayer", &CMultiPropertyGroup::GetLayer)
        .function("RemoveLayer", &CMultiPropertyGroup::RemoveLayer)
    ;

    class_<CImage3D> ("CImage3D")
        .function("GetName", &CImage3D::GetName)
        .function("SetName", &CImage3D::SetName)
        .function("IsImageStack", &CImage3D::IsImageStack)
    ;

    class_<CImageStack> ("CImageStack")
        .function("GetRowCount", &CImageStack::GetRowCount)
        .function("SetRowCount", &CImageStack::SetRowCount)
        .function("GetColumnCount", &CImageStack::GetColumnCount)
        .function("SetColumnCount", &CImageStack::SetColumnCount)
        .function("GetSheetCount", &CImageStack::GetSheetCount)
        .function("GetSheet", &CImageStack::GetSheet)
        .function("SetSheet", &CImageStack::SetSheet)
        .function("CreateEmptySheet", &CImageStack::CreateEmptySheet)
        .function("CreateSheetFromBuffer", &CImageStack::CreateSheetFromBuffer)
        .function("CreateSheetFromFile", &CImageStack::CreateSheetFromFile)
    ;

    class_<CAttachment> ("CAttachment")
        .function("GetPath", &CAttachment::GetPath)
        .function("SetPath", &CAttachment::SetPath)
        .function("PackagePart", &CAttachment::PackagePart)
        .function("GetRelationShipType", &CAttachment::GetRelationShipType)
        .function("SetRelationShipType", &CAttachment::SetRelationShipType)
        .function("WriteToFile", &CAttachment::WriteToFile)
        .function("ReadFromFile", &CAttachment::ReadFromFile)
        // .function("ReadFromCallback", &CAttachment::ReadFromCallback)
        .function("GetStreamSize", &CAttachment::GetStreamSize)
        // .function("WriteToBuffer", &CAttachment::WriteToBuffer)
        .function("ReadFromBuffer", &CAttachment::ReadFromBuffer)
    ;

    class_<CTexture2D> ("CTexture2D")
        .function("GetAttachment", &CTexture2D::GetAttachment)
        .function("SetAttachment", &CTexture2D::SetAttachment)
        .function("GetContentType", &CTexture2D::GetContentType)
        .function("SetContentType", &CTexture2D::SetContentType)
        // .function("GetTileStyleUV", &CTexture2D::GetTileStyleUV)
        .function("SetTileStyleUV", &CTexture2D::SetTileStyleUV)
        .function("GetFilter", &CTexture2D::GetFilter)
        .function("SetFilter", &CTexture2D::SetFilter)
    ;

    class_<CImplicitPort> ("CImplicitPort")
        .function("GetIdentifier", &CImplicitPort::GetIdentifier)
        .function("SetIdentifier", &CImplicitPort::SetIdentifier)
        .function("GetDisplayName", &CImplicitPort::GetDisplayName)
        .function("SetDisplayName", &CImplicitPort::SetDisplayName)
        .function("SetType", &CImplicitPort::SetType)
        .function("GetType", &CImplicitPort::GetType)
        .function("GetReference", &CImplicitPort::GetReference)
        .function("SetReference", &CImplicitPort::SetReference)
    ;

    class_<CIterator> ("CIterator")
        .function("MoveNext", &CIterator::MoveNext)
        .function("MovePrevious", &CIterator::MovePrevious)
        .function("Count", &CIterator::Count)
    ;

    class_<CImplicitPortIterator> ("CImplicitPortIterator")
        .function("GetCurrent", &CImplicitPortIterator::GetCurrent)
    ;

    class_<CImplicitNode> ("CImplicitNode")
        .function("GetIdentifier", &CImplicitNode::GetIdentifier)
        .function("SetIdentifier", &CImplicitNode::SetIdentifier)
        .function("GetDisplayName", &CImplicitNode::GetDisplayName)
        .function("SetDisplayName", &CImplicitNode::SetDisplayName)
        .function("GetTag", &CImplicitNode::GetTag)
        .function("SetTag", &CImplicitNode::SetTag)
        .function("GetNodeType", &CImplicitNode::GetNodeType)
        .function("AddInput", &CImplicitNode::AddInput)
        .function("GetInputs", &CImplicitNode::GetInputs)
        .function("AddOutput", &CImplicitNode::AddOutput)
        .function("GetOutputs", &CImplicitNode::GetOutputs)
        .function("FindInput", &CImplicitNode::FindInput)
        .function("FindOutput", &CImplicitNode::FindOutput)
        .function("AreTypesValid", &CImplicitNode::AreTypesValid)
    ;

    class_<COneInputNode> ("COneInputNode")
        .function("GetInputA", &COneInputNode::GetInputA)
        .function("GetOutputResult", &COneInputNode::GetOutputResult)
    ;

    class_<CSinNode> ("CSinNode")
    ;

    class_<CCosNode> ("CCosNode")
    ;

    class_<CTanNode> ("CTanNode")
    ;

    class_<CArcSinNode> ("CArcSinNode")
    ;

    class_<CArcCosNode> ("CArcCosNode")
    ;

    class_<CArcTanNode> ("CArcTanNode")
    ;

    class_<CSinhNode> ("CSinhNode")
    ;

    class_<CCoshNode> ("CCoshNode")
    ;

    class_<CTanhNode> ("CTanhNode")
    ;

    class_<CRoundNode> ("CRoundNode")
    ;

    class_<CCeilNode> ("CCeilNode")
    ;

    class_<CFloorNode> ("CFloorNode")
    ;

    class_<CSignNode> ("CSignNode")
    ;

    class_<CFractNode> ("CFractNode")
    ;

    class_<CAbsNode> ("CAbsNode")
    ;

    class_<CExpNode> ("CExpNode")
    ;

    class_<CLogNode> ("CLogNode")
    ;

    class_<CLog2Node> ("CLog2Node")
    ;

    class_<CLog10Node> ("CLog10Node")
    ;

    class_<CLengthNode> ("CLengthNode")
    ;

    class_<CTransposeNode> ("CTransposeNode")
    ;

    class_<CInverseNode> ("CInverseNode")
    ;

    class_<CSqrtNode> ("CSqrtNode")
    ;

    class_<CResourceIdNode> ("CResourceIdNode")
        .function("SetResource", &CResourceIdNode::SetResource)
        .function("GetResource", &CResourceIdNode::GetResource)
        .function("GetOutputValue", &CResourceIdNode::GetOutputValue)
    ;

    class_<CTwoInputNode> ("CTwoInputNode")
        .function("GetInputB", &CTwoInputNode::GetInputB)
    ;

    class_<CAdditionNode> ("CAdditionNode")
    ;

    class_<CSubtractionNode> ("CSubtractionNode")
    ;

    class_<CMultiplicationNode> ("CMultiplicationNode")
    ;

    class_<CDivisionNode> ("CDivisionNode")
    ;

    class_<CDotNode> ("CDotNode")
    ;

    class_<CCrossNode> ("CCrossNode")
    ;

    class_<CArcTan2Node> ("CArcTan2Node")
    ;

    class_<CMatVecMultiplicationNode> ("CMatVecMultiplicationNode")
    ;

    class_<CMinNode> ("CMinNode")
    ;

    class_<CMaxNode> ("CMaxNode")
    ;

    class_<CFmodNode> ("CFmodNode")
    ;

    class_<CModNode> ("CModNode")
    ;

    class_<CPowNode> ("CPowNode")
    ;

    class_<CSelectNode> ("CSelectNode")
        .function("GetInputB", &CSelectNode::GetInputB)
        .function("GetInputC", &CSelectNode::GetInputC)
        .function("GetInputD", &CSelectNode::GetInputD)
    ;

    class_<CClampNode> ("CClampNode")
        .function("GetInputMin", &CClampNode::GetInputMin)
        .function("GetInputMax", &CClampNode::GetInputMax)
    ;

    class_<CComposeVectorNode> ("CComposeVectorNode")
        .function("GetInputX", &CComposeVectorNode::GetInputX)
        .function("GetInputY", &CComposeVectorNode::GetInputY)
        .function("GetInputZ", &CComposeVectorNode::GetInputZ)
        .function("GetOutputResult", &CComposeVectorNode::GetOutputResult)
    ;

    class_<CVectorFromScalarNode> ("CVectorFromScalarNode")
    ;

    class_<CDecomposeVectorNode> ("CDecomposeVectorNode")
        .function("GetInputA", &CDecomposeVectorNode::GetInputA)
        .function("GetOutputX", &CDecomposeVectorNode::GetOutputX)
        .function("GetOutputY", &CDecomposeVectorNode::GetOutputY)
        .function("GetOutputZ", &CDecomposeVectorNode::GetOutputZ)
    ;

    class_<CComposeMatrixNode> ("CComposeMatrixNode")
        .function("GetInputM00", &CComposeMatrixNode::GetInputM00)
        .function("GetInputM01", &CComposeMatrixNode::GetInputM01)
        .function("GetInputM02", &CComposeMatrixNode::GetInputM02)
        .function("GetInputM03", &CComposeMatrixNode::GetInputM03)
        .function("GetInputM10", &CComposeMatrixNode::GetInputM10)
        .function("GetInputM11", &CComposeMatrixNode::GetInputM11)
        .function("GetInputM12", &CComposeMatrixNode::GetInputM12)
        .function("GetInputM13", &CComposeMatrixNode::GetInputM13)
        .function("GetInputM20", &CComposeMatrixNode::GetInputM20)
        .function("GetInputM21", &CComposeMatrixNode::GetInputM21)
        .function("GetInputM22", &CComposeMatrixNode::GetInputM22)
        .function("GetInputM23", &CComposeMatrixNode::GetInputM23)
        .function("GetInputM30", &CComposeMatrixNode::GetInputM30)
        .function("GetInputM31", &CComposeMatrixNode::GetInputM31)
        .function("GetInputM32", &CComposeMatrixNode::GetInputM32)
        .function("GetInputM33", &CComposeMatrixNode::GetInputM33)
        .function("GetOutputResult", &CComposeMatrixNode::GetOutputResult)
    ;

    class_<CMatrixFromRowsNode> ("CMatrixFromRowsNode")
        .function("GetInputA", &CMatrixFromRowsNode::GetInputA)
        .function("GetInputB", &CMatrixFromRowsNode::GetInputB)
        .function("GetInputC", &CMatrixFromRowsNode::GetInputC)
        .function("GetInputD", &CMatrixFromRowsNode::GetInputD)
        .function("GetOutputResult", &CMatrixFromRowsNode::GetOutputResult)
    ;

    class_<CMatrixFromColumnsNode> ("CMatrixFromColumnsNode")
        .function("GetInputA", &CMatrixFromColumnsNode::GetInputA)
        .function("GetInputB", &CMatrixFromColumnsNode::GetInputB)
        .function("GetInputC", &CMatrixFromColumnsNode::GetInputC)
        .function("GetInputD", &CMatrixFromColumnsNode::GetInputD)
        .function("GetOutputResult", &CMatrixFromColumnsNode::GetOutputResult)
    ;

    class_<CConstantNode> ("CConstantNode")
        .function("SetConstant", &CConstantNode::SetConstant)
        .function("GetConstant", &CConstantNode::GetConstant)
        .function("GetOutputValue", &CConstantNode::GetOutputValue)
    ;

    class_<CConstVecNode> ("CConstVecNode")
        .function("SetVector", &CConstVecNode::SetVector)
        .function("GetVector", &CConstVecNode::GetVector)
        .function("GetOutputVector", &CConstVecNode::GetOutputVector)
    ;

    class_<CConstMatNode> ("CConstMatNode")
        .function("SetMatrix", &CConstMatNode::SetMatrix)
        .function("GetMatrix", &CConstMatNode::GetMatrix)
        .function("GetOutputMatrix", &CConstMatNode::GetOutputMatrix)
    ;

    class_<CMeshNode> ("CMeshNode")
        .function("GetInputMesh", &CMeshNode::GetInputMesh)
        .function("GetInputPos", &CMeshNode::GetInputPos)
        .function("GetOutputDistance", &CMeshNode::GetOutputDistance)
    ;

    class_<CUnsignedMeshNode> ("CUnsignedMeshNode")
        .function("GetInputMesh", &CUnsignedMeshNode::GetInputMesh)
        .function("GetInputPos", &CUnsignedMeshNode::GetInputPos)
        .function("GetOutputDistance", &CUnsignedMeshNode::GetOutputDistance)
    ;

    class_<CFunctionCallNode> ("CFunctionCallNode")
        .function("GetInputFunctionID", &CFunctionCallNode::GetInputFunctionID)
    ;

    class_<CNodeIterator> ("CNodeIterator")
        .function("GetCurrent", &CNodeIterator::GetCurrent)
    ;

    class_<CFunction> ("CFunction")
        .function("GetDisplayName", &CFunction::GetDisplayName)
        .function("SetDisplayName", &CFunction::SetDisplayName)
        .function("AddInput", &CFunction::AddInput)
        .function("GetInputs", &CFunction::GetInputs)
        .function("RemoveInput", &CFunction::RemoveInput)
        .function("AddOutput", &CFunction::AddOutput)
        .function("GetOutputs", &CFunction::GetOutputs)
        .function("RemoveOutput", &CFunction::RemoveOutput)
        .function("FindInput", &CFunction::FindInput)
        .function("FindOutput", &CFunction::FindOutput)
    ;

    class_<CImplicitFunction> ("CImplicitFunction")
        .function("GetIdentifier", &CImplicitFunction::GetIdentifier)
        .function("SetIdentifier", &CImplicitFunction::SetIdentifier)
        .function("AddNode", &CImplicitFunction::AddNode)
        .function("AddSinNode", &CImplicitFunction::AddSinNode)
        .function("AddCosNode", &CImplicitFunction::AddCosNode)
        .function("AddTanNode", &CImplicitFunction::AddTanNode)
        .function("AddArcSinNode", &CImplicitFunction::AddArcSinNode)
        .function("AddArcCosNode", &CImplicitFunction::AddArcCosNode)
        .function("AddArcTan2Node", &CImplicitFunction::AddArcTan2Node)
        .function("AddSinhNode", &CImplicitFunction::AddSinhNode)
        .function("AddCoshNode", &CImplicitFunction::AddCoshNode)
        .function("AddTanhNode", &CImplicitFunction::AddTanhNode)
        .function("AddRoundNode", &CImplicitFunction::AddRoundNode)
        .function("AddCeilNode", &CImplicitFunction::AddCeilNode)
        .function("AddFloorNode", &CImplicitFunction::AddFloorNode)
        .function("AddSignNode", &CImplicitFunction::AddSignNode)
        .function("AddFractNode", &CImplicitFunction::AddFractNode)
        .function("AddAbsNode", &CImplicitFunction::AddAbsNode)
        .function("AddExpNode", &CImplicitFunction::AddExpNode)
        .function("AddLogNode", &CImplicitFunction::AddLogNode)
        .function("AddLog2Node", &CImplicitFunction::AddLog2Node)
        .function("AddLog10Node", &CImplicitFunction::AddLog10Node)
        .function("AddLengthNode", &CImplicitFunction::AddLengthNode)
        .function("AddTransposeNode", &CImplicitFunction::AddTransposeNode)
        .function("AddInverseNode", &CImplicitFunction::AddInverseNode)
        .function("AddSqrtNode", &CImplicitFunction::AddSqrtNode)
        .function("AddResourceIdNode", &CImplicitFunction::AddResourceIdNode)
        .function("AddAdditionNode", &CImplicitFunction::AddAdditionNode)
        .function("AddSubtractionNode", &CImplicitFunction::AddSubtractionNode)
        .function("AddMultiplicationNode", &CImplicitFunction::AddMultiplicationNode)
        .function("AddDivisionNode", &CImplicitFunction::AddDivisionNode)
        .function("AddDotNode", &CImplicitFunction::AddDotNode)
        .function("AddCrossNode", &CImplicitFunction::AddCrossNode)
        .function("AddMatVecMultiplicationNode", &CImplicitFunction::AddMatVecMultiplicationNode)
        .function("AddMinNode", &CImplicitFunction::AddMinNode)
        .function("AddMaxNode", &CImplicitFunction::AddMaxNode)
        .function("AddFmodNode", &CImplicitFunction::AddFmodNode)
        .function("AddPowNode", &CImplicitFunction::AddPowNode)
        .function("AddSelectNode", &CImplicitFunction::AddSelectNode)
        .function("AddClampNode", &CImplicitFunction::AddClampNode)
        .function("AddComposeVectorNode", &CImplicitFunction::AddComposeVectorNode)
        .function("AddVectorFromScalarNode", &CImplicitFunction::AddVectorFromScalarNode)
        .function("AddDecomposeVectorNode", &CImplicitFunction::AddDecomposeVectorNode)
        .function("AddComposeMatrixNode", &CImplicitFunction::AddComposeMatrixNode)
        .function("AddMatrixFromRowsNode", &CImplicitFunction::AddMatrixFromRowsNode)
        .function("AddMatrixFromColumnsNode", &CImplicitFunction::AddMatrixFromColumnsNode)
        .function("AddConstantNode", &CImplicitFunction::AddConstantNode)
        .function("AddConstVecNode", &CImplicitFunction::AddConstVecNode)
        .function("AddConstMatNode", &CImplicitFunction::AddConstMatNode)
        .function("AddMeshNode", &CImplicitFunction::AddMeshNode)
        .function("AddUnsignedMeshNode", &CImplicitFunction::AddUnsignedMeshNode)
        .function("AddFunctionCallNode", &CImplicitFunction::AddFunctionCallNode)
        .function("GetNodes", &CImplicitFunction::GetNodes)
        .function("RemoveNode", &CImplicitFunction::RemoveNode)
        .function("AddLink", &CImplicitFunction::AddLink)
        .function("AddLinkByNames", &CImplicitFunction::AddLinkByNames)
        .function("Clear", &CImplicitFunction::Clear)
        .function("SortNodesTopologically", &CImplicitFunction::SortNodesTopologically)
    ;

    class_<CFunctionFromImage3D> ("CFunctionFromImage3D")
        .function("GetImage3D", &CFunctionFromImage3D::GetImage3D)
        .function("SetImage3D", &CFunctionFromImage3D::SetImage3D)
        .function("SetFilter", &CFunctionFromImage3D::SetFilter)
        .function("GetFilter", &CFunctionFromImage3D::GetFilter)
        .function("SetTileStyles", &CFunctionFromImage3D::SetTileStyles)
        // .function("GetTileStyles", &CFunctionFromImage3D::GetTileStyles)
        .function("GetOffset", &CFunctionFromImage3D::GetOffset)
        .function("SetOffset", &CFunctionFromImage3D::SetOffset)
        .function("GetScale", &CFunctionFromImage3D::GetScale)
        .function("SetScale", &CFunctionFromImage3D::SetScale)
    ;

    class_<CBuildItem> ("CBuildItem")
        .function("GetObjectResource", &CBuildItem::GetObjectResource)
        // .function("GetUUID", &CBuildItem::GetUUID)
        .function("SetUUID", &CBuildItem::SetUUID)
        .function("GetObjectResourceID", &CBuildItem::GetObjectResourceID)
        .function("HasObjectTransform", &CBuildItem::HasObjectTransform)
        .function("GetObjectTransform", &CBuildItem::GetObjectTransform)
        .function("SetObjectTransform", &CBuildItem::SetObjectTransform)
        .function("GetPartNumber", &CBuildItem::GetPartNumber)
        .function("SetPartNumber", &CBuildItem::SetPartNumber)
        .function("GetMetaDataGroup", &CBuildItem::GetMetaDataGroup)
        .function("GetOutbox", &CBuildItem::GetOutbox)
    ;

    class_<CBuildItemIterator> ("CBuildItemIterator")
        .function("MoveNext", &CBuildItemIterator::MoveNext)
        .function("MovePrevious", &CBuildItemIterator::MovePrevious)
        .function("GetCurrent", &CBuildItemIterator::GetCurrent)
        .function("Clone", &CBuildItemIterator::Clone)
        .function("Count", &CBuildItemIterator::Count)
    ;

    class_<CSlice> ("CSlice")
        .function("SetVertices", &CSlice::SetVertices)
        // .function("GetVertices", &CSlice::GetVertices)
        .function("GetVertexCount", &CSlice::GetVertexCount)
        .function("AddPolygon", &CSlice::AddPolygon)
        .function("GetPolygonCount", &CSlice::GetPolygonCount)
        .function("SetPolygonIndices", &CSlice::SetPolygonIndices)
        // .function("GetPolygonIndices", &CSlice::GetPolygonIndices)
        .function("GetPolygonIndexCount", &CSlice::GetPolygonIndexCount)
        .function("GetZTop", &CSlice::GetZTop)
    ;

    class_<CSliceStack> ("CSliceStack")
        .function("GetBottomZ", &CSliceStack::GetBottomZ)
        .function("GetSliceCount", &CSliceStack::GetSliceCount)
        .function("GetSlice", &CSliceStack::GetSlice)
        .function("AddSlice", &CSliceStack::AddSlice)
        .function("GetSliceRefCount", &CSliceStack::GetSliceRefCount)
        .function("AddSliceStackReference", &CSliceStack::AddSliceStackReference)
        .function("GetSliceStackReference", &CSliceStack::GetSliceStackReference)
        .function("CollapseSliceReferences", &CSliceStack::CollapseSliceReferences)
        .function("SetOwnPath", &CSliceStack::SetOwnPath)
        .function("GetOwnPath", &CSliceStack::GetOwnPath)
    ;

    class_<CConsumer> ("CConsumer")
        .function("GetConsumerID", &CConsumer::GetConsumerID)
        .function("GetKeyID", &CConsumer::GetKeyID)
        .function("GetKeyValue", &CConsumer::GetKeyValue)
    ;

    class_<CAccessRight> ("CAccessRight")
        .function("GetConsumer", &CAccessRight::GetConsumer)
        .function("GetWrappingAlgorithm", &CAccessRight::GetWrappingAlgorithm)
        .function("GetMgfAlgorithm", &CAccessRight::GetMgfAlgorithm)
        .function("GetDigestMethod", &CAccessRight::GetDigestMethod)
    ;

    class_<CContentEncryptionParams> ("CContentEncryptionParams")
        .function("GetEncryptionAlgorithm", &CContentEncryptionParams::GetEncryptionAlgorithm)
        // .function("GetKey", &CContentEncryptionParams::GetKey)
        // .function("GetInitializationVector", &CContentEncryptionParams::GetInitializationVector)
        // .function("GetAuthenticationTag", &CContentEncryptionParams::GetAuthenticationTag)
        .function("SetAuthenticationTag", &CContentEncryptionParams::SetAuthenticationTag)
        // .function("GetAdditionalAuthenticationData", &CContentEncryptionParams::GetAdditionalAuthenticationData)
        .function("GetDescriptor", &CContentEncryptionParams::GetDescriptor)
        .function("GetKeyUUID", &CContentEncryptionParams::GetKeyUUID)
    ;

    class_<CResourceData> ("CResourceData")
        .function("GetPath", &CResourceData::GetPath)
        .function("GetEncryptionAlgorithm", &CResourceData::GetEncryptionAlgorithm)
        .function("GetCompression", &CResourceData::GetCompression)
        // .function("GetAdditionalAuthenticationData", &CResourceData::GetAdditionalAuthenticationData)
    ;

    class_<CResourceDataGroup> ("CResourceDataGroup")
        .function("GetKeyUUID", &CResourceDataGroup::GetKeyUUID)
        .function("AddAccessRight", &CResourceDataGroup::AddAccessRight)
        .function("FindAccessRightByConsumer", &CResourceDataGroup::FindAccessRightByConsumer)
        .function("RemoveAccessRight", &CResourceDataGroup::RemoveAccessRight)
    ;

    class_<CKeyStore> ("CKeyStore")
        .function("AddConsumer", &CKeyStore::AddConsumer)
        .function("GetConsumerCount", &CKeyStore::GetConsumerCount)
        .function("GetConsumer", &CKeyStore::GetConsumer)
        .function("RemoveConsumer", &CKeyStore::RemoveConsumer)
        .function("FindConsumer", &CKeyStore::FindConsumer)
        .function("GetResourceDataGroupCount", &CKeyStore::GetResourceDataGroupCount)
        .function("AddResourceDataGroup", &CKeyStore::AddResourceDataGroup)
        .function("GetResourceDataGroup", &CKeyStore::GetResourceDataGroup)
        .function("RemoveResourceDataGroup", &CKeyStore::RemoveResourceDataGroup)
        .function("FindResourceDataGroup", &CKeyStore::FindResourceDataGroup)
        .function("AddResourceData", &CKeyStore::AddResourceData)
        .function("RemoveResourceData", &CKeyStore::RemoveResourceData)
        .function("FindResourceData", &CKeyStore::FindResourceData)
        .function("GetResourceDataCount", &CKeyStore::GetResourceDataCount)
        .function("GetResourceData", &CKeyStore::GetResourceData)
        // .function("GetUUID", &CKeyStore::GetUUID)
        .function("SetUUID", &CKeyStore::SetUUID)
    ;

    class_<CModel> ("CModel")
        .function("RootModelPart", &CModel::RootModelPart)
        .function("FindOrCreatePackagePart", &CModel::FindOrCreatePackagePart)
        .function("SetUnit", &CModel::SetUnit)
        .function("GetUnit", &CModel::GetUnit)
        .function("GetLanguage", &CModel::GetLanguage)
        .function("SetLanguage", &CModel::SetLanguage)
        .function("QueryWriter", &CModel::QueryWriter)
        .function("QueryReader", &CModel::QueryReader)
        .function("GetResourceByID", &CModel::GetResourceByID)
        .function("GetTexture2DByID", &CModel::GetTexture2DByID)
        .function("GetPropertyTypeByID", &CModel::GetPropertyTypeByID)
        .function("GetBaseMaterialGroupByID", &CModel::GetBaseMaterialGroupByID)
        .function("GetTexture2DGroupByID", &CModel::GetTexture2DGroupByID)
        .function("GetCompositeMaterialsByID", &CModel::GetCompositeMaterialsByID)
        .function("GetMultiPropertyGroupByID", &CModel::GetMultiPropertyGroupByID)
        .function("GetMeshObjectByID", &CModel::GetMeshObjectByID)
        .function("GetComponentsObjectByID", &CModel::GetComponentsObjectByID)
        .function("GetColorGroupByID", &CModel::GetColorGroupByID)
        .function("GetSliceStackByID", &CModel::GetSliceStackByID)
        .function("GetLevelSetByID", &CModel::GetLevelSetByID)
        // .function("GetBuildUUID", &CModel::GetBuildUUID)
        .function("SetBuildUUID", &CModel::SetBuildUUID)
        .function("GetBuildItems", &CModel::GetBuildItems)
        .function("GetOutbox", &CModel::GetOutbox)
        .function("GetResources", &CModel::GetResources)
        .function("GetObjects", &CModel::GetObjects)
        .function("GetMeshObjects", &CModel::GetMeshObjects)
        .function("GetComponentsObjects", &CModel::GetComponentsObjects)
        .function("GetTexture2Ds", &CModel::GetTexture2Ds)
        .function("GetBaseMaterialGroups", &CModel::GetBaseMaterialGroups)
        .function("GetColorGroups", &CModel::GetColorGroups)
        .function("GetTexture2DGroups", &CModel::GetTexture2DGroups)
        .function("GetCompositeMaterials", &CModel::GetCompositeMaterials)
        .function("GetMultiPropertyGroups", &CModel::GetMultiPropertyGroups)
        .function("GetSliceStacks", &CModel::GetSliceStacks)
        .function("GetImage3Ds", &CModel::GetImage3Ds)
        .function("MergeToModel", &CModel::MergeToModel)
        .function("MergeFromModel", &CModel::MergeFromModel)
        .function("AddMeshObject", &CModel::AddMeshObject)
        .function("AddComponentsObject", &CModel::AddComponentsObject)
        .function("AddSliceStack", &CModel::AddSliceStack)
        .function("AddTexture2DFromAttachment", &CModel::AddTexture2DFromAttachment)
        .function("AddBaseMaterialGroup", &CModel::AddBaseMaterialGroup)
        .function("AddColorGroup", &CModel::AddColorGroup)
        .function("AddTexture2DGroup", &CModel::AddTexture2DGroup)
        .function("AddCompositeMaterials", &CModel::AddCompositeMaterials)
        .function("AddMultiPropertyGroup", &CModel::AddMultiPropertyGroup)
        .function("AddImageStack", &CModel::AddImageStack)
        .function("GetImageStackByID", &CModel::GetImageStackByID)
        .function("AddBuildItem", &CModel::AddBuildItem)
        .function("RemoveBuildItem", &CModel::RemoveBuildItem)
        .function("GetMetaDataGroup", &CModel::GetMetaDataGroup)
        .function("AddAttachment", &CModel::AddAttachment)
        .function("RemoveAttachment", &CModel::RemoveAttachment)
        .function("GetAttachment", &CModel::GetAttachment)
        .function("FindAttachment", &CModel::FindAttachment)
        .function("GetAttachmentCount", &CModel::GetAttachmentCount)
        .function("HasPackageThumbnailAttachment", &CModel::HasPackageThumbnailAttachment)
        .function("CreatePackageThumbnailAttachment", &CModel::CreatePackageThumbnailAttachment)
        .function("GetPackageThumbnailAttachment", &CModel::GetPackageThumbnailAttachment)
        .function("RemovePackageThumbnailAttachment", &CModel::RemovePackageThumbnailAttachment)
        .function("AddCustomContentType", &CModel::AddCustomContentType)
        .function("RemoveCustomContentType", &CModel::RemoveCustomContentType)
        // .function("SetRandomNumberCallback", &CModel::SetRandomNumberCallback)
        .function("GetKeyStore", &CModel::GetKeyStore)
        .function("GetFunctions", &CModel::GetFunctions)
        .function("AddImplicitFunction", &CModel::AddImplicitFunction)
        .function("AddFunctionFromImage3D", &CModel::AddFunctionFromImage3D)
        .function("AddVolumeData", &CModel::AddVolumeData)
        .function("AddLevelSet", &CModel::AddLevelSet)
        .function("GetLevelSets", &CModel::GetLevelSets)
        .function("RemoveResource", &CModel::RemoveResource)
    ;
}
