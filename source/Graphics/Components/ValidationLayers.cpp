#include "ValidationLayers.h"

#ifdef NDEBUG
bool enableValidationLayers = false;
#else
bool enableValidationLayers = true;
#endif

std::vector<const char*> validationLayers = {
	"VK_LAYER_KHRONOS_validation"
};