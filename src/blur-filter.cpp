#include "blur-filter.hpp"

BlurFilter::~BlurFilter() {}

void BlurFilter::RegisterSource()
{
	source_info.id = "ashmanix_plugin_blur_filter";
	source_info.type = OBS_SOURCE_TYPE_FILTER;
	source_info.output_flags = OBS_SOURCE_VIDEO;
	source_info.get_name = BlurFilterGetName;
	source_info.create = BlurFilterCreate;
	source_info.destroy = BlurFilterDestroy;
	source_info.video_render = BlurFilterRender;
	source_info.update = BlurFilterUpdate;
	source_info.get_properties = BlurFilterProperties;
	source_info.get_defaults = BlurFilterDefaults;

	obs_register_source(&source_info);
}

const char *BlurFilter::BlurFilterGetName(void *unused)
{
	UNUSED_PARAMETER(unused);
	return obs_module_text("BlurFilter");
}

void BlurFilter::ChangeFilterSelection(struct gaussian_blur_data *filter)
{
	char *effect_path = obs_module_file(filter->selectedFileName);
	// blog(LOG_INFO, "Path is: %s", effect_path);
	obs_enter_graphics();
	if (filter->effect) {
		gs_effect_destroy(filter->effect);
	}
	filter->effect = gs_effect_create_from_file(effect_path, NULL);
	obs_leave_graphics();

	if (!filter->effect) {
		blog(LOG_ERROR, "Could not load blur effect file '%s'",
		     effect_path);
	}

	filter->blurSizeParam =
		gs_effect_get_param_by_name(filter->effect, "blurSize");
	filter->targetWidthParam =
		gs_effect_get_param_by_name(filter->effect, "targetWidth");
	filter->targetHeightParam =
		gs_effect_get_param_by_name(filter->effect, "targetHeight");

	bfree(effect_path);
}

void *BlurFilter::BlurFilterCreate(obs_data_t *settings, obs_source_t *source)
{
	UNUSED_PARAMETER(settings);
	struct gaussian_blur_data *filter =
		(struct gaussian_blur_data *)bzalloc(
			sizeof(struct gaussian_blur_data));
	// blog(LOG_INFO, "Creating Filter");

	filter->context = source;
	filter->selectedFileName = bstrdup("box_blur.hlsl");
	ChangeFilterSelection(filter); // Default to box blur
	obs_source_update(source, settings);

	return filter;
}

void BlurFilter::BlurFilterDestroy(void *data)
{
	struct gaussian_blur_data *filter = (struct gaussian_blur_data *)data;
	if (filter->effect) {
		obs_enter_graphics();
		gs_effect_destroy(filter->effect);
		obs_leave_graphics();
	}
	bfree(data);
}

void BlurFilter::BlurFilterUpdate(void *data, obs_data_t *settings)
{
	// blog(LOG_INFO, "Updating Filter");
	struct gaussian_blur_data *filter = (struct gaussian_blur_data *)data;
	double blurSize = obs_data_get_double(settings, SETTING_BLUR_SIZE);
	filter->blurSize = (float)blurSize;
	long long blurType = obs_data_get_int(settings, SETTING_BLUR_TYPE);

	char *effect_file = new char[30];
	switch (blurType) {
	case box:
		strcpy(effect_file, "box_blur.hlsl");
		break;
	case gaussian:
		strcpy(effect_file, "gaussian_blur.hlsl");
		break;

	default:
		strcpy(effect_file, "box_blur.hlsl");
		break;
	}
	if (strcmp(effect_file, filter->selectedFileName) != 0) {
		blog(LOG_INFO, "Changing effect file to: %s", effect_file);
		filter->selectedFileName = bstrdup(effect_file);
		ChangeFilterSelection(filter);
	}
	delete[] effect_file;
}

obs_properties_t *BlurFilter::BlurFilterProperties(void *data)
{
	obs_properties_t *properties = obs_properties_create();
	obs_property_t *dropdown_property = obs_properties_add_list(
		properties, SETTING_BLUR_TYPE,
		obs_module_text("BurTypeDropdownSelectTitle"),
		OBS_COMBO_TYPE_LIST, OBS_COMBO_FORMAT_INT);
	obs_properties_add_float_slider(properties, SETTING_BLUR_SIZE,
					obs_module_text("BlurSizeSelectTitle"),
					0, 10.0, 0.1);
	obs_property_list_add_int(dropdown_property,
				  obs_module_text("BlurTypeSelectOptionBox"),
				  box);
	obs_property_list_add_int(
		dropdown_property,
		obs_module_text("BlurTypeSelectOptionGaussian"), gaussian);

	UNUSED_PARAMETER(data);
	return properties;
}

void BlurFilter::BlurFilterDefaults(obs_data_t *settings)
{
	obs_data_set_default_double(settings, SETTING_BLUR_SIZE, 0.1);
	obs_data_set_default_int(settings, SETTING_BLUR_TYPE, 0);
}

void BlurFilter::BlurFilterRender(void *data, gs_effect_t *effect)
{
	struct gaussian_blur_data *filter = (struct gaussian_blur_data *)data;

	if (!obs_source_process_filter_begin(filter->context, GS_RGBA,
					     OBS_ALLOW_DIRECT_RENDERING))
		return;

	gs_effect_set_float(filter->blurSizeParam, (filter->blurSize == 0.0)
							   ? 0.01f
							   : filter->blurSize);
	gs_effect_set_float(filter->targetWidthParam,
			    (float)obs_source_get_width(filter->context));
	gs_effect_set_float(filter->targetHeightParam,
			    (float)obs_source_get_height(filter->context));

	if (!filter->effect) {
		obs_source_process_filter_end(filter->context, effect, 0, 0);
	} else {
		obs_source_process_filter_end(filter->context, filter->effect,
					      0, 0);
	}

	// Second pass
	// if (!obs_source_process_filter_begin(filter->context, GS_RGBA,
	// 				     OBS_ALLOW_DIRECT_RENDERING))
	// 	return;

	// gs_effect_set_float(filter->blurSizeParam,
	// 		    (filter->blurSize == 0.0 ? 0.01
	// 					     : filter->blurSize));
	// gs_effect_set_float(filter->targetWidthParam,
	// 		    (float)obs_source_get_width(filter->context));
	// gs_effect_set_float(filter->targetHeightParam,
	// 		    (float)obs_source_get_height(filter->context));

	// if (!filter->effect) {
	// 	obs_source_process_filter_end(filter->context, effect, 0, 0);
	// } else {
	// 	obs_source_process_filter_end(filter->context, filter->effect,
	// 				      0, 0);
	// }
}

BlurFilter::BlurFilter() {}
