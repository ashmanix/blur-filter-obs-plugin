#include "test-filter.hpp"

TestFilter::TestFilter()
{
	char *effect_path = obs_module_file(shaderFileName);

	bfree(effect_path);
}

TestFilter::~TestFilter() {}

void TestFilter::SetFilterParameters(gs_effect_t *effect)
{
	blurSizeParam = gs_effect_get_param_by_name(effect, "blurSize");
	targetWidthParam = gs_effect_get_param_by_name(effect, "targetWidth");
	targetHeightParam = gs_effect_get_param_by_name(effect, "targetHeight");
}

void TestFilter::ConfigureFilter(obs_data_t *settings,
					   obs_source_t *source)
{
	UNUSED_PARAMETER(settings);
	UNUSED_PARAMETER(source);
	obs_log(LOG_INFO, "something");
}

void TestFilter::UpdateFilter(obs_data_t *settings)
{
	double settingsBlurSize =
		obs_data_get_double(settings, SETTING_TEST_FILTER_SIZE);
	blurSize = (float)settingsBlurSize;
}

void TestFilter::SetProperties(obs_properties_t *mainProperties,
					 std::string name)
{
	filterProperties = obs_properties_create();

	obs_properties_add_float_slider(filterProperties,
					SETTING_TEST_FILTER_SIZE,
					obs_module_text("BlurSizeSelectTitle"),
					0, 10.0, 0.1);

	// filterPropertiesGroup = obs_properties_add_group(
	// 	mainProperties, name.c_str(),
	// 	obs_module_text("FilterOptionsGroupTitle"), OBS_GROUP_NORMAL, filterProperties);

	filterPropertiesGroup = obs_properties_add_group(
		mainProperties, name.c_str(), name.c_str(), OBS_GROUP_NORMAL,
		filterProperties);
}

void TestFilter::SetPropertyDefaults(obs_data_t *settings)
{
	obs_data_set_default_double(settings, SETTING_TEST_FILTER_SIZE,
				    0.1);
	obs_data_set_default_int(settings, SETTING_TEST_FILTER_TYPE, 0);
}

void TestFilter::Render(obs_source_t *context)
{
	gs_effect_set_float(blurSizeParam,
			    (blurSize == 0.0) ? 0.01f : blurSize);
	gs_effect_set_float(targetWidthParam,
			    (float)obs_source_get_width(context));
	gs_effect_set_float(targetHeightParam,
			    (float)obs_source_get_height(context));
}

void TestFilter::HidePropertiesGroup()
{
	obs_property_set_visible(filterPropertiesGroup, false);
}

void TestFilter::ShowPropertiesGroup()
{
	obs_property_set_visible(filterPropertiesGroup, true);
}

const char *TestFilter::GetShaderFilePath()
{
	const char *path = obs_module_file(shaderFileName);
	return path;
}
