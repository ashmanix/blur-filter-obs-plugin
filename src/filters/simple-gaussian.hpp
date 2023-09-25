#ifndef SIMPLEGUASSIAN_H
#define SIMPLEGUASSIAN_H

#include "base-filter.hpp"
#include <plugin-support.h>
#include <obs-module.h>
#include <string>
#include <memory>


#define SETTING_SIMPLE_GAUSSIAN_SIZE "simple_gaussian_blur_size"

class SimpleGaussianFilter : public BaseFilter {
private:
	gs_eparam_t *blurSizeParam = nullptr;
	gs_eparam_t *targetWidthParam = nullptr;
	gs_eparam_t *targetHeightParam = nullptr;

	float blurSize;
	obs_properties_t *filterProperties = nullptr;
	obs_property_t *filterPropertiesGroup = nullptr;

	char const *shaderFileName = "simple_gaussian_blur.effect";
	char const *filterName =
		obs_module_text("BlurTypeSelectOptionSimpleGaussian");

public:
	SimpleGaussianFilter();
	~SimpleGaussianFilter();
	void SetParameters(gs_effect_t *effect);
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
