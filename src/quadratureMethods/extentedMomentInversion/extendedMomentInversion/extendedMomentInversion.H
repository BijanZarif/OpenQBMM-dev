/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | Copyright (C) 2014-2015 Alberto Passalacqua
     \\/     M anipulation  |
-------------------------------------------------------------------------------
License
    This file is derivative work of OpenFOAM.

    OpenFOAM is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    OpenFOAM is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with OpenFOAM.  If not, see <http://www.gnu.org/licenses/>.

Class
    Foam::extendedMomentInversion

Description
    Abstract class to allow run-time selection of the algorithm to be used 
    for the extended quadrature method of moments.
        
    References
    \verbatim
        "An extended quadrature method of moments for population balance 
        equations"
        C Yuan, F Laurent, R O Fox
        Journal of Aerosol Science
        Volume 51, Pages 1-23, 2012
    \endverbatim
    
    \verbatim
        "The theory of canonical moments with applications in Statistics, 
        Probability and Analysis"
        H Dette, W J Studden 
        Wiley & Sons, 1997
    \endverbatim
    
    \verbatim
        "Orthogonal Polynomials: Computation and Approximation" 
        W Gautschi 
        Oxford University Press, 2004
    \endverbatim
    
SourceFiles
    extendedMomentInversion.C
    newExtendedMomentInversion.C

\*---------------------------------------------------------------------------*/

#ifndef extendedMomentInversion_H
#define extendedMomentInversion_H

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "scalar.H"
#include "scalarMatrices.H"
#include "dictionary.H"
#include "runTimeSelectionTables.H"
#include "univariateMomentSet.H"

namespace Foam
{

/*---------------------------------------------------------------------------*\
                     Class extendedMomentInversion Declaration
\*---------------------------------------------------------------------------*/

class extendedMomentInversion
{
    // Private member functions

        //- Bracket minimum of the target function
        void bracketMinimumTargetFunction
        (
            scalar a, 
            scalar b, 
            scalar& newA,
            scalar& newB,
            scalar& newC,
            const univariateMomentSet& moments,
            univariateMomentSet& momentsStar
        );

        //- Attempt to find a min or max of the target function on an interval
        scalar minimizeTargetFunction
        (
            scalar sigmaLow, 
            scalar sigmaHigh,
            const univariateMomentSet& moments,
            univariateMomentSet& momentsStar
        );

        //- Reset inverter
        void reset();

        //- Compute secondary weigths and abscissae
        void secondaryQuadrature(const univariateMomentSet& moments);
        
        //- Compute target function whose root is sigma_
        scalar targetFunction
        (
            scalar sigma, 
            const univariateMomentSet& moments,
            univariateMomentSet& momentsStar
        );
    
        //- Disallow default bitwise copy construct
        extendedMomentInversion(const extendedMomentInversion&);       

        //- Disallow default bitwise assignment
        void operator=(const extendedMomentInversion&);


protected:

    // Protected data

        //- Number of moments
        label nMoments_;

        //- Number of primary quadrature nodes
        label nPrimaryNodes_;

        //- Number of secondary quadrature nodes
        label nSecondaryNodes_;

        //- Primary quadrature weights
        scalarDiagonalMatrix primaryWeights_;
        
        //- Primary quadrature abscissae
        scalarDiagonalMatrix primaryAbscissae_;
        
        //- Parameter sigma of the kernel density function
        scalar sigma_;

        //- Secondary quadrature weights
        scalarRectangularMatrix secondaryWeights_;
            
        //- Secondary quadrature abscissae
        scalarRectangularMatrix secondaryAbscissae_;

        //- Maximum number of iterations allowed to find sigma_
        label maxSigmaIter_;

        //- Tolerance on moment conservation
        scalar momentsTol_;

        //- Tolerance for the change in the sigma parameter
        scalar sigmaTol_;

        //- Tolerance for the target function
        scalar targetFunctionTol_;
        
        //- Bool to track values of sigma_ that lead to unrealizable moments
        bool foundUnrealizableSigma_;
        
        //- Bool to track if sigma = 0 is root
        bool nullSigma_;
        
        //- Bool to check if sigma is bracketed
        bool sigmaBracketed_;

    //- Protected member functions
    
        //- Computes kernel density function from abscissa and sigma
        virtual scalar secondaryAbscissa
        (
            scalar primaryAbscissa,
            scalar secondaryAbscissa,
            scalar sigma
        ) = 0;

        //- Compute moments from starred moments
        virtual void momentsStarToMoments
        (
            scalar sigma,
            univariateMomentSet& moments,
            const univariateMomentSet& momentsStar
        ) = 0;

        //- Compute the starred moments from moments
        virtual void momentsToMomentsStar
        (
            scalar sigma,
            const univariateMomentSet& moments,
            univariateMomentSet& momentsStar
        ) = 0;

        //- Compute the last moment from starred moments
        virtual scalar m2N(scalar sigma, univariateMomentSet momentsStar) = 0;
        
        //- Compute the normalized moment error
        scalar normalizedMomentError
        (
            scalar sigma,
            const univariateMomentSet& moments,
            univariateMomentSet& momentsStar
        );
        
        //- Recurrence relation for polynomials orthogonal to kernel function
        virtual void recurrenceRelation
        (
            scalarDiagonalMatrix& a, 
            scalarDiagonalMatrix& b,
            scalar primaryAbscissa
        ) = 0;
        
        //- Compute maximum value of sigma to ensure realizability
        //  The value is exact for the two-node case. For cases with a larger
        //  number of nodes, this provides an over-estimation of the maximum
        //  value of the sigma parameter which ensures moment realizability.
        virtual scalar sigmaMax(const univariateMomentSet& moments) = 0;


public:

    //- Runtime type information
    TypeName("extendedMomentInversion");


    // Declare runtime construction

        declareRunTimeSelectionTable
        (
            autoPtr,
            extendedMomentInversion,
            dictionary,
            (
                const dictionary& dict,
                const label nMoments,
                const label nSecondaryNodes
            ),
            (dict, nMoments, nSecondaryNodes)
        );

    // Constructors

        // Construct from dictionary and label
        extendedMomentInversion
        (
            const dictionary& dict,
            const label nMoments,
            const label nSecondaryNodes
        );


    //- Destructor
    virtual ~extendedMomentInversion();


    // Selectors

        static autoPtr<extendedMomentInversion> New
        (
            const dictionary& dict,
            const label nMoments,
            const label nSecondaryNodes
        );


    // Member Functions

        //- Invert moments to find weight, abscissae and sigma
        void invert(const univariateMomentSet& moments);
        
        //- Return number of moments
        inline label nMoments()
        {
            return nMoments_;
        }

        //- Return number of primary quadrature nodes
        inline label nPrimaryNodes()
        {
            return nPrimaryNodes_;
        }

        //- Return number of secondary quadrature nodes
        inline label nSecondaryNodes()
        {
            return nSecondaryNodes_;
        }

        //- Return the value of the sigma parameter
        inline scalar sigma() const
        {
            return sigma_;
        }

        //- Return primary quadrature weights
        inline const scalarDiagonalMatrix& primaryWeights() const
        {
            return primaryWeights_;
        }

        //- Return primary quadrature abscissae
        inline const scalarDiagonalMatrix& primaryAbscissae() const
        {
            return primaryAbscissae_;
        }

        //- Return secondary quadrature weights
        inline const scalarRectangularMatrix& secondaryWeights() const
        {
            return secondaryWeights_;
        }

        //- Return secondary quadrature abscissae
        inline const scalarRectangularMatrix& secondaryAbscissae() const
        {
            return secondaryAbscissae_;
        }

};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
