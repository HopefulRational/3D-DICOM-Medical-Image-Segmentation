
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkImage.h"
#include "itkNeighborhoodAlgorithm.h"
#include "itkNeighborhoodIterator.h"
#include "itkImageRegionIterator.h"
#include "itkSize.h"

#include<math.h>
#include<stdio.h>
#include<stdlib.h>


int main( int argc, char ** argv )
{
  // Verify the number of parameters in the command line
  if( argc < 4 )
    {
    std::cerr << "Usage: " << std::endl;
    std::cerr << argv[0] << " inputImageFile  Res_Image outputImageFile" << std::endl;
    return EXIT_FAILURE;
    }

  typedef short		 PixelType1;
  typedef short 	 PixelType2;
  const   unsigned int  Dimension = 3;
  typedef itk::Image< PixelType1, Dimension >    ImageType;
  typedef itk::Image< PixelType2, Dimension >    ImageType2;
  unsigned int size[3];
  int i,j,k;
  int m,n,o;
  int ***src, ***dest;

  typedef itk::ImageFileReader< ImageType >  ReaderType;
  typedef itk::ImageFileWriter< ImageType2 >  WriterType;
  typedef itk::ImageRegionIterator< ImageType> IteratorType;
  typedef itk::NeighborhoodIterator< ImageType > NeighborhoodIteratorType;
  
  ImageType::RegionType region1;
  ImageType::RegionType region2;
  ImageType2::RegionType region;
  ImageType::IndexType pixelIndex;
  
  ReaderType::Pointer reader1 = ReaderType::New();
  ReaderType::Pointer reader2 = ReaderType::New();
  WriterType::Pointer writer = WriterType::New();


  const char * inputFilename1  = argv[1];
  const char * inputFilename2  = argv[2];
  const char * outputFilename  = argv[3];

  // Read the input image
  reader1->SetFileName( inputFilename1  );
  reader2->SetFileName( inputFilename2  );
  writer->SetFileName( outputFilename );
  ImageType::Pointer image1 = reader1->GetOutput();
  ImageType::Pointer image2 = reader2->GetOutput();
  reader1->Update();
  reader2->Update();
  region1 = image1->GetLargestPossibleRegion();
  region2 = image2->GetLargestPossibleRegion();
  
  // Check dimensions of input image
  std::cout << "Input image" << std::endl;
  for(i=0; i<3; i++)
    std::cout << region1.GetSize()[i] << std::endl;

  // Create an output image
  ImageType2::Pointer newImage = ImageType2::New();
  newImage->SetRegions( image1->GetLargestPossibleRegion() );
  newImage->Allocate();
  region = newImage->GetLargestPossibleRegion();
  m = region1.GetSize()[0];
  n = region1.GetSize()[1];
  o = region1.GetSize()[2];
  short tmp;
  short low = 0;
  short high = 255;
  for(i=0; i<m; i++)
    for(j=0; j<n; j++)
      for(k=0; k<o; k++)
      {
        pixelIndex[0] = i;
        pixelIndex[1] = j;
        pixelIndex[2] = k;
	tmp = image1->GetPixel(pixelIndex);
	if(tmp == 255 && tmp != image2->GetPixel(pixelIndex))
        	newImage->SetPixel(pixelIndex,255);
	else
		newImage->SetPixel(pixelIndex,0);
/*	std::cout << tmp << std::endl;
	if(tmp > 0)
        	newImage->SetPixel(pixelIndex,255);
	else
		newImage->SetPixel(pixelIndex,0);
*/     }

  
  // Write the image
  writer->SetInput( newImage );

  try
    {
    writer->Update();
    }
  catch( itk::ExceptionObject & err )
    {
    std::cerr << "ExceptionObject caught !" << std::endl;
    std::cerr << err << std::endl;
    return EXIT_FAILURE;
    }

  return EXIT_SUCCESS;
}


