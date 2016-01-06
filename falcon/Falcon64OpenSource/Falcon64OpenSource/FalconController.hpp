#ifndef FALCON_CONTROLLER
#define FALCON_CONTROLLER

#include <falcon/core/FalconDevice.h>
#include <falcon/grip/FalconGripFourButton.h>
#include <falcon/firmware/FalconFirmwareNovintSDK.h>
#include <falcon/util/FalconFirmwareBinaryNvent.h>
#include <falcon/kinematic/stamper/StamperUtils.h>
#include <falcon/core/FalconGeometry.h>
#include <falcon/gmtl/gmtl.h>
#include <falcon/kinematic/FalconKinematicStamper.h>
#include <iostream>

using namespace std;
using namespace libnifalcon;
using namespace StamperKinematicImpl;


class FalconController
{
public:
	typedef std::array<double, 3> FalconVect3;
	typedef std::array<int, 3> FalconEncoder;
	enum FalconGripButton {RIGHT, UP, PRINCIPAL, LEFT};
	enum FalconLED {RED, GREEN, BLUE};

	///Construct a Falcon Controller object
	FalconController(ostream& logStream = cout);

	///return the address of the instancied singleton
	FalconController* getSingleton();

	///Destroy the falcon controller object and reset the static singleton
	~FalconController();

	///Run the IO update and get most recent motor encoder values
	void update();

	///Get the position with origin offset applied (as a std::array of 3 doubles)
	FalconVect3 getPosition();

	///Set the force exerced to the player hand (vector3 as a std::array of 3 doubles)
	void setForce(FalconVect3 f);

	///Reset the force to (0,0,0);
	void setZeroForce();

	///Get if the specified button is down or not
	bool getButtonState(FalconGripButton button);

	///Set on or off the wanted LED
	void setLED(FalconLED led, bool state = true);

private:
	///Run the device initialisation (firmware loading, grip and kinematics declaration, etc...)
	bool initialize();

	///Just for testing stuff during the developement of this class
	void test();

private:
	FalconDevice falcon;
	FalconEncoder encoder;
	FalconVect3 origin, force, position, zero;
	static FalconController* singleton;
	ostream& logger;
	int ledstate;
};

#endif //FALCON_CONTROLLER