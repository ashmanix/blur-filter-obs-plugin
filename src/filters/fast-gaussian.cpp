#include "fast-gaussian.hpp"

FastGaussianFilter::FastGaussianFilter() {}

FastGaussianFilter::~FastGaussianFilter() {}

void FastGaussianFilter::SetParameters(gs_effect_t *effect)
{
	blurDirectionsParam =
		gs_effect_get_param_by_name(effect, "blurDirections");
	blurQualityParam = gs_effect_get_param_by_name(effect, "blurQuality");
	blurSizeParam = gs_effect_get_param_by_name(effect, "blurSize");

	targetWidthParam = gs_effect_get_param_by_name(effect, "targetWidth");
	targetHeightParam = gs_effect_get_param_by_name(effect, "targetHeight");
}

void FastGaussianFilter::UpdateFilter(obs_data_t *settings)
{
	double settingsDirections =
		obs_data_get_double(settings, SETTING_FAST_GAUSSIAN_DIRECTIONS);
	blurDirections = (float)settingsDirections;

	double settingsBlurQuality =
		obs_data_get_double(settings, SETTING_FAST_GAUSSIAN_QUALITY);
	blurQuality = (float)settingsBlurQuality;

	double settingsBlurSize =
		obs_data_get_double(settings, SETTING_FAST_GAUSSIAN_SIZE);
	blurSize = (float)settingsBlurSize;
}

void FastGaussianFilter::SetProperties(obs_properties_t *mainProperties,
				       std::string name)
{
	filterProperties = obs_properties_create();

	obs_properties_add_float_slider(
		filterProperties, SETTING_FAST_GAUSSIAN_DIRECTIONS,
		obs_module_text("BlurDirectionsSelectTitle"), 1.0, 20.0, 0.1);

	obs_properties_add_float_slider(
		filterProperties, SETTING_FAST_GAUSSIAN_QUALITY,
		obs_module_text("BlurQualitySelectTitle"), 1.0, 5.0, 0.1);

	obs_properties_add_float_slider(filterProperties,
					SETTING_FAST_GAUSSIAN_SIZE,
					obs_module_text("BlurSizeSelectTitle"),
					0.1, 100.0, 0.1);

	filterPropertiesGroup = obs_properties_add_group(
		mainProperties, name.c_str(),
		obs_module_text("FilterOptionsGroupTitle"), OBS_GROUP_NORMAL,
		filterProperties);
}

void FastGaussianFilter::SetPropertyDefaults(obs_data_t *settings)
{
	obs_data_set_default_double(settings, SETTING_FAST_GAUSSIAN_DIRECTIONS,
				    10.0);
	obs_data_set_default_double(settings, SETTING_FAST_GAUSSIAN_QUALITY,
				    3.0);
	obs_data_set_default_double(settings, SETTING_FAST_GAUSSIAN_SIZE, 20.0);
}

void FastGaussianFilter::Render(obs_source_t *context)
{
	gs_effect_set_float(blurDirectionsParam, blurDirections);
	gs_effect_set_float(blurQualityParam, blurQuality);
	gs_effect_set_float(blurSizeParam, blurSize);

	gs_effect_set_float(targetWidthParam,
			    (float)obs_source_get_width(context));
	gs_effect_set_float(targetHeightParam,
			    (float)obs_source_get_height(context));
}

void FastGaussianFilter::HidePropertiesGroup()
{
	if (filterPropertiesGroup) obs_property_set_visible(filterPropertiesGroup, false);
}

void FastGaussianFilter::ShowPropertiesGroup()
{
	if (filterPropertiesGroup) obs_property_set_visible(filterPropertiesGroup, true);
}

const char *FastGaussianFilter::GetShaderFilePath()
{
	const char *path = obs_module_file(shaderFileName);
	return path;
}

const char *FastGaussianFilter::GetFilterName()
{
	return filterName;
}
