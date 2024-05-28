#ifndef BUFFER_H
#define BUFFER_H

#include <vulkan/vulkan.h>

#include <Util/Debug.h>

namespace OddityEngine::Graphics::Vulkan {
    static uint32_t find_memory_type(VkPhysicalDevice physicalDevice, uint32_t typeFilter, VkMemoryPropertyFlags properties) {
        VkPhysicalDeviceMemoryProperties memProperties;
        vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memProperties);

        for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
            if (typeFilter & (1 << i) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
                return i;
            }
        }

        Debug::error("Failed to find memory type");
    }

    template<typename T>
    class Buffer {
    protected:
        VkPhysicalDevice _physicalDevice;
        VkDevice _device;

        VkBuffer _buffer;
        VkDeviceMemory _memory;

        T* _data;
        size_t _count;

        VkBufferUsageFlags _usage;
        VkMemoryPropertyFlags _properties;

        bool active = false;


    public:
        void create(VkPhysicalDevice physicalDevice, VkDevice device, size_t count, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, const T* data = nullptr) {
            if (!active) {
                _physicalDevice = physicalDevice, _device = device, _count = count, _usage = usage, _properties = properties;

                VkBufferCreateInfo bufferInfo{};
                bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
                bufferInfo.size = count * sizeof(T);
                bufferInfo.usage = usage;
                bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

                Debug::assert_error(vkCreateBuffer(device, &bufferInfo, nullptr, &_buffer) != VK_SUCCESS, "Failed to create buffer");

                VkMemoryRequirements memRequirements;
                vkGetBufferMemoryRequirements(device, _buffer, &memRequirements);

                VkMemoryAllocateInfo allocInfo{};
                allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
                allocInfo.allocationSize = memRequirements.size;
                allocInfo.memoryTypeIndex = find_memory_type(physicalDevice, memRequirements.memoryTypeBits, properties);

                Debug::assert_error(vkAllocateMemory(device, &allocInfo, nullptr, &_memory) != VK_SUCCESS, "Failed to allocate buffer memory");

                vkBindBufferMemory(device, _buffer, _memory, 0);

                vkMapMemory(device, _memory, 0, bufferInfo.size, 0, reinterpret_cast<void**>(&_data));
                if (data != nullptr) {
                    memcpy(_data, data, bufferInfo.size);
                }

                active = true;
            }
        }

        void destroy() {
            if (active) {
                active = false;

                vkUnmapMemory(_device, _memory);
                vkDestroyBuffer(_device, _buffer, nullptr);
                vkFreeMemory(_device, _memory, nullptr);
            }
        }

        Buffer() = default;

        Buffer(VkPhysicalDevice physicalDevice, VkDevice device, size_t count, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, const T* data = nullptr) {
            create(physicalDevice, device, count, usage, properties, data);
        }

        ~Buffer() {
            destroy();
        }

        void resize(size_t count) {
            auto old_size = size();
            void* data = malloc(old_size);
            memcpy(data, _data, old_size);
            destroy();
            create(_physicalDevice, _device, count, _usage, _properties);
            memcpy(_data, data, old_size);
        }

        T& operator [](size_t index) {
            return _data[index];
        }

        size_t count() const {
            return _count;
        }

        size_t size() const {
            return _count * sizeof(T);
        }

        operator VkBuffer () const {
            return _buffer;
        }

        operator VkDeviceMemory () const {
            return _memory;
        }
    };
}

#endif //BUFFER_H
