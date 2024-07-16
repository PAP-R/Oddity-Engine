#include "ValidationLayers.h"

#ifdef NDEBUG
bool enableValidationLayers = true;
#else
bool enableValidationLayers = false;
#endif

std::vector<const char*> validationLayers = {
	"VK_LAYER_KHRONOS_validation"
};