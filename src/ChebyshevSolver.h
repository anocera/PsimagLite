// BEGIN LICENSE BLOCK
/*
Copyright (c) 2009-2011, UT-Battelle, LLC
All rights reserved

[PsimagLite, Version 1.0.0]
[by G.A., Oak Ridge National Laboratory]

UT Battelle Open Source Software License 11242008

OPEN SOURCE LICENSE

Subject to the conditions of this License, each
contributor to this software hereby grants, free of
charge, to any person obtaining a copy of this software
and associated documentation files (the "Software"), a
perpetual, worldwide, non-exclusive, no-charge,
royalty-free, irrevocable copyright license to use, copy,
modify, merge, publish, distribute, and/or sublicense
copies of the Software.

1. Redistributions of Software must retain the above
copyright and license notices, this list of conditions,
and the following disclaimer.  Changes or modifications
to, or derivative works of, the Software should be noted
with comments and the contributor and organization's
name.

2. Neither the names of UT-Battelle, LLC or the
Department of Energy nor the names of the Software
contributors may be used to endorse or promote products
derived from this software without specific prior written
permission of UT-Battelle.

3. The software and the end-user documentation included
with the redistribution, with or without modification,
must include the following acknowledgment:

"This product includes software produced by UT-Battelle,
LLC under Contract No. DE-AC05-00OR22725  with the
Department of Energy."
 
*********************************************************
DISCLAIMER

THE SOFTWARE IS SUPPLIED BY THE COPYRIGHT HOLDERS AND
CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED
WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
COPYRIGHT OWNER, CONTRIBUTORS, UNITED STATES GOVERNMENT,
OR THE UNITED STATES DEPARTMENT OF ENERGY BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH
DAMAGE.

NEITHER THE UNITED STATES GOVERNMENT, NOR THE UNITED
STATES DEPARTMENT OF ENERGY, NOR THE COPYRIGHT OWNER, NOR
ANY OF THEIR EMPLOYEES, REPRESENTS THAT THE USE OF ANY
INFORMATION, DATA, APPARATUS, PRODUCT, OR PROCESS
DISCLOSED WOULD NOT INFRINGE PRIVATELY OWNED RIGHTS.

*********************************************************


*/
// END LICENSE BLOCK
/** \ingroup PsimagLite */
/*@{*/

/*! \file ChebyshevSolver.h
 *
 *  A class to represent a generic Lanczos Solver
 *
 */

#ifndef CHEBYSHEV_SOLVER_H_
#define CHEBYSHEV_SOLVER_H_
#include "ProgressIndicator.h"
#include "TridiagonalMatrix.h"
#include "Vector.h"
#include "Matrix.h"
#include "Random48.h"
#include "TypeToString.h"

namespace PsimagLite {

	//! MatrixType must have the following interface:
	//! 	RealType type to indicate the matrix type
	//! 	rank() member function to indicate the rank of the matrix
	//! 	matrixVectorProduct(std::vector<RealType>& x,const std::vector<RealType>& const y) 
	//!    	   member function that implements the operation x += Hy

	template<typename SolverParametersType,typename MatrixType,typename VectorType>
	class ChebyshevSolver {
		
	public:
		typedef typename SolverParametersType::RealType RealType;
		typedef MatrixType LanczosMatrixType;
		typedef TridiagonalMatrix<RealType> TridiagonalMatrixType;
		typedef typename VectorType::value_type VectorElementType;
		typedef Matrix<VectorElementType> DenseMatrixType;
		enum {WITH_INFO=1,DEBUG=2,ALLOWS_ZERO=4};

		ChebyshevSolver(MatrixType const &mat,const SolverParametersType& params)
		: progress_("ChebyshevSolver",0),
		  mat_(mat),
		  steps_(params.steps),
		  mode_(WITH_INFO),
		  rng_(343311)
		{
			setMode(params.options);
			std::ostringstream msg;
			msg<<"Constructing... mat.rank="<<mat_.rank()<<" steps="<<steps_;
			progress_.printline(msg,std::cout);
		}

		void computeGroundState(RealType& gsEnergy,VectorType& z)
		{
			unimplemented("computeGroundState");
		}

		void computeGroundState(
    				RealType &gsEnergy,
				VectorType &z,
	    			const VectorType& initialVector)
		{
			if (mode_ & DEBUG) {
				computeGroundStateTest(gsEnergy,z,initialVector);
				return;
			}
			unimplemented("computeGroundState");
		}

		//! ab.a contains the even moments
		//! ab.b contains the odd moments
		void decomposition(const VectorType& initVector,
    	                   TridiagonalMatrixType& ab,
		                   DenseMatrixType& lanczosVectors)
		{
			VectorType x(initVector.size(),0.0);
			VectorType y = initVector;

			lanczosVectors.resize(y.size(),steps_);
			ab.resize(steps_,0);
			for (size_t j=0; j < steps_; j++) {
				for (size_t i = 0; i < mat_.rank(); i++)
					lanczosVectors(i,j) = y[i];
				RealType atmp = 0;
				RealType btmp = 0;
				oneStepDecomposition(x,y,atmp,btmp,j==0);
				ab.a(j) = 2*atmp-ab.a(0);
				ab.a(j) = 2*btmp-ab.b(0);
			}
		}
		
		//! atmp = < phi_n | phi_n>
		//! btmp = < phi_n | phi_{n+1}>
		void oneStepDecomposition(VectorType& x,
				VectorType& y,
				RealType& atmp,
				RealType& btmp,
				bool isFirst) const
		{
			VectorType z(x.size(),0.0);
			mat_.matrixVectorProduct (z, y); // z+= Hy
			RealType val = (isFirst) ? 1.0 : 2.0;
			
			atmp = 0.0;
			for (size_t i = 0; i < mat_.rank(); i++) 
				atmp += std::real(y[i]*std::conj(y[i]));

			for (size_t i = 0; i < mat_.rank(); i++) {
				VectorElementType tmp = val*z[i] - x[i];
				x[i] = y[i];
				y[i] = tmp;
			}
			
			btmp = 0.0;
			for (size_t i = 0; i < mat_.rank(); i++)
				btmp += std::real(y[i]*std::conj(x[i]));
		}

		size_t steps() const {return steps_; }

	private:

		void unimplemented(const std::string& s) const
		{
			std::string s2("Hmmm...this ain't looking good...");
			s2 += std::string(__FILE__) + " " + ttos(__LINE__) + " ";
			s2 += s;
			throw std::runtime_error(s);
		}

		void setMode(const std::string& options)
		{
			if (options.find("lanczosdebug")!=std::string::npos) mode_ |=  DEBUG;
			if (options.find("lanczosAllowsZero")!=std::string::npos) mode_ |= ALLOWS_ZERO;
		}

		void info(RealType energyTmp,const VectorType& x,std::ostream& os)
		{
			RealType norma=norm(x);
			size_t& iter = steps_;
			
			if (norma<1e-5 || norma>100) throw std::runtime_error("Norm\n");
			
			std::ostringstream msg;
			msg.precision(8);
			msg<<"Found Energy="<<energyTmp<<" after "<<iter;
			msg<<" iterations, "<<" orig. norm="<<norma;
			progress_.printline(msg,os);
		}

		//! only for debugging:
		void computeGroundStateTest(RealType &gsEnergy,
				VectorType& z,
				const VectorType& initialVector)
		{
			unimplemented("computeGroundStateTest");
		}

		ProgressIndicator progress_;
		MatrixType const& mat_;
		size_t steps_;
		size_t mode_;
		PsimagLite::Random48<RealType> rng_;
	}; // class ChebyshevSolver
} // namespace PsimagLite

/*@}*/
#endif