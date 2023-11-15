#ifndef TESTSPIEL_SHAPE_H
#define TESTSPIEL_SHAPE_H

#include <glm/glm.hpp>

#include <vector>
#include <string>

#include <Graphics/Buffer/Buffer.h>
#include <Graphics/Buffer/Bufferobject.h>

namespace OddityEngine {
    namespace Graphics {

        struct alignas(16) buffershape {
            GLuint shape;
            GLuint vertex_start;
            GLuint vertex_size;
            GLuint normal_start;
            GLuint normal_size;
            GLuint uv_start;
            GLuint uv_size;
            glm::vec3 bounding_box_center;
            glm::vec3 bounding_box_size;
        };

        class Shape {
        public:
            enum Shapes {
                SPHERE,
                CUBE,
                MESH,
                CURVE,
            };

        protected:
            static std::vector<Shape*> instances;
            static Buffer<buffershape>* active_shape_buffer;
            static Buffer<glm::vec3>* active_vertex_buffer;
            static Buffer<glm::vec3>* active_normal_buffer;
            static Buffer<glm::vec3>* active_uv_buffer;

        public:
            static Buffer<buffershape>* create_shape_buffer();
            static void set_shape_buffer(Buffer<buffershape>* buffer);
            static Buffer<buffershape>* get_shape_buffer();
            static Buffer<glm::vec3>* create_vertex_buffer();
            static void set_vertex_buffer(Buffer<glm::vec3>* buffer);
            static Buffer<glm::vec3>* get_vertex_buffer();
            static Buffer<glm::vec3>* create_normal_buffer();
            static void set_normal_buffer(Buffer<glm::vec3>* buffer);
            static Buffer<glm::vec3>* get_normal_buffer();
            static Buffer<glm::vec3>* create_uv_buffer();
            static void set_uv_buffer(Buffer<glm::vec3>* buffer);
            static Buffer<glm::vec3>* get_uv_buffer();

            static void bind_buffer();

        protected:
            std::vector<Bufferobject<glm::vec3>>* vertices;
            std::vector<Bufferobject<glm::vec3>>* normals;
            std::vector<Bufferobject<glm::vec3>>* uvs;

            glm::vec3 bounding_box_center = {0, 0, 0};
            glm::vec3 bounding_box_size = {0, 0, 0};

            Bufferobject<buffershape> shape;

        public:
            Shape(Shapes shape, const std::vector<glm::vec3>& vertices, const std::vector<glm::vec3>& normals, const std::vector<glm::vec3>& uvs);
//            Shape(Shapes shape, std::vector<Bufferobject<glm::vec3>>* vertices, std::vector<Bufferobject<glm::vec3>>* normals, std::vector<Bufferobject<glm::vec3>>* uvs);

            GLuint index();
        };

    } // OddityEngine
} // Graphics

#endif //TESTSPIEL_SHAPE_H
