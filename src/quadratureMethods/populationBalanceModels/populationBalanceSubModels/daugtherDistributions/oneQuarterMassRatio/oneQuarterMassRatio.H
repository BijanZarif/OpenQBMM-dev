/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | Copyright (C) 2015 Alberto Passalacqua
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
    Foam::populationBalanceModels::daughterDistribution::oneQuarterMassRatio

Description
    Daughter distribution of fragments with one quarter mass ratio 
    fragmentation.

SourceFiles
    oneQuarterMassRatio.C

\*---------------------------------------------------------------------------*/

#ifndef oneQuarterMassRatio_H
#define oneQuarterMassRatio_H

#include "daughterDistribution.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{
namespace populationBalanceSubModels
{
namespace daughterDistributions
{

/*---------------------------------------------------------------------------*\
                    Class oneQuarterMassRatio Declaration
\*---------------------------------------------------------------------------*/

class oneQuarterMassRatio
:
    public daughterDistribution
{
public:

        //- Runtime type information
        TypeName("oneQuarterMassRatio");


    // Constructors

        //- Construct from components
        oneQuarterMassRatio(const dictionary& dict);


    //- Destructor
    virtual ~oneQuarterMassRatio();


    // Member Functions

        //- Moment of the daughter distribution function
        virtual tmp<volScalarField> mD
        (
            const label order,
            const volScalarField& abscissa
        ) const;

};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace daughterDistributions
} // End namespace populationBalanceSubModels
} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
