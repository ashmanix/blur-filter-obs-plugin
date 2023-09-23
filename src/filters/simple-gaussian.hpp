#ifndef SIMPLEGUASSIAN_H
#define SIMPLEGUASSIAN_H

#include "base-filter.hpp"
#include <plugin-support.h>
#include <obs-module.h>
#include <string>

#define SETTING_SIMPLE_GAUSSIAN_SIZE "simple_gaussian_blur_size"
#define SETTING_SIMPLE_GAUSSIAN_TYPE "simple_guassian_blur_type"

class SimpleGaussianFilter : public BaseFilter {
private:
	gs_effect_t *effect;

	gs_eparam_t *blurSizeParam;
	gs_eparam_t *targetWidthParam;
	gs_eparam_t *targetHeightParam;

	float blurSize;
	obs_properties_t *filterProperties;
	obs_property_t *filterPropertiesGroup;

	char const *shaderFileName = "simple_gaussian_blur.effect";
	char const *filterName = obs_module_text("BlurTypeSelectOptionSimpleGaussian");

public:
	SimpleGaussianFilter();
	~SimpleGaussianFilter();
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

#endif // SIMPLEGAUSSIAN_H
