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

\*---------------------------------------------------------------------------*/

#include "populationBalanceModel.H"

// * * * * * * * * * * * * * * * * Selectors * * * * * * * * * * * * * * * * //

Foam::autoPtr<Foam::populationBalanceModel>
Foam::populationBalanceModel::New
(
    const dictionary& dict,
    const volVectorField& U,
    const surfaceScalarField& phi
)
{
    word populationBalanceModelType(dict.lookup("populationBalanceModel"));

    Info<< "Selecting populationBalanceModel "
        << populationBalanceModelType << endl;

    dictionaryConstructorTable::iterator cstrIter =
        dictionaryConstructorTablePtr_->find(populationBalanceModelType);

    if (cstrIter == dictionaryConstructorTablePtr_->end())
    {
        FatalErrorIn("populationBalanceModel::New")
            << "(" << endl
            << "    const dictionary&" << endl 
            << "    const volVectorField&" << endl
            << "    const surfaceScalarField&" << endl
            << ") : " << endl
            << "    unknown populationBalanceModelType type "
            << populationBalanceModelType
            << ", constructor not in hash table" << endl << endl
            << "    Valid populationBalanceModelType types are :" << endl
            << dictionaryConstructorTablePtr_->sortedToc()
            << abort(FatalError);
    }

    return
        autoPtr<populationBalanceModel>
        (
            cstrIter()
            (
                dict.subDict(populationBalanceModelType + "Coeffs"),
                U,
                phi
            )
        );
}


// ************************************************************************* //
