/*
 * Copyright © 2006 Keith Packard
 * Copyright © 2007 Intel Corporation
 *   Jesse Barnes <jesse.barnes@intel.com>
 */

/*
 * The DRM mode setting helper functions are common code for drivers to use if they wish.
 * Drivers are not forced to use this code in their implementations but it would be useful
 * if they code they do use at least provides a consistent interface and operation to userspace
 */

#ifndef __DRM_CRTC_HELPER_H__
#define __DRM_CRTC_HELPER_H__

#include <linux/i2c.h>
#include <linux/spinlock.h>
#include <linux/types.h>
#include <linux/idr.h>

#include <linux/fb.h>

struct drm_crtc_helper_funcs {
	/*
	 * Control power levels on the CRTC.  If the mode passed in is
	 * unsupported, the provider must use the next lowest power level.
	 */
	void (*dpms)(struct drm_crtc *crtc, int mode);
  	void (*prepare)(struct drm_crtc *crtc);
	void (*commit)(struct drm_crtc *crtc);

	/* Provider can fixup or change mode timings before modeset occurs */
	bool (*mode_fixup)(struct drm_crtc *crtc,
			   struct drm_display_mode *mode,
			   struct drm_display_mode *adjusted_mode);
	/* Actually set the mode */
	void (*mode_set)(struct drm_crtc *crtc, struct drm_display_mode *mode,
			 struct drm_display_mode *adjusted_mode, int x, int y);

	/* Move the crtc on the current fb to the given position *optional* */
	void (*mode_set_base)(struct drm_crtc *crtc, int x, int y);
};

struct drm_encoder_helper_funcs {
	void (*dpms)(struct drm_encoder *encoder, int mode);
	void (*save)(struct drm_encoder *encoder);
	void (*restore)(struct drm_encoder *encoder);

	bool (*mode_fixup)(struct drm_encoder *encoder,
			   struct drm_display_mode *mode,
			   struct drm_display_mode *adjusted_mode);
	void (*prepare)(struct drm_encoder *encoder);
	void (*commit)(struct drm_encoder *encoder);
	void (*mode_set)(struct drm_encoder *encoder,
			 struct drm_display_mode *mode,
			 struct drm_display_mode *adjusted_mode);
};

struct drm_connector_helper_funcs {
	int (*get_modes)(struct drm_connector *connector);
  	int (*mode_valid)(struct drm_connector *connector,
			  struct drm_display_mode *mode);
	struct drm_encoder *(*best_encoder)(struct drm_connector *connector);
};
	
extern void drm_helper_probe_single_connector_modes(struct drm_connector *connector, uint32_t maxX, uint32_t maxY);
extern void drm_helper_disable_unused_functions(struct drm_device *dev);
extern int drm_helper_hotplug_stage_two(struct drm_device *dev);
extern bool drm_helper_initial_config(struct drm_device *dev, bool can_grow);
extern int drm_crtc_helper_set_config(struct drm_mode_set *set);
extern bool drm_crtc_helper_set_mode(struct drm_crtc *crtc, struct drm_display_mode *mode,
				     int x, int y);
extern bool drm_helper_crtc_in_use(struct drm_crtc *crtc);

extern int drm_helper_mode_fill_fb_struct(struct drm_framebuffer *fb,
					  struct drm_mode_fb_cmd *mode_cmd);

static inline void drm_crtc_helper_add(struct drm_crtc *crtc, const struct drm_crtc_helper_funcs *funcs)
{
	crtc->helper_private = (void *)funcs;
}

static inline void drm_encoder_helper_add(struct drm_encoder *encoder, const struct drm_encoder_helper_funcs *funcs)
{
	encoder->helper_private = (void *)funcs;
}

static inline void drm_connector_helper_add(struct drm_connector *connector, const struct drm_connector_helper_funcs *funcs)
{
	connector->helper_private = (void *)funcs;
}

extern int drm_get_buffer_object(struct drm_device *dev, struct drm_buffer_object **bo, unsigned long handle);

#endif