#ifndef __vtkNRRDWriterCIP_h
#define __vtkNRRDWriterCIP_h

#include "vtkWriter.h"

#include "vtkMatrix4x4.h"
#include "vtkDoubleArray.h"
#include "teem/nrrd.h"

#include "vtkCIPUtilitiesConfigure.h"

class vtkImageData;
class AttributeMapType;

/// \brief Writes PNG files.
///
/// vtkNRRDWriterCIP writes NRRD files.
///
/// \sa vtkNRRDReaderCIP
class VTK_CIP_UTILITIES_EXPORT vtkNRRDWriterCIP : public vtkWriter
{
public:

  vtkTypeMacro(vtkNRRDWriterCIP,vtkWriter);
  void PrintSelf(ostream& os, vtkIndent indent) override;

  static vtkNRRDWriterCIP *New();

  ///
  /// Get the input to this writer.
  vtkImageData* GetInput();
  vtkImageData* GetInput(int port);

  ///
  /// Specify file name of vtk polygon data file to write.
  vtkSetStringMacro(FileName);
  vtkGetStringMacro(FileName);

  vtkSetObjectMacro(DiffusionGradients,vtkDoubleArray);
  vtkGetObjectMacro(DiffusionGradients,vtkDoubleArray);

  vtkSetObjectMacro(BValues,vtkDoubleArray);
  vtkGetObjectMacro(BValues,vtkDoubleArray);

  vtkSetObjectMacro(IJKToRASMatrix,vtkMatrix4x4);
  vtkGetObjectMacro(IJKToRASMatrix,vtkMatrix4x4);

  vtkSetObjectMacro(MeasurementFrameMatrix,vtkMatrix4x4);
  vtkGetObjectMacro(MeasurementFrameMatrix,vtkMatrix4x4);

  vtkSetMacro(UseCompression,int);
  vtkGetMacro(UseCompression,int);
  vtkBooleanMacro(UseCompression,int);

  vtkSetClampMacro(FileType,int,VTK_ASCII,VTK_BINARY);
  vtkGetMacro(FileType,int);
  void SetFileTypeToASCII() {this->SetFileType(VTK_ASCII);};
  void SetFileTypeToBinary() {this->SetFileType(VTK_BINARY);};

  vtkBooleanMacro(WriteError, int);
  vtkSetMacro(WriteError, int);
  vtkGetMacro(WriteError, int);

  /// Method to set an attribute that will be passed into the NRRD
  /// file on write
  void SetAttribute(const std::string& name, const std::string& value);

protected:
  vtkNRRDWriterCIP();
  ~vtkNRRDWriterCIP();

  virtual int FillInputPortInformation(int port, vtkInformation *info) override;

  ///
  /// Write method. It is called by vtkWriter::Write();
  void WriteData() override;

  ///
  /// Flag to set to on when a write error occured
  int WriteError;

  char *FileName;

  vtkDoubleArray *BValues;
  vtkDoubleArray *DiffusionGradients;

  vtkMatrix4x4 *IJKToRASMatrix;
  vtkMatrix4x4 *MeasurementFrameMatrix;

  int UseCompression;
  int FileType;

  AttributeMapType *Attributes;

private:
  vtkNRRDWriterCIP(const vtkNRRDWriterCIP&);  /// Not implemented.
  void operator=(const vtkNRRDWriterCIP&);  /// Not implemented.
  void vtkImageDataInfoToNrrdInfo(vtkImageData *in, int &nrrdKind, size_t &numComp, int &vtkType, void **buffer);
  int VTKToNrrdPixelType( const int vtkPixelType );
  int DiffusionWeigthedData;
};

#endif
