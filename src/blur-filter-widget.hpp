#ifndef BLURFILTERWIDGET_H
#define BLURFILTERWIDGET_H

#include <obs-module.h>
#include <plugin-support.h>
#include <string>
#include <vector>
#include "src/filters/base-filter.hpp"
#include "src/filters/simple-gaussian.hpp"

#define SETTING_BLUR_SIZE "blur_size"
#define SETTING_BLUR_TYPE "blur_type"

class BlurFilterWidget {
private:
	struct filter_data {
		obs_source_t *context;
		gs_effect_t *effect;

		gs_eparam_t *blurSizeParam;
		gs_eparam_t *targetWidthParam;
		gs_eparam_t *targetHeightParam;

		float blurSize;
		char *selectedFileName;
		// std::string selected_file = "";
		int selectedFilterIndex = 0;
		std::vector<std::unique_ptr<BaseFilter>> filterArray; 
	};

	// BaseFilter** filterArray = new BaseFilter*[2];
	

	enum BlurType { box, simple_gaussian };
	obs_source_info source_info = {};

	static const char *BlurFilterWidgetGetName(void *unused);
	static void *BlurFilterWidgetCreate(obs_data_t *settings,
					    obs_source_t *source);
	static void BlurFilterWidgetDestroy(void *data);
	static void BlurFilterWidgetUpdate(void *data, obs_data_t *settings);
	static obs_properties_t *BlurFilterWidgetProperties(void *data);
	static void BlurFilterWidgetDefaults(obs_data_t *settings);
	static void BlurFilterWidgetRender(void *data, gs_effect_t *effect);
	static void ChangeFilterSelection(struct filter_data *filter);

public:
	BlurFilterWidget();
	~BlurFilterWidget();
	void RegisterSource();
};

#endif // BLURFILTERWIDGET_H
