#ifndef BLURFILTERWIDGET_H
#define BLURFILTERWIDGET_H

#include <obs-module.h>
#include <util/platform.h>
#include <graphics/image-file.h>
#include <graphics/graphics.h>
#include <graphics/matrix4.h>
#include <graphics/vec2.h>
#include <graphics/vec4.h>
#include <string>

#define SETTING_BLUR_SIZE "blur_size"
#define SETTING_BLUR_TYPE "blur_type"

class BlurFilterWidget {
private:
	struct gaussian_blur_data {
		obs_source_t *context;
		gs_effect_t *effect;

		gs_eparam_t *blurSizeParam;
		gs_eparam_t *targetWidthParam;
		gs_eparam_t *targetHeightParam;

		float blurSize;
		char *selectedFileName;
		std::string selected_file = "";
	};

	enum BlurType { box, gaussian };
	obs_source_info source_info = {};

	static const char *BlurFilterWidgetGetName(void *unused);
	static void *BlurFilterWidgetCreate(obs_data_t *settings,
					    obs_source_t *source);
	static void BlurFilterWidgetDestroy(void *data);
	static void BlurFilterWidgetUpdate(void *data, obs_data_t *settings);
	static obs_properties_t *BlurFilterWidgetProperties(void *data);
	static void BlurFilterWidgetDefaults(obs_data_t *settings);
	static void BlurFilterWidgetRender(void *data, gs_effect_t *effect);
	static void ChangeFilterSelection(struct gaussian_blur_data *filter);

public:
	BlurFilterWidget();
	~BlurFilterWidget();
	void RegisterSource();
};

#endif // BLURFILTERWIDGET_H
