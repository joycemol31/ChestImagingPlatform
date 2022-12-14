/*=========================================================================
 *
 *  Copyright Insight Software Consortium
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *         http://www.apache.org/licenses/LICENSE-2.0.txt
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 *=========================================================================*/
#ifndef __itkBinaryFunctorImageFilter2_h
#define __itkBinaryFunctorImageFilter2_h

#include "itkInPlaceImageFilter.h"
#include "itkBinaryFunctorBase.h"
#include "itkSimpleDataObjectDecorator.h"

namespace itk
{
/** \class BinaryFunctorImageFilter2
 * \brief Implements pixel-wise generic operation of two images,
 * or of an image and a constant.
 *
 * This class is parameterized over the types of the two input images
 * and the type of the output image.  It is also parameterized by the
 * operation to be applied.  A Functor style is used.
 *
 * The constant must be of the same type than the pixel type of the corresponding
 * image. It is wrapped in a SimpleDataObjectDecorator so it can be updated through
 * the pipeline. The SetConstant() and GetConstant() methods are provided as shortcuts
 * to set or get the constant value without manipulating the decorator.
 *
 * Compared to the original BinaryFunctorImageFilter this class is not
 * templated over the Functor type, but allows run-time setting of the
 * Functor, as long as it derives from a BinaryFunctorBase class.
 *
 * \sa UnaryFunctorImageFilter2 BinaryFunctorBase
 *
 * \ingroup IntensityImageFilters   MultiThreaded
 * \ingroup ITKImageFilterBase
 *
 * \wiki
 * \wikiexample{ImageProcessing/BinaryFunctorImageFilter,Apply a predefined operation to corresponding pixels in two images}
 * \wikiexample{ImageProcessing/BinaryFunctorImageFilterCustom,Apply a custom operation to corresponding pixels in two images}
 * \endwiki
 */
template< class TInputImage1, class TInputImage2, class TOutputImage >
class ITK_EXPORT BinaryFunctorImageFilter2
  : public InPlaceImageFilter< TInputImage1, TOutputImage >
{
public:
  /** Standard class typedefs. */
  typedef BinaryFunctorImageFilter2                        Self;
  typedef InPlaceImageFilter< TInputImage1, TOutputImage > Superclass;
  typedef SmartPointer< Self >                             Pointer;
  typedef SmartPointer< const Self >                       ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro( Self );

  /** Run-time type information (and related methods). */
  itkTypeMacro( BinaryFunctorImageFilter2, InPlaceImageFilter );

  /** Some convenient typedefs. */
  typedef TInputImage1                            Input1ImageType;
  typedef typename Input1ImageType::ConstPointer  Input1ImagePointer;
  typedef typename Input1ImageType::RegionType    Input1ImageRegionType;
  typedef typename Input1ImageType::PixelType     Input1ImagePixelType;
  typedef SimpleDataObjectDecorator<Input1ImagePixelType>
                                                  DecoratedInput1ImagePixelType;
  typedef TInputImage2                            Input2ImageType;
  typedef typename Input2ImageType::ConstPointer  Input2ImagePointer;
  typedef typename Input2ImageType::RegionType    Input2ImageRegionType;
  typedef typename Input2ImageType::PixelType     Input2ImagePixelType;
  typedef SimpleDataObjectDecorator<Input2ImagePixelType>
                                                  DecoratedInput2ImagePixelType;
  typedef TOutputImage                            OutputImageType;
  typedef typename OutputImageType::Pointer       OutputImagePointer;
  typedef typename OutputImageType::RegionType    OutputImageRegionType;
  typedef typename OutputImageType::PixelType     OutputImagePixelType;

  /** Set a functor deriving from BinaryFunctorBase. */
  typedef Functor::BinaryFunctorBase<
    Input1ImagePixelType, Input2ImagePixelType,
    OutputImagePixelType>                         FunctorType;
  itkSetObjectMacro( Functor, FunctorType );

  /** Connect one of the operands for pixel-wise operation */
  virtual void SetInput1(const TInputImage1 *image1);
  virtual void SetInput1(const DecoratedInput1ImagePixelType *input1);
  virtual void SetInput1(const Input1ImagePixelType &input1);

  /** Set the first operand as a constant */
  virtual void SetConstant1(const Input1ImagePixelType &input1);

  /** Get the constant value of the first operand. An exception is sent if
   * the first operand is not a constant
   */
  virtual const Input1ImagePixelType & GetConstant1() const;

  /** Connect one of the operands for pixel-wise operation */
  virtual void SetInput2(const TInputImage2 *image2);
  virtual void SetInput2(const DecoratedInput2ImagePixelType *input2);
  virtual void SetInput2(const Input2ImagePixelType &input2);

  /** Set the second operand as a constant */
  virtual void SetConstant2(const Input2ImagePixelType &input2);
  void SetConstant(Input2ImagePixelType ct)
  {
    this->SetConstant2(ct);
  }
  const Input2ImagePixelType & GetConstant() const
  {
    return this->GetConstant2();
  }

  /** Get the constant value of the second operand. An exception is sent if
   * the second operand is not a constant.
   */
  virtual const Input2ImagePixelType & GetConstant2() const;

  /** ImageDimension constants */
  itkStaticConstMacro(
    InputImage1Dimension, unsigned int, TInputImage1::ImageDimension);
  itkStaticConstMacro(
    InputImage2Dimension, unsigned int, TInputImage2::ImageDimension);
  itkStaticConstMacro(
    OutputImageDimension, unsigned int, TOutputImage::ImageDimension);

#ifdef ITK_USE_CONCEPT_CHECKING
  /** Begin concept checking */
  itkConceptMacro( SameDimensionCheck1,
                   ( Concept::SameDimension< itkGetStaticConstMacro(InputImage1Dimension),
                                             itkGetStaticConstMacro(InputImage2Dimension) > ) );
  itkConceptMacro( SameDimensionCheck2,
                   ( Concept::SameDimension< itkGetStaticConstMacro(InputImage1Dimension),
                                             itkGetStaticConstMacro(OutputImageDimension) > ) );
  /** End concept checking */
#endif
protected:
  BinaryFunctorImageFilter2();
  virtual ~BinaryFunctorImageFilter2() {}

  /** BinaryFunctorImageFilter can be implemented as a multithreaded filter.
   * Therefore, this implementation provides a ThreadedGenerateData() routine
   * which is called for each processing thread. The output image data is
   * allocated automatically by the superclass prior to calling
   * ThreadedGenerateData().  ThreadedGenerateData can only write to the
   * portion of the output image specified by the parameter
   * "outputRegionForThread"
   *
   * \sa ImageToImageFilter::ThreadedGenerateData(),
   *     ImageToImageFilter::GenerateData()  */
  virtual void ThreadedGenerateData(
    const OutputImageRegionType & outputRegionForThread,
    ThreadIdType threadId ) override;

  // needed to take the image information from the 2nd input, if the first one is
  // a simple decorated object
  virtual void GenerateOutputInformation( void ) override;

private:
  BinaryFunctorImageFilter2(const Self &); // purposely not implemented
  void operator=(const Self &);            // purposely not implemented

  typename FunctorType::Pointer m_Functor;
};
} // end namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkBinaryFunctorImageFilter2.hxx"
#endif

#endif
