#ifndef BOXBLURFILTER_H
#define BOXBLURFILTER_H

#include "base-filter.hpp"
#include <plugin-support.h>
#include <obs-module.h>
#include <string>
#include <memory>

#define SETTING_BOX_FILTER_SIZE "box_filter_blur_size"

class BoxBlurFilter : public BaseFilter {
private:
	gs_eparam_t *blurSizeParam = nullptr;
	gs_eparam_t *targetWidthParam = nullptr;
	gs_eparam_t *targetHeightParam = nullptr;

	float blurSize;
	obs_properties_t *filterProperties;
	obs_property_t *filterPropertiesGroup = nullptr;

	char const *shaderFileName = "box_blur.effect";
	char const *filterName = obs_module_text("BlurTypeSelectOptionBox");

public:
	BoxBlurFilter();
	~BoxBlurFilter();
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

#endif // BOXBLURFILTER_H
