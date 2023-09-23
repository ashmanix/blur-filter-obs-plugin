#include "simple-gaussian.hpp"

SimpleGaussianFilter::SimpleGaussianFilter()
{
	char *effect_path = obs_module_file(shaderFileName);

	bfree(effect_path);
}

SimpleGaussianFilter::~SimpleGaussianFilter() {}

void SimpleGaussianFilter::SetFilterParameters(gs_effect_t *effect)
{
	blurSizeParam = gs_effect_get_param_by_name(effect, "blurSize");
	targetWidthParam = gs_effect_get_param_by_name(effect, "targetWidth");
	targetHeightParam = gs_effect_get_param_by_name(effect, "targetHeight");
}

void SimpleGaussianFilter::ConfigureFilter(obs_data_t *settings,
					   obs_source_t *source)
{
	UNUSED_PARAMETER(settings);
	UNUSED_PARAMETER(source);
	obs_log(LOG_INFO, "something");
}

void SimpleGaussianFilter::UpdateFilter(obs_data_t *settings)
{
	double settingsBlurSize =
		obs_data_get_double(settings, SETTING_SIMPLE_GAUSSIAN_SIZE);
	blurSize = (float)settingsBlurSize;
}

void SimpleGaussianFilter::SetProperties(obs_properties_t *mainProperties,
					 std::string name)
{
	filterProperties = obs_properties_create();

	obs_properties_add_float_slider(filterProperties,
					SETTING_SIMPLE_GAUSSIAN_SIZE,
					obs_module_text("BlurSizeSelectTitle"),
					0, 10.0, 0.1);

	// filterPropertiesGroup = obs_properties_add_group(
	// 	mainProperties, name.c_str(),
	// 	obs_module_text("FilterOptionsGroupTitle"), OBS_GROUP_NORMAL, filterProperties);

	filterPropertiesGroup = obs_properties_add_group(
		mainProperties, name.c_str(), name.c_str(), OBS_GROUP_NORMAL,
		filterProperties);
}

void SimpleGaussianFilter::SetPropertyDefaults(obs_data_t *settings)
{
	obs_data_set_default_double(settings, SETTING_SIMPLE_GAUSSIAN_SIZE,
				    0.1);
	obs_data_set_default_int(settings, SETTING_SIMPLE_GAUSSIAN_TYPE, 0);
}

void SimpleGaussianFilter::Render(obs_source_t *context)
{
	gs_effect_set_float(blurSizeParam,
			    (blurSize == 0.0) ? 0.01f : blurSize);
	gs_effect_set_float(targetWidthParam,
			    (float)obs_source_get_width(context));
	gs_effect_set_float(targetHeightParam,
			    (float)obs_source_get_height(context));
}

void SimpleGaussianFilter::HidePropertiesGroup()
{
	obs_property_set_visible(filterPropertiesGroup, false);
}

void SimpleGaussianFilter::ShowPropertiesGroup()
{
	obs_property_set_visible(filterPropertiesGroup, true);
}

const char *SimpleGaussianFilter::GetShaderFilePath()
{
	const char *path = obs_module_file(shaderFileName);
	return path;
}

const char *SimpleGaussianFilter::GetFilterName()
{
	return filterName;
}
