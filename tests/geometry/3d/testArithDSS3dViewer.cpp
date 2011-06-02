/**
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as
 *  published by the Free Software Foundation, either version 3 of the
 *  License, or  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 **/

/**
 * @file testArithDSS3dViewer.cpp
 * @ingroup Tests
 * @author Tristan Roussillon (\c tristan.roussillon@liris.cnrs.fr )
 * Laboratoire d'InfoRmatique en Image et Systèmes d'information - LIRIS (CNRS, UMR 5205), CNRS, France
 *
 *
 * @date 2011/06/01
 *
 * This file is part of the DGtal library
 */

/**
 * Description of testArithDSS3dViewer <p>
 * Aim: simple test of \ref ArithmeticalDSS3d
 */




#include <iostream>

#include <QtGui/qapplication.h>
#include "DGtal/io-viewers/3dViewers/DGtalQGLViewer.h"
#include "DGtal/io-viewers/readers/PointListReader.h"


#include "DGtal/base/Common.h"
#include "DGtal/helpers/StdDefs.h"
#include "../examples/ConfigExamples.h"

#include "DGtal/geometry/3d/ArithmeticalDSS3d.h"
#include "DGtal/geometry/2d/GreedyDecomposition.h"



using namespace std;
using namespace DGtal;
using namespace Z3i;


// Standard services - public :

int main( int argc, char** argv )
{


	typedef PointVector<3,int> Point;
	typedef std::vector<Point>::iterator Iterator;
	typedef ArithmeticalDSS3d<Iterator,int,8> SegmentComputer;  
	typedef GreedyDecomposition<SegmentComputer> Decomposition;

  string inputFilename = examplesPath + "samples/spheric1.dat"; 
	vector<Point> sequence = PointListReader<Point>::getPointsFromFile(inputFilename); 


	SegmentComputer algo;
	Decomposition theDecomposition(sequence.begin(), sequence.end(), algo, false);
	
	///////////////////////////////////
	//display			
	QApplication application(argc,argv);
	DGtalQGLViewer viewer;
	viewer.show();
 
		unsigned int c = 0;
		Decomposition::ConstIterator i = theDecomposition.begin();
		for ( ; i != theDecomposition.end(); ++i) {
			SegmentComputer currentSegmentComputer(*i);
			if (c%2==0) {
				viewer << CustomColors3D(QColor(250, 0,0),QColor(250, 0,0));
			} else {
				viewer << CustomColors3D(QColor(0, 250,0),QColor(0, 250,0));
			}
			viewer << currentSegmentComputer;	//view voxels
			c++;
		} 
 
	viewer << DGtalQGLViewer::updateDisplay;
	return application.exec();
}

