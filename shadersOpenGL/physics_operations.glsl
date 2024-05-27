#include <object.glsl>

#include <physics.comp>

void operation_selector:simple_move(uint operation, uint self) {
    objects[self].state &= ~HIT;

    float Air_Density = 1;

    vec3 v = objects[self].velocity.xyz;

    float c = QUARTER_PI * Air_Density * pow(objects[self].radius, 2);

    objects[self].acceleration.xyz -= c * v * abs(v);

    objects[self].velocity.xyz += objects[self].acceleration.xyz * time_delta * 0.5;
    objects[self].position.xyz += objects[self].velocity.xyz * time_delta;
    objects[self].velocity.xyz += objects[self].acceleration.xyz * time_delta * 0.5;

    objects[self].angle_velocity.xyz += objects[self].angle_acceleration.xyz * time_delta * 0.5;
    objects[self].angle.xyz += objects[self].angle_velocity.xyz * time_delta;
    objects[self].angle_velocity.xyz += objects[self].angle_acceleration.xyz * time_delta * 0.5;

    objects[self].acceleration.xyz = vec3(0);

    objects[self].transform = transform(self);
    objects[self].inverse_transform = inverse(objects[self].transform);
}