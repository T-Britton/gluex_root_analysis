#ifndef _DCutActions_
#define _DCutActions_

#include <set>
#include <string>
#include <map>
#include <iostream>
#include <sstream>

#include "TLorentzVector.h"
#include "TVector3.h"
#include "TF1.h"

#include "particleType.h"

#include "DParticleCombo.h"
#include "DParticleComboStep.h"
#include "DKinematicData.h"
#include "DBeamParticle.h"
#include "DMCThrown.h"
#include "DChargedTrackHypothesis.h"
#include "DNeutralParticleHypothesis.h"
#include "DAnalysisUtilities.h"
#include "DAnalysisAction.h"

using namespace std;

class DCutAction_PIDDeltaT : public DAnalysisAction
{
	public:
		DCutAction_PIDDeltaT(const DParticleCombo* locParticleComboWrapper, bool locUseKinFitFlag, double locDeltaTCut, Particle_t locPID = Unknown, DetectorSystem_t locSystem = SYS_NULL, string locActionUniqueString = "") :
			DAnalysisAction(locParticleComboWrapper, "Cut_PIDDeltaT", locUseKinFitFlag, locActionUniqueString),
			dDeltaTCut(locDeltaTCut), dPID(locPID), dSystem(locSystem) {}

		string Get_ActionName(void) const;
		void Initialize(void);
		void Reset_NewEvent(void){}
		bool Perform_Action(void); // flag to reject combos

	private:
		double dDeltaTCut;
		Particle_t dPID;
		DetectorSystem_t dSystem;

		double dTargetCenterZ;
};

class DCutAction_NoPIDHit : public DAnalysisAction
{
	public:
		DCutAction_NoPIDHit(const DParticleCombo* locParticleComboWrapper, Particle_t locPID = Unknown, DetectorSystem_t locSystem = SYS_NULL, string locActionUniqueString = "") :
			DAnalysisAction(locParticleComboWrapper, "Cut_NoPIDHit", false, locActionUniqueString),
			dPID(locPID), dSystem(locSystem) {}

		string Get_ActionName(void) const;
		void Initialize(void){}
		void Reset_NewEvent(void){}
		bool Perform_Action(void); // flag to reject combos

	private:
		Particle_t dPID;
		DetectorSystem_t dSystem;
};

class DCutAction_dEdxProton : public DAnalysisAction
{
	public:
		DCutAction_dEdxProton(const DParticleCombo* locParticleComboWrapper, bool locUseKinFitFlag = false, Particle_t locPID = Unknown, DetectorSystem_t locSystem = SYS_NULL, string locActionUniqueString = "") :
			DAnalysisAction(locParticleComboWrapper, "Cut_dEdxProton", locUseKinFitFlag, locActionUniqueString),
			dPID(locPID), dSystem(locSystem) {}

		void Initialize(void);
		void Reset_NewEvent(void){}
		bool Perform_Action(void); // flag to reject combos

	private:

		Particle_t dPID;
		DetectorSystem_t dSystem;
		TF1 *f;
};

class DCutAction_KinFitFOM : public DAnalysisAction
{
	public:
		DCutAction_KinFitFOM(const DParticleCombo* locParticleComboWrapper, double locMinimumConfidenceLevel, string locActionUniqueString = "") :
			DAnalysisAction(locParticleComboWrapper, "Cut_KinFitFOM", true, locActionUniqueString), dMinimumConfidenceLevel(locMinimumConfidenceLevel){}

		string Get_ActionName(void) const;
		void Initialize(void){};
		void Reset_NewEvent(void){}
		bool Perform_Action(void); // flag to reject combos

	private:
		const string dKinFitName;
		double dMinimumConfidenceLevel;
};

class DCutAction_MissingMass : public DAnalysisAction
{
	public:
		DCutAction_MissingMass(const DParticleCombo* locParticleComboWrapper, bool locUseKinFitFlag, double locMinimumMissingMass, double locMaximumMissingMass, string locActionUniqueString = "") :
			DAnalysisAction(locParticleComboWrapper, "Cut_MissingMass", locUseKinFitFlag, locActionUniqueString),
			dMinimumMissingMass(locMinimumMissingMass), dMaximumMissingMass(locMaximumMissingMass), dMissingMassOffOfStepIndex(0){}

		//E.g. If:
		//g, p -> K+, K+, Xi-
		//                Xi- -> pi-, Lambda
		//                            Lambda -> (p), pi-
		//And:
		//locMissingMassOffOfStepIndex = 0, locMissingMassOffOfPIDs = K+, K+
		//Then: Will cut missing-mass: g, p -> K+, K+, (X)
		//Also:
		//locMissingMassOffOfStepIndex = 1, locMissingMassOffOfPID = pi-
		//Then: Will cut missing-mass: g, p -> K+, K+, pi- (from Xi- decay)
		//But:
		//locMissingMassOffOfStepIndex = 0, locMissingMassOffOfPIDs = K+
		//Then: Will cut only missing-mass: g, p -> K+_1, (X)    and NOT K+_2!!!
		DCutAction_MissingMass(const DParticleCombo* locParticleComboWrapper, bool locUseKinFitFlag, int locMissingMassOffOfStepIndex, deque<Particle_t> locMissingMassOffOfPIDs, double locMinimumMissingMass, double locMaximumMissingMass, string locActionUniqueString = "") :
			DAnalysisAction(locParticleComboWrapper, "Cut_MissingMass", locUseKinFitFlag, locActionUniqueString),
			dMinimumMissingMass(locMinimumMissingMass), dMaximumMissingMass(locMaximumMissingMass), dMissingMassOffOfStepIndex(locMissingMassOffOfStepIndex),
			dMissingMassOffOfPIDs(locMissingMassOffOfPIDs) {}

		DCutAction_MissingMass(const DParticleCombo* locParticleComboWrapper, bool locUseKinFitFlag, int locMissingMassOffOfStepIndex, Particle_t locMissingMassOffOfPID, double locMinimumMissingMass, double locMaximumMissingMass, string locActionUniqueString = "") :
			DAnalysisAction(locParticleComboWrapper, "Cut_MissingMass", locUseKinFitFlag, locActionUniqueString),
			dMinimumMissingMass(locMinimumMissingMass), dMaximumMissingMass(locMaximumMissingMass), dMissingMassOffOfStepIndex(locMissingMassOffOfStepIndex),
			dMissingMassOffOfPIDs(deque<Particle_t>(1, locMissingMassOffOfPID)) {}

		string Get_ActionName(void) const;
		void Initialize(void){};
		void Reset_NewEvent(void){}
		bool Perform_Action(void); // flag to reject combos

	private:

		double dMinimumMissingMass;
		double dMaximumMissingMass;
		int dMissingMassOffOfStepIndex;
		deque<Particle_t> dMissingMassOffOfPIDs;

		DAnalysisUtilities dAnalysisUtilities;
};

class DCutAction_MissingMassSquared : public DAnalysisAction
{
	public:
		DCutAction_MissingMassSquared(const DParticleCombo* locParticleComboWrapper, bool locUseKinFitFlag, double locMinimumMissingMassSq, double locMaximumMissingMassSq, string locActionUniqueString = "") :
			DAnalysisAction(locParticleComboWrapper, "Cut_MissingMassSquared", locUseKinFitFlag, locActionUniqueString),
			dMinimumMissingMassSq(locMinimumMissingMassSq), dMaximumMissingMassSq(locMaximumMissingMassSq), dMissingMassOffOfStepIndex(0){}

		//E.g. If:
		//g, p -> K+, K+, Xi-
		//                Xi- -> pi-, Lambda
		//                            Lambda -> (p), pi-
		//And:
		//locMissingMassOffOfStepIndex = 0, locMissingMassOffOfPIDs = K+, K+
		//Then: Will cut missing-mass: g, p -> K+, K+, (X)
		//Also:
		//locMissingMassOffOfStepIndex = 1, locMissingMassOffOfPID = pi-
		//Then: Will cut missing-mass: g, p -> K+, K+, pi-
		//But:
		//locMissingMassOffOfStepIndex = 0, locMissingMassOffOfPIDs = K+
		//Then: Will cut only if BOTH missing-mass: g, p -> K+_1, (X)   AND   g, p -> K+_2, (X) fail.
		DCutAction_MissingMassSquared(const DParticleCombo* locParticleComboWrapper, bool locUseKinFitFlag, int locMissingMassOffOfStepIndex, deque<Particle_t> locMissingMassOffOfPIDs, double locMinimumMissingMassSq, double locMaximumMissingMassSq, string locActionUniqueString = "") :
			DAnalysisAction(locParticleComboWrapper, "Cut_MissingMassSquared", locUseKinFitFlag, locActionUniqueString),
			dMinimumMissingMassSq(locMinimumMissingMassSq), dMaximumMissingMassSq(locMaximumMissingMassSq), dMissingMassOffOfStepIndex(locMissingMassOffOfStepIndex),
			dMissingMassOffOfPIDs(locMissingMassOffOfPIDs) {}

		DCutAction_MissingMassSquared(const DParticleCombo* locParticleComboWrapper, bool locUseKinFitFlag, int locMissingMassOffOfStepIndex, Particle_t locMissingMassOffOfPID, double locMinimumMissingMassSq, double locMaximumMissingMassSq, string locActionUniqueString = "") :
			DAnalysisAction(locParticleComboWrapper, "Cut_MissingMassSquared", locUseKinFitFlag, locActionUniqueString),
			dMinimumMissingMassSq(locMinimumMissingMassSq), dMaximumMissingMassSq(locMaximumMissingMassSq), dMissingMassOffOfStepIndex(locMissingMassOffOfStepIndex),
			dMissingMassOffOfPIDs(deque<Particle_t>(1, locMissingMassOffOfPID)) {}

		string Get_ActionName(void) const;
		void Initialize(void){};
		void Reset_NewEvent(void){}
		bool Perform_Action(void); // flag to reject combos

	private:

		double dMinimumMissingMassSq;
		double dMaximumMissingMassSq;
		int dMissingMassOffOfStepIndex;
		deque<Particle_t> dMissingMassOffOfPIDs;

		DAnalysisUtilities dAnalysisUtilities;
};

class DCutAction_MissingEnergy : public DAnalysisAction
{
	public:
		DCutAction_MissingEnergy(const DParticleCombo* locParticleComboWrapper, bool locUseKinFitFlag, double locMinimumMissingEnergy, double locMaximumMissingEnergy, string locActionUniqueString = "") :
			DAnalysisAction(locParticleComboWrapper, "Cut_MissingEnergy", locUseKinFitFlag, locActionUniqueString),
			dMinimumMissingEnergy(locMinimumMissingEnergy), dMaximumMissingEnergy(locMaximumMissingEnergy), dMissingEnergyOffOfStepIndex(0){}

		DCutAction_MissingEnergy(const DParticleCombo* locParticleComboWrapper, bool locUseKinFitFlag, int locMissingEnergyOffOfStepIndex, deque<Particle_t> locMissingEnergyOffOfPIDs, double locMinimumMissingEnergy, double locMaximumMissingEnergy, string locActionUniqueString = "") :
			DAnalysisAction(locParticleComboWrapper, "Cut_MissingEnergy", locUseKinFitFlag, locActionUniqueString),
			dMinimumMissingEnergy(locMinimumMissingEnergy), dMaximumMissingEnergy(locMaximumMissingEnergy), dMissingEnergyOffOfStepIndex(locMissingEnergyOffOfStepIndex),
			dMissingEnergyOffOfPIDs(locMissingEnergyOffOfPIDs) {}

		DCutAction_MissingEnergy(const DParticleCombo* locParticleComboWrapper, bool locUseKinFitFlag, int locMissingEnergyOffOfStepIndex, Particle_t locMissingEnergyOffOfPID, double locMinimumMissingEnergy, double locMaximumMissingEnergy, string locActionUniqueString = "") :
			DAnalysisAction(locParticleComboWrapper, "Cut_MissingEnergy", locUseKinFitFlag, locActionUniqueString),
			dMinimumMissingEnergy(locMinimumMissingEnergy), dMaximumMissingEnergy(locMaximumMissingEnergy), dMissingEnergyOffOfStepIndex(locMissingEnergyOffOfStepIndex),
			dMissingEnergyOffOfPIDs(deque<Particle_t>(1, locMissingEnergyOffOfPID)) {}

		string Get_ActionName(void) const;
		void Initialize(void){};
		void Reset_NewEvent(void){}
		bool Perform_Action(void); // flag to reject combos

	private:

		double dMinimumMissingEnergy;
		double dMaximumMissingEnergy;
		int dMissingEnergyOffOfStepIndex;
		deque<Particle_t> dMissingEnergyOffOfPIDs;

		DAnalysisUtilities dAnalysisUtilities;
};

class DCutAction_InvariantMass : public DAnalysisAction
{
	public:
		DCutAction_InvariantMass(const DParticleCombo* locParticleComboWrapper, bool locUseKinFitFlag, Particle_t locInitialPID, double locMinMass, double locMaxMass, string locActionUniqueString = "") :
			DAnalysisAction(locParticleComboWrapper, "Cut_InvariantMass", locUseKinFitFlag, locActionUniqueString),
			dInitialPID(locInitialPID), dStepIndex(-1), dToIncludePIDs(deque<Particle_t>()), dMinMass(locMinMass), dMaxMass(locMaxMass){}

		//e.g. if g, p -> pi+, pi-, p
			//call with step = 0, PIDs = pi+, pi-, and will histogram rho mass
		DCutAction_InvariantMass(const DParticleCombo* locParticleComboWrapper, bool locUseKinFitFlag, size_t locStepIndex, deque<Particle_t> locToIncludePIDs, double locMinMass, double locMaxMass, string locActionUniqueString = "") :
			DAnalysisAction(locParticleComboWrapper, "Cut_InvariantMass", locUseKinFitFlag, locActionUniqueString),
			dInitialPID(Unknown), dStepIndex(locStepIndex), dToIncludePIDs(locToIncludePIDs), dMinMass(locMinMass), dMaxMass(locMaxMass){}

		string Get_ActionName(void) const;
		void Initialize(void){};
		void Reset_NewEvent(void){}
		bool Perform_Action(void); // flag to reject combos

	private:
		Particle_t dInitialPID;
		int dStepIndex;
		deque<Particle_t> dToIncludePIDs;

		double dMinMass;
		double dMaxMass;
		DAnalysisUtilities dAnalysisUtilities;
};

class DCutAction_BeamEnergy : public DAnalysisAction
{
	public:
		DCutAction_BeamEnergy(const DParticleCombo* locParticleComboWrapper, bool locUseKinFitFlag, double locMinBeamEnergy, double locMaxBeamEnergy, string locActionUniqueString = "") :
			DAnalysisAction(locParticleComboWrapper, "Cut_BeamEnergy", locUseKinFitFlag, locActionUniqueString),
			dMinBeamEnergy(locMinBeamEnergy), dMaxBeamEnergy(locMaxBeamEnergy){}

		string Get_ActionName(void) const;
		void Initialize(void){};
		void Reset_NewEvent(void){}
		bool Perform_Action(void); // flag to reject combos

	private:

		double dMinBeamEnergy;
		double dMaxBeamEnergy;
};

#endif // _DCutActions_
