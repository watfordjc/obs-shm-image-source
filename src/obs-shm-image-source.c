/* Copyright notice and (MIT) licence notice at end of file */

#include "obs-shm-image-source.h"

struct shm_image_source {
	obs_source_t* source;
};

struct obs_source_info shm_image_source_info = {
		.id = "shm_image_source",
		.type = OBS_SOURCE_TYPE_INPUT,
		.output_flags = OBS_SOURCE_VIDEO,
		.get_name = shm_image_source_get_name,
		.create = shm_image_source_create,
		.destroy = shm_image_source_destroy,
		.update = shm_image_source_update,
		.video_render = shm_image_source_render,
		.get_width = shm_image_source_get_width,
		.get_height = shm_image_source_get_height
};

static const char* shm_image_source_get_name(void* unused)
{
	UNUSED_PARAMETER(unused);
	return obs_module_text("SharedMemoryImageInput");
}

static void shm_image_source_load(struct shm_image_source* context)
{
	/* Have nothing to load yet */
}

static void shm_image_source_unload(struct shm_image_source* context)
{
	/* Have nothing to free yet */
}

static void shm_image_source_update(void* data, obs_data_t* settings)
{
	struct shm_image_source* context = data;
	/* Have nothing to update yet */
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
	struct shm_image_source* context = data;
	/* Have nothing to render yet */
}

static uint32_t shm_image_source_get_width(void* data)
{
	struct shm_image_source* context = data;
	return 0; /* Have nothing with a width yet */
}

static uint32_t shm_image_source_get_height(void* data)
{
	struct shm_image_source* context = data;
	return 0; /* Have nothing with a height yet */
}


/*
MIT License

Copyright (c) 2020 John Cook <https://JohnCook.UK>

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
of the Software, and to permit persons to whom the Software is furnished to do
so, subject to the following conditions:

The above copyright notice and this permission notice (including the next
paragraph) shall be included in all copies or substantial portions of the
Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/
