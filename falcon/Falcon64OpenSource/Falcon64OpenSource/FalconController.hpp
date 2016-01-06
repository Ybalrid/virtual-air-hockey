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

	FalconController(ostream& logStream = cout) :
		logger(logStream)
	{
		if(singleton)
		{
			logger << "You cannot create more than one FalconController!" << endl;
		}
		initialize();
		singleton = this;
		zero[0] = 0;
		zero[1] = 0;
		zero[2] = 0;
		update();
	}

	~FalconController()
	{
		singleton = nullptr;
		if(falcon.isOpen()) falcon.close();
	}

	void update()
	{
		falcon.runIOLoop();
		encoder = falcon.getFalconFirmware()->getEncoderValues();
		//test();
	}

	FalconVect3 getPosition()
	{
		falcon.getFalconKinematic()->getPosition(encoder, position);
		return position;
	}

	void setForce(FalconVect3 f)
	{
		force = f;
		falcon.setForce(force);
	}

	void setZeroForce()
	{
		setForce(zero);
	}

	bool getButtonState(FalconGripButton button)
	{
		return falcon.getFalconGrip()->getDigitalInput(static_cast<unsigned int>(button));
	}

private:
	bool initialize()
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
	void test()
	{
		for(int i(0);  i < 4; i++)
			logger << "digital " << i << " = " << falcon.getFalconGrip()->getDigitalInput(i) << endl;
	}
private:
	FalconDevice falcon;
	FalconEncoder encoder;
	FalconVect3 origin, force, position, zero;
	static FalconController* singleton;
	ostream& logger;
};
FalconController* FalconController::singleton(nullptr);

#endif //FALCON_CONTROLLER