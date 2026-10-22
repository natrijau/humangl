#include "human/human.hpp"

#include <algorithm>
#include <cmath>

#include "graphics/cube.hpp"
#include "graphics/shader.hpp"
#include "math/matrix_stack.hpp"

namespace
{
    float clampValue(float value, float minimum, float maximum)
    {
        return std::max(minimum, std::min(value, maximum));
    }
}

Human::Human()
    : _torsoSize(0.85f, 1.15f, 0.42f),
      _headSize(0.48f, 0.50f, 0.48f),
      _upperArmSize(0.22f, 0.58f, 0.22f),
      _foreArmSize(0.20f, 0.54f, 0.20f),
      _thighSize(0.28f, 0.68f, 0.28f),
      _shinSize(0.24f, 0.62f, 0.24f),
      _motion(Motion::Idle),
      _time(0.0f),
      _jumpTime(0.0f),
      _bodyY(0.0f),
      _rootY(0.0f)
{
    rebuild();
}

void Human::rebuild()
{
    const float shoulderX = _torsoSize.x * 0.5f + _upperArmSize.x * 0.5f;
    const float shoulderY = _torsoSize.y * 0.5f - _upperArmSize.x * 0.5f;
    const float hipX = _torsoSize.x * 0.25f;
    const float hipY = -_torsoSize.y * 0.5f;

    _parts.assign(10, Part());

    // Torso: root of the hierarchy, centered on its own joint.
    _parts[Torso].pivot = Vec3(0.0f, 0.0f, 0.0f);
    _parts[Torso].size = _torsoSize;
    _parts[Torso].offset = Vec3(0.0f, 0.0f, 0.0f);
    _parts[Torso].color = Vec3(0.30f, 0.48f, 0.78f);
    _parts[Torso].parent = -1;

    // Head sits on top of the torso.
    _parts[Head].pivot = Vec3(0.0f, _torsoSize.y * 0.5f + _headSize.y * 0.5f, 0.0f);
    _parts[Head].size = _headSize;
    _parts[Head].offset = Vec3(0.0f, 0.0f, 0.0f);
    _parts[Head].color = Vec3(0.95f, 0.79f, 0.62f);
    _parts[Head].parent = Torso;

    // Left arm: the shoulder is the joint, the cube hangs below it.
    _parts[LeftUpperArm].pivot = Vec3(shoulderX, shoulderY, 0.0f);
    _parts[LeftUpperArm].size = _upperArmSize;
    _parts[LeftUpperArm].offset = Vec3(0.0f, -_upperArmSize.y * 0.5f, 0.0f);
    _parts[LeftUpperArm].color = Vec3(0.88f, 0.42f, 0.36f);
    _parts[LeftUpperArm].parent = Torso;

    _parts[LeftForeArm].pivot = Vec3(0.0f, -_upperArmSize.y, 0.0f);
    _parts[LeftForeArm].size = _foreArmSize;
    _parts[LeftForeArm].offset = Vec3(0.0f, -_foreArmSize.y * 0.5f, 0.0f);
    _parts[LeftForeArm].color = Vec3(0.93f, 0.55f, 0.46f);
    _parts[LeftForeArm].parent = LeftUpperArm;

    // Right arm, mirrored on X.
    _parts[RightUpperArm].pivot = Vec3(-shoulderX, shoulderY, 0.0f);
    _parts[RightUpperArm].size = _upperArmSize;
    _parts[RightUpperArm].offset = Vec3(0.0f, -_upperArmSize.y * 0.5f, 0.0f);
    _parts[RightUpperArm].color = Vec3(0.88f, 0.42f, 0.36f);
    _parts[RightUpperArm].parent = Torso;

    _parts[RightForeArm].pivot = Vec3(0.0f, -_upperArmSize.y, 0.0f);
    _parts[RightForeArm].size = _foreArmSize;
    _parts[RightForeArm].offset = Vec3(0.0f, -_foreArmSize.y * 0.5f, 0.0f);
    _parts[RightForeArm].color = Vec3(0.93f, 0.55f, 0.46f);
    _parts[RightForeArm].parent = RightUpperArm;

    // Left leg: hip joint at the bottom of the torso.
    _parts[LeftThigh].pivot = Vec3(hipX, hipY, 0.0f);
    _parts[LeftThigh].size = _thighSize;
    _parts[LeftThigh].offset = Vec3(0.0f, -_thighSize.y * 0.5f, 0.0f);
    _parts[LeftThigh].color = Vec3(0.40f, 0.44f, 0.54f);
    _parts[LeftThigh].parent = Torso;

    _parts[LeftShin].pivot = Vec3(0.0f, -_thighSize.y, 0.0f);
    _parts[LeftShin].size = _shinSize;
    _parts[LeftShin].offset = Vec3(0.0f, -_shinSize.y * 0.5f, 0.0f);
    _parts[LeftShin].color = Vec3(0.55f, 0.60f, 0.72f);
    _parts[LeftShin].parent = LeftThigh;

    // Right leg, mirrored on X.
    _parts[RightThigh].pivot = Vec3(-hipX, hipY, 0.0f);
    _parts[RightThigh].size = _thighSize;
    _parts[RightThigh].offset = Vec3(0.0f, -_thighSize.y * 0.5f, 0.0f);
    _parts[RightThigh].color = Vec3(0.40f, 0.44f, 0.54f);
    _parts[RightThigh].parent = Torso;

    _parts[RightShin].pivot = Vec3(0.0f, -_thighSize.y, 0.0f);
    _parts[RightShin].size = _shinSize;
    _parts[RightShin].offset = Vec3(0.0f, -_shinSize.y * 0.5f, 0.0f);
    _parts[RightShin].color = Vec3(0.55f, 0.60f, 0.72f);
    _parts[RightShin].parent = RightThigh;

    // Put the feet exactly on y = 0, whatever the torso/leg sizes are.
    _rootY = _torsoSize.y * 0.5f + _thighSize.y + _shinSize.y;
    resetPose();
}

void Human::setTorsoHeight(float height)
{
    _torsoSize.y = clampValue(height, 0.60f, 1.80f);
    rebuild();
}

void Human::growTorso(float delta)
{
    setTorsoHeight(_torsoSize.y + delta);
}

void Human::growArms(float delta)
{
    _upperArmSize.y = clampValue(_upperArmSize.y + delta, 0.35f, 1.10f);
    _foreArmSize.y = clampValue(_foreArmSize.y + delta, 0.30f, 1.00f);
    rebuild();
}

void Human::growLegs(float delta)
{
    _thighSize.y = clampValue(_thighSize.y + delta, 0.40f, 1.20f);
    _shinSize.y = clampValue(_shinSize.y + delta, 0.35f, 1.10f);
    rebuild();
}

void Human::resetPose()
{
    for (std::size_t i = 0; i < _parts.size(); ++i)
    {
        _parts[i].angleX = 0.0f;
        _parts[i].angleY = 0.0f;
        _parts[i].angleZ = 0.0f;
    }
}

void Human::setMotion(Motion motion)
{
    if (motion == Motion::Jump)
        _jumpTime = 0.0f;
    _motion = motion;
    resetPose();
}

Motion Human::motion() const
{
    return _motion;
}

void Human::update(float deltaTime)
{
    _time += deltaTime;

    if (_motion == Motion::Idle)
    {
        resetPose();
        _bodyY = 0.0f;
    }
    else if (_motion == Motion::Walk)
    {
        const float swing = std::sin(_time * 4.0f);

        _parts[LeftUpperArm].angleX = 0.65f * swing;
        _parts[RightUpperArm].angleX = -0.65f * swing;
        _parts[LeftForeArm].angleX = -0.25f - 0.30f * std::max(0.0f, -swing);
        _parts[RightForeArm].angleX = -0.25f - 0.30f * std::max(0.0f, swing);

        _parts[LeftThigh].angleX = -0.75f * swing;
        _parts[RightThigh].angleX = 0.75f * swing;
        _parts[LeftShin].angleX = 0.55f * std::max(0.0f, -swing);
        _parts[RightShin].angleX = 0.55f * std::max(0.0f, swing);

        _bodyY = 0.05f * std::fabs(std::cos(_time * 4.0f));
    }
    else
    {
        _jumpTime += deltaTime;
        const float duration = 1.15f;
        const float progress = _jumpTime / duration;

        if (progress >= 1.0f)
        {
            _motion = Motion::Idle;
            resetPose();
            _bodyY = 0.0f;
            return;
        }

        const float tuck = std::sin(3.14159265f * progress);
        _bodyY = 0.85f * 4.0f * progress * (1.0f - progress);

        _parts[LeftUpperArm].angleX = -1.10f * tuck;
        _parts[RightUpperArm].angleX = -1.10f * tuck;
        _parts[LeftForeArm].angleX = -0.60f * tuck;
        _parts[RightForeArm].angleX = -0.60f * tuck;

        _parts[LeftThigh].angleX = -0.75f * tuck;
        _parts[RightThigh].angleX = -0.75f * tuck;
        _parts[LeftShin].angleX = 1.30f * tuck;
        _parts[RightShin].angleX = 1.30f * tuck;
    }
}

void Human::draw(const Shader &shader, const Cube &cube) const
{
    MatrixStack stack;
    stack.translate(Vec3(0.0f, _rootY + _bodyY, 0.0f));
    drawPart(Torso, stack, shader, cube);
}

void Human::drawPart(std::size_t index, MatrixStack &stack, const Shader &shader, const Cube &cube) const
{
    const Part &part = _parts[index];

    // Joint frame: children inherit this matrix, never the box scaling below.
    stack.push();
    stack.translate(part.pivot);
    stack.rotateX(part.angleX);
    stack.rotateY(part.angleY);
    stack.rotateZ(part.angleZ);

    // Draw this part's 1x1x1 cube at the origin of the current matrix.
    stack.push();
    stack.translate(part.offset);
    stack.scale(part.size);
    shader.setMat4("uModel", stack.top());
    shader.setVec3("uColor", part.color);
    cube.draw();
    stack.pop();

    // Then recurse into every child while the joint matrix is still active.
    for (std::size_t child = 0; child < _parts.size(); ++child)
    {
        if (_parts[child].parent == static_cast<int>(index))
            drawPart(child, stack, shader, cube);
    }

    stack.pop();
}
