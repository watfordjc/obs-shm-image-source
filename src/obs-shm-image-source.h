/* Copyright notice and (MIT) licence notice at end of file */

#include <obs-module.h>

static const char* shm_image_source_get_name(void* unused);
static void shm_image_source_load(struct shm_image_source* context);
static void shm_image_source_unload(struct shm_image_source* context);
static void shm_image_source_update(void* data, obs_data_t* settings);
static void* shm_image_source_create(obs_data_t* settings, obs_source_t* source);
static void shm_image_source_destroy(void* data);
static void shm_image_source_render(void* data, gs_effect_t* effect);
static uint32_t shm_image_source_get_width(void* data);
static uint32_t shm_image_source_get_height(void* data);


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
