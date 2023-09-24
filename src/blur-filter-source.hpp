#ifndef BLURFILTERSOURCE_H
#define BLURFILTERSOURCE_H

#include <obs-module.h>
#include <plugin-support.h>
#include <string>
#include <vector>
#include <memory>
#include "src/filters/base-filter.hpp"
#include "src/filters/simple-gaussian.hpp"
#include "src/filters/box-blur-filter.hpp"
#include "src/filters/fast-gaussian.hpp"

#define SETTING_BLUR_SIZE "blur_size"
#define SETTING_BLUR_TYPE "blur_type"

class BlurFilterSource {
private:
	struct filter_data {
		obs_source_t *context;
		gs_effect_t *effect;

		char *selectedFileName;
		long long selectedFilterIndex;
		std::vector<std::unique_ptr<BaseFilter>> filterArray;

		obs_properties_t *mainProperties;
		obs_properties_t *filterProperties;
		obs_property_t *filterPropertiesGroup;
	};

	obs_source_info source_info = {};

	static const char *GetName(void *unused);
	static void *CreateSource(obs_data_t *settings, obs_source_t *source);
	static void DestroySource(void *data);
	static void UpdateSource(void *data, obs_data_t *settings);
	static obs_properties_t *GetProperties(void *data);
	static void SetDefaultProperties(filter_data *filterData,
					 obs_data_t *settings);
	static void RenderSource(void *data, gs_effect_t *effect);
	static void ChangeFilterSelection(struct filter_data *filter,
					  obs_data_t *settings);
	static bool FilterSelectionChangeCallback(void *data,
						  obs_properties_t *props,
						  obs_property_t *list,
						  obs_data_t *settings);
	static void TogglePropertyGroupVisibility(filter_data *data);

public:
	BlurFilterSource();
	~BlurFilterSource();
	void RegisterSource();
};

#endif // BLURFILTERSOURCE_H
