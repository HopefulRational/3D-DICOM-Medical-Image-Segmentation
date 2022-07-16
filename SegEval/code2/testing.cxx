
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
  if( argc < 3 )
    {
    std::cerr << "Usage: " << std::endl;
    std::cerr << argv[0] << " inputImageFile  outputImageFile [ radius ]" << std::endl;
    return EXIT_FAILURE;
    }
  int radius = 1;
  if(argc>=4)
    sscanf(argv[3],"%d",&radius);

  typedef unsigned char  PixelType1;
  typedef short 	 PixelType2;
  const   unsigned int  Dimension = 3;
  typedef itk::Image< PixelType2, Dimension >    ImageType;
  typedef itk::Image< PixelType2, Dimension >    ImageType2;
  unsigned int size[3];
  int i,j,k;
  int m,n,o;
  int ***src, ***dest;

  typedef itk::ImageFileReader< ImageType >  ReaderType;
  typedef itk::ImageFileWriter< ImageType2 >  WriterType;
  typedef itk::ImageRegionIterator< ImageType> IteratorType;
  typedef itk::NeighborhoodIterator< ImageType > NeighborhoodIteratorType;
  
  ImageType::RegionType region;
  ImageType::IndexType pixelIndex;
  
  ReaderType::Pointer reader  = ReaderType::New();
  ReaderType::Pointer reader1 = ReaderType::New();
  WriterType::Pointer writer  = WriterType::New();


  const char * inputFilename  = argv[1];
  const char * inputFilename1 = argv[2];
  const char * outputFilename = argv[3];

  // Read the input image
  reader->SetFileName( inputFilename  );
  reader1->SetFileName( inputFilename1 );
  writer->SetFileName( outputFilename );
  ImageType::Pointer image = reader->GetOutput();
  ImageType::Pointer image1 = reader1->GetOutput();
  reader->Update();
  reader1->Update();
  region = image->GetLargestPossibleRegion();
  
  // Check dimensions of input image
  std::cout << "Input image" << std::endl;
  for(i=0; i<3; i++)
    std::cout << region.GetSize()[i] << std::endl;

  // Create an output image
  ImageType2::Pointer newImage = ImageType2::New();
  newImage->SetRegions( image->GetLargestPossibleRegion() );
  newImage->Allocate();
  region = newImage->GetLargestPossibleRegion();
  m = region.GetSize()[0];
  n = region.GetSize()[1];
  o = region.GetSize()[2];
  short tmp;
  short val;
  for(i=0; i<m; i++)
    for(j=0; j<n; j++)
      for(k=0; k<o; k++)
      {
        pixelIndex[0] = i;
        pixelIndex[1] = j;
        pixelIndex[2] = k;
	tmp = image->GetPixel(pixelIndex);
	std::cout << tmp << std::endl;
	if(tmp > 0)
        	newImage->SetPixel(pixelIndex,image1->GetPixel(pixelIndex));
	else
		newImage->SetPixel(pixelIndex,0);
      }

  
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


