#ifndef TESTFILTER_H
#define TESTFILTER_H

#include "base-filter.hpp"
#include <plugin-support.h>
#include <obs-module.h>
#include <string>

#define SETTING_TEST_FILTER_SIZE "test_filter_blur_size"
#define SETTING_TEST_FILTER_TYPE "test_filter_blur_type"

class TestFilter : public BaseFilter {
private:
	gs_effect_t *effect;

	gs_eparam_t *blurSizeParam;
	gs_eparam_t *targetWidthParam;
	gs_eparam_t *targetHeightParam;

	float blurSize;
	obs_properties_t *filterProperties;
	obs_property_t *filterPropertiesGroup;

	char const *shaderFileName = "test.effect";
	char const *filterName = obs_module_text("Test");

public:
	TestFilter();
	~TestFilter();
	void SetFilterParameters(gs_effect_t *effect);
	void ConfigureFilter(obs_data_t *settings, obs_source_t *source);
	void UpdateFilter(obs_data_t *settings);
	void SetProperties(obs_properties_t *mainProperties, std::string name);
	void SetPropertyDefaults(obs_data_t *settings);
	void Render(obs_source_t *context);
	void HidePropertiesGroup();
	void ShowPropertiesGroup();
	const char *GetShaderFilePath();
	const char *GetFilterName();
};

#endif // TESTFILTER_H
