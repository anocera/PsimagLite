// BEGIN LICENSE BLOCK
/*
Copyright (c) 2009, UT-Battelle, LLC
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

/*! \file ConcurrencySerial.h
 *
 * Implements the Concurrency.h interface in the Serial case
 */
#ifndef CONCURRENCY_SERIAL_HEADER_H
#define CONCURRENCY_SERIAL_HEADER_H
#include <vector>
#include "Concurrency.h"

namespace PsimagLite {
	//! Implements the Concurrency.h interface in the Serial case
	template<typename BogusType=int>
	class ConcurrencySerial : public Concurrency<BogusType> {

	public:

		typedef int CommType;
		typedef std::pair<CommType,CommType> CommPairType;

		static const CommType COMM_WORLD = 0;

		ConcurrencySerial(int argc,char *argv[]) : step_(-1),total_(0)
		{}

		int nprocs(CommType comm1=COMM_WORLD) const { return 1;}

		int rank(CommType comm1=COMM_WORLD) const { return 0; }

		std::string name() const { return "serial"; }

		template<typename DataType>
		void reduce(DataType& v,CommType mpiComm=COMM_WORLD) {}

		template<typename DataType>
		void allReduce(DataType& v,CommType mpiComm=COMM_WORLD) const {}

		template<typename DataType>
		void gather(DataType &v,CommType mpiComm=COMM_WORLD) { }

		template<typename DataType>
		void broadcast(DataType &v,CommType mpiComm=COMM_WORLD) { }

		CommPairType newCommFromSegments(size_t x,CommType mpiComm=0)
		{
			return CommPairType(0,0);
		}

		bool root(CommType commm1=COMM_WORLD) { return true; }

		void barrier() { }

	private:

		int step_;
		size_t total_;
	}; // Class ConcurrencySerial
} // namespace Dmrg

/*@}*/
#endif
