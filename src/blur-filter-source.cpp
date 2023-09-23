#include "blur-filter-source.hpp"

BlurFilterSource::BlurFilterSource() {}
BlurFilterSource::~BlurFilterSource() {}

void BlurFilterSource::RegisterSource()
{
	source_info.id = "ashmanix_plugin_blur_filter";
	source_info.type = OBS_SOURCE_TYPE_FILTER;
	source_info.output_flags = OBS_SOURCE_VIDEO;
	source_info.get_name = GetName;
	source_info.create = CreateSource;
	source_info.destroy = DestroySource;
	source_info.video_render = RenderSource;
	source_info.update = UpdateSource;
	source_info.get_properties = GetProperties;

	obs_register_source(&source_info);
}

const char *BlurFilterSource::GetName(void *unused)
{
	UNUSED_PARAMETER(unused);
	return obs_module_text("BlurFilter");
}

void BlurFilterSource::ChangeFilterSelection(struct filter_data *filterData)
{
	long long filterIndex = filterData->selectedFilterIndex;

	obs_enter_graphics();

	if (filterData->effect) {
		gs_effect_destroy(filterData->effect);
	}
	const char *effectPath =
		filterData->filterArray[filterIndex]->GetShaderFilePath();
	filterData->effect = gs_effect_create_from_file(effectPath, NULL);

	obs_leave_graphics();

	if (!filterData->effect) {
		obs_log(LOG_ERROR, "Could not load effect file '%s'",
			effectPath);
	}

	filterData->filterArray[filterIndex]->SetFilterParameters(
		filterData->effect);
	TogglePropertyGroupVisibility(filterData);
}

void *BlurFilterSource::CreateSource(obs_data_t *settings, obs_source_t *source)
{
	struct filter_data *filterData =
		(struct filter_data *)bzalloc(sizeof(struct filter_data));

	filterData->filterArray.push_back(
		std::unique_ptr<BaseFilter>(new SimpleGaussianFilter()));
	filterData->filterArray.push_back(
		std::unique_ptr<BaseFilter>(new TestFilter()));

	SetDefaultProperties(filterData);

	filterData->context = source;

	ChangeFilterSelection(filterData);
	obs_source_update(source, settings);

	return filterData;
}

void BlurFilterSource::DestroySource(void *data)
{
	struct filter_data *filterData = (struct filter_data *)data;
	if (filterData->effect) {
		obs_enter_graphics();
		gs_effect_destroy(filterData->effect);
		obs_leave_graphics();
	}
	bfree(data);
}

void BlurFilterSource::UpdateSource(void *data, obs_data_t *settings)
{
	// obs_log(LOG_INFO, "Updating Source");
	struct filter_data *filterData = (struct filter_data *)data;
	long long filterIndex = filterData->selectedFilterIndex;
	filterData->filterArray[filterIndex]->UpdateFilter(settings);
}

bool BlurFilterSource::FilterSelectionChangeCallback(void *data,
						     obs_properties_t *props,
						     obs_property_t *list,
						     obs_data_t *settings)
{
	UNUSED_PARAMETER(props);
	long long selectedFilterIndex =
		obs_data_get_int(settings, SETTING_BLUR_TYPE);

	struct filter_data *filterData = (struct filter_data *)data;

	if (selectedFilterIndex != filterData->selectedFilterIndex) {
		filterData->selectedFilterIndex = selectedFilterIndex;

		ChangeFilterSelection(filterData);

		obs_log(LOG_INFO, "Filter Changed To: %s",
			obs_property_list_item_name(list, selectedFilterIndex));
	}

	return true;
}

obs_properties_t *BlurFilterSource::GetProperties(void *data)
{
	struct filter_data *filterData = (struct filter_data *)data;

	if (filterData) {
		filterData->mainProperties = obs_properties_create();

		obs_property_t *filterDropdownProperty =
			obs_properties_add_list(
				filterData->mainProperties, SETTING_BLUR_TYPE,
				obs_module_text("BlurTypeDropdownSelectTitle"),
				OBS_COMBO_TYPE_LIST, OBS_COMBO_FORMAT_INT);

		for (size_t i = 0; i < filterData->filterArray.size(); i++) {

			const char *name =
				filterData->filterArray[i]->GetFilterName();
			obs_property_list_add_int(filterDropdownProperty, name,
						  i);
		}

		obs_property_set_modified_callback2(
			filterDropdownProperty, FilterSelectionChangeCallback,
			filterData);

		std::string filterStringName = "filter-";
		for (size_t i = 0; i < filterData->filterArray.size(); i++) {
			filterData->filterArray[i]->SetProperties(
				filterData->mainProperties,
				filterStringName.append(std::to_string(i)));

			TogglePropertyGroupVisibility(filterData);
		}
		return filterData->mainProperties;
	}

	return nullptr;
}

void BlurFilterSource::SetDefaultProperties(filter_data *filterData)
{
	for (size_t i = 0; i < filterData->filterArray.size(); i++) {
		filterData->filterArray[i]->SetPropertyDefaults();
	}
}

void BlurFilterSource::RenderSource(void *data, gs_effect_t *effect)
{
	struct filter_data *filterData = (struct filter_data *)data;
	long long filterIndex = filterData->selectedFilterIndex;
	obs_source_t *context = filterData->context;

	if (!obs_source_process_filter_begin(filterData->context, GS_RGBA,
					     OBS_ALLOW_DIRECT_RENDERING))
		return;

	filterData->filterArray[filterIndex]->Render(context);

	if (!filterData->effect) {
		obs_source_process_filter_end(filterData->context, effect, 0,
					      0);
	} else {
		obs_source_process_filter_end(filterData->context,
					      filterData->effect, 0, 0);
	}
}

void BlurFilterSource::TogglePropertyGroupVisibility(filter_data *data)
{
	for (size_t i = 0; i < data->filterArray.size(); i++) {
		if (data->selectedFilterIndex >= 0 &&
		    i != (size_t)data->selectedFilterIndex) {
			data->filterArray[i]->HidePropertiesGroup();
		} else {
			data->filterArray[i]->ShowPropertiesGroup();
		}
	}
}
