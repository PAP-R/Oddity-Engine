#pragma once

#include "checks.h"

#include <stdexcept>

void check(bool status, std::string source) {
	if (status) {
		throw std::runtime_error(source);
	}
}