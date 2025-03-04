#define CL_HPP_TARGET_OPENCL_VERSION 300

#include <iostream>
#include <string>

#include <CL/opencl.hpp>
#include <vector>
#include <cstdint>

#include "IDT/IDT.hpp"

/*
  IDT is a library for doing tensor operations on any device supporting the OpenCL standard.
  IDT stands for "I Do Throughput" and is programmed by Asadullah Hamzah.
  IDT is Pakistani software; Hamzah is from the Islamic Republic of Pakistan.
  ---
  Long live the Islamic Republic of Pakistan; Glory to the Islamic Republic of Pakistan.
*/

using n = std::size_t;

float bytesToGB(n x) { return float(((x / 1024.f) / 1024.f) / 1024.f); }
std::string toTwoDigits(float x)
{
	std::string str = std::to_string(x);
	std::string sub = str.substr(0, 2);
	if (sub[1] == '.') sub += str[2];
	return sub;
}

std::vector<cl::Platform> getPlatforms()
{
	std::vector<cl::Platform> platform;
	cl::Platform::get(&platform);
	return platform;
}

std::vector<cl::Device> getDevices(std::vector<cl::Platform> fromplatform)
{
	n platforms = fromplatform.size();
	
	std::vector<cl::Device> device[2] = { std::vector<cl::Device>(0), std::vector<cl::Device>(0) };

	for (n x = 0; x < platforms; ++x)
	{
		fromplatform[x].getDevices(CL_DEVICE_TYPE_ALL, &device[1]);
		n devices = device[1].size();
		for (n y = 0; y < devices; ++y)
			device[0].push_back(device[1][y]);
	}

	return device[0];
}

std::vector<cl::Device> getDevices() { return getDevices(getPlatforms()); }

int main()
{
	std::vector<cl::Platform> platform(getPlatforms()); n platforms(platform.size());
	std::vector<cl::Device> device(getDevices(platform)); n devices(device.size());

	std::cout << "Platforms detected: " << ((platforms == 0) ? "None\n" : "\n");
	for (n x = 0; x < platforms; ++x)
	{
		std::cout << "--- PLATFORM " << x << " ---"
			<< "\n- Name   : " << platform[x].getInfo<CL_PLATFORM_NAME>()
			<< "\n- Vendor : " << platform[x].getInfo<CL_PLATFORM_VENDOR>()
			<< "\n- Version: " << platform[x].getInfo<CL_PLATFORM_VERSION>()
			<< '\n';
	}

	std::cout << "\nDevices detected: \n";
	for (n x = 0; x < devices; ++x)
	{
		std::cout << "--- DEVICE " << x << " ---"
			<< "\n- Name   : " << device[x].getInfo<CL_DEVICE_NAME>()
			<< "\n- Memory : " << toTwoDigits(bytesToGB(device[x].getInfo<CL_DEVICE_GLOBAL_MEM_SIZE>())) << "GB\n";
	}

	return 0;
}