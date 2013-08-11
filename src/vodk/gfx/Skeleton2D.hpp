
#ifndef VODK_GFX_SKELETON2D_HPP
#define VODK_GFX_SKELETON2D_HPP

#include <assert.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "vodk/core/algebra2D.hpp"

namespace vodk {

typedef uint16_t JointIndex;
typedef uint16_t JointFlags;

const JointFlags JOINT_CARTESIAN = 1 << 0;

struct Joint2D {
	float length;
	JointIndex parent;
	JointFlags flags;
};

struct EuclidianJointState2D {
	float angle; // [0..2*pi]
	float length;
};

struct CartesianJointState2D {
	float x;
	float y;
};

struct JointState2D {
	union {
		EuclidianJointState2D euclidian;
		Vec2 cartesian;
	};
};

struct SkeletonModel2D {
	std::vector<Joint2D> joints;
	std::vector<std::string> joint_names;

	Joint2D& get(JointIndex index) {
		return joints[index];
	}

	const Joint2D& get(JointIndex index) const {
		return joints[index];
	}

	JointIndex get_index(const std::string& name) {
		JointIndex i = 0;
		for(auto &n : joint_names) {
			if (n == name) {
				return i;
			}
			++i;
		}
		return joints.size();
	}
};

struct JointMatrices {
	vodk::Mat3 base;
	vodk::Mat3 tip;
};

struct Skeleton2D {

	JointState2D& get(JointIndex index) {
		assert(index < _joints.size());
		return _joints[index];
	}
	JointState2D get(const std::string& name) {
		JointIndex i = _model->get_index(name);
		assert(i < _joints.size());
		return _joints[i];
	}

	void apply_transforms() {
		for (JointIndex i = 0; i<_joints.size(); ++i) {
			const Joint2D& model = _model->	get(i);
			const JointState2D& joint = _joints[i];
			const Mat3& previous_tip = _matrices[_model->get(i).parent].tip;
			Mat3& current_base = _matrices[i].base;
			// apply model.offset translation
			//current_base = previous_tip * glm::rotate(joint.euclidian.angle);
		}
	}

	SkeletonModel2D* _model;
	std::vector<JointState2D> _joints;
	std::vector<JointMatrices> _matrices;
};

} // vdk

#endif
