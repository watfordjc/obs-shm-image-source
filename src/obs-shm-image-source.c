/* Copyright notice and (GPL-2.0-or-later) licence notice at end of file */

#include "obs-shm-image-source.h"
#include <stdlib.h>

#define blog(log_level, format, ...)						\
		blog(log_level, "[shm_image_source: '%s'] " format, \
			obs_source_get_name(context->source), ##__VA_ARGS__)

#define debug(format, ...) blog(LOG_DEBUG, format, ##__VA_ARGS__)
#define info(format, ...) blog(LOG_INFO, format, ##__VA_ARGS__)
#define warn(format, ...) blog(LOG_WARNING, format, ##__VA_ARGS__)

struct shm_image_source {
	obs_source_t* source;
	gs_texture_t* texture;
	uint32_t shared_handle;
	uint32_t shared_texture_mutex_key;
	uint32_t width;
	uint32_t height;
};

struct obs_source_info shm_image_source_info = {
		.id = "shm_image_source",
		.type = OBS_SOURCE_TYPE_INPUT,
		.output_flags = OBS_SOURCE_VIDEO,
		.get_properties = shm_image_source_get_properites,
		.get_name = shm_image_source_get_name,
		.create = shm_image_source_create,
		.destroy = shm_image_source_destroy,
		.update = shm_image_source_update,
		.video_render = shm_image_source_render,
		.get_width = shm_image_source_get_width,
		.get_height = shm_image_source_get_height,
		.icon_type = OBS_ICON_TYPE_IMAGE
};

static obs_properties_t* shm_image_source_get_properites(void* data)
{
	UNUSED_PARAMETER(data);
	obs_properties_t* ppts = obs_properties_create();
	obs_properties_add_text(ppts, "shared_handle", obs_module_text("SharedTextureHandle"), OBS_TEXT_DEFAULT);
	obs_properties_add_int(ppts, "shared_texture_mutex_key", obs_module_text("SharedTextureMutexKey"), 0, 65555, 0);
	return ppts;
}

static const char* shm_image_source_get_name(void* unused)
{
	UNUSED_PARAMETER(unused);
	return obs_module_text("SharedMemoryImageInput");
}

static void shm_image_source_load(struct shm_image_source* context)
{
	if (context->shared_handle == 0x0)
	{
		return;
	}
	obs_enter_graphics();
	context->texture = gs_texture_open_shared(context->shared_handle);
	context->width = gs_texture_get_width(context->texture);
	context->height = gs_texture_get_height(context->texture);
	obs_leave_graphics();

	debug("Shared memory image source loaded from 0x%x with dimensions %dx%d.\n", context->shared_handle, context->width, context->height);
}

static void shm_image_source_unload(struct shm_image_source* context)
{
	context->shared_handle = 0x0;
	context->texture = NULL;
	context->width = 0;
	context->height = 0;
}

static void shm_image_source_update(void* data, obs_data_t* settings)
{
	struct shm_image_source* context = data;
	long long tmpHandle = strtoll(obs_data_get_string(settings, "shared_handle"), NULL, 0);
	debug("Shared handle is 0x%x", tmpHandle);
	if (tmpHandle > 0)
	{
		context->shared_handle = tmpHandle & 0xFFFFFFFF;
	}
	context->shared_texture_mutex_key = obs_data_get_int(settings, "shared_texture_mutex_key") & 0xFFFFFFFF;
	shm_image_source_load(context);
}

static void* shm_image_source_create(obs_data_t* settings, obs_source_t* source)
{
	struct shm_image_source* context = bzalloc(sizeof(struct shm_image_source));
	context->source = source;

	shm_image_source_update(context, settings);
	return context;
}

static void shm_image_source_destroy(void* data)
{
	struct shm_image_source* context = data;
	shm_image_source_unload(context);
	bfree(context);
}

static void shm_image_source_render(void* data, gs_effect_t* effect)
{
	UNUSED_PARAMETER(effect);
	struct shm_image_source* context = data;
	if (context->texture == NULL)
	{
		return;
	}

	int retval = gs_texture_acquire_sync(context->texture, context->shared_texture_mutex_key, 16);
	if (retval != 0)
	{
		return;
	}
	gs_effect_set_texture(gs_effect_get_param_by_name(effect, "image"), context->texture);
	gs_draw_sprite(context->texture, 0, context->width, context->height);
	gs_texture_release_sync(context->texture, context->shared_texture_mutex_key);
}

static uint32_t shm_image_source_get_width(void* data)
{
	struct shm_image_source* context = data;
	return context->texture != NULL ? context->width : 0;
}

static uint32_t shm_image_source_get_height(void* data)
{
	struct shm_image_source* context = data;
	return context->texture != NULL ? context->height : 0;
}


/*
obs-shm-image-source
Copyright (c) 2020 John Cook <https://JohnCook.UK>

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License along
with this program. If not, see <https://www.gnu.org/licenses/>
*/
