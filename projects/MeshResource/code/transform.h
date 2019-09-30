#pragma once

#include "matrix4.h"

namespace efiilj
{
	class TransformModel
	{
	private:

		Vector3 position;
		Vector3 scale;
		Vector3 rotation;

		Matrix4 model;
		bool is_dirty;

		void SetDirty(bool dirty = true) { this->is_dirty = dirty; }

	public:

		TransformModel(Vector3 pos = Vector3(0, 0, 0), Vector3 rot = Vector3(0, 0, 0), Vector3 scale = Vector3(1, 1, 1));

		void Position(const Vector3& xyz, bool relative = false) { relative ? position += xyz : position = xyz; SetDirty(); }
		const Vector3& Position() const { return this->position; }

		void Rotation(const Vector3& xyz, bool relative = false) { relative ? rotation+= xyz : rotation = xyz; SetDirty(); }
		const Vector3& Rotation() const { return this->rotation; }

		void Scale(const Vector3& xyz, bool relative = false) { relative ? scale += xyz : scale = xyz; SetDirty(); }
		const Vector3& Scale() const { return this->scale; }

		const Matrix4& Model();

		~TransformModel() { }

	};
}