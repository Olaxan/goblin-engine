#include "def_rend.h"

#include "GL/glew.h"

#include <cassert>
#include <chrono>

namespace efiilj
{
	deferred_renderer::deferred_renderer
		(
			renderer_settings& settings,	
			std::shared_ptr<shader_program> geometry_, 
			std::shared_ptr<shader_program> lighting
		)
		: gbo_(0), rbo_(0), ubo_(0), pos_(0), norm_(0), cspec_(0), active_camera_(0), 
		settings_(settings), geometry_(std::move(geometry_)), lighting_(std::move(lighting))
	{
		// Setup gbuffer
		glGenFramebuffers(1, &gbo_);
		glBindFramebuffer(GL_FRAMEBUFFER, gbo_);

		gen_buffer(&pos_, GL_FLOAT);
		gen_buffer(&norm_, GL_FLOAT);
		gen_buffer(&cspec_, GL_UNSIGNED_BYTE);

		attachments_[0] = GL_COLOR_ATTACHMENT0;
		attachments_[1] = GL_COLOR_ATTACHMENT1;
		attachments_[2] = GL_COLOR_ATTACHMENT3;

		glDrawBuffers(3, attachments_);

		// Setup render buffer + depth
		glGenRenderbuffers(1, &rbo_);
		glBindRenderbuffer(GL_RENDERBUFFER, rbo_);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, settings_.width, settings_.height);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rbo_);

		// Check if framebuffer is ready
		assert(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		setup_camera();
		setup_ubo();

		last_frame_ = frame_timer::now(); 
	}

	void deferred_renderer::gen_buffer(unsigned* handle, unsigned type)
	{
		glGenTextures(1, handle);
		glBindTexture(GL_TEXTURE_2D, *handle);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, settings_.width, settings_.height, 0, GL_RGBA, type, nullptr);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, *handle, 0);
	}

	void deferred_renderer::setup_camera()
	{
		float aspect = float(settings_.width) / float(settings_.height);
		auto trans_ptr = std::make_shared<transform_model>(vector3(0, 0, 0), vector3(0), vector3(1, 1, 1));
		auto default_cam = std::make_shared<camera_model>(settings_.fov, aspect, settings_.near, settings_.far, trans_ptr, vector3(0, 1, 0));
		cameras_.push_back(default_cam);
	}

	void deferred_renderer::setup_ubo()
	{
		geometry_->bind_block("Matrices", 0);

		glGenBuffers(1, &ubo_);
		glBindBuffer(GL_UNIFORM_BUFFER, ubo_);
		glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(matrix4), nullptr, GL_DYNAMIC_DRAW);

		glBindBufferRange(GL_UNIFORM_BUFFER, 0, ubo_, 0, 2 * sizeof(matrix4));
		glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(matrix4), &cameras_[0]->get_perspective());
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
	}	

	void deferred_renderer::add_nodes(const std::vector<std::shared_ptr<graphics_node>>& nodes)
	{
		for (auto& node : nodes)
			this->nodes_.push_back(node);
	}

	void deferred_renderer::render() 
	{
		glBindFramebuffer(GL_FRAMEBUFFER, gbo_);
		glClearColor(0.0, 0.0, 0.0, 1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		geometry_->use();

		glBindBuffer(GL_UNIFORM_BUFFER, ubo_);
		glBufferSubData(GL_UNIFORM_BUFFER, sizeof(matrix4), sizeof(matrix4), &cameras_[active_camera_]->get_view());
		glBindBuffer(GL_UNIFORM_BUFFER, 0);

		duration dt = frame_timer::now() - last_frame_;
		last_frame_ = frame_timer::now();

		geometry_->set_uniform(settings_.uniform_camera.c_str(), cameras_[active_camera_]->get_transform()->position);
		geometry_->set_uniform(settings_.uniform_dt_seconds.c_str(), dt.count());

		for (auto& node : nodes_)
		{
			node->draw();
		}
	}
	
	void deferred_renderer::reload_shaders()
	{
		geometry_->reload();
		lighting_->reload();
	}

	bool deferred_renderer::set_camera(unsigned active)
	{
		if (active < cameras_.size())
		{
			active_camera_ = active;

			glBindBufferRange(GL_UNIFORM_BUFFER, 0, ubo_, 0, 2 * sizeof(matrix4));
			glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(matrix4), &cameras_[active_camera_]->get_perspective());
			glBindBuffer(GL_UNIFORM_BUFFER, 0);

			return true;
		}

		return false;
	}	
}
