#include "FalconController.hpp"

using namespace std;
using namespace libnifalcon;
using namespace StamperKinematicImpl;
FalconController* FalconController::singleton(nullptr);



FalconController::FalconController(ostream& logStream) :
	logger(logStream),
	ledstate(FalconFirmware::RED_LED | FalconFirmware::GREEN_LED | FalconFirmware::BLUE_LED)
{
	if(singleton)
	{
		logger << "You cannot create more than one FalconController!" << endl;
		exit(-1);
	}
	initialize();
	singleton = this;
	zero[0] = 0;
	zero[1] = 0;
	zero[2] = 0;
	falcon.getFalconFirmware()->setLEDStatus(ledstate);
	update();
}

///return the address of the instancied singleton
FalconController* FalconController::getSingleton()
{
	return singleton;
}

///Destroy the falcon controller object and reset the static singleton
FalconController::~FalconController()
{
	singleton = nullptr;
	if(falcon.isOpen()) falcon.close();
}

///Run the IO update and get most recent motor encoder values
void FalconController::update()
{
	falcon.runIOLoop();
	encoder = falcon.getFalconFirmware()->getEncoderValues();
	//test();
}

///Get the position with origin offset applied (as a std::array of 3 doubles)
FalconController::FalconVect3 FalconController::getPosition()
{
	falcon.getFalconKinematic()->getPosition(encoder, position);
	for(size_t i(0); i < 3; i++) position[i] -= origin[i];
	return position;
}

///Set the force exerced to the player hand (vector3 as a std::array of 3 doubles)
void FalconController::setForce(FalconVect3 f)
{
	force = f;
	falcon.setForce(force);
}

///Reset the force to (0,0,0);
void FalconController::setZeroForce()
{
	setForce(zero);
}

///Get if the specified button is down or not
bool FalconController::getButtonState(FalconGripButton button)
{
	return falcon.getFalconGrip()->getDigitalInput(static_cast<unsigned int>(button));
}

///Set on or off the wanted LED
void FalconController::setLED(FalconLED led, bool state)
{
	int theLed;
	switch(led)
	{
	case RED:
		theLed =  FalconFirmware::RED_LED;
		break;
	case GREEN:
		theLed =  FalconFirmware::GREEN_LED;
		break;
	case BLUE:
		theLed = FalconFirmware::BLUE_LED;
		break;
	}

	if(state)
		ledstate |= theLed;
	else
		ledstate &= ~theLed;

	falcon.getFalconFirmware()->setLEDStatus(ledstate);
}



///Run the device initialisation (firmware loading, grip and kinematics declaration, etc...)
bool FalconController::initialize()
{
	logger << "Init falcon controller" << endl;

	falcon.setFalconFirmware<FalconFirmwareNovintSDK>();

	logger << "Setting up comm interface for Falcon comms" << endl;

	unsigned int count;
	falcon.getDeviceCount(count);
	logger << "Connected Device Count: " << count << endl;

	//Open the device number:
	int deviceNum = 0;
	logger << "Attempting to open Falcon device:  " << deviceNum << endl;
	if(!falcon.open(deviceNum))
	{
		logger << "Cannot open falcon device index " << deviceNum << " - Lib Error Code: " << falcon.getErrorCode() << " Device Error Code: " << falcon.getFalconComm()->getDeviceErrorCode() << endl;
		return false;
	}
	else
	{
		logger << "Connected to Falcon device " << deviceNum << endl ;
	}

	bool skip_checksum = false;
	//See if we have firmware
	bool firmware_loaded = false;
	firmware_loaded = falcon.isFirmwareLoaded();
	if(!firmware_loaded)
	{
		std::cout << "Loading firmware" << std::endl;
		uint8_t* firmware_block;
		long firmware_size;
		{
			firmware_block = const_cast<uint8_t*>(NOVINT_FALCON_NVENT_FIRMWARE);
			firmware_size = NOVINT_FALCON_NVENT_FIRMWARE_SIZE;

			for(int i = 0; i < 10; ++i)
			{
				if(!falcon.getFalconFirmware()->loadFirmware(skip_checksum, NOVINT_FALCON_NVENT_FIRMWARE_SIZE, const_cast<uint8_t*>(NOVINT_FALCON_NVENT_FIRMWARE)))

				{
					logger << "Firmware loading try failed";
				}
				else
				{
					firmware_loaded = true;
					break;
				}
			}
		}
	}
	else if(!firmware_loaded)
	{
		std::cout << "No firmware loaded to device, and no firmware specified to load (--nvent_firmware, --test_firmware, etc...). Cannot continue" << std::endl;
		//return false;
	}
	else
	{
		//return true;
	}
	if(!firmware_loaded || !falcon.isFirmwareLoaded())
	{
		//std::cout << "No firmware loaded to device, cannot continue" << std::endl;
		//return false;
	}
	logger << "Firmware loaded" << std::endl;

	//Seems to be important to run the io loop once to be sure of sensible values next time:
	falcon.runIOLoop();

	falcon.getFalconFirmware()->setHomingMode(true);
	falcon.setFalconKinematic<FalconKinematicStamper>();
	falcon.setFalconGrip<FalconGripFourButton>();

	falcon.getFalconKinematic()->getWorkspaceOrigin(origin);
	logger << "Origin is : "
		<< fixed << origin[0] << ", "
		<< fixed << origin[1] << ", "
		<< fixed << origin[2] << ")"
		<< endl;

	return true;
}

///Just for testing stuff during the developement of this class
void FalconController::test()
{
	for(int i(0);  i < 4; i++)
		logger << "digital " << i << " = " << falcon.getFalconGrip()->getDigitalInput(i) << endl;
}
