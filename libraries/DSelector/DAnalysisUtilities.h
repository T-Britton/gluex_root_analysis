#ifndef _DAnalysisUtilities_
#define _DAnalysisUtilities_

#include <deque>
#include <set>

#include "TSystem.h"

#include "DKinematicData.h"
#include "DParticleCombo.h"

using namespace std;

class DAnalysisUtilities
{
	public:
		TLorentzVector Calc_MissingP4(const DParticleCombo* locParticleComboWrapper, bool locUseKinFitDataFlag) const;
		TLorentzVector Calc_MissingP4(const DParticleCombo* locParticleComboWrapper, map<Particle_t, set<Int_t> >& locSourceObjects, bool locUseKinFitDataFlag) const;
		TLorentzVector Calc_MissingP4(const DParticleCombo* locParticleComboWrapper, size_t locStepIndex, int locUpToStepIndex, set<size_t> locUpThroughIndices, bool locUseKinFitDataFlag) const;
		TLorentzVector Calc_MissingP4(const DParticleCombo* locParticleComboWrapper, size_t locStepIndex, int locUpToStepIndex, set<size_t> locUpThroughIndices, map<Particle_t, set<Int_t> >& locSourceObjects, bool locUseKinFitDataFlag) const;
		TLorentzVector Calc_FinalStateP4(const DParticleCombo* locParticleComboWrapper, size_t locStepIndex, bool locUseKinFitDataFlag) const;
		TLorentzVector Calc_FinalStateP4(const DParticleCombo* locParticleComboWrapper, size_t locStepIndex, map<Particle_t, set<Int_t> >& locSourceObjects, bool locUseKinFitDataFlag) const;
		TLorentzVector Calc_FinalStateP4(const DParticleCombo* locParticleComboWrapper, size_t locStepIndex, set<size_t> locToIncludeIndices, bool locUseKinFitDataFlag) const;
		TLorentzVector Calc_FinalStateP4(const DParticleCombo* locParticleComboWrapper, size_t locStepIndex, set<size_t> locToIncludeIndices, map<Particle_t, set<Int_t> >& locSourceObjects, bool locUseKinFitDataFlag) const;

		bool Get_IsPolarizedBeam(int locRunNumber, bool& locIsPARAFlag) const; //RCDB environment must be setup!!
		double Get_BeamBunchPeriod(int locRunNumber) const; //CCDB environment must be setup!!

		double Calc_ProdPlanePhi_Pseudoscalar(double locBeamEnergy, Particle_t locTargetPID, const TLorentzVector& locMesonP4) const;
		double Calc_DecayPlanePsi_Vector_2BodyDecay(double locBeamEnergy, Particle_t locTargetPID, const TLorentzVector& locBaryonP4, const TLorentzVector& locMesonP4, const TLorentzVector& locMesonProduct1P4, double& locDecayPlaneTheta) const;
		double Calc_DecayPlanePsi_Vector_3BodyDecay(double locBeamEnergy, Particle_t locTargetPID, const TLorentzVector& locBaryonP4, const TLorentzVector& locMesonP4, const TLorentzVector& locMesonProduct1P4, const TLorentzVector& locMesonProduct2P4, double& locDecayPlaneTheta) const;

		set<set<size_t> > Build_IndexCombos(const DParticleComboStep* locParticleComboStepWrapper, deque<Particle_t> locToIncludePIDs) const;
		double* Generate_LogBinning(int locLowest10Power, int locHighest10Power, unsigned int locNumBinsPerPower, int& locNumBins) const;

	private:

		bool Handle_Decursion(int& locParticleIndex, deque<size_t>& locComboDeque, deque<int>& locResumeAtIndices, deque<deque<size_t> >& locPossibilities) const;
};

#endif // _DAnalysisUtilities_
