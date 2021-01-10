#ifdef __APPLE__
#include <CUESDK/CUESDK.h>
#else
#include <CUESDK.h>
#endif

#include <iostream>
#include <atomic>
#include <thread>
#include <vector>
#include <unordered_map>
#include <cmath>

using LedColorsVector = std::vector<CorsairLedColor>;
using AvailableKeys = std::unordered_map<int /*device index*/, LedColorsVector>;

const char* toString(CorsairError error)
{
	switch (error) {
	case CE_Success:
		return "CE_Success";
	case CE_ServerNotFound:
		return "CE_ServerNotFound";
	case CE_NoControl:
		return "CE_NoControl";
	case CE_ProtocolHandshakeMissing:
		return "CE_ProtocolHandshakeMissing";
	case CE_IncompatibleProtocol:
		return "CE_IncompatibleProtocol";
	case CE_InvalidArguments:
		return "CE_InvalidArguments";
	default:
		return "unknown error";
	}
}

AvailableKeys getAvailableKeys()
{
	AvailableKeys availableKeys;
	for (auto deviceIndex = 0; deviceIndex < CorsairGetDeviceCount(); deviceIndex++) {
		if (const auto ledPositions = CorsairGetLedPositionsByDeviceIndex(deviceIndex)) {
			LedColorsVector keys;
			for (auto i = 0; i < ledPositions->numberOfLed; i++) {
				const auto ledId = ledPositions->pLedPosition[i].ledId;
				keys.push_back(CorsairLedColor{ ledId, 0, 0, 0 });
			}
			availableKeys[deviceIndex] = keys;
		}
	}
	return availableKeys;
}

void performPulseEffect(int waveDuration, AvailableKeys &availableKeys)
{
	const auto timePerFrame = 25;
	for (auto x = .0; x < 2; x += static_cast<double>(timePerFrame) / waveDuration) {
		auto val = static_cast<int>((1 - pow(x - 1, 2)) * 255);
		for (auto &ledColorsByDeviceIndex : availableKeys) {
			auto &deviceIndex = ledColorsByDeviceIndex.first;
			auto &ledColorsVec = ledColorsByDeviceIndex.second;
			for (auto &ledColor : ledColorsVec) {
				ledColor.g = val;
			}
			CorsairSetLedsColorsBufferByDeviceIndex(0, static_cast<int>(ledColorsVec.size()), ledColorsVec.data());
		}
		CorsairSetLedsColorsFlushBufferAsync(nullptr, nullptr);
		std::this_thread::sleep_for(std::chrono::milliseconds(timePerFrame));
	}
}

void performPulseEffect2(int waveDuration, AvailableKeys& availableKeys)
{
	std::cout << "ayylmao" << std::endl;
	const auto timePerFrame = 25;
	for (auto x = .0; x < 2; x += static_cast<double>(timePerFrame) / waveDuration) {
		auto val = static_cast<int>((1 - pow(x - 1, 2)) * 255);
		for (auto& ledColorsByDeviceIndex : availableKeys) {
			auto& deviceIndex = ledColorsByDeviceIndex.first;
			auto& ledColorsVec = ledColorsByDeviceIndex.second;
			auto counter = 0;
			for (auto& ledColor : ledColorsVec) {
				ledColor.b = counter;

				counter += 10;
				if (counter > 255) {
					counter = 0;
				}

				CorsairSetLedsColorsBufferByDeviceIndex(deviceIndex, static_cast<int>(ledColorsVec.size()), ledColorsVec.data());
				std::cout << deviceIndex << std::endl;
				CorsairSetLedsColorsFlushBufferAsync(nullptr, nullptr);
				std::this_thread::sleep_for(std::chrono::milliseconds(10));

			}
			for (auto& ledColorsByDeviceIndex : availableKeys) {
				auto& deviceIndex = ledColorsByDeviceIndex.first;
				auto& ledColorsVec = ledColorsByDeviceIndex.second;
				for (auto& ledColor : ledColorsVec) {
					ledColor.b = 0;
				}
				CorsairSetLedsColorsBufferByDeviceIndex(deviceIndex, static_cast<int>(ledColorsVec.size()), ledColorsVec.data());
			}
			CorsairSetLedsColorsFlushBufferAsync(nullptr, nullptr);
			
			//CorsairSetLedsColorsBufferByDeviceIndex(deviceIndex, static_cast<int>(ledColorsVec.size()), ledColorsVec.data());	
		}
		//CorsairSetLedsColorsFlushBufferAsync(nullptr, nullptr);
		//std::this_thread::sleep_for(std::chrono::milliseconds(timePerFrame));		
	}
}

void performBaronEffect_fans_on() {
	// deviceId = 1 - LIGHT NODES (3 fans)
	const auto ledPositionsFans = CorsairGetLedPositionsByDeviceIndex(1);
	LedColorsVector keysFans;
	for (auto i = 0; i < ledPositionsFans->numberOfLed; i++) {
		const auto ledId = ledPositionsFans->pLedPosition[i].ledId;
		if (ledId < 204) {
			keysFans.push_back(CorsairLedColor{ ledId, 255, 255, 255 });
		}
		else if (ledId >= 204 && ledId < 216) {
			keysFans.push_back(CorsairLedColor{ ledId, 255, 0, 0 });
		}

		if (ledId >= 216 && ledId < 220) {
			keysFans.push_back(CorsairLedColor{ ledId, 255, 255, 255 });
		}
		else if (ledId >= 220 && ledId < 232) {
			keysFans.push_back(CorsairLedColor{ ledId, 0, 255, 0 });
		}

		if (ledId >= 232 && ledId < 236) {
			keysFans.push_back(CorsairLedColor{ ledId, 255, 255, 255 });
		}
		else if (ledId >= 236 && ledId < 248) {
			keysFans.push_back(CorsairLedColor{ ledId, 0, 0, 255 });
		}

		CorsairSetLedsColorsBufferByDeviceIndex(1, static_cast<int>(keysFans.size()), keysFans.data());
		CorsairSetLedsColorsFlushBufferAsync(nullptr, nullptr);
		std::this_thread::sleep_for(std::chrono::milliseconds(25));
	}
}

void performBaronEffect_fans_off() {
	// deviceId = 1 - LIGHT NODES (3 fans)
	const auto ledPositionsFans = CorsairGetLedPositionsByDeviceIndex(1);
	LedColorsVector keysFans;
	for (auto i = 0; i < ledPositionsFans->numberOfLed; i++) {
		const auto ledId = ledPositionsFans->pLedPosition[i].ledId;
		if (ledId < 204) {
			keysFans.push_back(CorsairLedColor{ ledId, 0, 0, 0 });
		}
		else if (ledId >= 204 && ledId < 216) {
			keysFans.push_back(CorsairLedColor{ ledId, 0, 0, 0 });
		}

		if (ledId >= 216 && ledId < 220) {
			keysFans.push_back(CorsairLedColor{ ledId, 0, 0, 0 });
		}
		else if (ledId >= 220 && ledId < 232) {
			keysFans.push_back(CorsairLedColor{ ledId, 0, 0, 0 });
		}

		if (ledId >= 232 && ledId < 236) {
			keysFans.push_back(CorsairLedColor{ ledId, 0, 0, 0 });
		}
		else if (ledId >= 236 && ledId < 248) {
			keysFans.push_back(CorsairLedColor{ ledId, 0, 0, 0 });
		}


		CorsairSetLedsColorsBufferByDeviceIndex(1, static_cast<int>(keysFans.size()), keysFans.data());
		CorsairSetLedsColorsFlushBufferAsync(nullptr, nullptr);
		std::this_thread::sleep_for(std::chrono::milliseconds(15));
	}
}

float apply_rgb_pulse(float frequency, float time, float extraTime) {
	float pi = 3.141592;
	float result =   0.5 * (1 + sin(2 * pi * frequency * (time + extraTime)));
	if (result < 0.3) {
		return result * 0.5;
	}
	return result;
}

float get_tracing_color(float val, int main, int secondary) {
	if (val <= 0.5) {
		return main;
	}
	else {
		return secondary;
	}
}


void generatePulse() {
	// deviceId = 1 - LIGHT NODES (3 fans)
	const auto ledPositionsFans = CorsairGetLedPositionsByDeviceIndex(0);
	std::vector<CorsairLedColor> keysFans;
	for (auto i = 0; i < ledPositionsFans->numberOfLed; i++) {
		const auto ledId = ledPositionsFans->pLedPosition[i].ledId;
		if (ledId < 204) {
			keysFans.push_back(CorsairLedColor{ ledId, 255, 0, 255 });
		}
		else if (ledId >= 204 && ledId < 216) {
			keysFans.push_back(CorsairLedColor{ ledId, 255, 0, 255 });
		}

		if (ledId >= 216 && ledId < 220) {
			keysFans.push_back(CorsairLedColor{ ledId, 255, 0, 255 });
		}
		else if (ledId >= 220 && ledId < 232) {
			keysFans.push_back(CorsairLedColor{ ledId, 255, 0, 255 });
		}

		if (ledId >= 232 && ledId < 236) {
			keysFans.push_back(CorsairLedColor{ ledId, 255, 0, 255 });
		}
		else if (ledId >= 236 && ledId < 248) {
			keysFans.push_back(CorsairLedColor{ ledId, 255, 0, 255 });
		}
	}
	
	CorsairSetLedsColorsBufferByDeviceIndex(0, static_cast<int>(keysFans.size()), keysFans.data());
	CorsairSetLedsColorsFlushBufferAsync(nullptr, nullptr);
	std::this_thread::sleep_for(std::chrono::milliseconds(200));

	bool forward = true;
	
	for (float time = 0; time < 6; time += 0.013) {
		float extraTime = 0;
		for (auto& ledColor : keysFans) {

			if (ledColor.ledId < 204) {
				ledColor.r = apply_rgb_pulse(1, time, -(204 - ledColor.ledId) / 4.0) * 0;
				ledColor.g = apply_rgb_pulse(1, time, -(204 - ledColor.ledId) / 4.0) * 255;
				ledColor.b = apply_rgb_pulse(1, time, -(204 - ledColor.ledId) / 4.0) * 0;
			}
			else if (ledColor.ledId >= 204 && ledColor.ledId < 216) {
				
				ledColor.r = apply_rgb_pulse(1, time, (216 - ledColor.ledId) / 12.0 ) * 255;
				ledColor.g = apply_rgb_pulse(1, time, (216 - ledColor.ledId) / 12.0 ) * 0;
				ledColor.b = apply_rgb_pulse(1, time, (216 - ledColor.ledId) / 12.0 ) * 255;
			}

			if (ledColor.ledId >= 216 && ledColor.ledId < 220) {
				ledColor.r = apply_rgb_pulse(1, time, -(220 - ledColor.ledId) / 4.0) * 0;
				ledColor.g = apply_rgb_pulse(1, time, -(220 - ledColor.ledId) / 4.0) * 255;
				ledColor.b = apply_rgb_pulse(1, time, -(220 - ledColor.ledId) / 4.0) * 0;
			}
			else if (ledColor.ledId >= 220 && ledColor.ledId < 232) {
				ledColor.r = apply_rgb_pulse(1, time, (232 - ledColor.ledId) / 12.0) * 255;
				ledColor.g = apply_rgb_pulse(1, time, (232 - ledColor.ledId) / 12.0) * 0;
				ledColor.b = apply_rgb_pulse(1, time, (232 - ledColor.ledId) / 12.0) * 255;
			}

			if (ledColor.ledId >= 232 && ledColor.ledId < 236) {
				ledColor.r = apply_rgb_pulse(1, time, -(236 - ledColor.ledId) / 4.0) * 0;
				ledColor.g = apply_rgb_pulse(1, time, -(236 - ledColor.ledId) / 4.0) * 255;
				ledColor.b = apply_rgb_pulse(1, time, -(236 - ledColor.ledId) / 4.0) * 0;
			}
			else if (ledColor.ledId >= 236 && ledColor.ledId < 248) {
				ledColor.r = apply_rgb_pulse(1, time, (248 - ledColor.ledId) / 12.0) * 255;
				ledColor.g = apply_rgb_pulse(1, time, (248 - ledColor.ledId) / 12.0) * 0;
				ledColor.b = apply_rgb_pulse(1, time, (248 - ledColor.ledId) / 12.0) * 255;
			}


			CorsairSetLedsColorsBufferByDeviceIndex(0, static_cast<int>(keysFans.size()), keysFans.data());
			CorsairSetLedsColorsFlushBufferAsync(nullptr, nullptr);
		}
		
		
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}

}

void generatePulseFinal(AvailableKeys& availableKeys, int r_main, int g_main, int b_main, int r_sec, int g_sec, int b_sec) {
	for (float time = 0; time < 6; time += 0.013) {
		for (auto& ledColorsByDeviceIndex : availableKeys) {
			auto& deviceIndex = ledColorsByDeviceIndex.first;
			auto& ledColorsVec = ledColorsByDeviceIndex.second;
			for (auto& ledColor : ledColorsVec) {
				// TRIPLE FAN
				if (ledColor.ledId >= 200 && ledColor.ledId < 204) {
					ledColor.r = apply_rgb_pulse(1, time, -(204 - ledColor.ledId) / 4.0) * r_main;
					ledColor.g = apply_rgb_pulse(1, time, -(204 - ledColor.ledId) / 4.0) * g_main;
					ledColor.b = apply_rgb_pulse(1, time, -(204 - ledColor.ledId) / 4.0) * b_main;
				}
				else if (ledColor.ledId >= 204 && ledColor.ledId < 216) {

					ledColor.r = apply_rgb_pulse(1, time, (216 - ledColor.ledId) / 12.0) * r_sec;
					ledColor.g = apply_rgb_pulse(1, time, (216 - ledColor.ledId) / 12.0) * g_sec;
					ledColor.b = apply_rgb_pulse(1, time, (216 - ledColor.ledId) / 12.0) * b_sec;
				}

				if (ledColor.ledId >= 216 && ledColor.ledId < 220) {
					ledColor.r = apply_rgb_pulse(1, time, -(220 - ledColor.ledId) / 4.0) * r_main;
					ledColor.g = apply_rgb_pulse(1, time, -(220 - ledColor.ledId) / 4.0) * g_main;
					ledColor.b = apply_rgb_pulse(1, time, -(220 - ledColor.ledId) / 4.0) * b_main;
				}
				else if (ledColor.ledId >= 220 && ledColor.ledId < 232) {
					ledColor.r = apply_rgb_pulse(1, time, (232 - ledColor.ledId) / 12.0) * r_sec;
					ledColor.g = apply_rgb_pulse(1, time, (232 - ledColor.ledId) / 12.0) * g_sec;
					ledColor.b = apply_rgb_pulse(1, time, (232 - ledColor.ledId) / 12.0) * b_sec;
				}

				if (ledColor.ledId >= 232 && ledColor.ledId < 236) {
					ledColor.r = apply_rgb_pulse(1, time, -(236 - ledColor.ledId) / 4.0) * r_main;
					ledColor.g = apply_rgb_pulse(1, time, -(236 - ledColor.ledId) / 4.0) * g_main;
					ledColor.b = apply_rgb_pulse(1, time, -(236 - ledColor.ledId) / 4.0) * b_main;
				}
				else if (ledColor.ledId >= 236 && ledColor.ledId < 249) {
					ledColor.r = apply_rgb_pulse(1, time, (248 - ledColor.ledId) / 12.0) * r_sec;
					ledColor.g = apply_rgb_pulse(1, time, (248 - ledColor.ledId) / 12.0) * g_sec;
					ledColor.b = apply_rgb_pulse(1, time, (248 - ledColor.ledId) / 12.0) * b_sec;
				}



				if (ledColor.ledId >= 762 && ledColor.ledId < 778) {
					float redFactor = apply_rgb_pulse(1, time, -(762 - ledColor.ledId) / 16.0);
					ledColor.r = redFactor * float(get_tracing_color(redFactor, r_main, r_sec));

					float greenFactor = apply_rgb_pulse(1, time, -(762 - ledColor.ledId) / 16.0);
					ledColor.g = greenFactor * float(get_tracing_color(greenFactor, g_main, g_sec));

					float blueFactor = apply_rgb_pulse(1, time, -(762 - ledColor.ledId) / 16.0);
					ledColor.b = blueFactor * float(get_tracing_color(blueFactor, b_main, b_sec));
				}
				else if (ledColor.ledId >= 778 && ledColor.ledId < 786) {

					ledColor.r = apply_rgb_pulse(1, time, (778 - ledColor.ledId) / 12.0) * r_main;
					ledColor.g = apply_rgb_pulse(1, time, (778 - ledColor.ledId) / 12.0) * g_main;
					ledColor.b = apply_rgb_pulse(1, time, (778 - ledColor.ledId) / 12.0) * b_main;
				}

				if (ledColor.ledId >= 600 && ledColor.ledId < 612) {
					float redFactor = apply_rgb_pulse(1, time, -(600 - ledColor.ledId) / 12.0);
					ledColor.r = redFactor * float(get_tracing_color(redFactor, r_main, r_sec));

					float greenFactor = apply_rgb_pulse(1, time, -(600 - ledColor.ledId) / 12.0);
					ledColor.g = greenFactor * float(get_tracing_color(greenFactor, g_main, g_sec));

					float blueFactor = apply_rgb_pulse(1, time, -(600 - ledColor.ledId) / 12.0);
					ledColor.b = blueFactor * float(get_tracing_color(blueFactor, b_main, b_sec));
				}
			}
			CorsairSetLedsColorsBufferByDeviceIndex(deviceIndex, static_cast<int>(ledColorsVec.size()), ledColorsVec.data());
		}		
		CorsairSetLedsColorsFlushBufferAsync(nullptr, nullptr);
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}

}

void performBaronEffect() {
	// deviceId = 0 - PUMP AND FANS
	const auto ledPositionsPump = CorsairGetLedPositionsByDeviceIndex(0);


	// deviceId = 1 - LIGHT NODES (3 fans)
	const auto ledPositionsFans = CorsairGetLedPositionsByDeviceIndex(1);
	LedColorsVector keysFans;
	for (auto i = 0; i < ledPositionsFans->numberOfLed; i++) {
		const auto ledId = ledPositionsFans->pLedPosition[i].ledId;
		if (ledId < 204) {
			keysFans.push_back(CorsairLedColor{ ledId, 255, 255, 255 });
		}
		else if (ledId >= 204 && ledId < 216) {
			keysFans.push_back(CorsairLedColor{ ledId, 255, 0, 0 });
		}

		if (ledId >= 216 && ledId < 220) {
			keysFans.push_back(CorsairLedColor{ ledId, 255, 255, 255 });
		}
		else if (ledId >= 220 && ledId < 232) {
			keysFans.push_back(CorsairLedColor{ ledId, 0, 255, 0 });
		}

		if (ledId >= 232 && ledId < 236) {
			keysFans.push_back(CorsairLedColor{ ledId, 255, 255, 255 });
		}
		else if (ledId >= 236 && ledId < 248) {
			keysFans.push_back(CorsairLedColor{ ledId, 0, 0, 255 });
		}


		CorsairSetLedsColorsBufferByDeviceIndex(1, static_cast<int>(keysFans.size()), keysFans.data());
		CorsairSetLedsColorsFlushBufferAsync(nullptr, nullptr);
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}



	// deviceId = 2 - DIMM LEFT
	const auto ledPositionsDimmLeft = CorsairGetLedPositionsByDeviceIndex(2);
	// deviceId = 3 - DIMM RIGHT
	const auto ledPositionsDiffRight = CorsairGetLedPositionsByDeviceIndex(3);

	auto device = 1;

	if (const auto ledPositions = CorsairGetLedPositionsByDeviceIndex(device)) {
		LedColorsVector keys;
		for (auto i = 0; i < ledPositions->numberOfLed; i++) {
			const auto ledId = ledPositions->pLedPosition[i].ledId;
			
			if (ledId < 216) {
				keys.push_back(CorsairLedColor{ ledId, 255, 0, 0 });
			}
			else if(ledId >= 216 && ledId < 232) {
				keys.push_back(CorsairLedColor{ ledId, 0, 255, 0 });
			}
			else {
				keys.push_back(CorsairLedColor{ ledId, 0, 0, 255 });
			}
			
			CorsairSetLedsColorsBufferByDeviceIndex(device, static_cast<int>(keys.size()), keys.data());
			CorsairSetLedsColorsFlushBufferAsync(nullptr, nullptr);
			std::this_thread::sleep_for(std::chrono::milliseconds(1000));
		}
		auto counter = 0;
		for (auto& ledColor : keys) {
			counter += 10;
			if (counter > 255) {
				counter = 0;
			}
			ledColor.b = counter;
			CorsairSetLedsColorsBufferByDeviceIndex(device, static_cast<int>(keys.size()), keys.data());
			CorsairSetLedsColorsFlushBufferAsync(nullptr, nullptr);
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
		}
			
			

	}
}

int main()
{
	CorsairPerformProtocolHandshake();
	if (const auto error = CorsairGetLastError()) {
		std::cout << "Handshake failed: " << toString(error) << "\nPress any key tro quit." << std::endl;
		getchar();
		return -1;
	}
	auto availableKeys = getAvailableKeys();
		if (availableKeys.empty()) {
			return 1;
	}
	//generatePulseFinal(availableKeys, 255, 0, 255, 0, 255, 0); // baron
	//generatePulseFinal(availableKeys, 255, 0, 255, 0, 0, 139); // herald
	//generatePulseFinal(availableKeys, 135, 206, 235, 0, 191, 255); // cloud
	//generatePulseFinal(availableKeys, 65, 105, 255, 75, 0, 130); // elder
	//generatePulseFinal(availableKeys, 255, 69, 0, 255, 0, 0); // infernal
	//generatePulseFinal(availableKeys, 160, 82, 45, 210, 105, 30); // mountain
	//generatePulseFinal(availableKeys, 32, 178, 170, 0, 255, 82); // ocean
	generatePulseFinal(availableKeys, 50, 0, 0, 100, 0, 0); // tower/inhib
	

	//std::atomic_int waveDuration{ 500 };
	//std::atomic_bool continueExecution{ true };

	//auto availableKeys = getAvailableKeys();
	//if (availableKeys.empty()) {
	//	return 1;
	//}

	//performPulseEffect(waveDuration, availableKeys);

	//std::cout << "Working... Use \"+\" or \"-\" to increase or decrease speed.\nPress \"q\" to close program..." << std::endl;

	//std::thread lightingThread([&waveDuration, &continueExecution, &availableKeys] {
	//	while (continueExecution) {
	//		performPulseEffect2(waveDuration, availableKeys);
	//	}
	//});

	//while (continueExecution) {
	//	char c = getchar();
	//	switch (c) {
	//	case '+':
	//		if (waveDuration > 100)
	//			waveDuration -= 100;
	//		break;
	//	case '-':
	//		if (waveDuration < 2000)
	//			waveDuration += 100;
	//		break;
	//	case 'Q':
	//	case 'q':
	//		continueExecution = false;
	//		break;
	//	default:
	//		break;
	//	}
	//}
	//lightingThread.join();
	return 0;
}
