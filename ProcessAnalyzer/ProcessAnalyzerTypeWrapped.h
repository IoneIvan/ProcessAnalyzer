#pragma once
#include<Windows.h>
#include<memoryapi.h>
#include<TlHelp32.h>

#include<iostream>
#include<vector>

#include <cstdint> 
#include "ProcessAnalyzer.h"
class ProcessAnalyzerTypeWrapped : public ProcessAnalyzer {

public:

	using ProcessAnalyzer::readData;
	using ProcessAnalyzer::writeData;
	using ProcessAnalyzer::getProcess;


	HANDLE getProcess(std::string processName)
	{
		getProcess(processName.c_str());
	}
	template <typename T>
	void writeData(HANDLE process, LPVOID address, T data)
	{
		ProcessAnalyzer::writeData(process, address, convertToUint8Vector(data));
	}
	template <typename T>
	T readData(HANDLE process, LPVOID address, size_t byesSize)
	{
		return convertToType<T>(ProcessAnalyzer::readData(process, address, byesSize));
	}




	template <typename T>
	std::vector<uint8_t> convertToUint8Vector(const T& value) {
		static_assert(std::is_trivially_copyable<T>::value, "Type T must be trivially copyable");

		std::vector<uint8_t> byteVector(sizeof(T));
		std::memcpy(byteVector.data(), &value, sizeof(T));

		return byteVector;
	}

	// Template function to convert a vector of any trivially copyable type to std::vector<uint8_t>
	template <typename T>
	std::vector<uint8_t> convertToUint8Vector(const std::vector<T>& vec) {
		static_assert(std::is_trivially_copyable<T>::value, "Type T must be trivially copyable");

		std::vector<uint8_t> byteVector;
		byteVector.reserve(vec.size() * sizeof(T)); // Reserve space for the bytes

		for (const auto& num : vec) {
			uint8_t bytes[sizeof(T)];
			std::memcpy(bytes, &num, sizeof(T));
			byteVector.insert(byteVector.end(), bytes, bytes + sizeof(T)); // Append the bytes
		}

		return byteVector;
	}


	template <typename T>
	std::vector<T> convertToVector(const std::vector<uint8_t>& data) {
		size_t numElements = data.size() / sizeof(T);
		std::vector<T> result(numElements);

		std::memcpy(result.data(), data.data(), data.size());

		return result;
	}

	template <typename T>
	T convertToType(const std::vector<uint8_t>& data) {
		if (data.size() < sizeof(T)) {
			throw std::runtime_error("Not enough data to convert to type T");
		}

		T result;
		std::memcpy(&result, data.data(), sizeof(T));

		return result;
	}
};